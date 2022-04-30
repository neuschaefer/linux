/*
===============================================================================
                Copyright (C) 2008-2009 by STMicroelectronics.
                              All rights reserved.

FILE:        sound/oss/stw5094a.c
VERSION:     1.0
UPDATED:     20/04/2009
AUTHOR(S):   STM, Computer Systems Division, Beijing/Italy VoIP Team
DESCRIPTION:
Audio driver for IP phones based on SPEAr Basic SoC and STw5094A codec.
Designed to be used as dynamically loaded kernel module (LKM).
Compliant to specific OSS API subset, exposed through /dev/dsp
and /dev/mixer logical devices.

general Kconfig options:

CONFIG_SOUND_OSS required in kernel configuration file
to enable OSS subsystem.

Changes are required to default sound/oss/Makefile for building.
The following line must be appended to the list of existing drivers:

obj-$(CONFIG_SOUND_SPR_IPHONE)	+= stw5094a.o
===============================================================================
*/

#include "stw5094a.h"

#define ST5094_TRACE(x...) 
//#define ST5094_TRACE(x...) printk(x)
//#define ST5094_ERROR(x...)
#define ST5094_ERROR(x...) printk(x)

#define CLEARTDMIRQ {  volatile unsigned char dummy; dummy = *VA_ITtdm; }

//#define MANUAL_BB_MANAG
#define CHECK_DSPWRITES
#define DEFAULT_TDMOPEN_30MSEC

#define NUM_IGNORED_TDM_IRQ 1

#define SPIRIT_DBG

static struct dev_state stw5094a_dev_state;

volatile unsigned int irc_counter = 0;
static wait_queue_head_t rd_wq;
static bool rd_data_ready;
static int mic_gains[3] = {0x9, 0xF, 0x9};
static int output_gains[3] = {0x0, 0x0, 0x0};

#ifdef DEFAULT_TDMOPEN_30MSEC
	static unsigned int current_datasize = 480;
#else
	static unsigned int current_datasize = 160;
#endif
static int changeTDMBufferLenght = 0;
static int closingDSP = 0;

//enable output only on first dsp_write()
//if firstWrite == 0 normal operation
//if firstWrite == 1 mute
//if firstWrite == 2 skipping first write 
static int firstWrite = 1;

static unsigned int current_bufferbank = 0;
static unsigned int rt_violations = 0;

extern unsigned int total_interrupts;
extern unsigned int tdm_interrupts;
extern volatile char *TDM_CLEAR_REGISTER;
extern unsigned int TDM_DEBUG_MSG_LEVEL;

/******************************************************************************
                             I2C Driver functions
******************************************************************************/
extern void SPEAR_I2C_GetAccess(unsigned char Address);
extern void SPEAR_I2C_ReleaseAccess(void);
extern int SPEAR_I2C_Write8(unsigned int subaddress, unsigned char value);
extern int SPEAR_I2C_Read8(unsigned int subaddress);



/******************************************************************************
                             FORWARD DECLARATIONS
******************************************************************************/

static void set_narrowband(void);
static void set_wideband(void);
static void tdm_init(void);
static void tdm_open(void);
irqreturn_t tdm_interrupt(int irq,void *dev_id,struct pt_regs *regs);
static void tdm_exit(void);


static void update_datasize(void)
{
	int samples = 0;
	
	switch(stw5094a_dev_state.buf_len)
	{
		case BL_10ms:
			samples = 80;
		break;
			
		case BL_20ms:
			samples = 160;				
		break;
		
		case BL_30ms:
			samples = 240;
		break;
		default:
			samples = 80;
		break;
	}
	
	switch(stw5094a_dev_state.rate)
	{
		case RATE_NARROWBAND:
			current_datasize = samples * 2;
		break;
			
		case RATE_WIDEBAND:
			current_datasize = samples * 4;		
		break;
			
		default:
			current_datasize = samples * 2;
		break;
	}	
}

/******************************************************************************
                            DSP LOGICAL DEVICE
******************************************************************************/

/*
-------------------------------------------------------------------------------
PRIVATE: dsp_open

Invoked when the logical "DSP" device is opened.
The device is opened in narrowband mode and handset mode.
Use relevant IOCTLs to select wideband mode and/or handsfree/headset modes.
-------------------------------------------------------------------------------
*/

