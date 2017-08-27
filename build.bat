
set PATH=%cd%\mingw32\bin\;%PATH%

echo ***********************
echo * MinGW Makefiles
echo ***********************

cd build_MinGW

..\cmake\bin\cmake.exe -G "MinGW Makefiles"^
						-D"CMAKE_MAKE_PROGRAM:PATH=mingw32-make.exe"						

..\mingw32\bin\mingw32-make.exe

cd..


echo ***********************
echo * Visual Studio 14
echo ***********************

cd build_MSVC
set PATH=C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;%PATH%

..\cmake\bin\cmake -G "Visual Studio 14"
echo Loading Microsoft Visual Studio 14.0 environment 
 
CALL "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86_amd64
msbuild priority_queue.vcxproj
  

cd ..