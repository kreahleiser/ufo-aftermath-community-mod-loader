$ErrorActionPreference = "Stop"
$here = Split-Path -Parent $MyInvocation.MyCommand.Path
$dist = Join-Path $here "dist"
$stage = Join-Path $dist "UFO Aftermath Community Mod Loader"
Remove-Item $dist -Recurse -Force -ErrorAction SilentlyContinue
New-Item -ItemType Directory -Force -Path $stage | Out-Null
Copy-Item (Join-Path $here "opengl32.dll") $stage
Copy-Item (Join-Path $here "ufo_mod_manager.exe") $stage
Copy-Item (Join-Path $here "ufo_mod_setup.exe") $stage
Copy-Item (Join-Path $here "README.md") $stage
Copy-Item (Join-Path $here "LICENSE") $stage
Copy-Item (Join-Path $here "bundled_plugins") (Join-Path $stage "plugins") -Recurse
$zip = Join-Path $dist "ufo-aftermath-community-mod-loader.zip"
$seven = Join-Path $dist "ufo-aftermath-community-mod-loader.7z"
if (Test-Path $zip) { Remove-Item $zip }
if (Test-Path $seven) { Remove-Item $seven }
Compress-Archive -Path (Join-Path $stage "*") -DestinationPath $zip
$z7 = "C:\Program Files\7-Zip\7z.exe"
if (Test-Path $z7) {
  & $z7 a -t7z -mx=9 $seven "$stage\*" | Out-Null
}
Write-Output "Wrote $zip"
if (Test-Path $seven) { Write-Output "Wrote $seven" }
Get-ChildItem $dist | Select-Object Name, Length
