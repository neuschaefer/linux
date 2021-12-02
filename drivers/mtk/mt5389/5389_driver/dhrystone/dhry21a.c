/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
/*
 *************************************************************************
 *
 *                   "DHRYSTONE" Benchmark Program
 *                   -----------------------------
 *
 *  Version:    C, Version 2.1
 *
 *  File:       dhry_1.c (part 2 of 3)
 *
 *  Date:       May 25, 1988
 *
 *  Author:     Reinhold P. Weicker
 *
 *************************************************************************
 */


#include "dhry.h"

/* Global Variables: */

Rec_Pointer     Ptr_Glob,
                Next_Ptr_Glob;
int             Int_Glob;
Boolean         Bool_Glob;
char            Ch_1_Glob,
                Ch_2_Glob;
int             Arr_1_Glob [50];
int             Arr_2_Glob [50] [50];

char Reg_Define[] = "Register option selected.";

//extern char     *malloc ();
//Enumeration     Func_1 ();
Enumeration Func_1 (Capital_Letter, Capital_Letter);
  /* forward declaration necessary since Enumeration may not simply be int */


#ifndef ROPT
#define REG
        /* REG becomes defined as empty */
        /* i.e. no register variables   */
#else
#define REG register
#endif

void Proc_1(REG Rec_Pointer);
void Proc_2 (One_Fifty *);
void Proc_3 (Rec_Pointer *);
void Proc_4 (void);
void Proc_5 (void);
#ifdef  NOSTRUCTASSIGN
 void memcpy (register char *, register char *, register int);
#endif

extern Boolean Func_2 (Str_30, Str_30);
extern void Proc_6 (Enumeration, Enumeration *);
extern void Proc_7 (One_Fifty, One_Fifty, One_Fifty *);
extern void Proc_8 (Arr_1_Dim, Arr_2_Dim, int, int);

extern UINT64 u8Div6464(UINT64 u8Dividend, UINT64 u8Divider, UINT64 *pu8Remainder);


/* variables for time measurement: */

#define Too_Small_Time 2000000
                /* Measurements should last at least 2 seconds */

long          Begin_Time,
                End_Time,
                User_Time;

long          Microseconds,
                Dhrystones_Per_Second,
                Vax_Mips;

/* end of variables for time measurement */


