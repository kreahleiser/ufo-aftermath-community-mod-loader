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

cl /nologo /O2 /LD /EHsc ufo_display.cpp gl_forwards.cpp /Fe:opengl32.dll /link /DEF:opengl32.def /DLL /INCREMENTAL:NO user32.lib kernel32.lib gdi32.lib winmm.lib
if errorlevel 1 exit /b 1

echo Built %cd%\opengl32.dll
dir opengl32.dll
