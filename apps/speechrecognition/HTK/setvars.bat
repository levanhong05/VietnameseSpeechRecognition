@echo off

set LWD=%~d0
cd %1
set HOME=%~dps0
set HOME=%HOME:~0,-1%

rem IF "%PROCESSOR_ARCHITECTURE%"=="x86" (set OS=32BIT) else (set OS=64BIT)

rem if %OS%==32BIT echo This is a 32bit operating system
rem if %OS%==64BIT echo This is a 64bit operating system

rem perl -e1 2>NUL
rem if errorlevel 1 (
rem     echo Perl is not installed
rem )

rem set USER=%USERNAME%
set HTK_INST_DIR=%HOME%
set WM_PROJECT_INST_DIR=%HTK_INST_DIR%

set PATH=%PATH%;%WM_PROJECT_INST_DIR%;

%~d1
rem %1 %2 %3 %4 %5 %6 %7 %8 %9

cd %LWD%