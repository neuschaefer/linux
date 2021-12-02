@echo off
ECHO *******************************
ECHO  Building Nucleus PLUS Library
ECHO *******************************

REM ** NOTE: Define "INCLUDE_PROVIEW" to include hooks for Nucleus ProView
REM **       support.  To do this use the -pd option on assembler line for TCT.S:
REM **                  -pd "INCLUDE_PROVIEW SETL {TRUE}"


REM ** NOTE: Define "NU_INT_ROM_SUPPORT" to run out of Flash.
REM **       To do this use the -pd option on the assembler line for int.s:
REM **                  -pd "NU_INT_ROM_SUPPORT SETL {TRUE}"    */


REM ** NOTE: Define "NU_UHAL_SUPPORT" when using uHAL.
REM **       To do this use the -pd option on the assembler line for int.s:
REM **                  -pd "NU_UHAL_SUPPORT SETL {TRUE}"
REM ** Also for uHal you must add -DNU_UHAL_SUPPORT to the line compiling SDC.C


REM ** NOTE: Define "NU_940T_SUPPORT" when using the 940T processor.
REM **       To do this use the -pd option on the assembler line for int.s:
REM **                  -pd "NU_940T_SUPPORT SETL {TRUE}"


REM ** To change the cpu designator, change the following line:
REM ** (options are ARM7TDMI / ARM720T / ARM920T / ARM940T / ARM966E-S)
REM ** Set this according to the Core Module being used
REM SET CPU=ARM7TDMI
SET CPU=ARM926EJ-S

SET LIBPATH=.\lib
SET LIB=plus.lib

if not exist O\ mkdir O
if exist O\*.o del O\*.o >nul
if not exist %LIBPATH% mkdir %LIBPATH%
if exist %LIBPATH%\%LIB% del %LIBPATH%\%LIB% >nul

SET THUMB=
SET ASM=ARMASM
SET COMPILER=ARMCC
SET AR=ARMAR

if "%1"=="thumb" set THUMB=-16
if "%1"=="thumb" set COMPILER=TCC

if "%1"=="THUMB" set THUMB=-16
if "%1"=="THUMB" set COMPILER=TCC

SET APCS=-apcs /inter

SET DEBUG=

SET DEBUGFLAG=
SET OPTIMIZE=-O2
if "%DEBUG%"=="TRUE" set DEBUGFLAG=-g --dwarf2 -DNU_ENABLE_STACK_CHECK -DNU_ERROR_STRING
if "%DEBUG%"=="TRUE" set OPTIMIZE=

REM *** SET CFLAGS=-O2 -c -cpu %CPU% %APCS%
SET CFLAGS=%OPTIMIZE% %DEBUGFLAG% -c -cpu %CPU% %APCS%
SET ASMFLAGS=%THUMB% %DEBUGFLAG% -cpu %CPU% %APCS%
SET ARFLAGS=-r %LIBPATH%\%LIB%

@echo on

%ASM% %ASMFLAGS% -o  O\tmt.o tmt.s
%ASM% %ASMFLAGS% -o  O\tct.o tct.s

