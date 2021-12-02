@echo off
REM @set CUST_DIR=mtk
IF EXIST %MT8205_C51PATH%\bin\c51.exe goto CHK_PARAM

:SET_C51_PATH
@ECHO please add "set MT8205_C51PATH=" in autoexec.bat
@goto END

:CHK_PARAM
@IF "%1" == "" goto SHOW_USAGE
@IF "%1" == "clean" goto CLEAN
@IF "%1" == "CLEAN" goto CLEAN
@IF "%1" == "mt82xx" goto 82XX_CHK_PARAM
@IF "%1" == "mt53xx" goto 53XX_CHK_PARAM
@echo SUBDIRS = %1 > ..\src\custom\Makefile
@echo CUST_INCPATH = $(TOPDIR)/src/custom/%1 > ..\Makefile.cus
@cd ..\src\custom
@IF not exist "%1" goto SHOW_USAGE
@goto BUILD_ALL
REM @goto BUILD_ALL

:82XX_CHK_PARAM
@IF "%2" == "" goto SHOW_USAGE
@echo SUBDIRS = %2 > ..\src\custom\Makefile
@echo CUST_INCPATH = $(TOPDIR)/src/custom/%2 > ..\Makefile.cus
@cd ..\src\custom
@IF not exist "%2" goto SHOW_USAGE
@goto BUILD_ALL_82xx

:53XX_CHK_PARAM
@IF "%2" == "" goto SHOW_USAGE
@echo SUBDIRS = %2 > ..\src\custom\Makefile
@echo CUST_INCPATH = $(TOPDIR)/src/custom/%2 > ..\Makefile.cus
@cd ..\src\custom
@IF not exist "%2" goto SHOW_USAGE
@goto BUILD_ALL_53xx

:BUILD_ALL_82xx
@cd ..\Driver
@echo SRC = startup_82xx.a51\> Makefile
@type CMakefile >> Makefile
@cd ..\..\workspace
@..\bin\GetBL51Cmd.exe system_82xx.cfg SYSTEM_DEMO_US
@cd ..\
@mkdir lib
@bin\makemake "Makefile.inc" "Makefile.rule" "lib\lib.log" ".\src"
@cd workspace
@call build.bat all
@..\bin\ParseBuild build.log
@IF ERRORLEVEL 2 GOTO BUILD_ERROR
@IF ERRORLEVEL 1 GOTO BUILD_WARNING
@goto LINK_PRC
REM @goto END

:BUILD_ALL_53xx
@cd ..\Driver
@echo SRC = startup.a51\> Makefile
@type CMakefile >> Makefile
@cd ..\..\workspace
@..\bin\GetBL51Cmd.exe system.cfg SYSTEM_DEMO_US
@cd ..\
@mkdir lib
@bin\makemake "Makefile.inc" "Makefile.rule" "lib\lib.log" ".\src"
@cd workspace
@call build.bat all
@..\bin\ParseBuild build.log
@IF ERRORLEVEL 2 GOTO BUILD_ERROR
@IF ERRORLEVEL 1 GOTO BUILD_WARNING
@goto LINK_PRC
REM @goto END

:BUILD_ALL
@cd ..\Driver
@echo SRC = startup.a51\> Makefile
@type CMakefile >> Makefile
@cd ..\..\workspace
@..\bin\GetBL51Cmd.exe system.cfg SYSTEM_DEMO_US
@cd ..\
@mkdir lib
@bin\makemake "Makefile.inc" "Makefile.rule" "lib\lib.log" ".\src"
@cd workspace
@call build.bat all
@..\bin\ParseBuild build.log
@IF ERRORLEVEL 2 GOTO BUILD_ERROR
@IF ERRORLEVEL 1 GOTO BUILD_WARNING
@goto LINK_PRC
REM @goto END

:LINK_PRC
@del *.Hex %1*.omf %1*.m51 %1*.bin
@%MT8205_C51PATH%\bin\bl51 @link.dat > link.log
@IF ERRORLEVEL 2 GOTO LINK_ERROR
@..\bin\parsem51.exe ui.m51 ..\lib\lib.log > %1.log
@echo %1.log has been created !!!
@echo %1.log has been created !!! >> link.log
@..\bin\ParseLink link.log
@IF ERRORLEVEL 2 GOTO LINK_ERROR
@ren ui.omf %1.OMF
@ren ui.m51 %1.M51
@FIND "?C_INITSEG" %1.m51 > tmp
@IF ERRORLEVEL 1 goto CHK_CODE_TABLE
@IF ERRORLEVEL 0 goto VAR_INIT_ERROR

:SHOW_USAGE
@IF not exist "b2h.exe" cd ..\..\workspace
REM @ECHO Usage: link.bat [output bin file] or clean
@ECHO Usage: link.bat [mt82xx or mt53xx][PCB TYPE] or clean
@ECHO example1: link clean
@ECHO example2: link mt82xx mt8223p1v1
@ECHO example3: link mt53xx mt5363m1v1
@ECHO [PCB TYPE]:
@dir ..\src\custom /B /AD
@goto END

:CHK_CODE_TABLE
@call ..\bin\chkm51 > chkm51.txt
@FIND "Code table error" chkm51.txt > tmp1
@IF ERRORLEVEL 1 goto LINK_PRC2
@IF ERRORLEVEL 0 goto CODE_TABLE_ERROR