static int dsp_open(struct inode *inode, struct file *file)
{
	firstWrite = 1;
    closingDSP = 0;
    
    ST5094_TRACE("dsp_open\n");
    // initial configuration is narrowband, handset mode, 10ms buffer
    
    stw5094a_dev_state.mode = MODE_HANDSET;
    stw5094a_dev_state.rate = RATE_NARROWBAND;
#ifdef DEFAULT_TDMOPEN_30MSEC
  	stw5094a_dev_state.buf_len = BL_30ms;
#else
   	stw5094a_dev_state.buf_len = BL_10ms;
#endif
    changeTDMBufferLenght = 0;
    update_datasize();
    
    irc_counter = 0;
    total_interrupts = 0;
    tdm_interrupts = 0;
    rd_data_ready = false;
    init_waitqueue_head(&rd_wq);
        
    mic_gains[0] = 0x9;
    mic_gains[1] = 0xF;
    mic_gains[2] = 0x9;
    
    SPEAR_I2C_GetAccess(0x71);	
    if(stw5094a_dev_state.rate == RATE_NARROWBAND)
    {
    	SPEAR_I2C_Write8(0,0x00); // 512 KHz clock, 8 KHz sampling, linear PCM, two's complement
    }
    else
    {
    	SPEAR_I2C_Write8(0,0x20); // 512 KHz clock, 16 KHz sampling, linear PCM, two's complement
    }
    SPEAR_I2C_Write8(1,0x84); // non delayed normal data timing, PCM I/F enabled    
    SPEAR_I2C_Write8(4,0x60 |(mic_gains[stw5094a_dev_state.mode] & 0x0F) );	// MIC1, MBIAS enabled, 20dB preamp. gain, default transmit amp. gain
    SPEAR_I2C_Write8(5,0x0F);  // HPF enabled for 8 KHZ, sidetone disabled
    SPEAR_I2C_Write8(6,0x2A);  // muted output deriver operative, Ring/Tone to LS or HP disabled, Voice signal to LS or HP enabled
    SPEAR_I2C_Write8(7,0x00);  // loudspeaker gain, initially 6dB
    output_gains[1] = 0x0;
    SPEAR_I2C_Write8(8,0x00); // handset gain, initially 0dB
    output_gains[0] = 0x0;
    SPEAR_I2C_Write8(9,0x00); // headset gain, initially 0dB
    output_gains[2] = 0x0;
    SPEAR_I2C_Write8(18,0xE0);//(to be adjusted according mike)
    SPEAR_I2C_ReleaseAccess();
    
    // initialize TDM controller for STw5094a interfacing
   	tdm_open();
   
    return 0;
}

/*
-------------------------------------------------------------------------------
PRIVATE: dsp_write

Get audio data from passed user space buffer and copy them to TDM HW buffer.
The "count" argument must match the size of current mode
(e.g. for 10 milliseconds):
narrowband = 160 (bytes)
wideband   = 320 (bytes)
-------------------------------------------------------------------------------
*/
static ssize_t dsp_write (struct file *file,const char __user  *buffer,size_t count,loff_t *ppos)
{
    ssize_t result;
#ifdef CHECK_DSPWRITES
	char checkbuf[current_datasize];
#endif

    ST5094_TRACE("dsp_write\n");
    if (count != current_datasize) { result = -1; goto wr_exit; }

#ifdef MANUAL_BB_MANAG
    if(*TDM_SYNC_GEN & 0x40000000)//BB bit
    {
    	if(copy_from_user((char *)TDM_BUFFER,buffer,count))
		{
			return -EFAULT;
		}
    }
    else
    {
        if(copy_from_user((char *)TDM_BUFFER1,buffer,count))
        {
			return -EFAULT;
		}
    }
#else
    if(copy_from_user((char *)((int)TDM_BUFFER),buffer,count))
    {
		return -EFAULT;
	}    
#ifdef CHECK_DSPWRITES
	if(copy_from_user(checkbuf,buffer,count))
    {
		return -EFAULT;
	}    
	if(memcmp((char *)TDM_BUFFER, checkbuf, count) != 0 )
	{
		ST5094_ERROR("ERROR! Write check failed\n");
	}
#endif
#endif

    result = count;

    //check for Real Time violations
	if ((current_bufferbank & 0x1 )!= (unsigned int)(((*TDM_SYNC_GEN & 0x40000000) >> 30) & 0x1) )
	{
		rt_violations++;
	}

    if(firstWrite == 1)
    {
        ST5094_TRACE("De-Muting\n");
        firstWrite = 0;
        SPEAR_I2C_GetAccess(0x71);
        switch(stw5094a_dev_state.mode)
        {
        	case MODE_HANDSET:
        		SPEAR_I2C_Write8(6,0x0A);
        	break;
        	case MODE_HANDSFREE:
        		SPEAR_I2C_Write8(6,0x12);
        	break;
        	case MODE_HEADSET:
        		SPEAR_I2C_Write8(6,0x06);
        	break;
        	case MODE_HANDSET_LS:
        		SPEAR_I2C_Write8(6,0x1A);
        	break;
        	case MODE_HEADSET_LS:
        		SPEAR_I2C_Write8(6,0x16);
        	break;
        	
        	
        	
        	
        }
        SPEAR_I2C_ReleaseAccess();
    }
wr_exit:
    return result;
}

/*
-------------------------------------------------------------------------------
PRIVATE: dsp_read

Get audio data from TDM HW buffer and copy them to user space buffer.
The "count" argument must match the size of current mode:
 (e.g. for 10 milliseconds buffer)
narrowband = 160 (bytes)
wideband   = 320 (bytes)
-------------------------------------------------------------------------------
*/
static ssize_t dsp_read (struct file *file,char *buffer,size_t count,loff_t *ppos)
{
    ssize_t result;

	if (count != current_datasize) { result = -1; goto rd_exit; }
    wait_event_interruptible(rd_wq, rd_data_ready);

	if((*TDM_SYNC_GEN & 0x40000000) == 0)
	{
		current_bufferbank = 0;
	}
	else
	{
		current_bufferbank = 1;
	}

#ifdef MANUAL_BB_MANAG
    if(*TDM_SYNC_GEN & 0x40000000)//BB bit
    {
    	if(copy_to_user(buffer,(char *)TDM_BUFFER,count);)
		{
			return -EFAULT;
		}
    }
    else
    {
        if(copy_to_user(buffer,(char *)TDM_BUFFER1,count))
        {
			return -EFAULT;
		}
    }
#else
    if(copy_to_user(buffer,(char *)((int) TDM_BUFFER),count))
    {
        return -EFAULT;
    }

#endif

    rd_data_ready = false; // inform IRQ handler that read is completed
    result = count;

rd_exit:
    return result;
}

