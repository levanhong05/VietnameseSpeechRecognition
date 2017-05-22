@echo off

set LWD=%~d0
cd %1
set HOME=%~dps0
set HOME=%HOME:~0,-1%

@for %%C in ("%CD%") do cd %%~fsC

rem quick jump if just one available
set countver=0
if exist %HOME%\DOFI-2.2.2\lib\linuxmingw32DPOpt set /A countver=countver+1 & set jumpto=w32
if exist %HOME%\DOFI-2.2.2\lib\linuxmingw-w32SPOpt set /A countver=countver+1 & set jumpto=ww32S
if exist %HOME%\DOFI-2.2.2\lib\linuxmingw-w32DPOpt set /A countver=countver+1 & set jumpto=ww32D
if exist %HOME%\DOFI-2.2.2\lib\linuxmingw-w64DPOpt set /A countver=countver+1 & set jumpto=ww64
if exist %HOME%\DOFI-2.2.2\lib\linux64mingw-w64DPOpt set /A countver=countver+1 & set jumpto=ww64_x64
if exist %HOME%\DOFI-2.2.2\lib\linuxi586-mingw32msvcSPOpt set /A countver=countver+1 & set jumpto=mw32S
if exist %HOME%\DOFI-2.2.2\lib\linuxi586-mingw32msvcDPOpt set /A countver=countver+1 & set jumpto=mw32
if exist %HOME%\DOFI-2.2.2\lib\linuxamd64-mingw32msvcDPOpt set /A countver=countver+1 & set jumpto=mw32_x64
if exist %HOME%\DOFI-2.2.2\lib\linuxi686-w64-mingw32SPOpt set /A countver=countver+1 & set jumpto=ww32S_686
if exist %HOME%\DOFI-2.2.2\lib\linuxi686-w64-mingw32DPOpt set /A countver=countver+1 & set jumpto=ww32_686
if exist %HOME%\DOFI-2.2.2\lib\linuxx86_64-w64-mingw32DPOpt set /A countver=countver+1 & set jumpto=ww64_x64
if "%countver%"=="1" goto %jumpto%

