/*
 *
 * cpswhalcommon_miimdio.c
 * Description:
 * see below
 *
 *
 * Copyright (C) 2008, Texas Instruments, Incorporated
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 */


/***************************************************************************
**  TNETxxxx Software Support
**
**  FILE: cpswhalcommon_miimdio.c
**
**  DESCRIPTION:
**    MDIO Polling State Machine API. Functions will enable mii-Phy
**    negotiation.
**
**  HISTORY:
**  Date      Modifier         Ver    Notes
**  01Jan01 Denis, Bill             Original
**  27Mar02 Michael Hanrahan        (modified from emacmdio.c)
**  07May02 Michael Hanrahan        replaced clockwait for code delay
**  10Jul02 Michael Hanrahan        more debug, if fallback link is selected
**  04June04 Anant Gole             Ported MDIX related code from PSP5.7.2.8
**  18Jan05 Greg Guyotte            Created a merge of Mick's latest common
**                                  release (bubble 14 in halsync/common) with
**                                  Anant's PSP version.  Debug support must be
**                                  compiled in, and Anant's modification to
**                                  remove the MDIO reset from this code has
**                                  been made, to comply with PSPF.  The reset
**                                  is done externally (in PAL, via DDC_Open).
*****************************************************************************/
#include "cpswhalcommon_stddef.h"
#ifdef CPMAC_NO_ARCH_DRIVER
#include "cpswhalcommon_support.h"
#endif

/* 
 * R A N D O M  N U M B E R  S U P P O R T
 */

#define COMMON_RANDOM_MAX        0xFFFF
static unsigned long RandomSeed = 1;

static int
cpswHalCommonRandom (void)
{
  RandomSeed = RandomSeed * 1103515245 + 12345;
  return ((bit32u) (RandomSeed/65536) % (COMMON_RANDOM_MAX + 1));
}


static int
cpswHalCommonRandomRange (int min, int max)
{
  int iTmp;

  iTmp =  cpswHalCommonRandom();
  iTmp %= ((max-min)+1);
  iTmp += min;
  return(iTmp);
}

typedef struct _phy_device
{
  bit32u miibase;
  bit32u inst;
  bit32u PhyState;
  bit32u MdixMask;
  bit32u PhyMask;
  bit32u MLinkMask;
  bit32u PhyMode;
  bit32u SPEED_1000; /* set to 1 for gig capable phys */
  int   (*Printf)(const char *Format, ...);
} PHY_DEVICE;

#include "cpswhalcommon_miimdio.h"
#include "cpswhalcommon_miimdio_regs.h"

#ifdef __CPHAL_DEBUG
 /*  Is this global allowable??? */
 static char *lstate[]={"NULL","INIT","FINDING","FOUND","NWAY_START","NWAY_WAIT","LINK_WAIT","LINKED","LOOPBACK"};
 #define mdioPrintf(format, args...)  PhyDev->Printf(format, ## args);
 static void _cpswHalCommonMiiMdioDumpPhy(PHY_DEVICE *PhyDev, bit32u p);
 static void _cpswHalCommonMiiMdioDumpState(PHY_DEVICE *PhyDev);
 static void _cpswHalCommonMiiMdioDumpPhyDetailed(PHY_DEVICE *PhyDev);
#else
 #define mdioPrintf(format, args...)  {}
#endif

static void   _cpswHalCommonMiiMdioWaitForAccessComplete(PHY_DEVICE *PhyDev);
static void   _cpswHalCommonMiiMdioUserAccess(PHY_DEVICE *PhyDev, bit32u method, bit32u regadr, bit32u phyadr, bit32u data);


static void _cpswHalCommonMiiMdioDisablePhy(PHY_DEVICE *PhyDev,bit32u PhyNum);
static void _cpswHalCommonMiiMdioPhyTimeOut(PHY_DEVICE *PhyDev);
static void _cpswHalCommonMiiMdioResetPhy(PHY_DEVICE *PhyDev,bit32u PhyNum);


static void _cpswHalCommonMiiMdioDefaultState  (PHY_DEVICE *PhyDev);
static void _cpswHalCommonMiiMdioFindingState  (PHY_DEVICE *PhyDev);
static void _cpswHalCommonMiiMdioFoundState    (PHY_DEVICE *PhyDev);
static void _cpswHalCommonMiiMdioInitState     (PHY_DEVICE *PhyDev);
static void _cpswHalCommonMiiMdioLinkedState   (PHY_DEVICE *PhyDev);
static void _cpswHalCommonMiiMdioLinkWaitState (PHY_DEVICE *PhyDev);
static void _cpswHalCommonMiiMdioLoopbackState (PHY_DEVICE *PhyDev);
static void _cpswHalCommonMiiMdioNwayStartState(PHY_DEVICE *PhyDev);
static void _cpswHalCommonMiiMdioNwayWaitState (PHY_DEVICE *PhyDev);
/* Below Declarations added to recognize Gigabit phy  and Agere phy. 
 * set the trim values as mentioned in Agere errata sheet
 */
static void _cpswHalCommonMiiMdioSetPhyTrim(PHY_DEVICE *PhyDev,bit32u PhyNum);
 
 
#define PHY_NOT_FOUND  0xFFFF    /*  Used in Phy Detection */

/*PhyState breakout                                                          */

#define PHY_DEV_OFFSET      (0)
#define PHY_DEV_SIZE        (5)
#define PHY_DEV_MASK        (0x1f<<PHY_DEV_OFFSET)

#define PHY_STATE_OFFSET    (PHY_DEV_SIZE+PHY_DEV_OFFSET)
#define PHY_STATE_SIZE      (5)
#define PHY_STATE_MASK      (0x1f<<PHY_STATE_OFFSET)
#define INIT       (1<<PHY_STATE_OFFSET)
#define FINDING    (2<<PHY_STATE_OFFSET)
#define FOUND      (3<<PHY_STATE_OFFSET)
#define NWAY_START (4<<PHY_STATE_OFFSET)
#define NWAY_WAIT  (5<<PHY_STATE_OFFSET)
#define LINK_WAIT  (6<<PHY_STATE_OFFSET)
#define LINKED     (7<<PHY_STATE_OFFSET)
#define LOOPBACK   (8<<PHY_STATE_OFFSET)

#define PHY_SPEED_OFFSET    (PHY_STATE_OFFSET+PHY_STATE_SIZE)
#define PHY_SPEED_SIZE      (1)
#define PHY_SPEED_MASK      (1<<PHY_SPEED_OFFSET)

#define PHY_DUPLEX_OFFSET   (PHY_SPEED_OFFSET+PHY_SPEED_SIZE)
#define PHY_DUPLEX_SIZE     (1)
#define PHY_DUPLEX_MASK     (1<<PHY_DUPLEX_OFFSET)