/*
-------------------------------------------------------------------------------
PRIVATE: dsp_ioctl

Execute specified "cmd" using "arg" if applicable.
-------------------------------------------------------------------------------
*/

static int dsp_ioctl(struct inode *inode,struct file *file,unsigned int cmd,u_long arg)
{
    int result;
    void __user *argp = (void __user *)arg;
    int __user *p = argp;
    int val;

    ST5094_TRACE("dsp_ioctl:%x\n",cmd);

    switch (cmd)
    {
	    case SNDCTL_DSP_CHANNELS:	// must be 1, only monophonic mode is supported
    		if (get_user(val,p))
	        {
			    result= -EFAULT;
			    break;
		    }
    		val = 1;
    		result = put_user(val,p);
    		break;

	    case SNDCTL_DSP_GETCAPS:	// only DSP_CAP_DUPLEX is supported
            result = put_user(DSP_CAP_DUPLEX, p);
    		break;		    

        case SNDCTL_DSP_SETDUPLEX:	// always full-duplex mode
    		result = 0;
    		break;

        case SNDCTL_DSP_SETFMT:	// only AFMT_S16_LE (LPCM,16-bit,two's complement,little-endian)
    		if (get_user(val, p)) 
    		{
    		    return -EFAULT;
    		}
    		val = AFMT_S16_LE;
    		result =put_user(val, p);
    		break;

        case SNDCTL_DSP_SPEED:		// only 8000 (8 KHz) and 16000 (16 KHz) are supported
    		if (get_user(val, p))
            {
                result = -EFAULT;
                break;
            }
    		if (val == 8000)
            {
                set_narrowband();
                result = put_user(val,p);
                break;
            }
            else if (val == 16000)
            {
                set_wideband();
                result = put_user(val,p);
                break;
            }
            else
            {
                result = -EFAULT;
                break;
            }

        case SNDCTL_DSP_SETFRAGMENT://sets the TDM buffer lenght for 10/20/30 ms samples
            if (get_user(val, p))
            {
            	result = -EFAULT;
            	break;
            }
            switch(val)
            {
            	case 10:
            		//*TDM_FRAME_NBR = 80;//80 samples every 10 milliseconds
            		stw5094a_dev_state.buf_len = BL_10ms;
            	break;
            	case 20:
            		//*TDM_FRAME_NBR = 160;//80 samples every 10 milliseconds
            		stw5094a_dev_state.buf_len = BL_20ms;
            	break;
            	case 30:
            		//*TDM_FRAME_NBR = 240;//80 samples every 10 milliseconds
            		stw5094a_dev_state.buf_len = BL_30ms;
            	break;
            	default:
            		result = -EFAULT;
            	break;
            }
            update_datasize();
            changeTDMBufferLenght = 1;//to change frame number inside irq routine
            result = 0;
            break;
	  
    	case SNDCTL_DSP_GET_RTIME_VIOLATIONS:
    		result = put_user(rt_violations, p);
    		rt_violations = 0;
    		break;

    	case SNDCTL_DSP_READREG:
	        if (get_user(val, p))
            {
        	    result = -EFAULT;
        	    break;
            }    	        
	        SPEAR_I2C_GetAccess(0x71);
            result = put_user(SPEAR_I2C_Read8(val), p);
            SPEAR_I2C_ReleaseAccess();
    		break;

    	case SNDCTL_DSP_WRITEREG:
	        if (get_user(val, p))
            {
        	    result = -EFAULT;
        	    break;
            }    	        
	        SPEAR_I2C_GetAccess(0x71);
            SPEAR_I2C_Write8(val>>8, val & 0xFF);
            SPEAR_I2C_ReleaseAccess();
            result = 0;
     		break;
    	  
    	default:
    		result = -EINVAL;
    		break;
	}
    return result;
}

/*
-------------------------------------------------------------------------------
PRIVATE: dsp_close

Invoked when the logical device is closed.
-------------------------------------------------------------------------------
*/
static int dsp_close(struct inode *inode, struct file *file)
{
    ST5094_TRACE("dsp_close\n");
	closingDSP = 1;
    firstWrite = 1;
    SPEAR_I2C_GetAccess(0x71);
    SPEAR_I2C_Write8(6, 0x2A);//muted, handset
    SPEAR_I2C_ReleaseAccess();
    
    ST5094_TRACE("Total handler number: %u\n", irc_counter);
    ST5094_TRACE("Total interrupt number: %u\n", total_interrupts);
    ST5094_TRACE("Total TDM interrupt number: %u\n", tdm_interrupts);
    return 0;
}