void
DhrystoneTest (int i4Runs)
/*****/

  /* main program, corresponds to procedures        */
  /* Main and Proc_0 in the Ada version             */
{
  unsigned int   dtime();

        One_Fifty       Int_1_Loc = 0;
  REG   One_Fifty       Int_2_Loc = 0;
        One_Fifty       Int_3_Loc = 0;
  REG   char            Ch_Index;
        Enumeration     Enum_Loc = (Enumeration)0;
        Str_30          Str_1_Loc;
        Str_30          Str_2_Loc;
  REG   int             Run_Index;
  REG   int             Number_Of_Runs;

//        FILE            *Ap;

  /* Initializations */
/*
 #ifdef riscos
  if ((Ap = fopen("dhry/res","a+")) == NULL)
 #else
  if ((Ap = fopen("dhry.res","a+")) == NULL)
 #endif
    {
       Printf("Can not open dhry.res\n\n");
       exit(1);
    }
*/
/*
  Next_Ptr_Glob = (Rec_Pointer) malloc (sizeof (Rec_Type));
  Ptr_Glob = (Rec_Pointer) malloc (sizeof (Rec_Type));
*/
  Next_Ptr_Glob = (Rec_Pointer) x_mem_alloc (sizeof (Rec_Type));
  Ptr_Glob = (Rec_Pointer) x_mem_alloc (sizeof (Rec_Type));

  if ((Next_Ptr_Glob == NULL) ||
      (Ptr_Glob == NULL))
  {
      Printf("x_mem_alloc fail\n");
      return;
  }

  Ptr_Glob->Ptr_Comp                    = Next_Ptr_Glob;
  Ptr_Glob->Discr                       = Ident_1;
  Ptr_Glob->variant.var_1.Enum_Comp     = Ident_3;
  Ptr_Glob->variant.var_1.Int_Comp      = 40;
  strcpy (Ptr_Glob->variant.var_1.Str_Comp,
          "DHRYSTONE PROGRAM, SOME STRING");
  strcpy (Str_1_Loc, "DHRYSTONE PROGRAM, 1'ST STRING");

  Arr_2_Glob [8][7] = 10;
        /* Was missing in published program. Without this statement,    */
        /* Arr_2_Glob [8][7] would have an undefined value.             */
        /* Warning: With 16-Bit processors and Number_Of_Runs > 32000,  */
        /* overflow may occur for this array element.                   */

  Printf ("\n");
  Printf ("Dhrystone Benchmark, Version 2.1 (Language: C)\n");
  Printf ("\n");
/*
  if (Reg)
  {
    Printf ("Program compiled with 'register' attribute\n");
    Printf ("\n");
  }
  else
  {
    Printf ("Program compiled without 'register' attribute\n");
    Printf ("\n");
  }
*/
/*
  Printf ("Please give the number of runs through the benchmark: ");
  {
    int n;
    scanf ("%d", &n);
    Number_Of_Runs = n;
  }
*/
    Number_Of_Runs = i4Runs;
  Printf ("\n");

  Printf ("Execution starts, %d runs through Dhrystone\n",Number_Of_Runs);

  /***************/
  /* Start timer */
  /***************/

  Begin_Time = dtime();

  for (Run_Index = 1; Run_Index <= Number_Of_Runs; ++Run_Index)
  {

    Proc_5();
    Proc_4();
      /* Ch_1_Glob == 'A', Ch_2_Glob == 'B', Bool_Glob == true */
    Int_1_Loc = 2;
    Int_2_Loc = 3;
    strcpy (Str_2_Loc, "DHRYSTONE PROGRAM, 2'ND STRING");
    Enum_Loc = Ident_2;
    Bool_Glob = ! Func_2 (Str_1_Loc, Str_2_Loc);
      /* Bool_Glob == 1 */
    while (Int_1_Loc < Int_2_Loc)  /* loop body executed once */
    {
      Int_3_Loc = 5 * Int_1_Loc - Int_2_Loc;
        /* Int_3_Loc == 7 */
      Proc_7 (Int_1_Loc, Int_2_Loc, &Int_3_Loc);
        /* Int_3_Loc == 7 */
      Int_1_Loc += 1;
    } /* while */
      /* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */
    Proc_8 (Arr_1_Glob, Arr_2_Glob, Int_1_Loc, Int_3_Loc);
      /* Int_Glob == 5 */
    Proc_1 (Ptr_Glob);
    for (Ch_Index = 'A'; Ch_Index <= Ch_2_Glob; ++Ch_Index)
                             /* loop body executed twice */
    {
      if (Enum_Loc == Func_1 (Ch_Index, 'C'))
          /* then, not executed */
        {
        Proc_6 (Ident_1, &Enum_Loc);
        strcpy (Str_2_Loc, "DHRYSTONE PROGRAM, 3'RD STRING");
        Int_2_Loc = Run_Index;
        Int_Glob = Run_Index;
        }
    }
      /* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */
    Int_2_Loc = Int_2_Loc * Int_1_Loc;
    Int_1_Loc = Int_2_Loc / Int_3_Loc;
    Int_2_Loc = 7 * (Int_2_Loc - Int_3_Loc) - Int_1_Loc;
      /* Int_1_Loc == 1, Int_2_Loc == 13, Int_3_Loc == 7 */
    Proc_2 (&Int_1_Loc);
      /* Int_1_Loc == 5 */

  } /* loop "for Run_Index" */

  /**************/
  /* Stop timer */
  /**************/

  End_Time = dtime();

  Printf ("Execution ends\n");
  Printf ("\n");
  Printf ("Final values of the variables used in the benchmark:\n");
  Printf ("\n");
  Printf ("Int_Glob:            %d\n", Int_Glob);
  Printf ("        should be:   %d\n", 5);
  Printf ("Bool_Glob:           %d\n", Bool_Glob);
  Printf ("        should be:   %d\n", 1);
  Printf ("Ch_1_Glob:           %c\n", Ch_1_Glob);
  Printf ("        should be:   %c\n", 'A');
  Printf ("Ch_2_Glob:           %c\n", Ch_2_Glob);
  Printf ("        should be:   %c\n", 'B');
  Printf ("Arr_1_Glob[8]:       %d\n", Arr_1_Glob[8]);
  Printf ("        should be:   %d\n", 7);
  Printf ("Arr_2_Glob[8][7]:    %d\n", Arr_2_Glob[8][7]);
  Printf ("        should be:   Number_Of_Runs + 10\n");
  Printf ("Ptr_Glob->\n");
  Printf ("  Ptr_Comp:          %d\n", (int) Ptr_Glob->Ptr_Comp);
  Printf ("        should be:   (implementation-dependent)\n");
  Printf ("  Discr:             %d\n", Ptr_Glob->Discr);
  Printf ("        should be:   %d\n", 0);
  Printf ("  Enum_Comp:         %d\n", Ptr_Glob->variant.var_1.Enum_Comp);
  Printf ("        should be:   %d\n", 2);
  Printf ("  Int_Comp:          %d\n", Ptr_Glob->variant.var_1.Int_Comp);
  Printf ("        should be:   %d\n", 17);
  Printf ("  Str_Comp:          %s\n", Ptr_Glob->variant.var_1.Str_Comp);
  Printf ("        should be:   DHRYSTONE PROGRAM, SOME STRING\n");
  Printf ("Next_Ptr_Glob->\n");
  Printf ("  Ptr_Comp:          %d\n", (int) Next_Ptr_Glob->Ptr_Comp);
  Printf ("        should be:   (implementation-dependent), same as above\n");
  Printf ("  Discr:             %d\n", Next_Ptr_Glob->Discr);
  Printf ("        should be:   %d\n", 0);
  Printf ("  Enum_Comp:         %d\n", Next_Ptr_Glob->variant.var_1.Enum_Comp);
  Printf ("        should be:   %d\n", 1);
  Printf ("  Int_Comp:          %d\n", Next_Ptr_Glob->variant.var_1.Int_Comp);
  Printf ("        should be:   %d\n", 18);
  Printf ("  Str_Comp:          %s\n", Next_Ptr_Glob->variant.var_1.Str_Comp);
  Printf ("        should be:   DHRYSTONE PROGRAM, SOME STRING\n");
  Printf ("Int_1_Loc:           %d\n", Int_1_Loc);
  Printf ("        should be:   %d\n", 5);
  Printf ("Int_2_Loc:           %d\n", Int_2_Loc);
  Printf ("        should be:   %d\n", 13);
  Printf ("Int_3_Loc:           %d\n", Int_3_Loc);
  Printf ("        should be:   %d\n", 7);
  Printf ("Enum_Loc:            %d\n", Enum_Loc);
  Printf ("        should be:   %d\n", 1);
  Printf ("Str_1_Loc:           %s\n", Str_1_Loc);
  Printf ("        should be:   DHRYSTONE PROGRAM, 1'ST STRING\n");
  Printf ("Str_2_Loc:           %s\n", Str_2_Loc);
  Printf ("        should be:   DHRYSTONE PROGRAM, 2'ND STRING\n");
  Printf ("\n");

  User_Time = End_Time - Begin_Time;

  if (User_Time < Too_Small_Time)
  {
    Printf ("Measured time too small to obtain meaningful results\n");
    Printf ("Please increase number of runs\n");
    Printf ("\n");
  }
  else
  {
    //char buf[32];

//    Printf("Used time: %f seconds\n", User_Time);
    //Printf("Used time: %s seconds\n", ftoa(buf, User_Time, 6));
    Printf("Used time: %d microseconds\n", User_Time);

    //Microseconds = User_Time * Mic_secs_Per_Second / (unsigned int) Number_Of_Runs;
    Microseconds = u8Div6464(User_Time, Number_Of_Runs, NULL);

    //Dhrystones_Per_Second = (long) Number_Of_Runs / User_Time;
    Dhrystones_Per_Second = u8Div6464((long long)Number_Of_Runs*1000000, User_Time, NULL);

    //Vax_Mips = Dhrystones_Per_Second / 1757;
    Vax_Mips = u8Div6464(Dhrystones_Per_Second, 1757, NULL);

#ifdef ROPT
    Printf ("Register option selected?  YES\n");
#else
    Printf ("Register option selected?  NO\n");
    strcpy(Reg_Define, "Register option not selected.");
#endif
    Printf ("Microseconds for one run through Dhrystone = %d \n", Microseconds);
//    Printf ("%7.1f \n", Microseconds);
    //Printf ("%s \n", ftoa(buf, Microseconds, 1));
    Printf ("Dhrystones per Second = %d\n", Dhrystones_Per_Second);
//    Printf ("%10.1f \n", Dhrystones_Per_Second);
    //Printf ("%s \n", ftoa(buf, Dhrystones_Per_Second, 1));
//    Printf ("VAX MIPS rating = %10.3f \n",Vax_Mips);
    //Printf ("VAX MIPS rating = %s \n",ftoa(buf, Vax_Mips, 3));
    Printf ("VAX MIPS rating = %d \n",Vax_Mips);
    Printf ("\n");

/*
  fprintf(Ap,"\n");
  fprintf(Ap,"Dhrystone Benchmark, Version 2.1 (Language: C)\n");
  fprintf(Ap,"%s\n",Reg_Define);
  fprintf(Ap,"Microseconds for one loop: %7.1f\n",Microseconds);
  fprintf(Ap,"Dhrystones per second: %10.1f\n",Dhrystones_Per_Second);
  fprintf(Ap,"VAX MIPS rating: %10.3f\n",Vax_Mips);
  fclose(Ap);
*/
  }

  // Release resources
  x_mem_free(Ptr_Glob);
  x_mem_free(Next_Ptr_Glob);
}


