@setlocal
@set DOINST=1
@set INSTDBG=1
@set TMPPRJ=dirent
@set TMPSRC=..
@set TMPINST=D:\Projects\3rdParty
@set TMPGEN=Visual Studio 16 2019
@set TMP3RD=D:\Projects\3rdParty

@if NOT EXIST %TMPSRC%\nul goto NOSRC
@set TMPLOG=bldlog-1.txt


@set TMPOPTS=
@set TMPOPTS=%TMPOPTS% -G "%TMPGEN%" -A Win32
@set TMPOPTS=%TMPOPTS% -DCMAKE_INSTALL_PREFIX:PATH=%TMP3RD%

@call chkmsvc %TMPPRJ%
@REM call setupqt32

@echo Begin %DATE% %TIME% > %TMPLOG%

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
@set TMPI=Release
@if %INSTDBG% EQU 1 (
@set TMPI=%TMPI% and Debug
)

@echo Continue with install %TMPI% to %TMPINST%? Only Ctrl+C aborts...
@echo.
@pause

@if NOT %INSTDBG% EQU 1 goto DNDBGI
@echo Doing: 'cmake --build . --config Debug --target INSTALL' output to %TMPLOG%
@echo Doing: 'cmake --build . --config Debug --target INSTALL' >>%TMPLOG%
cmake --build . --config Debug --target INSTALL >>%TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR4
@if EXIST install_manifest.txt @copy install_manifest.txt install_manifest_debug.txt > nul
:DNDBGI

@echo Doing: 'cmake --build . --config Release --target INSTALL' output to %TMPLOG%
@echo Doing: 'cmake --build . --config Release --target INSTALL' >>%TMPLOG%
cmake --build . --config Release --target INSTALL >>%TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR5
@if EXIST install_manifest.txt @copy install_manifest.txt install_manifest_release.txt > nul
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