/*
-------------------------------------------------------------------------------
PRIVATE: set_narrowband

Switch mode to narrowband
-------------------------------------------------------------------------------
*/
static void set_narrowband(void)
{
    ST5094_TRACE("set_narrowband\n");
	*TDM_CONF = 0x02180005;// stops any clock before changes
	SPEAR_I2C_GetAccess(0x71);
    SPEAR_I2C_Write8(0,0); // 8 KHz sampling, 512 KHz clock
    SPEAR_I2C_ReleaseAccess();

    // change TDM
    *(TDM_ACTION_MEMORY + 0) = 0x01600015;
    *(TDM_ACTION_MEMORY + 1) = 0x01200015;
    *(TDM_ACTION_MEMORY + 2) = 0x00000000;
    *(TDM_ACTION_MEMORY + 3) = 0x00000000;
    *(TDM_ACTION_MEMORY + 4) = 0x00000000;
    *(TDM_ACTION_MEMORY + 5) = 0x00000000;
    *(TDM_ACTION_MEMORY + 6) = 0x00000000;
    *(TDM_ACTION_MEMORY + 7) = 0x00000000;
#ifdef MANUAL_BB_MANAG
	*TDM_SYNC_GEN = 0x03000005;
#else
	*TDM_SYNC_GEN = 0x13000005;
#endif
    *TDM_CONF = 0x06180005;  // restore clock

    stw5094a_dev_state.rate = RATE_NARROWBAND;
    update_datasize();
}

/*
-------------------------------------------------------------------------------
PRIVATE: set_wideband

Switch mode to wideband
-------------------------------------------------------------------------------
*/

static void set_wideband(void)
{
    ST5094_TRACE("set_wideband\n");
	*TDM_CONF = 0x02180005;// stops any clock before changes
	SPEAR_I2C_GetAccess(0x71);
    SPEAR_I2C_Write8(0,0x20); // change codec : 16 KHz sampling, 512 KHz clock
    SPEAR_I2C_ReleaseAccess();
    
    // change TDM
    *(TDM_ACTION_MEMORY + 0) = 0x01700015;
    *(TDM_ACTION_MEMORY + 1) = 0x01300015;
    *(TDM_ACTION_MEMORY + 2) = 0x00000000;
    *(TDM_ACTION_MEMORY + 3) = 0x00000000;
    *(TDM_ACTION_MEMORY + 4) = 0x01F00015;
    *(TDM_ACTION_MEMORY + 5) = 0x01B00015;
    *(TDM_ACTION_MEMORY + 6) = 0x00000000;
    *(TDM_ACTION_MEMORY + 7) = 0x00000000;

#ifdef MANUAL_BB_MANAG
	 *TDM_SYNC_GEN = 0x0300000D;
#else
	 *TDM_SYNC_GEN = 0x1300000D;
#endif
    *TDM_CONF = 0x06180005;  // restore clock

    stw5094a_dev_state.rate = RATE_WIDEBAND;
    update_datasize();
}

/******************************************************************************
                           MIXER LOGICAL DEVICE
******************************************************************************/

/*
-------------------------------------------------------------------------------
PRIVATE: mixer_ioctl

Execute specified "cmd" using "arg" if applicable.
-------------------------------------------------------------------------------
*/

