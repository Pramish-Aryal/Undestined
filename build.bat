@echo off

setlocal

set SourceFiles=../code/*.cpp
set OutputName=Undestined.exe

set CLANGFlags=-O2 -gcodeview

if "%1" neq "debug" goto DoneConfig
set CLANGFlags=-g -gcodeview
echo DEBUG Build configured
:DoneConfig

if not exist "code/extern" mkdir "code/extern"
if not exist "libs" mkdir libs
if exist "libs/SDL2" goto SkipDownloadSDL

pushd libs
mkdir SDL2
curl "https://www.libsdl.org/release/SDL2-devel-2.0.14-VC.zip" --output SDL2.zip
tar -zxvf SDL2.zip -C SDL2
del SDL2.zip
ren "SDL2\SDL2-2.0.14\include" "SDL2"
popd

:SkipDownloadSDL

pushd "code/extern"
curl "https://raw.githubusercontent.com/nothings/stb/master/stb_image.h" --output stb_image.h
popd

set Header_Includes="../code/headers/"
set Extern_Include="../code/extern/"
set SDL2_Include="../libs/SDL2/SDL2-2.0.14/"
set SDL2_Library="../libs/SDL2/SDL2-2.0.14/lib/x64/"
set SDL2_DLL="../libs\SDL2\SDL2-2.0.14\lib\x64\SDL2.dll"

if not exist "build" mkdir build
where clang >nul 2>nul
IF %ERRORLEVEL% NEQ 0 goto SkipCLANG
echo Building with CLANG
pushd build
xcopy %SDL2_DLL% .\ /Y
call clang -I%SDL2_Include% -I%Extern_Include% -I%Header_Includes% -L%SDL2_Library% %CLANGFlags% %SourceFiles% -o %OutputName% -lSDL2main -lSDL2 -lShell32 -Xlinker -subsystem:windows
popd
goto Finished
:SkipCLANG
echo CLANG not found, please setup clang.
:Finished
xcopy "data" "build/data" /E /C /I
echo -------------------
