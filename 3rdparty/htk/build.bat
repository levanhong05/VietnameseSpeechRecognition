@echo off

echo building HTK for windows

echo setting environment variables;

set PATH=%PATH%;C:\Program Files (x86)\CMake\bin\

mkdir bin.win32

cd HTKLib
nmake /f htk_htklib_nt.mkf all
cd ..

cd HTKTools
nmake /f htk_htktools_nt.mkf all
cd ..

cd HLMLib
nmake /f htk_hlmlib_nt.mkf all
cd ..

cd HLMTools
nmake /f htk_hlmtools_nt.mkf all
cd ..

echo copy HTK tools

mkdir ..\..\dist\bin\HTK

copy bin.win32\*.exe ..\..\dist\bin\HTK\

exit 0