static int mixer_ioctl(struct inode *inode,struct file *file,unsigned int cmd, u_long arg)
{
    void __user *argp = (void __user *)arg;
    int __user *p = argp;
    int val;
    int result;

    ST5094_TRACE("mixer_ioctl:%x\n",cmd);

    switch (cmd)
    {
    	case SOUND_MIXER_WRITE_RECSRC: // change mode
#ifdef SPIRIT_DBG
    	    ST5094_TRACE("Mixer IOCTL: Setting operation mode\n");
#endif
    		get_user(val,p);
	        SPEAR_I2C_GetAccess(0x71);
    		switch(val)
    		{
                case 0: // HANDSET
            		if(firstWrite == 1)
            		{
        				SPEAR_I2C_Write8(6,0x2A);//muted
        			}
        			else
        			{
        			    SPEAR_I2C_Write8(6,0x0A);
        			}
                    SPEAR_I2C_Write8(4,0x60 | (mic_gains[0] & 0xF));	// MIC1, MBIAS enabled, 20dB preamp. gain, 9db transmit amp. gain
        			stw5094a_dev_state.mode = MODE_HANDSET;
#ifdef SPIRIT_DBG
            	    ST5094_TRACE("Handset mode (0)\n");
#endif
        		    break;
        
        		case 1: // HANDSFREE
                    if(firstWrite == 1)
                    {
        				SPEAR_I2C_Write8(6,0x32);//muted
        			}
        			else
        			{
        			    SPEAR_I2C_Write8(6,0x12);
        			}
                    SPEAR_I2C_Write8(4,0xA0 | (mic_gains[1] & 0xF));	// MIC2, MBIAS enabled, 20dB preamp. gain, 9db transmit amp. gain
        			stw5094a_dev_state.mode = MODE_HANDSFREE;
#ifdef SPIRIT_DBG
        		    ST5094_TRACE("Handsfree mode (1)\n");
#endif
        			break;
        
                case 2: // HEADSET
        			if(firstWrite == 1)
        			{
        				SPEAR_I2C_Write8(6,0x26);//muted
        			}
        			else
        			{
        				SPEAR_I2C_Write8(6,0x06);
        			}
                    SPEAR_I2C_Write8(4,0xE0 | (mic_gains[2] & 0xF)); // MIC3
        			stw5094a_dev_state.mode = MODE_HEADSET;
#ifdef SPIRIT_DBG
        		    ST5094_TRACE("Headset mode (2)\n");
#endif
                    break;
                    
                case 3: // HANDSET_LS
            		if(firstWrite == 1)
            		{
        				SPEAR_I2C_Write8(6,0x3A);//muted
        			}
        			else
        			{
        			    SPEAR_I2C_Write8(6,0x1A);
        			}
                    SPEAR_I2C_Write8(4,0x60 | (mic_gains[0] & 0xF));	// MIC1, MBIAS enabled, 20dB preamp. gain, 9db transmit amp. gain
        			stw5094a_dev_state.mode = MODE_HANDSET;
#ifdef SPIRIT_DBG
            	    ST5094_TRACE("Handset mode + Speaker (3)\n");
#endif
        		    break;                    

                case 4: // HEADSET + Speaker
        			if(firstWrite == 1)
        			{
        				SPEAR_I2C_Write8(6,0x36);//muted
        			}
        			else
        			{
        				SPEAR_I2C_Write8(6,0x16);
        			}
                    SPEAR_I2C_Write8(4,0xE0 | (mic_gains[2] & 0xF)); // MIC3
        			stw5094a_dev_state.mode = MODE_HEADSET;
#ifdef SPIRIT_DBG
        		    ST5094_TRACE("Headset mode (2)\n");
#endif
                    break;
                    
                default:
                    SPEAR_I2C_ReleaseAccess();
        			return -EINVAL;
        	}
        	SPEAR_I2C_ReleaseAccess();
    		return 0;

        case SOUND_MIXER_WRITE_VOLUME: // change output gain for current mode
		    get_user(val,p);
		    SPEAR_I2C_GetAccess(0x71);
    		switch(stw5094a_dev_state.mode)
		    {
			    case MODE_HANDSET: // check range: 0 = 0 dB, 31 = -40 dB (2 dB steps)
			        if (val < 0 || val > 20) 
			        {
			            SPEAR_I2C_ReleaseAccess();
			            return -EINVAL;
			        }			        
                    SPEAR_I2C_Write8(8,val);
		            output_gains[0] = val;
			        break;
			    case MODE_HANDSFREE: // check range: 0 = +6 dB, 15 = -24 dB (2 dB steps)
			    case MODE_HANDSET_LS:
			    case MODE_HEADSET_LS:			    
			        if (val < 0 || val > 15) 
			        {
			            SPEAR_I2C_ReleaseAccess();
			            return -EINVAL;
			        }
                    SPEAR_I2C_Write8(7,val);
		            output_gains[1] = val;
			        break;
			    case MODE_HEADSET:  // check range: 0 = 0 dB, 31 = -40 dB (2 dB steps)
			        if (val < 0 || val > 20)
			        {
			            SPEAR_I2C_ReleaseAccess();
			            return -EINVAL;
			        }
			        SPEAR_I2C_Write8(9,val);
			        output_gains[2] = val;
			        break;
	            default:
                    SPEAR_I2C_ReleaseAccess();
        			return -EINVAL;		        
		    }
		    SPEAR_I2C_ReleaseAccess();
		    return 0;

        case SOUND_MIXER_READ_VOLUME:
        	SPEAR_I2C_GetAccess(0x71);
            switch(stw5094a_dev_state.mode)
            {
                case MODE_HANDSET:
                    val = SPEAR_I2C_Read8(8);
                    break;
                case MODE_HANDSFREE:
			    case MODE_HANDSET_LS:
			    case MODE_HEADSET_LS:		                
                    val = SPEAR_I2C_Read8(7);
                    break;
                case MODE_HEADSET:
                    val = SPEAR_I2C_Read8(9);
                    break;
                default:
                    SPEAR_I2C_ReleaseAccess();
                    return -EFAULT;
                break;
            }
            SPEAR_I2C_ReleaseAccess();
            if (val != output_gains[stw5094a_dev_state.mode])
            {
                ST5094_ERROR("ERROR: register (HW) and stored output gain are different!\n");
                result = copy_to_user(argp, &output_gains[stw5094a_dev_state.mode], sizeof(int));
                return -EFAULT;
            }
            else
            {
        		return copy_to_user(argp, &output_gains[stw5094a_dev_state.mode], sizeof(int));
    	    }
	  
        case SOUND_MIXER_WRITE_MIC: // change MIC gain for current mode
    		get_user(val,p);
    		// check range: 0 = 0 dB, 15 = 22.5 dB (1.5 dB steps)
    		if (val < 0 || val > 15) return -EINVAL;
            SPEAR_I2C_GetAccess(0x71);
    		switch(stw5094a_dev_state.mode)
  		    {
    			case MODE_HANDSET:
    			case MODE_HANDSET_LS:
                    SPEAR_I2C_Write8(4,0x60 | (val & 0x0F));
    	     	    break;
      			case MODE_HANDSFREE:
                    SPEAR_I2C_Write8(4,0xA0 | (val & 0x0F));
       	 		    break;
    			case MODE_HEADSET:
    			case MODE_HEADSET_LS:
    			    SPEAR_I2C_Write8(4,0xE0 | (val & 0x0F));
    			    break;
                default:
                    SPEAR_I2C_ReleaseAccess();
                    return -EFAULT;
                break;    			    
    		}
    		SPEAR_I2C_ReleaseAccess();
    		mic_gains[stw5094a_dev_state.mode] = (val & 0x0F);
    		return 0;

        case SOUND_MIXER_READ_MIC:
        	SPEAR_I2C_GetAccess(0x71);
            val = (SPEAR_I2C_Read8(4) & 0x0F);
            SPEAR_I2C_ReleaseAccess();
            if (val != mic_gains[stw5094a_dev_state.mode])
            {
                ST5094_ERROR("ERROR: register (HW) and stored mic gain are different!\n");
                result = copy_to_user(argp, &mic_gains[stw5094a_dev_state.mode], sizeof(int));
                return -EFAULT;
            }
            else
            {  
                return copy_to_user(argp, &mic_gains[stw5094a_dev_state.mode], sizeof(int));
            }
		
        case SOUND_MIXER_WRITE_SPEAKER:
        	//used to enable/disable loudspeaker indipendently from current mode
        	//if we are in handsfree, there is no effect
        	//if we are in handset/headset:
        	//  the loudspeaker is turned on if arg == 1
        	//  the loudspeaker is turned off if arg == 0
        	//other values of arg are ignored and nothing is performed
        	//note: this operation also forces de-mute of codec output (bit 5 of C.R. 6)
            /*get_user(val,p);
            flag = (val == 0 ? 0x00 : 0x10 );		  
            SPEAR_I2C_GetAccess(0x71);
            switch(stw5094a_dev_state.mode)
            {
                case MODE_HANDSET:
                	SPEAR_I2C_Write8(6, flag|0x0A);
                break;                	  
                case MODE_HANDSFREE:
                    //nothing to do
                break;                	  
                case MODE_HEADSET:
                	SPEAR_I2C_Write8(6, flag|0x06);
                break;
                default:
                    SPEAR_I2C_ReleaseAccess();
                    return -EFAULT;
            }
            SPEAR_I2C_ReleaseAccess();
            return 0;
            */		  
        default:
            return -EINVAL;
    }
}