#define PHY_TIM_OFFSET      (PHY_DUPLEX_OFFSET+PHY_DUPLEX_SIZE)
#define PHY_TIM_SIZE        (10)
#define PHY_TIM_MASK        (0x3ff<<PHY_TIM_OFFSET)
#define PHY_FIND_TO (  2<<PHY_TIM_OFFSET)
#define PHY_RECK_TO (200<<PHY_TIM_OFFSET)
#define PHY_LINK_TO (500<<PHY_TIM_OFFSET)
#define PHY_NWST_TO (500<<PHY_TIM_OFFSET)
#define PHY_NWDN_TO (800<<PHY_TIM_OFFSET)
#define PHY_MDIX_TO (274<<PHY_TIM_OFFSET) /* 2.74 Seconds <--Spec and empirical */

#define PHY_SMODE_OFFSET    (PHY_TIM_OFFSET+PHY_TIM_SIZE)
#define PHY_SMODE_SIZE      (7)
#define PHY_SMODE_MASK      (0x7f<<PHY_SMODE_OFFSET)
#define SMODE_LPBK   (0x40<<PHY_SMODE_OFFSET)
#define SMODE_AUTO   (0x20<<PHY_SMODE_OFFSET)
#define SMODE_FD1000 (0x10<<PHY_SMODE_OFFSET)
#define SMODE_FD100  (0x08<<PHY_SMODE_OFFSET)
#define SMODE_HD100  (0x04<<PHY_SMODE_OFFSET)
#define SMODE_FD10   (0x02<<PHY_SMODE_OFFSET)
#define SMODE_HD10   (0x01<<PHY_SMODE_OFFSET)
#define SMODE_ALL    (0x1f<<PHY_SMODE_OFFSET)

#define PHY_CHNG_OFFSET    (PHY_SMODE_OFFSET+PHY_SMODE_SIZE)
#define PHY_CHNG_SIZE      (1)
#define PHY_CHNG_MASK      (1<<PHY_CHNG_OFFSET)
#define PHY_CHANGE (1<<PHY_CHNG_OFFSET)

#define PHY_TIMEDOUT_OFFSET    (PHY_CHNG_OFFSET+PHY_CHNG_SIZE)
#define PHY_TIMEDOUT_SIZE      (1)  /*  31 Bits used */
#define PHY_TIMEDOUT_MASK      (1<<PHY_TIMEDOUT_OFFSET)
#define PHY_MDIX_SWITCH  (1<<PHY_TIMEDOUT_OFFSET)

#define PHY_MDIX_OFFSET    (PHY_TIMEDOUT_OFFSET+PHY_TIMEDOUT_SIZE)
#define PHY_MDIX_SIZE      (1)     /*  32 Bits used */
#define PHY_MDIX_MASK      (1<<PHY_MDIX_OFFSET)
#define PHY_MDIX     (1<<PHY_MDIX_OFFSET)

/*  Local MDIO Register Macros    */

#define myMDIO_ALIVE           MDIO_ALIVE     (PhyDev->miibase)
#define myMDIO_CONTROL         MDIO_CONTROL   (PhyDev->miibase)
#define myMDIO_LINK            MDIO_LINK      (PhyDev->miibase)
#define myMDIO_LINKINT         MDIO_LINKINT   (PhyDev->miibase)
#define myMDIO_USERACCESS      MDIO_USERACCESS(PhyDev->miibase, PhyDev->inst)
#define myMDIO_USERPHYSEL      MDIO_USERPHYSEL(PhyDev->miibase, PhyDev->inst)
#define myMDIO_VER             MDIO_VER       (PhyDev->miibase)

#ifndef VOLATILE32
#define VOLATILE32(addr) (*((volatile bit32u *)(addr)))
#endif


/*User Calls*********************************************************       */

void
cpswHalCommonMiiMdioClose (PHY_DEVICE *PhyDev, int Full)
  {
  }


int
cpswHalCommonMiiMdioInit (PHY_DEVICE * PhyDev, bit32u miibase, bit32u inst,
               bit32u PhyMask, bit32u MLinkMask, bit32u MdixMask, bit32u ResetReg,
               bit32u ResetBit, bit32u MdioBusFreq,
               bit32u MdioClockFreq, int verbose,
               int (*osprintf)(const char *Format, ...))
{
  bit32u HighestChannel;
  bit32u ControlState;
  bit32u *PhyState = &PhyDev->PhyState;
  bit32u clkdiv;

  PhyDev->miibase   = miibase;
  PhyDev->inst      = inst;
  PhyDev->PhyMask   = PhyMask;
  PhyDev->MLinkMask = MLinkMask;
  PhyDev->MdixMask  = MdixMask;
  PhyDev->Printf    = osprintf;

  *PhyState &= ~PHY_MDIX_MASK;   /* Set initial State to MDI */

  /* Check that the channel supplied is within range */
  /* GSG 1/18/05 - Applied fix identified by Denis.  The definition of the
     Control register changed between versions of MDIO. */
  HighestChannel = myMDIO_CONTROL >> 8;
  if ((myMDIO_VER & 0x0ffff) > 0x0102)
    HighestChannel = myMDIO_CONTROL >> 24;
  HighestChannel &= 0x1f;
  
  if (inst > HighestChannel)
    return(HighestChannel);

  /*Setup MII MDIO access regs                                              */

  /* Calculate the correct value for the mclkdiv */
  if (MdioClockFreq)                                              /*MJH+030402*/
    clkdiv = (MdioBusFreq / MdioClockFreq) - 1;                  /*MJH+030402*/
   else                                                            /*MJH+030402*/
    clkdiv = 0xFF;                                  /*MJH+030402*/

  ControlState  = MDIO_CONTROL_ENABLE;
  ControlState |= ((clkdiv) & MDIO_CONTROL_CLKDIV);

#ifdef CPMAC_NO_ARCH_DRIVER
  /*
      If mii is not out of reset or if the Control Register is not set correctly
      then initalize
  */
  if( !(VOLATILE32(ResetReg) & (1 << ResetBit)) ||
     ((myMDIO_CONTROL & (MDIO_CONTROL_CLKDIV | MDIO_CONTROL_ENABLE)) != ControlState) )/*GSG~030404*/
       {
    cpswHalCommonUnReset(ResetReg,ResetBit);
    myMDIO_CONTROL = ControlState;  /* Enable MDIO   */
    }
#else
    myMDIO_CONTROL = ControlState;  /* Enable MDIO   */
#endif

  *PhyState=INIT;

  mdioPrintf("cpswHalCommonMiiMdioInit\n");
#ifdef __CPHAL_DEBUG
  _cpswHalCommonMiiMdioDumpState(PhyDev);
#endif
  return(0);
  }

