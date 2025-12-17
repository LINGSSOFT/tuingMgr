param(
    [string]$Configuration = "Debug",
    [string]$Platform = "x64"
)

$ErrorActionPreference = "Stop"

$vswhere = "C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe"
if (-not (Test-Path $vswhere)) {
    throw "vswhere.exe not found. Install Visual Studio Build Tools 2022 or update .vscode/build.ps1 with the correct path."
}

$vsPath = & $vswhere -latest -products * -requires Microsoft.Component.MSBuild -property installationPath
if (-not $vsPath) {
    throw "MSBuild not found. Install Visual Studio Build Tools 2022 or a full Visual Studio instance."
}

$msbuild = Join-Path $vsPath "MSBuild\Current\Bin\MSBuild.exe"
$devCmd = Join-Path $vsPath "Common7\Tools\VsDevCmd.bat"

# VsDevCmd expects x86 for Win32 builds.
$devArch = if ($Platform -eq "Win32") { "x86" } else { $Platform }

$solution = Join-Path $PSScriptRoot "..\TuingMgr.sln"
$cmdLine = "`"$devCmd`" -no_logo -arch=$devArch -host_arch=x64 && `"$msbuild`" `"$solution`" /p:Configuration=$Configuration /p:Platform=$Platform /m"
cmd /c $cmdLine
