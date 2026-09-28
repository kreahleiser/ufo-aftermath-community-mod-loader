@echo off
setlocal EnableExtensions
cd /d "%~dp0"

set "VCVARS="
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if exist "%VSWHERE%" (
  for /f "usebackq delims=" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do (
    set "VCVARS=%%i\VC\Auxiliary\Build\vcvarsall.bat"
  )
)
if not defined VCVARS (
  echo Could not find Visual Studio with the x86 C++ toolset.
  exit /b 1
)

call "%VCVARS%" x86
if errorlevel 1 exit /b 1

set LUA=third_party\lua-5.1.5\src
set MINIZ=third_party\miniz-3.0.2

echo Compiling Lua 5.1...
cl /nologo /c /O2 /W0 /I%LUA% ^
  %LUA%\lapi.c %LUA%\lauxlib.c %LUA%\lbaselib.c %LUA%\lcode.c %LUA%\ldblib.c ^
  %LUA%\ldebug.c %LUA%\ldo.c %LUA%\ldump.c %LUA%\lfunc.c %LUA%\lgc.c %LUA%\linit.c ^
  %LUA%\liolib.c %LUA%\llex.c %LUA%\lmathlib.c %LUA%\lmem.c %LUA%\loadlib.c ^
  %LUA%\lobject.c %LUA%\lopcodes.c %LUA%\loslib.c %LUA%\lparser.c %LUA%\lstate.c ^
  %LUA%\lstring.c %LUA%\lstrlib.c %LUA%\ltable.c %LUA%\ltablib.c %LUA%\ltm.c ^
  %LUA%\lundump.c %LUA%\lvm.c %LUA%\lzio.c
if errorlevel 1 exit /b 1

echo Compiling miniz...
cl /nologo /c /O2 /W0 /I%MINIZ% /DMINIZ_NO_STDIO /DMINIZ_NO_ARCHIVE_APIS /DMINIZ_NO_TIME ^
  %MINIZ%\miniz.c %MINIZ%\miniz_tinfl.c %MINIZ%\miniz_tdef.c
if errorlevel 1 exit /b 1

echo Compiling helper...
cl /nologo /O2 /LD /EHsc /I%LUA% /I%MINIZ% ufo_display.cpp gl_forwards.cpp vfs.cpp fontpad.cpp modloader.cpp ^
  lapi.obj lauxlib.obj lbaselib.obj lcode.obj ldblib.obj ldebug.obj ldo.obj ldump.obj ^
  lfunc.obj lgc.obj linit.obj liolib.obj llex.obj lmathlib.obj lmem.obj loadlib.obj ^
  lobject.obj lopcodes.obj loslib.obj lparser.obj lstate.obj lstring.obj lstrlib.obj ^
  ltable.obj ltablib.obj ltm.obj lundump.obj lvm.obj lzio.obj ^
  miniz.obj miniz_tinfl.obj miniz_tdef.obj ^
  /Fe:opengl32.dll /link /DEF:opengl32.def /DLL /INCREMENTAL:NO user32.lib kernel32.lib gdi32.lib winmm.lib
if errorlevel 1 exit /b 1

echo Compiling bake_test...
cl /nologo /O2 /EHsc /I%LUA% /I%MINIZ% bake_test.cpp vfs.cpp fontpad.cpp modloader.cpp ^
  lapi.obj lauxlib.obj lbaselib.obj lcode.obj ldblib.obj ldebug.obj ldo.obj ldump.obj ^
  lfunc.obj lgc.obj linit.obj liolib.obj llex.obj lmathlib.obj lmem.obj loadlib.obj ^
  lobject.obj lopcodes.obj loslib.obj lparser.obj lstate.obj lstring.obj lstrlib.obj ^
  ltable.obj ltablib.obj ltm.obj lundump.obj lvm.obj lzio.obj ^
  miniz.obj miniz_tinfl.obj miniz_tdef.obj ^
  /Fe:bake_test.exe /link /INCREMENTAL:NO user32.lib kernel32.lib
if errorlevel 1 exit /b 1

echo Compiling mod manager...
cl /nologo /O2 /EHsc mod_manager.cpp /Fe:ufo_mod_manager.exe /link /SUBSYSTEM:WINDOWS /INCREMENTAL:NO user32.lib kernel32.lib gdi32.lib comctl32.lib shell32.lib
if errorlevel 1 exit /b 1

echo Built %cd%\opengl32.dll
dir opengl32.dll bake_test.exe ufo_mod_manager.exe