:LINK_PRC2
@%MT8205_C51PATH%\bin\oc51 %1.omf
@if exist %1.b00 ren %1.b00 %1b00.omf
@if exist %1.b01 ren %1.b01 %1b01.omf
@if exist %1.b02 ren %1.b02 %1b02.omf
@if exist %1.b03 ren %1.b03 %1b03.omf
@if exist %1.b04 ren %1.b04 %1b04.omf
@if exist %1.b05 ren %1.b05 %1b05.omf
@if exist %1.b06 ren %1.b06 %1b06.omf
@if exist %1.b07 ren %1.b07 %1b07.omf
@if exist %1.b08 ren %1.b08 %1b08.omf
@if exist %1.b09 ren %1.b09 %1b09.omf
@if exist %1.b10 ren %1.b10 %1b10.omf
@if exist %1.b11 ren %1.b11 %1b11.omf
@if exist %1.b12 ren %1.b12 %1b12.omf
@if exist %1.b13 ren %1.b13 %1b13.omf
@if exist %1.b14 ren %1.b14 %1b14.omf
@if exist %1.b15 ren %1.b15 %1b15.omf
@if exist %1b00.omf %MT8205_C51PATH%\bin\oh51 %1b00.omf 
@if exist %1b01.omf %MT8205_C51PATH%\bin\oh51 %1b01.omf 
@if exist %1b02.omf %MT8205_C51PATH%\bin\oh51 %1b02.omf 
@if exist %1b03.omf %MT8205_C51PATH%\bin\oh51 %1b03.omf 
@if exist %1b04.omf %MT8205_C51PATH%\bin\oh51 %1b04.omf 
@if exist %1b05.omf %MT8205_C51PATH%\bin\oh51 %1b05.omf 
@if exist %1b06.omf %MT8205_C51PATH%\bin\oh51 %1b06.omf 
@if exist %1b07.omf %MT8205_C51PATH%\bin\oh51 %1b07.omf 
@if exist %1b08.omf %MT8205_C51PATH%\bin\oh51 %1b08.omf 
@if exist %1b09.omf %MT8205_C51PATH%\bin\oh51 %1b09.omf 
@if exist %1b10.omf %MT8205_C51PATH%\bin\oh51 %1b10.omf 
@if exist %1b11.omf %MT8205_C51PATH%\bin\oh51 %1b11.omf 
@if exist %1b12.omf %MT8205_C51PATH%\bin\oh51 %1b12.omf 
@if exist %1b13.omf %MT8205_C51PATH%\bin\oh51 %1b13.omf 
@if exist %1b14.omf %MT8205_C51PATH%\bin\oh51 %1b14.omf 
@if exist %1b15.omf %MT8205_C51PATH%\bin\oh51 %1b15.omf
@if exist %1b00.hex ..\bin\hex2bin %1b00.hex
@if exist %1b01.hex ..\bin\hex2bin %1b01.hex
@if exist %1b02.hex ..\bin\hex2bin %1b02.hex
@if exist %1b03.hex ..\bin\hex2bin %1b03.hex
@if exist %1b04.hex ..\bin\hex2bin %1b04.hex
@if exist %1b05.hex ..\bin\hex2bin %1b05.hex
@if exist %1b06.hex ..\bin\hex2bin %1b06.hex
@if exist %1b07.hex ..\bin\hex2bin %1b07.hex
@if exist %1b08.hex ..\bin\hex2bin %1b08.hex
@if exist %1b09.hex ..\bin\hex2bin %1b09.hex
@if exist %1b10.hex ..\bin\hex2bin %1b10.hex
@if exist %1b11.hex ..\bin\hex2bin %1b11.hex
@if exist %1b12.hex ..\bin\hex2bin %1b12.hex
@if exist %1b13.hex ..\bin\hex2bin %1b13.hex
@if exist %1b14.hex ..\bin\hex2bin %1b14.hex
@if exist %1b15.hex ..\bin\hex2bin %1b15.hex
@REM ..\bin\mymerge %1b00.bin %1b01.bin %1b02.bin %1b03.bin %1b04.bin %1b05.bin %1b06.bin %1b07.bin %1b08.bin %1b09.bin %1b10.bin %1b11.bin %1b12.bin %1b13.bin %1b14.bin %1b15.bin %1.bin
@ren %1b00.bin %1.bin
@rem @del %1b??.omf %1b??.bin %1b??.hex tmp tmp1
@echo Finished merge!
@echo Finished merge! >> link.log
@goto END

:CLEAN
@call build.bat clean
@del *.omf *.m51 *.bin *.hex *.b0? *.b1? *.log build.log tmp tmp1 chkm51.txt
@del AllInclude.txt
@del ..\*.opt /s
@del ..\*.cus /s
@del ..\Makefile.inc ..\Makefile.rule
@GOTO END

:BUILD_ERROR
@echo BUILD ERROR !! Please see build.log !!
@GOTO END

:BUILD_WARNING
@echo BUILD WARNING !! Please see build.log !!
@GOTO END

:LINK_ERROR
@echo LINKER ERROR !! Please see link.log !!
@GOTO END

:LINK_WARNING
@echo LINKER WARNING !! Please see link.log !!
@GOTO END

:VAR_INIT_ERROR
@echo Variable Initialization Error !!! Please try to remove ?C_INITSEG (Remove global initialized variable)!!
@echo Variable Initialization Error !!! Please try to remove ?C_INITSEG (Remove global initialized variable)!! >> link.log
@GOTO END

:CODE_TABLE_ERROR
@echo Code Table Error !!! Please check the code segment assignment in system.cfg!
@echo Code Table Error !!! Please check the code segment assignment in system.cfg! >> link.log
@GOTO END

:END