/******************************************************************************
    TDM CONTROLLER ACCESS ROUTINES - SPECIALIZED FOR IP PHONE APPLICATIONS
******************************************************************************/

/*
-------------------------------------------------------------------------------
PRIVATE: tdm_init

Configures TDM according to currently selected sampling rate.
-------------------------------------------------------------------------------
*/

static void tdm_init(void)
{
    int rc;
    unsigned int i;
        
    ST5094_TRACE("tdm_init\n");
    
    // init various registers
    TDM_DEBUG_MSG_LEVEL = 0;
    TDM_CLEAR_REGISTER = (unsigned char *)SPEARBASIC_IT_TDM;
    *TDM_CONF = 0x0;	// stops any clock before init
    *TDM_SYNC_GEN = 0x0;// was missing
    *TDM_TIMESLOT_NBR = 8; // 8 timeslots

    // clean action memory
    // action memory size is 0x1000 bytes ---> 0x400 = 1024 32bit ints
    for (i = 0; i < 1024; i++) 
    { 
        *(TDM_ACTION_MEMORY + i) = 0x0; 
    }
    // clean TDM buffer
    //(it is possible to clean all the buffer only if TDM_SYNC_GEN [24-27]= 0x0)
    //TDM buffer size is 0x8000 bytes --> 0x2000 = 8192 32bit ints
    memset((char*)TDM_BUFFER, 0 , 0x8000);
    *TDM_FRAME_NBR = 4;//to prevent loopback

    *SPEARBASIC_CORE_CLK_CFG = *SPEARBASIC_CORE_CLK_CFG | (0x040000);//source select
    *SPEARBASIC_RAS_CLK_ENB = *SPEARBASIC_RAS_CLK_ENB | 0x0800;//enable Synt4 clock
    *SPEARBASIC_RAS4_CLK_SYNT = 0x80100BB8;//512 KHz out if 192 MHz in
    *TDM_CONF = 0x02180005;

    *SPEARBASIC_TDM_IMASK = 0xC100;//mask tdm interrupts
    if(*SPEARBASIC_TDM_IMASK != 0xC100)
    {
        ST5094_ERROR("Error init masking: TDM_IMASK=%08X\n", *SPEARBASIC_TDM_IMASK);
    }

    CLEARTDMIRQ;

	rc = request_irq(IRQ_STW5094a_TDM, (void *)tdm_interrupt,SA_INTERRUPT,"stw5094a irq", NULL);
	if (rc == -EINVAL || rc == -EBUSY)
	{
		ST5094_ERROR(KERN_ERR "stw5094a: Failed to register irq, return value = %d\n",rc);
		return;
	}
}