rem user options interface
echo Setting up DOFI variables - Choose one option:
if exist %HOME%\DOFI-2.2.2\lib\linuxmingw32DPOpt            echo 1 - mingw32 Double Precision
if exist %HOME%\DOFI-2.2.2\lib\linuxmingw-w32SPOpt          echo 2 - mingw-w32 Single Precision (custom mingw-w64)
if exist %HOME%\DOFI-2.2.2\lib\linuxmingw-w32DPOpt          echo 3 - mingw-w32 Double Precision (custom mingw-w64)
if exist %HOME%\DOFI-2.2.2\lib\linuxmingw-w64DPOpt          echo 4 - mingw-w64 Double Precision (custom mingw-w64)
if exist %HOME%\DOFI-2.2.2\lib\linux64mingw-w64DPOpt        echo 4 - mingw-w64 Double Precision (custom mingw-w64)
if exist %HOME%\DOFI-2.2.2\lib\linuxi586-mingw32msvcSPOpt   echo 5 - mingw-w32 Single Precision (Ubuntu's mingw-w64)
if exist %HOME%\DOFI-2.2.2\lib\linuxi586-mingw32msvcDPOpt   echo 6 - mingw-w32 Double Precision (Ubuntu's mingw-w64)
if exist %HOME%\DOFI-2.2.2\lib\linuxamd64-mingw32msvcDPOpt  echo 7 - mingw-w64 Double Precision (Ubuntu's mingw-w64)
if exist %HOME%\DOFI-2.2.2\lib\linuxi686-w64-mingw32SPOpt   echo 8 - mingw-w32 Single Precision (sf.net mingw-w64)
if exist %HOME%\DOFI-2.2.2\lib\linuxi686-w64-mingw32DPOpt   echo 9 - mingw-w32 Double Precision (sf.net mingw-w64)
if exist %HOME%\DOFI-2.2.2\lib\linuxx86_64-w64-mingw32DPOpt echo 0 - mingw-w64 Double Precision (sf.net mingw-w64)
echo.                               
choice /C:1234567890 "Choose one:"
IF ERRORLEVEL 10 goto ww64_x64
IF ERRORLEVEL 9 goto ww32_686
IF ERRORLEVEL 8 goto ww32S_686
IF ERRORLEVEL 7 goto mw64
IF ERRORLEVEL 6 goto mw32
IF ERRORLEVEL 5 goto mw32S
IF ERRORLEVEL 4 goto ww64
IF ERRORLEVEL 3 goto ww32D
IF ERRORLEVEL 2 goto ww32S
IF ERRORLEVEL 1 goto w32


:w32
set WM_COMPILER=mingw32
set WM_PRECISION_OPTION=DP
set WM_ARCH_OPTION=32
echo "Setting environment for mingw32 Double Precision"
goto cont

:ww32S
set WM_COMPILER=mingw-w32
set WM_PRECISION_OPTION=SP
set WM_ARCH_OPTION=32
echo "Setting environment for mingw-w32 Single Precision (custom mingw-w64)"
goto cont

:ww32D
set WM_COMPILER=mingw-w32
set WM_PRECISION_OPTION=DP
set WM_ARCH_OPTION=32
echo "Setting environment for mingw-w32 Double Precision (custom mingw-w64)"
goto cont

:ww64
set WM_COMPILER=mingw-w64
set WM_PRECISION_OPTION=DP
set WM_ARCH_OPTION=64
echo "Setting environment for mingw-w64 Double Precision (custom mingw-w64)"
goto cont

:mw32S
set WM_COMPILER=i586-mingw32msvc
set WM_PRECISION_OPTION=SP
set WM_ARCH_OPTION=32
echo "Setting environment for mingw-w32 Single Precision (Ubuntu's mingw-w64)"
goto cont

:mw32
set WM_COMPILER=i586-mingw32msvc
set WM_PRECISION_OPTION=DP
set WM_ARCH_OPTION=32
echo "Setting environment for mingw-w32 Double Precision (Ubuntu's mingw-w64)"
goto cont

:mw32_x64
set WM_COMPILER=amd64-mingw32msvc
set WM_PRECISION_OPTION=DP
set WM_ARCH_OPTION=64
echo "Setting environment for mingw-w64 Double Precision (Ubuntu's mingw-w64)"
goto cont

:ww32S_686
set WM_COMPILER=i686-w64-mingw32
set WM_PRECISION_OPTION=SP
set WM_ARCH_OPTION=32
echo "Setting environment for mingw-w32 Single Precision (sf.net mingw-w64)"
goto cont

:ww32_686
set WM_COMPILER=i686-w64-mingw32
set WM_PRECISION_OPTION=DP
set WM_ARCH_OPTION=32
echo "Setting environment for mingw-w32 Double Precision (sf.net mingw-w64)"
goto cont

:ww64_x64
set WM_COMPILER=x86_64-w64-mingw32
set WM_PRECISION_OPTION=DP
set WM_ARCH_OPTION=64
echo Setting up "OpenFOAM for Windows" environment (mingw-w64 double precision)
goto cont

:cont
set jumpto=
set countver=

rem BEGGINING of User Modifiables --------------------------------------

rem PICK ONE from these:
rem   i686-w64-mingw32, x86_64-w64-mingw32  - using the compiler from mingw-w64 at sourceforge.net
rem   mingw32, mingw-w32, mingw-w64         - using the compiled mingw cross-compiler
rem   i586-mingw32msvc, amd64-mingw32msvc   - attempt to use Ubuntu's pre-built mingw-w64 binaries, but the 64bit version only worked in Windows XP x64
rem set WM_COMPILER=i686-w64-mingw32
rem set WM_COMPILER=x86_64-w64-mingw32
rem set WM_COMPILER=mingw32
rem set WM_COMPILER=mingw-w32
set WM_COMPILER=mingw-w64
rem set WM_COMPILER=i586-mingw32msvc
rem set WM_COMPILER=amd64-mingw32msvc

rem PICK ONE: dummy or MPICH or MSMPI
rem set WM_MPLIB=dummy
set WM_MPLIB=MSMPI

rem PICK ONE: in case you want to set a fixed precision
set WM_PRECISION_OPTION=DP
rem set WM_PRECISION_OPTION=SP

rem END OF USER MODIFIABLES ---------------------------------------------

set USER=%USERNAME%
set WM_PROJECT=DOFI
set WM_PROJECT_VERSION=2.2.2
set FOAM_INST_DIR=%HOME%
rem set FOAM_INST_DIR=%HOME%\%WM_PROJECT%
set WM_PROJECT_INST_DIR=%FOAM_INST_DIR%
set WM_PROJECT_DIR=%FOAM_INST_DIR%\%WM_PROJECT%-%WM_PROJECT_VERSION%
set WM_PROJECT_USER_DIR=%HOME%\%USER%-%WM_PROJECT_VERSION%
set WM_THIRD_PARTY_DIR=%WM_PROJECT_INST_DIR%\Lib-%WM_PROJECT_VERSION%
set WM_OS=MSwindows

set WM_ARCH=linux64
set WM_COMPILE_OPTION=Opt
set WM_JAVAC_OPTION=Opt

set FOAM_SIGFPE=""
set WM_COMPILER_ARCH=""
set WM_COMPILER_LIB_ARCH=""
set WM_CC=%WM_COMPILER%-gcc
set WM_CXX=%WM_COMPILER%-g++
set FOAM_JOB_DIR=%FOAM_INST_DIR%\jobControl
set WM_DIR=%WM_PROJECT_DIR%\wmake
set WM_LINK_LANGUAGE=c++
set WM_OPTIONS=%WM_ARCH%%WM_COMPILER%%WM_PRECISION_OPTION%%WM_COMPILE_OPTION%

set FOAM_SRC=%WM_PROJECT_DIR%\src
set FOAM_LIB=%WM_PROJECT_DIR%\lib
set FOAM_LIBBIN=%FOAM_LIB%\%WM_OPTIONS%
set FOAM_APP=%WM_PROJECT_DIR%\applications
set FOAM_APPBIN=%WM_PROJECT_DIR%\applications\bin\%WM_OPTIONS%

set FOAM_TUTORIALS=%WM_PROJECT_DIR%\tutorials
set FOAM_UTILITIES=%FOAM_APP%\utilities
set FOAM_SOLVERS=%FOAM_APP%\solvers
set FOAM_USER_LIBBIN=%WM_PROJECT_USER_DIR%\lib\%WM_OPTIONS%
set FOAM_USER_APPBIN=%WM_PROJECT_USER_DIR%\applications\bin\%WM_OPTIONS%
set FOAM_RUN=%WM_PROJECT_USER_DIR%\run

IF "%WM_MPLIB%"=="dummy" set mpi_version=dummy
IF "%WM_MPLIB%"=="MPICH" set mpi_version=mpich2-1.4.1p1
IF "%WM_MPLIB%"=="MSMPI" set mpi_version=msmpi

rem TODO: change MPI HOME and MPI ARCH PATH
rem set MPI_HOME=%WM_THIRD_PARTY_DIR%\%mpi_version%
rem set MPI_ARCH_PATH=%WM_THIRD_PARTY_DIR%\platforms\%WM_ARCH%%WM_COMPILER%\%mpi_version%
rem set MPI_ARCH_PATH=%WM_THIRD_PARTY_DIR%\%mpi_version%

rem set MPICH_ROOT=%MPI_ARCH_PATH%
rem set MPICH_PORT=41000
set MPI_BUFFER_SIZE=20000000
set FOAM_MPI_LIBBIN=%FOAM_LIBBIN%\%mpi_version%

@rem TODO: use detected paraview info
REM Paraview win 64
rem set ParaView_VERSION=4.2.0
rem set ParaView_INST_DIR=%WM_THIRD_PARTY_DIR%\paraview-%ParaView_VERSION%
rem set ParaView_DIR=%WM_THIRD_PARTY_DIR%\ParaView-%ParaView_VERSION%
rem set PYTHONPATH=%ParaView_DIR%\lib\paraview-4.2;%ParaView_DIR%\lib\paraview-4.2\site-packages


set PATH=%FOAM_MPI_LIBBIN%;%FOAM_JOB_DIR%;%WM_PROJECT_USER_DIR%\lib\%WM_OPTIONS%;%WM_PROJECT_USER_DIR%\applications\bin\%WM_OPTIONS%;%FOAM_APPBIN%;%FOAM_LIBBIN%;%WM_DIR%;%WM_PROJECT_DIR%\bin;%DOFIROOT%

%~d1
rem %1 %2 %3 %4 %5 %6 %7 %8 %9

cd %LWD%