void cpswHalCommonMiiMdioSetPhyMode(PHY_DEVICE *PhyDev,bit32u PhyMode)
  {
  bit32u *PhyState = &PhyDev->PhyState;
  bit32u CurrentState;

  PhyDev->PhyMode = PhyMode;   /* used for AUTOMIDX, planned to replace PhyState fields */

  *PhyState&=~PHY_SMODE_MASK;

  if (PhyMode&NWAY_LPBK)   *PhyState|=SMODE_LPBK;
  if (PhyMode&NWAY_AUTO)   *PhyState|=SMODE_AUTO;
  if (PhyMode&NWAY_FD1000) *PhyState|=SMODE_FD1000;
  if (PhyMode&NWAY_FD100)  *PhyState|=SMODE_FD100;
  if (PhyMode&NWAY_HD100)  *PhyState|=SMODE_HD100;
  if (PhyMode&NWAY_FD10)   *PhyState|=SMODE_FD10;
  if (PhyMode&NWAY_HD10)   *PhyState|=SMODE_HD10;

  CurrentState=*PhyState&PHY_STATE_MASK;
  if ((CurrentState==NWAY_START)||
      (CurrentState==NWAY_WAIT) ||
      (CurrentState==LINK_WAIT) ||
      (CurrentState==LINKED)    ||
      (CurrentState==LOOPBACK)    )
    *PhyState=(*PhyState&~PHY_STATE_MASK)|FOUND|PHY_CHANGE;

   mdioPrintf("SetPhyMode:%08X Auto:%d, FD10:%d, HD10:%d, FD100:%d, HD100:%d, FD1000:%d LPBK:%d\n", PhyMode,
     PhyMode&NWAY_AUTO, PhyMode&MII_NWAY_FD10, PhyMode&MII_NWAY_HD10, PhyMode&MII_NWAY_FD100,
     PhyMode&MII_NWAY_HD100, PhyMode&NWAY_FD1000,
     PhyMode&NWAY_LPBK);
#ifdef __CPHAL_DEBUG
  _cpswHalCommonMiiMdioDumpState(PhyDev);
#endif
  }

/* cpswHalCommonMiiMdioTic is called every 10 mili seconds to process Phy states         */

int cpswHalCommonMiiMdioTic(PHY_DEVICE *PhyDev)
  {
  bit32u *PhyState = &PhyDev->PhyState;
  bit32u  CurrentState;

  /*Act on current state of the Phy                                          */

  CurrentState=*PhyState;
  switch(CurrentState&PHY_STATE_MASK)
    {
    case INIT:  
                       _cpswHalCommonMiiMdioInitState(PhyDev);      break;
    case FINDING:  
                       _cpswHalCommonMiiMdioFindingState(PhyDev);   break;
    case FOUND:    
                       _cpswHalCommonMiiMdioFoundState(PhyDev);     break;
    case NWAY_START: 
                       _cpswHalCommonMiiMdioNwayStartState(PhyDev); break;
    case NWAY_WAIT: 
                       _cpswHalCommonMiiMdioNwayWaitState(PhyDev);  break;
    case LINK_WAIT: 
                       _cpswHalCommonMiiMdioLinkWaitState(PhyDev);  break;
    case LINKED:  
                       _cpswHalCommonMiiMdioLinkedState(PhyDev);    break;
    case LOOPBACK: 
                       _cpswHalCommonMiiMdioLoopbackState(PhyDev);  break;
    default:     
                       _cpswHalCommonMiiMdioDefaultState(PhyDev);   break;
    }

  /*Dump state info if a change has been detected                            */
#ifdef __CPHAL_DEBUG
  if ((CurrentState&~PHY_TIM_MASK)!=(*PhyState&~PHY_TIM_MASK))
    _cpswHalCommonMiiMdioDumpState(PhyDev);
#endif

  /*  Check is MDI/MDIX mode switch is needed */
  if(*PhyState & PHY_MDIX_SWITCH)
    {
    bit32u Mdix;

    *PhyState &= ~PHY_MDIX_SWITCH;  /* Clear Mdix Flip indicator */

    if(*PhyState & PHY_MDIX)
      Mdix = 1;
    else
      Mdix = 0;
    return(_MIIMDIO_MDIXFLIP|Mdix);
    }

  /*Return state change to user                                              */

  if (*PhyState&PHY_CHNG_MASK)
    {
    *PhyState&=~PHY_CHNG_MASK;
    return(TRUE);
    }
   else
    return(FALSE);
  }

/* cpswHalCommonMiiMdioGetDuplex is called to retrieve the Duplex info                   */

int cpswHalCommonMiiMdioGetDuplex(PHY_DEVICE *PhyDev)
  {
  bit32u *PhyState = &PhyDev->PhyState;
  return((*PhyState&PHY_DUPLEX_MASK)?1:0);  /* return 0 or a 1  */
  }

/* cpswHalCommonMiiMdioGetSpeed is called to retreive the Speed info                     */

int cpswHalCommonMiiMdioGetSpeed(PHY_DEVICE *PhyDev)
  {
  bit32u *PhyState = &PhyDev->PhyState;

  if (PhyDev->SPEED_1000 == 1)
    return (2); /* if we are linked at 1000, we return a value of 2 */
  else
    return(*PhyState&PHY_SPEED_MASK);
  }

/* cpswHalCommonMiiMdioGetPhyNum is called to retreive the Phy Device Adr info           */

int cpswHalCommonMiiMdioGetPhyNum(PHY_DEVICE *PhyDev)
  {
  bit32u *PhyState = &PhyDev->PhyState;
  return((*PhyState&PHY_DEV_MASK)>>PHY_DEV_OFFSET);
  }

/* cpswHalCommonMiiMdioGetLoopback is called to Determine if the LOOPBACK state has been reached*/

int cpswHalCommonMiiMdioGetLoopback(PHY_DEVICE *PhyDev)
  {
  bit32u *PhyState = &PhyDev->PhyState;
  return((*PhyState&PHY_STATE_MASK)==LOOPBACK);
  }

void cpswHalCommonMiiMdioLinkChange(PHY_DEVICE *PhyDev)
  {
  bit32u *PhyState = &PhyDev->PhyState;
  bit32u  PhyNum,PhyStatus;

  PhyNum=(*PhyState&PHY_DEV_MASK)>>PHY_DEV_OFFSET;

  if (cpswHalCommonMiiMdioGetLinked(PhyDev))
    {
    PhyStatus=_cpswHalCommonMiiMdioUserAccessRead(PhyDev, PHY_STATUS_REG, PhyNum);

    if ((PhyStatus&MII_PHY_LINKED)==0)
      {
      *PhyState&=~(PHY_TIM_MASK|PHY_STATE_MASK);
      if (*PhyState&SMODE_AUTO)
        {
        _cpswHalCommonMiiMdioUserAccessWrite(PhyDev, PHY_CONTROL_REG, PhyNum, MII_AUTO_NEGOTIATE_EN|MII_RENEGOTIATE);
        *PhyState|=PHY_CHANGE|PHY_NWST_TO|NWAY_START;
        }
       else
        {
        *PhyState|=PHY_CHANGE|PHY_LINK_TO|LINK_WAIT;
        }
      }
    }
  }

/* cpswHalCommonMiiMdioGetLinked is called to Determine if the LINKED state has been reached*/

int cpswHalCommonMiiMdioGetLinked(PHY_DEVICE *PhyDev)
  {
    bit32u *PhyState = &PhyDev->PhyState;
    return ((*PhyState&PHY_STATE_MASK)==LINKED);
  }