%COMPILER% %CFLAGS% tmse.c -o O\tmse.o
%COMPILER% %CFLAGS% tms.c -o O\tms.o
%COMPILER% %CFLAGS% tmi.c -o O\tmi.o
%COMPILER% %CFLAGS% tmf.c -o O\tmf.o
%COMPILER% %CFLAGS% tmd.c -o O\tmd.o
%COMPILER% %CFLAGS% tmc.c -o O\tmc.o
%COMPILER% %CFLAGS% tcse.c -o O\tcse.o
%COMPILER% %CFLAGS% tcs.c -o O\tcs.o
%COMPILER% %CFLAGS% tci.c -o O\tci.o
%COMPILER% %CFLAGS% tcfe.c -o O\tcfe.o
%COMPILER% %CFLAGS% tcf.c -o O\tcf.o
%COMPILER% %CFLAGS% tcd.c -o O\tcd.o
%COMPILER% %CFLAGS% tcce.c -o O\tcce.o
%COMPILER% %CFLAGS% tcc.c -o O\tcc.o
%COMPILER% %CFLAGS% smse.c -o O\smse.o
%COMPILER% %CFLAGS% sms.c -o O\sms.o
%COMPILER% %CFLAGS% smi.c -o O\smi.o
%COMPILER% %CFLAGS% smf.c -o O\smf.o
%COMPILER% %CFLAGS% smd.c -o O\smd.o
%COMPILER% %CFLAGS% smce.c -o O\smce.o
%COMPILER% %CFLAGS% smc.c -o O\smc.o
%COMPILER% %CFLAGS% rld.c -o O\rld.o
%COMPILER% %CFLAGS% rlc.c -o O\rlc.o
%COMPILER% %CFLAGS% quse.c -o O\quse.o
%COMPILER% %CFLAGS% qus.c -o O\qus.o
%COMPILER% %CFLAGS% qui.c -o O\qui.o
%COMPILER% %CFLAGS% quf.c -o O\quf.o
%COMPILER% %CFLAGS% qud.c -o O\qud.o
%COMPILER% %CFLAGS% quce.c -o O\quce.o
%COMPILER% %CFLAGS% quc.c -o O\quc.o
%COMPILER% %CFLAGS% pmi.c -o O\pmi.o
%COMPILER% %CFLAGS% pmf.c -o O\pmf.o
%COMPILER% %CFLAGS% pmd.c -o O\pmd.o
%COMPILER% %CFLAGS% pmce.c -o O\pmce.o
%COMPILER% %CFLAGS% pmc.c -o O\pmc.o
%COMPILER% %CFLAGS% pise.c -o O\pise.o
%COMPILER% %CFLAGS% pis.c -o O\pis.o
%COMPILER% %CFLAGS% pii.c -o O\pii.o
%COMPILER% %CFLAGS% pif.c -o O\pif.o
%COMPILER% %CFLAGS% pid.c -o O\pid.o
%COMPILER% %CFLAGS% pice.c -o O\pice.o
%COMPILER% %CFLAGS% pic.c -o O\pic.o
%COMPILER% %CFLAGS% mbse.c -o O\mbse.o
%COMPILER% %CFLAGS% mbs.c -o O\mbs.o
%COMPILER% %CFLAGS% mbi.c -o O\mbi.o
%COMPILER% %CFLAGS% mbf.c -o O\mbf.o
%COMPILER% %CFLAGS% mbd.c -o O\mbd.o
%COMPILER% %CFLAGS% mbce.c -o O\mbce.o
%COMPILER% %CFLAGS% mbc.c -o O\mbc.o
%COMPILER% %CFLAGS% lid.c -o O\lid.o
%COMPILER% %CFLAGS% lic.c -o O\lic.o
%COMPILER% %CFLAGS% ioi.c -o O\ioi.o
%COMPILER% %CFLAGS% iof.c -o O\iof.o
%COMPILER% %CFLAGS% iod.c -o O\iod.o
%COMPILER% %CFLAGS% ioce.c -o O\ioce.o
%COMPILER% %CFLAGS% ioc.c -o O\ioc.o
%COMPILER% %CFLAGS% inc.c -o O\inc.o
%COMPILER% %CFLAGS% hii.c -o O\hii.o
%COMPILER% %CFLAGS% hid.c -o O\hid.o
%COMPILER% %CFLAGS% hic.c -o O\hic.o
%COMPILER% %CFLAGS% evi.c -o O\evi.o
%COMPILER% %CFLAGS% evf.c -o O\evf.o
%COMPILER% %CFLAGS% evd.c -o O\evd.o
%COMPILER% %CFLAGS% evce.c -o O\evce.o
%COMPILER% %CFLAGS% evc.c -o O\evc.o
%COMPILER% %CFLAGS% eri.c -o O\eri.o
%COMPILER% %CFLAGS% erd.c -o O\erd.o
%COMPILER% %CFLAGS% erc.c -o O\erc.o
%COMPILER% %CFLAGS% dms.c -o O\dms.o
%COMPILER% %CFLAGS% dmi.c -o O\dmi.o
%COMPILER% %CFLAGS% dmf.c -o O\dmf.o
%COMPILER% %CFLAGS% dmd.c -o O\dmd.o
%COMPILER% %CFLAGS% dmce.c -o O\dmce.o
%COMPILER% %CFLAGS% dmc.c -o O\dmc.o
%COMPILER% %CFLAGS% csc.c -o O\csc.o

%AR% %ARFLAGS% O\tmt.o O\tct.o O\csc.o
%AR% %ARFLAGS% O\dmc.o O\dmce.o O\dmd.o
%AR% %ARFLAGS% O\dmf.o O\dmi.o O\dms.o O\erc.o
%AR% %ARFLAGS% O\erd.o O\eri.o O\evc.o
%AR% %ARFLAGS% O\evce.o O\evd.o O\evf.o
%AR% %ARFLAGS% O\evi.o O\hic.o O\hid.o
%AR% %ARFLAGS% O\hii.o O\inc.o O\ioc.o
%AR% %ARFLAGS% O\ioce.o O\iod.o O\iof.o
%AR% %ARFLAGS% O\ioi.o O\lic.o O\lid.o
%AR% %ARFLAGS% O\mbc.o O\mbce.o O\mbd.o
%AR% %ARFLAGS% O\mbf.o O\mbi.o O\mbs.o
%AR% %ARFLAGS% O\mbse.o O\pic.o O\pice.o
%AR% %ARFLAGS% O\pid.o O\pif.o O\pii.o
%AR% %ARFLAGS% O\pis.o O\pise.o O\pmc.o
%AR% %ARFLAGS% O\pmce.o O\pmd.o O\pmf.o
%AR% %ARFLAGS% O\pmi.o O\quc.o O\quce.o
%AR% %ARFLAGS% O\qud.o O\quf.o O\qui.o
%AR% %ARFLAGS% O\qus.o O\quse.o O\rlc.o
%AR% %ARFLAGS% O\rld.o O\smc.o O\smce.o
%AR% %ARFLAGS% O\smd.o O\smf.o O\smi.o
%AR% %ARFLAGS% O\sms.o O\smse.o O\tcc.o
%AR% %ARFLAGS% O\tcce.o O\tcd.o O\tcf.o
%AR% %ARFLAGS% O\tci.o O\tcs.o O\tcse.o
%AR% %ARFLAGS% O\tmc.o O\tmd.o O\tmf.o
%AR% %ARFLAGS% O\tmi.o O\tms.o O\tmse.o o\tcfe.o