void
Proc_1 (REG Rec_Pointer Ptr_Val_Par)
    /* executed once */
{
  REG Rec_Pointer Next_Record = Ptr_Val_Par->Ptr_Comp;
                                        /* == Ptr_Glob_Next */
  /* Local variable, initialized with Ptr_Val_Par->Ptr_Comp,    */
  /* corresponds to "rename" in Ada, "with" in Pascal           */

  structassign (*Ptr_Val_Par->Ptr_Comp, *Ptr_Glob);
  Ptr_Val_Par->variant.var_1.Int_Comp = 5;
  Next_Record->variant.var_1.Int_Comp
        = Ptr_Val_Par->variant.var_1.Int_Comp;
  Next_Record->Ptr_Comp = Ptr_Val_Par->Ptr_Comp;
  Proc_3 (&Next_Record->Ptr_Comp);
    /* Ptr_Val_Par->Ptr_Comp->Ptr_Comp
                        == Ptr_Glob->Ptr_Comp */
  if (Next_Record->Discr == Ident_1)
    /* then, executed */
  {
    Next_Record->variant.var_1.Int_Comp = 6;
    Proc_6 (Ptr_Val_Par->variant.var_1.Enum_Comp,
           &Next_Record->variant.var_1.Enum_Comp);
    Next_Record->Ptr_Comp = Ptr_Glob->Ptr_Comp;
    Proc_7 (Next_Record->variant.var_1.Int_Comp, 10,
           &Next_Record->variant.var_1.Int_Comp);
  }
  else /* not executed */
    structassign (*Ptr_Val_Par, *Ptr_Val_Par->Ptr_Comp);
} /* Proc_1 */