void cpswHalCommonMiiMdioGetVer(bit32u miibase, bit32u *ModID,  bit32u *RevMaj,  bit32u *RevMin)
  {
  bit32u  Ver;

  Ver = MDIO_VER(miibase);

  *ModID  = (Ver & MDIO_VER_MODID) >> 16;
  *RevMaj = (Ver & MDIO_VER_REVMAJ) >> 8;
  *RevMin = (Ver & MDIO_VER_REVMIN);
  }

int cpswHalCommonMiiMdioGetPhyDevSize(void)
  {
  return(sizeof(PHY_DEVICE));
  }

  /* returns 0 if current Phy has AutoMdix support, otherwise 0 */
int _cpswHalCommonMiiMdioMdixSupported(PHY_DEVICE *PhyDev)
  {
  bit32u *PhyState = &PhyDev->PhyState;
  bit32u PhyNum;

  if((PhyDev->PhyMode & NWAY_AUTOMDIX) == 0)
    return(0);  /* AutoMdix not turned on */

  PhyNum=(*PhyState&PHY_DEV_MASK)>>PHY_DEV_OFFSET;
  if( ((1<<PhyNum) & PhyDev->MdixMask) == 0)
    return(0);  /*  Phy does not support AutoMdix*/

  return(1);
  }

/* If current Phy has AutoMdix support add Mdix Delay to the Timer State Value */
void _cpswHalCommonMiiMdioMdixDelay(PHY_DEVICE *PhyDev)
  {
  int Delay;
  bit32u *PhyState = &PhyDev->PhyState;
  bit32u PhyNum;

  PhyNum=(*PhyState&PHY_DEV_MASK)>>PHY_DEV_OFFSET;

  if(_cpswHalCommonMiiMdioMdixSupported(PhyDev) == 0)
    return;  /* AutoMdix not supported */

  /* Get the Delay value in milli-seconds and convert to ten-milli second value */
  Delay = cpswHalCommonRandomRange(_AUTOMDIX_DELAY_MIN, _AUTOMDIX_DELAY_MAX);
  Delay /= 10;

  /*  Add AutoMidx Random Switch Delay to AutoMdix Link Delay */

  Delay += (PHY_MDIX_TO>>PHY_TIM_OFFSET);

  /* Change Timeout value to AutoMdix standard */
  *PhyState &= ~(PHY_TIM_MASK);  /* Clear current Time out value */
  *PhyState |=  (Delay<<PHY_TIM_OFFSET);     /* Set new value */

  }

#ifdef __CPHAL_DEBUG
void _cpswHalCommonMiiMdioDumpState(PHY_DEVICE *PhyDev)
  {
  bit32u state    = PhyDev->PhyState;

  mdioPrintf("Phy: %d, ",(state&PHY_DEV_MASK)>>PHY_DEV_OFFSET);
  mdioPrintf("State: %d/%s, ",(state&PHY_STATE_MASK)>>PHY_STATE_OFFSET,lstate[(state&PHY_STATE_MASK)>>PHY_STATE_OFFSET]);
  mdioPrintf("Speed: %d, ",(state&PHY_SPEED_MASK)>>PHY_SPEED_OFFSET);
  mdioPrintf("Dup: %d, ",(state&PHY_DUPLEX_MASK)>>PHY_DUPLEX_OFFSET);
  mdioPrintf("Tim: %d, ",(state&PHY_TIM_MASK)>>PHY_TIM_OFFSET);
  mdioPrintf("SMode: %d, ",(state&PHY_SMODE_MASK)>>PHY_SMODE_OFFSET);
  mdioPrintf("Chng: %d",(state&PHY_CHNG_MASK)>>PHY_CHNG_OFFSET);
  mdioPrintf("\n");

  if (((state&PHY_STATE_MASK)!=FINDING)&&((state&PHY_STATE_MASK)!=INIT))
    _cpswHalCommonMiiMdioDumpPhy(PhyDev, (state&PHY_DEV_MASK)>>PHY_DEV_OFFSET);
  }
#endif

void _cpswHalCommonMiiMdioDisablePhy(PHY_DEVICE *PhyDev,bit32u PhyNum)
  {
  _cpswHalCommonMiiMdioUserAccessWrite(PhyDev, PHY_CONTROL_REG, PhyNum, MII_PHY_ISOLATE|MII_PHY_PDOWN);

  mdioPrintf("cpswHalCommonMiiMdioDisablePhy(%d)\n",PhyNum);
  }

void _cpswHalCommonMiiMdioInitState(PHY_DEVICE *PhyDev)
  {
  bit32u *PhyState = &PhyDev->PhyState;
  bit32u CurrentState;

  CurrentState=*PhyState;
  CurrentState=(CurrentState&~PHY_TIM_MASK)|(PHY_FIND_TO);
  CurrentState=(CurrentState&~PHY_STATE_MASK)|(FINDING);
  CurrentState=(CurrentState&~PHY_SPEED_MASK);
  CurrentState=(CurrentState&~PHY_DUPLEX_MASK);
  CurrentState|=PHY_CHANGE;

  *PhyState=CurrentState;

  }

void _cpswHalCommonMiiMdioFindingState(PHY_DEVICE *PhyDev)
  {
  bit32u *PhyState = &PhyDev->PhyState;
  bit32u  PhyMask  = PhyDev->PhyMask;
  bit32u  PhyNum,i,j,PhyAcks;


  PhyNum=PHY_NOT_FOUND;

  if (*PhyState&PHY_TIM_MASK)
    {
    *PhyState=(*PhyState&~PHY_TIM_MASK)|((*PhyState&PHY_TIM_MASK)-(1<<PHY_TIM_OFFSET));
    }
   else
    {
    PhyAcks=myMDIO_ALIVE;
    PhyAcks&=PhyMask;   /* Only interested in 'our' Phys */
    
    for(i=0,j=1;(i<32)&&((j&PhyAcks)==0);i++,j<<=1);
    
    if ((PhyAcks)&&(i<32)) PhyNum=i;
    if (PhyNum!=PHY_NOT_FOUND)
      {
      /*  Phy Found! */
      *PhyState=(*PhyState&~PHY_DEV_MASK)|((PhyNum&PHY_DEV_MASK)<<PHY_DEV_OFFSET);
      *PhyState=(*PhyState&~PHY_STATE_MASK)|(FOUND);
      *PhyState|=PHY_CHANGE;
      mdioPrintf("cpswHalCommonMiiMdioFindingState: PhyNum: %d\n",PhyNum);
      }
     else
      {
      mdioPrintf("cpswHalCommonMiiMdioFindingState: Timed Out looking for a Phy!\n");
      *PhyState|=PHY_RECK_TO;  /* Set Timer */
      }
    }
  }

