@setlocal
@set DOINST=1
@set TMPPRJ=dirent
@set TMPSRC=..
@set TMPINST=..\..\software
@if NOT EXIST %TMPSRC%\nul goto NOSRC
@set TMPLOG=bldlog-1.txt

@REM ====== rename the log ================
@if NOT EXIST %TMPLOG% goto DNLOG
@if NOT EXIST %TMPLOG%.old goto RENOLD
@if EXIST %TMPLOG%.bak (
@del %TMPLOG%.bak
)
@ren %TMPLOG% %TMPLOG%.bak
@goto DNLOG
:RENOLD
@ren %TMPLOG% %TMPLOG%.old
:DNLOG
@REM =====================================

@set TMPOPTS=
@set TMPOPTS=-DCMAKE_INSTALL_PREFIX=%TMPINST%

@call chkmsvc %TMPPRJ%
@REM call setupqt32

@echo Begin %TIME% > %TMPLOG%

@if EXIST build-cmake.bat (
@call build-cmake >>%TMPLOG% 2>&1
)
@if NOT EXIST %TMPSRC%\CMakeLists.txt goto NOCM

@echo Doing: 'cmake %TMPSRC% %TMPOPTS%' output to %TMPLOG%
@echo Doing: 'cmake %TMPSRC% %TMPOPTS%' >>%TMPLOG%
cmake %TMPSRC% %TMPOPTS% >>%TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR1

@echo Doing: 'cmake --build . --config Debug' output to %TMPLOG%
@echo Doing: 'cmake --build . --config Debug' >>%TMPLOG%
cmake --build . --config Debug >>%TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR2

@echo Doing: 'cmake --build . --config Release' output to %TMPLOG%
@echo Doing: 'cmake --build . --config Release' >>%TMPLOG%
cmake --build . --config Release >>%TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR3

@echo Appears a successful build...
@echo.
@if "%DOINST%x" == "1x" goto DOINST
@echo Install is not configured. Set DOINST=1
@echo.
@goto END

:DOINST
@echo Continue with install to %TMPINST%? Only Ctrl+C aborts...
@echo.
@pause

@echo Doing: 'cmake --build . --config Debug --target INSTALL' output to %TMPLOG%
@echo Doing: 'cmake --build . --config Debug --target INSTALL' >>%TMPLOG%
cmake --build . --config Debug --target INSTALL >>%TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR4

@echo Doing: 'cmake --build . --config Release --target INSTALL' output to %TMPLOG%
@echo Doing: 'cmake --build . --config Release --target INSTALL' >>%TMPLOG%
cmake --build . --config Release --target INSTALL >>%TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR5
@echo.
@fa4 " -- " %TMPLOG%
@echo.
@echo Appears a successful build and install... see %TMPLOG% for details

@goto END

:NOSRC
@echo Can NOT locate source %TMPSRC%! *** FIX ME ***
@goto ISERR

:NOCM
@echo Can NOT locate source %TMPSRC%\CMakeLists.txt! *** FIX ME ***
@goto ISERR

:ERR4
:ERR5
@echo See %TMPLOG% for error
@goto ISERR

:Err1
@echo cmake config, gen error
@goto ISERR

:Err2
@echo debug build error
@goto ISERR

:Err3
@echo release build error
@goto ISERR

:ISERR
@endlocal
@exit /b 1

:END
@endlocal
@exit /b 0

@REM eof
