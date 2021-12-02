@echo off
IF EXIST %MT8205_C51PATH%\bin\c51.exe goto CHK_PARM

:SET_C51_PATH
@ECHO please add "set MT8205_C51PATH=" in autoexec.bat
goto END

:USAGE
@echo Usage: build.bat [option]
@echo   clean  - clean the generated obj and library.
@echo   depend - build the project dependency files.
@echo   all    - build the whold projects.
@echo   PS: if you do not specify any options, the default is "all"
goto END

:CHK_PARM
set C51INC=%MT8205_C51PATH%\INC
set C51LIB=%MT8205_C51PATH%\LIB
@IF "%1" == "clean" goto CLEAN
@IF "%1" == "CLEAN" goto CLEAN
@IF "%1" == "depend" goto DEPEND
@IF "%1" == "DEPEND" goto DEPEND
@IF "%1" == "all" goto ALL
@IF "%1" == "ALL" goto ALL
@IF "%1" == "" goto ALL
@echo "Unknown Option !!!"
goto USAGE

:CLEAN
cd ..\
@IF exist Makefile.opt goto CLEAN2
@IF exist Makefile.cus goto CLEAN2
cd workspace
goto END

:CLEAN2
bin\make %1 | bin\tee.exe workspace\build.log
cd workspace
@IF exist ..\lib\create_lib.bat del ..\lib\create_lib.bat
@IF exist ..\lib\lib.log del ..\lib\lib.log
goto END

:DEPEND
cd ..\
bin\make %1 | bin\tee.exe workspace\build.log
cd workpace
goto END

:ALL
@del ..\lib\create_lib.bat
@echo "del done"
@cd ..\
@bin\make %1 2>&1 | bin\tee.exe workspace\build.log
@cd workspace
@echo "make done"
@..\bin\ParseBuild build.log
@echo "parsebuild done"
@IF ERRORLEVEL 2 GOTO BUILD_ERROR
@IF ERRORLEVEL 1 GOTO BUILD_WARNING
@cd ..\lib
@IF exist create_lib.bat call create_lib.bat
@cd ..\workspace
goto END

:BUILD_ERROR
@echo BUILD ERROR !! Please see build.log !!
@GOTO END

:BUILD_WARNING
@echo BUILD WARNING !! Please see build.log !!
@GOTO END

:END
@echo on