void _cpswHalCommonMiiMdioFoundState(PHY_DEVICE *PhyDev)
  {
  bit32u *PhyState  = &PhyDev->PhyState;
  bit32u  PhyMask   = PhyDev->PhyMask;
  bit32u  MLinkMask = PhyDev->MLinkMask;
  bit32u  PhyNum,PhyStatus,NWAYadvertise,NWAY1000advertise=0,m,phynum,i,j,PhyAcks;
  bit32u  PhySel;

  if ((*PhyState&PHY_SMODE_MASK)==0) return;

  PhyNum=(*PhyState&PHY_DEV_MASK)>>PHY_DEV_OFFSET;

  PhyAcks=myMDIO_ALIVE;
  PhyAcks&=PhyMask;   /* Only interested in 'our' Phys */

  /* Will now isolate all our Phys, except the one we have decided to use */
  for(phynum=0,j=1;phynum<32;phynum++,j<<=1)
    {
    if (PhyAcks&j)
      {
        if (phynum!=PhyNum)  /* Do not disabled Found Phy */
          _cpswHalCommonMiiMdioDisablePhy(PhyDev,phynum);
      }
    }

  /*  Reset the Phy and proceed with auto-negotiation */
  _cpswHalCommonMiiMdioResetPhy(PhyDev,PhyNum);

  /* Now setup the MDIOUserPhySel register */

  PhySel=PhyNum;  /* Set the phy address */

  /*  Set the way Link will be Monitored */
  /* Check the Link Selection Method */
  if ((1 << PhyNum) & MLinkMask)
    PhySel |= MDIO_USERPHYSEL_LINKSEL;

  myMDIO_USERPHYSEL = PhySel;  /* update PHYSEL */

  /* Get the Phy Status */
  PhyStatus = _cpswHalCommonMiiMdioUserAccessRead(PhyDev, PHY_STATUS_REG, PhyNum);

  /*  For Phy Internal loopback test, need to wait until Phy
      found, then set Loopback */
  if (*PhyState&SMODE_LPBK)
    {
    /* Set Phy in Loopback */
    _cpswHalCommonMiiMdioUserAccessWrite(PhyDev, PHY_CONTROL_REG, PhyNum, MII_PHY_LOOP|MII_PHY_FD);
    /* Do a read to ensure PHY_LOOP has completed */
    _cpswHalCommonMiiMdioUserAccessRead(PhyDev, PHY_STATUS_REG, PhyNum);
    *PhyState=(*PhyState&~PHY_STATE_MASK)|(LOOPBACK);
    *PhyState|=PHY_CHANGE;
    return;
    }

  mdioPrintf("Enable Phy to negotiate external connection\n");

  NWAYadvertise=MII_NWAY_SEL;
  if (*PhyState&SMODE_FD100) NWAYadvertise|=MII_NWAY_FD100;
  if (*PhyState&SMODE_HD100) NWAYadvertise|=MII_NWAY_HD100;
  if (*PhyState&SMODE_FD10)  NWAYadvertise|=MII_NWAY_FD10;
  if (*PhyState&SMODE_HD10)  NWAYadvertise|=MII_NWAY_HD10;
  if (*PhyState&SMODE_FD1000) NWAY1000advertise|=MII_NWAY_MY_FD1000;

  *PhyState&=~(PHY_TIM_MASK|PHY_STATE_MASK);
  if ((PhyStatus&MII_NWAY_CAPABLE)&&(*PhyState&SMODE_AUTO))   /*NWAY Phy Detected*/
    {
    /*For NWAY compliant Phys                                                */
    _cpswHalCommonMiiMdioUserAccessWrite(PhyDev, NWAY_ADVERTIZE_REG, PhyNum, NWAYadvertise);
    
    /* for gig negotiation */
    _cpswHalCommonMiiMdioUserAccessWrite(PhyDev, NWAY_1000BT_ADVERTISE_REG, PhyNum, NWAY1000advertise);
    

    mdioPrintf("NWAY Advertising: ");
    if (*PhyState&SMODE_FD1000)       mdioPrintf("FullDuplex-1000 ");
    if (NWAYadvertise&MII_NWAY_FD100) mdioPrintf("FullDuplex-100 ");
    if (NWAYadvertise&MII_NWAY_HD100) mdioPrintf("HalfDuplex-100 ");
    if (NWAYadvertise&MII_NWAY_FD10)  mdioPrintf("FullDuplex-10 ");
    if (NWAYadvertise&MII_NWAY_HD10)  mdioPrintf("HalfDuplex-10 ");
    mdioPrintf("\n");

    _cpswHalCommonMiiMdioUserAccessWrite(PhyDev, PHY_CONTROL_REG, PhyNum, MII_AUTO_NEGOTIATE_EN);

    _cpswHalCommonMiiMdioUserAccessWrite(PhyDev, PHY_CONTROL_REG, PhyNum, MII_AUTO_NEGOTIATE_EN|MII_RENEGOTIATE);

    *PhyState|=PHY_CHANGE|PHY_NWST_TO|NWAY_START;
    }
   else
    {
    *PhyState&=~SMODE_AUTO;   /*The Phy is not capable of auto negotiation!  */
    m=NWAYadvertise;
    for(j=0x8000,i=0;(i<16)&&((j&m)==0);i++,j>>=1);
    m=j;
    j=0;
    if (m&(MII_NWAY_FD100|MII_NWAY_HD100))
      {
      j=MII_PHY_100;
      m&=(MII_NWAY_FD100|MII_NWAY_HD100);
      }
    if (m&(MII_NWAY_FD100|MII_NWAY_FD10))
      j |= MII_PHY_FD;

    mdioPrintf("Requested PHY mode %s Duplex %s Mbps\n",(j&MII_PHY_FD)?"Full":"Half",(j&MII_PHY_100)?"100":"10");

    _cpswHalCommonMiiMdioUserAccessWrite(PhyDev, PHY_CONTROL_REG, PhyNum, j);

    *PhyState&=~PHY_SPEED_MASK;
    if (j&MII_PHY_100)
      *PhyState|=(1<<PHY_SPEED_OFFSET);
    *PhyState&=~PHY_DUPLEX_MASK;
    if (j&MII_PHY_FD)
      *PhyState|=(1<<PHY_DUPLEX_OFFSET);
    *PhyState|=PHY_CHANGE|PHY_LINK_TO|LINK_WAIT;
    }
    _cpswHalCommonMiiMdioMdixDelay(PhyDev);  /* If AutoMdix add delay */
  }

