@echo off

echo building log4cpp for windows

echo setting environment variables;

set PATH=%PATH%;C:\Program Files (x86)\CMake\bin\

cd msvc12

msbuild /P:Configuration=Release log4cppLIB/log4cppLIB.vcxproj

msbuild /P:Configuration=Release log4cpp/log4cpp.vcxproj

copy log4cpp\x64\Release\log4cpp.* ..\..\..\dist\bin\

exit 0