/*
-------------------------------------------------------------------------------
PRIVATE: tdm_open

Enables TDM in dsp_open
-------------------------------------------------------------------------------
*/
static void tdm_open(void)
{
    ST5094_TRACE("tdm_open\n");

    memset((char*)TDM_BUFFER, 0 , 0x8000);
    
	if(stw5094a_dev_state.rate == RATE_NARROWBAND)
	{
		*(TDM_ACTION_MEMORY + 0) = 0x01600015;
		*(TDM_ACTION_MEMORY + 1) = 0x01200015;
		*(TDM_ACTION_MEMORY + 2) = 0x00000000;
		*(TDM_ACTION_MEMORY + 3) = 0x00000000;
		*(TDM_ACTION_MEMORY + 4) = 0x00000000;
		*(TDM_ACTION_MEMORY + 5) = 0x00000000;
		*(TDM_ACTION_MEMORY + 6) = 0x00000000;
		*(TDM_ACTION_MEMORY + 7) = 0x00000000;
	}
	else
	{
		*(TDM_ACTION_MEMORY + 0) = 0x01700015;                              
		*(TDM_ACTION_MEMORY + 1) = 0x01300015;                              
		*(TDM_ACTION_MEMORY + 2) = 0x00000000;                              
		*(TDM_ACTION_MEMORY + 3) = 0x00000000;                              
		*(TDM_ACTION_MEMORY + 4) = 0x01F00015;                              
		*(TDM_ACTION_MEMORY + 5) = 0x01B00015;                              
		*(TDM_ACTION_MEMORY + 6) = 0x00000000;                              
		*(TDM_ACTION_MEMORY + 7) = 0x00000000;                              
	}
	
	if(stw5094a_dev_state.rate == RATE_NARROWBAND)
	{
#ifdef MANUAL_BB_MANAG
		*TDM_SYNC_GEN = 0x03000005;
#else
		*TDM_SYNC_GEN = 0x13000005;
#endif
    }
    else
    {
#ifdef MANUAL_BB_MANAG
		*TDM_SYNC_GEN = 0x0300000D;
#else
		*TDM_SYNC_GEN = 0x1300000D;
#endif
	}
	
#ifdef DEFAULT_TDMOPEN_30MSEC
	*TDM_FRAME_NBR = 240;//narrowband 30 msec
#endif
	
    CLEARTDMIRQ;

	*SPEARBASIC_TDM_IMASK = 0xC108;//unmask tdm interrupts
	if(*SPEARBASIC_TDM_IMASK != 0xC108)
	{
	    ST5094_ERROR("Error open unmasking: TDM_IMASK=%08X\n", *SPEARBASIC_TDM_IMASK);
	}
	*TDM_CONF = 0x06180005;//TDM_CONF bit 26 = 1 (TDM Clock enabled)
}

/*
-------------------------------------------------------------------------------
PRIVATE: tdm_interrupt

TDM interrupt handler
-------------------------------------------------------------------------------
*/

irqreturn_t tdm_interrupt(int irq,void *dev_id,struct pt_regs *regs)
{

    irc_counter++;
	
    if ((*TLC_ISTATUS & 0x8) == 0) 
    {
        return IRQ_NONE;
    }
    
	tdm_interrupts++;

    //is better to change sample lenght inside irq,
    //so we can be sure internal TDM counter is at the beginning of the buffer
    ////not used now (done in the SET FRAGMENT dsp ioctl)
    if(changeTDMBufferLenght != 0)
    {
    	switch(stw5094a_dev_state.buf_len)
    	{
    			case BL_10ms:
    				*TDM_FRAME_NBR = 80;//80 samples every 10 milliseconds
    			break;
    			case BL_20ms:
    				*TDM_FRAME_NBR = 160;//80 samples every 10 milliseconds
    			break;
    			case BL_30ms:
    				*TDM_FRAME_NBR = 240;//80 samples every 10 milliseconds
    			break;
    			default:
    				*TDM_FRAME_NBR = 80;//defaults to 10 msec
    			break;
    	}
    	changeTDMBufferLenght = 0;
    }
    //close only in irq and if we are in buffer bank 0 (now 1)//FIXME 0 or 1?
    if( (closingDSP != 0) )
    {
        //FIXME it seems tdm_exit is not called
    	//printk("Closing DSP in irq routine\n");
    	tdm_exit();
    	closingDSP = 0;
    }

    rd_data_ready = true;
    wake_up_interruptible(&rd_wq);
    
    CLEARTDMIRQ;
    
    return IRQ_HANDLED;
}


/*
-------------------------------------------------------------------------------
PRIVATE: tdm_exit called when closing dsp
-------------------------------------------------------------------------------
*/

static void tdm_exit(void)
{
	int i = 0;

    ST5094_TRACE("tdm_exit\n");
    CLEARTDMIRQ;
    
	//masking tdm interrupts
	*SPEARBASIC_TDM_IMASK = 0xC100;
	if(*SPEARBASIC_TDM_IMASK != 0xC100)
	{
	    ST5094_ERROR("Error tdm_exit masking: TDM_IMASK=%08X\n", *SPEARBASIC_TDM_IMASK);
	}
	
	// clean action memory
	for (i = 0; i < 1024; i++) 
	{ 
	    *(TDM_ACTION_MEMORY + i) = 0x0; 
	}
	
	*TDM_SYNC_GEN = *TDM_SYNC_GEN & 0xFCFFFFFF;//TDM_SYNC_GEN [24-25]= 0x0
	
}