void
Proc_2 (One_Fifty *Int_Par_Ref)
/******************/
    /* executed once */
    /* *Int_Par_Ref == 1, becomes 4 */
{
  One_Fifty  Int_Loc;
  Enumeration   Enum_Loc = Ident_1;

  Int_Loc = *Int_Par_Ref + 10;
  do /* executed once */
    if (Ch_1_Glob == 'A')
      /* then, executed */
    {
      Int_Loc -= 1;
      *Int_Par_Ref = Int_Loc - Int_Glob;
      Enum_Loc = Ident_1;
    } /* if */
  while (Enum_Loc != Ident_1); /* true */
} /* Proc_2 */


void
Proc_3 (Rec_Pointer *Ptr_Ref_Par)
/******************/
    /* executed once */
    /* Ptr_Ref_Par becomes Ptr_Glob */
{
  if (Ptr_Glob != Null)
    /* then, executed */
    *Ptr_Ref_Par = Ptr_Glob->Ptr_Comp;
  Proc_7 (10, Int_Glob, &Ptr_Glob->variant.var_1.Int_Comp);
} /* Proc_3 */


void
Proc_4 (void) /* without parameters */
/*******/
    /* executed once */
{
  Boolean Bool_Loc;

  Bool_Loc = Ch_1_Glob == 'A';
  Bool_Glob = Bool_Loc | Bool_Glob;
  Ch_2_Glob = 'B';
} /* Proc_4 */


void
Proc_5 (void) 	/* without parameters */
		/* executed once */
{
  Ch_1_Glob = 'A';
  Bool_Glob = false;
} /* Proc_5 */


        /* Procedure for the assignment of structures,          */
        /* if the C compiler doesn't support this feature       */
#ifdef  NOSTRUCTASSIGN
void
memcpy (register char *d, register char *s, register int l)
/* register char   *d; register char   *s; register int    l; */
{
        while (l--) *d++ = *s++;
}
#endif
