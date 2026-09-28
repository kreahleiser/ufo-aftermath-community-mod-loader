# Regenerates gl_forwards.cpp and opengl32.def from the 32-bit system OpenGL.
$ErrorActionPreference = "Stop"
$sysGl = "$env:SystemRoot\SysWOW64\opengl32.dll"
if (-not (Test-Path $sysGl)) { $sysGl = "$env:SystemRoot\System32\opengl32.dll" }

$dumpbin = $null
$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
if (Test-Path $vswhere) {
  $vs = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
  $dumpbin = Get-ChildItem "$vs\VC\Tools\MSVC" -Recurse -Filter dumpbin.exe |
    Where-Object { $_.FullName -match "Hostx86\\x86\\dumpbin.exe$" -or $_.FullName -match "Hostx64\\x86\\dumpbin.exe$" } |
    Select-Object -First 1 -ExpandProperty FullName
}
if (-not $dumpbin) { throw "dumpbin.exe not found" }

$raw = & $dumpbin /EXPORTS $sysGl | Out-String
$exports = @()
foreach ($line in $raw -split "`r?`n") {
  if ($line -match '^\s+(\d+)\s+[0-9A-Fa-f]+\s+[0-9A-Fa-f]+\s+(\S+)\s*$') {
    $exports += [pscustomobject]@{ Ordinal = [int]$Matches[1]; Name = $Matches[2] }
  }
}
if ($exports.Count -lt 300) { throw "expected ~368 OpenGL exports, got $($exports.Count)" }

$hooked = [System.Collections.Generic.HashSet[string]]::new([string[]]@(
  "glClear","glViewport","glScissor","glOrtho","glGetIntegerv",
  "glBindTexture","glTexParameteri","glEnable","glDisable",
  "wglMakeCurrent","wglGetProcAddress"
))

$def = New-Object System.Text.StringBuilder
[void]$def.AppendLine("LIBRARY opengl32")
[void]$def.AppendLine("EXPORTS")
foreach ($e in $exports) {
  if ($hooked.Contains($e.Name)) {
    [void]$def.AppendLine(("    {0} @{1}" -f $e.Name, $e.Ordinal))
  } else {
    [void]$def.AppendLine(("    {0} = tramp_{0} @{1}" -f $e.Name, $e.Ordinal))
  }
}

$cpp = New-Object System.Text.StringBuilder
[void]$cpp.AppendLine("#include <windows.h>")
[void]$cpp.AppendLine("")
[void]$cpp.AppendLine('extern "C" {')
foreach ($e in $exports) {
  if (-not $hooked.Contains($e.Name)) {
    [void]$cpp.AppendLine(("    FARPROC p_{0};" -f $e.Name))
  }
}
[void]$cpp.AppendLine("}")
[void]$cpp.AppendLine("")
foreach ($e in $exports) {
  if (-not $hooked.Contains($e.Name)) {
    [void]$cpp.AppendLine(('extern "C" __declspec(naked) void tramp_{0}() {{ __asm {{ jmp dword ptr [p_{0}] }} }}' -f $e.Name))
  }
}
[void]$cpp.AppendLine("")
[void]$cpp.AppendLine("void InitForwards(HMODULE real)")
[void]$cpp.AppendLine("{")
foreach ($e in $exports) {
  if (-not $hooked.Contains($e.Name)) {
    [void]$cpp.AppendLine(('    p_{0} = GetProcAddress(real, "{0}");' -f $e.Name))
  }
}
[void]$cpp.AppendLine("}")

$here = Split-Path -Parent $MyInvocation.MyCommand.Path
[System.IO.File]::WriteAllText("$here\opengl32.def", $def.ToString())
[System.IO.File]::WriteAllText("$here\gl_forwards.cpp", $cpp.ToString())
Write-Output "Wrote $($exports.Count) forwards"