/******************************************************************************
                                    MODULE
******************************************************************************/

// block of pointers to DSP routines above
static struct file_operations dsp_operations =
{
    .owner 	 = THIS_MODULE,
    .open    = dsp_open,
    .read 	 = dsp_read,
    .write 	 = dsp_write,
    .ioctl   = dsp_ioctl,
    .release = dsp_close
};

// block of pointers to MIXER routines above
static struct file_operations mixer_operations =
{
    .owner = THIS_MODULE,
    .ioctl = mixer_ioctl
};

/*
-------------------------------------------------------------------------------
PRIVATE: STW5094a_Init

Invoked when the module is dynamically loaded.
STM8 driver must have been loaded.
-------------------------------------------------------------------------------
*/

static int __init STW5094a_Init(void)
{
    // virtual memory mapping
    
    SPEARBASIC_TDM_CTL = (unsigned int)ioremap(VA_SPEARBASIC_START_STW5094A_TDM_CTR,0x100);
    SPEARBASIC_TDM_ACT = (unsigned int)ioremap(VA_SPEARBASIC_START_STW5094A_TDM_ACT,0x1000); //action memory is 1024 * 4 length, which is 0x1000 hex
    SPEARBASIC_TDM_DAT = (unsigned int)ioremap(VA_SPEARBASIC_START_STW5094A_TDM_DAT,0x8000);  //one channel will be used	: TDM buffer size is 0x8000 bytes!!!	
    SPEARBASIC_IT_TDM  = (unsigned int)ioremap(VA_SPEARBASIC_IT_TDM,1);
    	
    SPEARBASIC_TDM_IMASK  = ioremap(SPEARBASIC_START_STW5094A_TDM+0x54,4);
    SPEARBASIC_RAS_CLK_ENB = ioremap(SPEARBASIC_START_MISC_REG + 0x034,4);
    SPEARBASIC_RAS4_CLK_SYNT = ioremap(SPEARBASIC_START_MISC_REG + 0x078,4);
    SPEARBASIC_CORE_CLK_CFG = ioremap(SPEARBASIC_START_MISC_REG + 0x024,4);
    
    // register dsp device
    stw5094a_dev_state.dsp = register_sound_dsp(&dsp_operations, -1);
    if (stw5094a_dev_state.dsp < 0)
    {
		ST5094_ERROR(KERN_ERR "stw5094a: Failed to install dsp.\n");
		goto out;
	}
    
    // register mixer device
    stw5094a_dev_state.mixer = register_sound_mixer(&mixer_operations,-1);
    if (stw5094a_dev_state.mixer < 0)
    {
    	ST5094_ERROR(KERN_ERR "stw5094a: Failed to install mixer.\n");
    	goto out;
    }
    
    tdm_init();

    SPEAR_I2C_GetAccess(0x71);
    SPEAR_I2C_Write8(21,0x02); // reset
    SPEAR_I2C_Write8(6,0x2A);  //muted handset
    SPEAR_I2C_Write8(21,0x21); // normal operation, voice mode, power up
    SPEAR_I2C_Write8(6,0x2A);  //muted handset
    SPEAR_I2C_ReleaseAccess();
    	
    out:
    printk("stw5094a module loaded, version %s\n", STW5094A_DRIVER_VERSION);
    return 0;
}

/*
-------------------------------------------------------------------------------
PRIVATE: STW5094a_Exit

Invoked when the module is dynamically unloaded.
-------------------------------------------------------------------------------
*/

static void __exit STW5094a_Exit(void)
{
	//was in dsp_close
	free_irq(IRQ_STW5094a_TDM, NULL);
	
    // remove virtual memory mapping
    iounmap((void*)SPEARBASIC_TDM_CTL);
    iounmap((void*)SPEARBASIC_TDM_ACT);
    iounmap((void*)SPEARBASIC_TDM_DAT);
    iounmap((void*)SPEARBASIC_IT_TDM);
    iounmap(SPEARBASIC_TDM_IMASK);
    iounmap(SPEARBASIC_RAS_CLK_ENB);
    iounmap(SPEARBASIC_RAS4_CLK_SYNT);
    iounmap(SPEARBASIC_CORE_CLK_CFG);
	

    unregister_sound_mixer(stw5094a_dev_state.mixer);
    unregister_sound_dsp(stw5094a_dev_state.dsp);
}

/*
-------------------------------------------------------------------------------
Standard module declarations
-------------------------------------------------------------------------------
*/

module_init(STW5094a_Init);
module_exit(STW5094a_Exit);

MODULE_DESCRIPTION("STw5094A audio driver for SPEAr Basic VoIP applications v2");
MODULE_AUTHOR("STMicroelectronics");
MODULE_LICENSE("GPL");                           

/******************************************************************************
                                      EOF
******************************************************************************/