static int _cpswHalIsAgerePhy(PHY_DEVICE *PhyDev,bit32u PhyNum)
{
    bit32u PhyId;

    PhyId =_cpswHalCommonMiiMdioUserAccessRead(PhyDev, PHY_IDENT_REG_1, PhyNum);
    if((PhyId & PHY_INDENT_REG_1_MASK) == 0x282)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


void _cpswHalCommonMiiMdioResetPhy(PHY_DEVICE *PhyDev,bit32u PhyNum)
{
  bit16u PhyControlReg;
  bit16u phytemp;

  _cpswHalCommonMiiMdioUserAccessWrite(PhyDev, PHY_CONTROL_REG, PhyNum, MII_PHY_RESET);

  mdioPrintf("cpswHalCommonMiiMdioPhYReset(%d)\n",PhyNum);

  /* Read control register until Phy Reset is complete */
  do
   {
        PhyControlReg = _cpswHalCommonMiiMdioUserAccessRead(PhyDev, PHY_CONTROL_REG, PhyNum);
   }
   while (PhyControlReg & MII_PHY_RESET); /* Wait for Reset to clear */

  /* Agere PHY related workarounds not necessary for all PHYS */  
  if(_cpswHalIsAgerePhy(PhyDev,PhyNum))
   {
      /* Set the PHY trim values for the Agere PHY as mentioned in 
       * Errata sheet of ET1011C.
       */
      _cpswHalCommonMiiMdioSetPhyTrim(PhyDev,PhyNum);
      phytemp = _cpswHalCommonMiiMdioUserAccessRead(PhyDev,PHY_CONFIG_REG,PhyNum);
      phytemp |= CRS_XMIT_ENABLE;
      _cpswHalCommonMiiMdioUserAccessWrite(PhyDev,PHY_CONFIG_REG,PhyNum,phytemp);
 
   }
}

void _cpswHalCommonMiiMdioNwayStartState(PHY_DEVICE *PhyDev)
  {
  bit32u *PhyState = &PhyDev->PhyState;
  bit32u PhyNum,PhyMode;

  PhyNum=(*PhyState&PHY_DEV_MASK)>>PHY_DEV_OFFSET;

  /*Wait for Negotiation to start                                            */

  PhyMode=_cpswHalCommonMiiMdioUserAccessRead(PhyDev, PHY_CONTROL_REG, PhyNum);

  if((PhyMode&MII_RENEGOTIATE)==0)
    {
    _cpswHalCommonMiiMdioUserAccessRead(PhyDev, PHY_STATUS_REG, PhyNum); /*Flush pending latch bits*/
    *PhyState&=~(PHY_STATE_MASK|PHY_TIM_MASK);
    *PhyState|=PHY_CHANGE|NWAY_WAIT|PHY_NWDN_TO;
    _cpswHalCommonMiiMdioMdixDelay(PhyDev);  /* If AutoMdix add delay */
    }
   else
    {
    if (*PhyState&PHY_TIM_MASK)
      *PhyState=(*PhyState&~PHY_TIM_MASK)|((*PhyState&PHY_TIM_MASK)-(1<<PHY_TIM_OFFSET));
     else
      _cpswHalCommonMiiMdioPhyTimeOut(PhyDev);
    }
  }

void _cpswHalCommonMiiMdioNwayWaitState(PHY_DEVICE *PhyDev)
  {
  bit32u *PhyState = &PhyDev->PhyState;
  bit32u  PhyNum,PhyStatus,NWAYadvertise,NWAYREadvertise,NegMode,i,j;
  bit32u NWAY1000advertise, NWAY1000REMadvertise;

  PhyNum=(*PhyState&PHY_DEV_MASK)>>PHY_DEV_OFFSET;

  PhyStatus=_cpswHalCommonMiiMdioUserAccessRead(PhyDev, PHY_STATUS_REG, PhyNum);

  if (PhyStatus&MII_NWAY_COMPLETE)
    {
    *PhyState|=PHY_CHANGE;
    *PhyState&=~PHY_SPEED_MASK;
    *PhyState&=~PHY_DUPLEX_MASK;

    NWAYadvertise   =_cpswHalCommonMiiMdioUserAccessRead(PhyDev, NWAY_ADVERTIZE_REG, PhyNum);
    NWAYREadvertise =_cpswHalCommonMiiMdioUserAccessRead(PhyDev, NWAY_REMADVERTISE_REG, PhyNum);

    /* read gig status */
    NWAY1000advertise   =_cpswHalCommonMiiMdioUserAccessRead(PhyDev, NWAY_1000BT_ADVERTISE_REG, PhyNum);
    NWAY1000REMadvertise =_cpswHalCommonMiiMdioUserAccessRead(PhyDev, NWAY_1000BT_REMADVERTISE_REG, PhyNum);
    

    /* figure out if gig connected at FD 1000 or not first */
    if ((NWAY1000advertise & MII_NWAY_MY_FD1000) && (NWAY1000REMadvertise & MII_NWAY_REM_FD1000))
      NegMode = MII_NWAY_MY_FD1000;
    else
      NegMode = 0;

    if (NegMode == 0)
      {
	/* continue checking for 100 and 10 connection */

	/* Negotiated mode is we and the remote have in common */
	NegMode = NWAYadvertise & NWAYREadvertise;

	mdioPrintf("Phy: %d, ",(*PhyState&PHY_DEV_MASK)>>PHY_DEV_OFFSET);
	mdioPrintf("NegMode %04X, NWAYadvertise %04X, NWAYREadvertise %04X\n",
		   NegMode,  NWAYadvertise, NWAYREadvertise);

	/* Limit negotiation to fields below */
	NegMode &= (MII_NWAY_FD100|MII_NWAY_HD100|MII_NWAY_FD10|MII_NWAY_HD10);
	
	if (NegMode==0)
	  {
	    NegMode=(MII_NWAY_HD100|MII_NWAY_HD10)&NWAYadvertise; /*or 10 ?? who knows, Phy is not MII compliant*/
	    
	    mdioPrintf("Mdio:WARNING: Negotiation complete but NO agreement, default is HD\n");
#ifdef __CPHAL_DEBUG
	    _cpswHalCommonMiiMdioDumpPhyDetailed(PhyDev);
#endif
	  }
	for(j=0x8000,i=0;(i<16)&&((j&NegMode)==0);i++,j>>=1);


	NegMode=j;

	mdioPrintf("Negotiated connection: ");
	if (NegMode&MII_NWAY_FD100) mdioPrintf("FullDuplex 100 Mbs\n");
	if (NegMode&MII_NWAY_HD100) mdioPrintf("HalfDuplex 100 Mbs\n");
	if (NegMode&MII_NWAY_FD10) mdioPrintf("FullDuplex 10 Mbs\n");
	if (NegMode&MII_NWAY_HD10) mdioPrintf("HalfDuplex 10 Mbs\n");

	PhyDev->SPEED_1000 = 0;
      }
    else
      {
	/* found 1000 negotiated connection! */
	mdioPrintf("Negotiated connection: ");
	mdioPrintf("FullDuplex 1000 Mbs\n");

	PhyDev->SPEED_1000 = 1;
      }

    if (NegMode!=0)
      {
      if (PhyStatus&MII_PHY_LINKED)
        *PhyState=(*PhyState&~PHY_STATE_MASK)|LINKED;
       else
        *PhyState=(*PhyState&~PHY_STATE_MASK)|LINK_WAIT;
      if (NegMode&(MII_NWAY_FD100|MII_NWAY_HD100))
        *PhyState=(*PhyState&~PHY_SPEED_MASK)|(1<<PHY_SPEED_OFFSET);
      if (NegMode&(MII_NWAY_FD100|MII_NWAY_FD10|MII_NWAY_MY_FD1000))
        *PhyState=(*PhyState&~PHY_DUPLEX_MASK)|(1<<PHY_DUPLEX_OFFSET);
      }
    }
   else
    {
    if (*PhyState&PHY_TIM_MASK)
      *PhyState=(*PhyState&~PHY_TIM_MASK)|((*PhyState&PHY_TIM_MASK)-(1<<PHY_TIM_OFFSET));
     else
      _cpswHalCommonMiiMdioPhyTimeOut(PhyDev);
    }
  }

void _cpswHalCommonMiiMdioLinkWaitState(PHY_DEVICE *PhyDev)
  {
  bit32u *PhyState = &PhyDev->PhyState;
  bit32u  PhyStatus;
  bit32u  PhyNum;

  PhyNum=(*PhyState&PHY_DEV_MASK)>>PHY_DEV_OFFSET;

  PhyStatus=_cpswHalCommonMiiMdioUserAccessRead(PhyDev, PHY_STATUS_REG, PhyNum);

  if (PhyStatus&MII_PHY_LINKED)
    {
    *PhyState=(*PhyState&~PHY_STATE_MASK)|LINKED;
    *PhyState|=PHY_CHANGE;
    }
   else
    {
    if (*PhyState&PHY_TIM_MASK)
      *PhyState=(*PhyState&~PHY_TIM_MASK)|((*PhyState&PHY_TIM_MASK)-(1<<PHY_TIM_OFFSET));
     else
      _cpswHalCommonMiiMdioPhyTimeOut(PhyDev);
    }
  }

void _cpswHalCommonMiiMdioPhyTimeOut(PHY_DEVICE *PhyDev)
  {
  bit32u *PhyState;

  if(_cpswHalCommonMiiMdioMdixSupported(PhyDev) == 0)
    return;  /* AutoMdix not supported */

  PhyState = &PhyDev->PhyState;

  /*  Indicate MDI/MDIX mode switch is needed */
  *PhyState|=PHY_MDIX_SWITCH;

  /* Toggle the MDIX mode indicatir */
  if(*PhyState & PHY_MDIX)
    *PhyState &= ~PHY_MDIX_MASK;       /* Current State is MDIX, set to MDI */
  else
    *PhyState |=  PHY_MDIX_MASK;      /* Current State is MDI, set to MDIX */

  /* Reset state machine to FOUND */
  *PhyState=(*PhyState&~PHY_STATE_MASK)|(FOUND);
  }


void _cpswHalCommonMiiMdioLoopbackState(PHY_DEVICE *PhyDev)
  {
  return;
  }

void _cpswHalCommonMiiMdioLinkedState(PHY_DEVICE *PhyDev)
  {
  bit32u *PhyState = &PhyDev->PhyState;
  bit32u  PhyNum   = (*PhyState&PHY_DEV_MASK)>>PHY_DEV_OFFSET;

  if (myMDIO_LINK&(1<<PhyNum)) return;  /* if still Linked, exit*/

  /* Not Linked */
  *PhyState&=~(PHY_STATE_MASK|PHY_TIM_MASK);
  if (*PhyState&SMODE_AUTO)
    *PhyState|=PHY_CHANGE|NWAY_WAIT|PHY_NWDN_TO;
   else
    *PhyState|=PHY_CHANGE|PHY_LINK_TO|LINK_WAIT;

  _cpswHalCommonMiiMdioMdixDelay(PhyDev);  /* If AutoMdix add delay */
  }

void _cpswHalCommonMiiMdioDefaultState(PHY_DEVICE *PhyDev)
  {
  bit32u *PhyState = &PhyDev->PhyState;
  /*Awaiting a cpswHalCommonMiiMdioInit call                                             */
  *PhyState|=PHY_CHANGE;
  }

#ifdef __CPHAL_DEBUG
void _cpswHalCommonMiiMdioDumpPhy(PHY_DEVICE *PhyDev, bit32u p)
  {
  bit32u j,n,PhyAcks;
  bit32u PhyRegAddr;
  bit32u phy_num;
  bit32u PhyMask  = PhyDev->PhyMask;

  PhyAcks=myMDIO_ALIVE;
  PhyAcks&=PhyMask;   /* Only interested in 'our' Phys */

  for(phy_num=0,j=1;phy_num<32;phy_num++,j<<=1)
    {
    if (PhyAcks&j)
      {
      mdioPrintf("%2d%s:",phy_num,(phy_num==p)?">":" ");
      for(PhyRegAddr=0;PhyRegAddr<6;PhyRegAddr++)
        {
        n = _cpswHalCommonMiiMdioUserAccessRead(PhyDev, PhyRegAddr, phy_num);
        mdioPrintf(" %04x",n&0x0ffff);
        }
      mdioPrintf("\n");
      }
    }
  _cpswHalCommonMiiMdioDumpPhyDetailed(PhyDev);
  }

void _cpswHalCommonMiiMdioDumpPhyDetailed(PHY_DEVICE *PhyDev)
{
  bit32u *PhyState = &PhyDev->PhyState;
  bit32u  PhyNum;
  int     RegData, RegData2;

  PhyNum=(*PhyState&PHY_DEV_MASK)>>PHY_DEV_OFFSET;

  RegData = _cpswHalCommonMiiMdioUserAccessRead(PhyDev, 0, PhyNum);
  mdioPrintf("PhyControl: %04X, Lookback=%s, Speed=%s, Duplex=%s\n",
    RegData,
    RegData&MII_PHY_LOOP?"On":"Off",
    RegData&MII_PHY_1000?"1000":(RegData&MII_PHY_100?"100":"10"),
    RegData&MII_PHY_FD?"Full":"Half");
  RegData = _cpswHalCommonMiiMdioUserAccessRead(PhyDev, 1, PhyNum);
  mdioPrintf("PhyStatus: %04X, AutoNeg=%s, Link=%s\n",
    RegData,
    RegData&MII_NWAY_COMPLETE?"Complete":"NotComplete",
    RegData&MII_PHY_LINKED?"Up":"Down");
  RegData = _cpswHalCommonMiiMdioUserAccessRead(PhyDev, 4, PhyNum);
  RegData2 = _cpswHalCommonMiiMdioUserAccessRead(PhyDev, 9, PhyNum);
  mdioPrintf("PhyMyCapability: %04X, 1000FD=%s, 100FD=%s, 100HD=%s, 10FD=%s, 10HD=%s\n",
    RegData,
    RegData2&MII_NWAY_MY_FD1000?"Yes":"No",
    RegData&MII_NWAY_FD100?"Yes":"No",
    RegData&MII_NWAY_HD100?"Yes":"No",
    RegData&MII_NWAY_FD10?"Yes":"No",
    RegData&MII_NWAY_HD10?"Yes":"No");

  RegData = _cpswHalCommonMiiMdioUserAccessRead(PhyDev, 5, PhyNum);
  RegData2 = _cpswHalCommonMiiMdioUserAccessRead(PhyDev, 10, PhyNum);
  mdioPrintf("PhyPartnerCapability: %04X, 1000FD=%s, 100FD=%s, 100HD=%s, 10FD=%s, 10HD=%s\n",
    RegData,
    RegData2&MII_NWAY_REM_FD1000?"Yes":"No",
    RegData&MII_NWAY_FD100?"Yes":"No",
    RegData&MII_NWAY_HD100?"Yes":"No",
    RegData&MII_NWAY_FD10?"Yes":"No",
    RegData&MII_NWAY_HD10?"Yes":"No");
}
#endif

/* This function is PHY specific needs modification for PHY other than ET1011C Agere PHY */
void _cpswHalCommonMiiMdioWriteEphyTrimRegs(PHY_DEVICE *PhyDev,bit32u MiiPhyNum, bit32u TestCfgRegNum, bit32u CfgValue)
{
	bit16u reg0 ;
	bit16u reg1;
	
	/* Put the PHY in low powerdown mode */
	reg0 = _cpswHalCommonMiiMdioUserAccessRead(PhyDev,PHY_CONTROL_REG ,MiiPhyNum );
	reg0 |= MII_PHY_PDOWN;
	_cpswHalCommonMiiMdioUserAccessWrite(PhyDev,PHY_CONTROL_REG,MiiPhyNum,reg0);
	
	/* Enable the diagnostic mode */
	reg1 = _cpswHalCommonMiiMdioUserAccessRead(PhyDev,DIAGNOSTIC_MODE_REG ,MiiPhyNum );
	reg1 |= DIAGNOSTIC_ENABLE ;
	_cpswHalCommonMiiMdioUserAccessWrite(PhyDev,DIAGNOSTIC_MODE_REG,MiiPhyNum,reg1);
	
	/* Now program the Phytrim registers according to mentioned in 
	 * errata sheet.  we Dont know the root cause just give hardcoded
	 * values as specified.
	 */
	 /* For 1000Mbps */
	 _cpswHalCommonMiiMdioUserAccessWrite(PhyDev,16,MiiPhyNum,0x8805);
	 _cpswHalCommonMiiMdioUserAccessWrite(PhyDev,17,MiiPhyNum,0xF03E);
	 
	 /* For 100Mbps */
	 _cpswHalCommonMiiMdioUserAccessWrite(PhyDev,16,MiiPhyNum,0x8806);
	 _cpswHalCommonMiiMdioUserAccessWrite(PhyDev,17,MiiPhyNum,0x003E);
	 
	 /* For 10Mbps */
	 _cpswHalCommonMiiMdioUserAccessWrite(PhyDev,16,MiiPhyNum,0x8807);
	 _cpswHalCommonMiiMdioUserAccessWrite(PhyDev,17,MiiPhyNum,0x1F00);
	 
	 /* Disable the Diagnostic mode */
	 reg1 = _cpswHalCommonMiiMdioUserAccessRead(PhyDev,DIAGNOSTIC_MODE_REG ,MiiPhyNum );
	 reg1 &= (~DIAGNOSTIC_ENABLE);
	 _cpswHalCommonMiiMdioUserAccessWrite(PhyDev,DIAGNOSTIC_MODE_REG,MiiPhyNum,reg1);
	
	 /* Put the PHY back in normal mode */
	 reg0 = _cpswHalCommonMiiMdioUserAccessRead(PhyDev,PHY_CONTROL_REG ,MiiPhyNum );
	 reg0 &= (~MII_PHY_PDOWN);
	 _cpswHalCommonMiiMdioUserAccessWrite(PhyDev,PHY_CONTROL_REG,MiiPhyNum,reg0);
	 
	
#if 0
   /* This sequence will change the PHY to test mode */
   _cpswHalCommonMiiMdioUserAccessWrite(PhyDev,20,MiiPhyNum,0x0000);
   _cpswHalCommonMiiMdioUserAccessWrite(PhyDev,20,MiiPhyNum,0x0400);
   _cpswHalCommonMiiMdioUserAccessWrite(PhyDev,20,MiiPhyNum,0x0000);
   _cpswHalCommonMiiMdioUserAccessWrite(PhyDev,20,MiiPhyNum,0x0400);
 
   /* Write register value */
   _cpswHalCommonMiiMdioUserAccessWrite(PhyDev,23,MiiPhyNum,(0x8100 | (CfgValue & 0xFF)));
   _cpswHalCommonMiiMdioUserAccessWrite(PhyDev,20,MiiPhyNum,(0x4400 | (TestCfgRegNum & 0xFF)));
 
   /* Back to normal mode */
   _cpswHalCommonMiiMdioUserAccessWrite(PhyDev,20,MiiPhyNum,0x0000);
#endif   
}

void _cpswHalCommonMiiMdioSetPhyTrim(PHY_DEVICE *PhyDev,bit32u PhyNum)
{
   
   /*Set Phy Trim for Agere ET1011C Gig Phy(external)*/   
   if(PhyNum != 1)
     return;  
   
   _cpswHalCommonMiiMdioWriteEphyTrimRegs(PhyDev,PhyNum,0,0);
 
}



/************************************
***
*** Waits for MDIO_USERACCESS to be ready and reads data
*** If 'WaitForData' set, waits for read to complete and returns Data,
*** otherwise returns 0
*** Note: 'data' is 16 bits but we use 32 bits
***        to be consistent with rest of the code.
***
**************************************/
bit32u _cpswHalCommonMiiMdioUserAccessRead(PHY_DEVICE *PhyDev, bit32u regadr, bit32u phyadr)
  {

  _cpswHalCommonMiiMdioWaitForAccessComplete(PhyDev);  /* Wait until UserAccess ready */
  _cpswHalCommonMiiMdioUserAccess(PhyDev, MDIO_USERACCESS_READ, regadr, phyadr, 0);
  _cpswHalCommonMiiMdioWaitForAccessComplete(PhyDev);  /* Wait for Read to complete */

  return(myMDIO_USERACCESS & MDIO_USERACCESS_DATA);
  }


/************************************
***
*** Waits for MDIO_USERACCESS to be ready and writes data
***
**************************************/
void _cpswHalCommonMiiMdioUserAccessWrite(PHY_DEVICE *PhyDev, bit32u regadr, bit32u phyadr, bit32u data)
  {
  _cpswHalCommonMiiMdioWaitForAccessComplete(PhyDev);  /* Wait until UserAccess ready */
  _cpswHalCommonMiiMdioUserAccess(PhyDev, MDIO_USERACCESS_WRITE, regadr, phyadr, data);
  }

void _cpswHalCommonMiiMdioWaitForAccessComplete(PHY_DEVICE *PhyDev)
  {
  while((myMDIO_USERACCESS & MDIO_USERACCESS_GO)!=0)
    {
    }
  }

void _cpswHalCommonMiiMdioUserAccess(PHY_DEVICE *PhyDev, bit32u method, bit32u regadr, bit32u phyadr, bit32u data)
  {
  bit32u  control;

  control =  MDIO_USERACCESS_GO |
             (method) |
             (((regadr) << 21) & MDIO_USERACCESS_REGADR) |
             (((phyadr) << 16) & MDIO_USERACCESS_PHYADR) |
             ((data) & MDIO_USERACCESS_DATA);

  myMDIO_USERACCESS = control;
  }


