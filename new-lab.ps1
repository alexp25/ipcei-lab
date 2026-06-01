#Requires -Version 5.1
<#
.SYNOPSIS
    Creates a new lab project from the hello_world template.
.DESCRIPTION
    Copies the template, rewires the __repo__ junction to the shared SDK,
    and patches all hardcoded paths and the project name in config files.
.EXAMPLE
    .\new-lab.ps1 -Dest src\lab2\blinky -Name blinky
.EXAMPLE
    .\new-lab.ps1 -Dest src\lab3\uart_demo -Name uart_demo -Template src\lab1\hello_world
#>
param(
    [Parameter(Mandatory, HelpMessage = "Destination relative to repo root, e.g. src\lab2\blinky")]
    [string]$Dest,

    [Parameter(Mandatory, HelpMessage = "CMake project name, e.g. blinky")]
    [string]$Name,

    [string]$Template = "src\lab1\hello_world"
)

$ErrorActionPreference = "Stop"

$root   = $PSScriptRoot
$src    = Join-Path $root $Template
$dst    = Join-Path $root $Dest
$sdkNew = Join-Path $root "src\sdks\mcuxsdk"

# ── Validate inputs ───────────────────────────────────────────────────────────
if (-not (Test-Path $src))  { throw "Template not found: $src" }
if (-not (Test-Path $sdkNew)) { throw "Shared SDK not found: $sdkNew" }
if (Test-Path $dst)           { throw "Destination already exists: $dst" }
if ($Name -notmatch '^[a-zA-Z_][a-zA-Z0-9_]*$') {
    throw "Name must be a valid C identifier (letters, digits, underscores, no spaces)"
}

# Detect old SDK path from the template's mcux_include.json
$presetsRaw = [IO.File]::ReadAllText("$src\mcux_include.json")
if ($presetsRaw -notmatch '"SdkRootDirPath":\s*"([^"]+)"') {
    throw "Cannot detect SDK path in $src\mcux_include.json"
}
$sdkOld = $Matches[1].Replace('/', '\')   # normalise to backslashes
$oldName = Split-Path $src -Leaf

Write-Host ""
Write-Host "Template : $Template"
Write-Host "Dest     : $Dest"
Write-Host "Name     : $Name"
Write-Host "SDK old  : $sdkOld"
Write-Host "SDK new  : src\sdks\mcuxsdk"
Write-Host ""

# ── 1. Copy files (skip the __repo__ junction and debug build folder) ─────────
Write-Host "Copying files..."
robocopy $src $dst /E /XJ /XD debug /NFL /NDL /NJH /NJS | Out-Null
Get-ChildItem $dst -Recurse -Filter "*.bak" | Remove-Item -Force
$processed = Join-Path $dst "cfg_tools\cfg_require.json.processed"
if (Test-Path $processed) { Remove-Item $processed -Force }

# ── 2. Re-create __repo__ junction pointing at the shared SDK ─────────────────
$junction = Join-Path $dst "__repo__"
if (Test-Path $junction) { cmd /c "rmdir `"$junction`"" | Out-Null }
cmd /c "mklink /J `"$junction`" `"$sdkNew`"" | Out-Null
Write-Host "Junction : __repo__ -> src\sdks\mcuxsdk"

# ── 3. Patch config files ─────────────────────────────────────────────────────
# Helper: replace a path string handling both C: and c: drive-letter casing,
# and both forward-slash and double-backslash (JSON) variants.
function Replace-Path([string]$text, [string]$oldAbs, [string]$newAbs) {
    # Build variants: forward-slash and double-backslash, uppercase and lowercase drive
    foreach ($driveSrc in @('C:', 'c:')) {
        foreach ($driveDst in @('C:', 'c:')) {
            # Only replace when same drive casing on both sides
            if ($driveSrc[0] -ne $driveDst[0]) { continue }
        }
        $driveDst = $driveSrc   # keep replacement drive letter matching original

        $oldFwd  = $driveSrc + $oldAbs.Substring(2).Replace('\', '/')
        $newFwd  = $driveDst + $newAbs.Substring(2).Replace('\', '/')
        $oldBack = $driveSrc + $oldAbs.Substring(2).Replace('\', '\\')
        $newBack = $driveDst + $newAbs.Substring(2).Replace('\', '\\')

        $text = $text.Replace($oldFwd,  $newFwd)
        $text = $text.Replace($oldBack, $newBack)
    }
    return $text
}

$patchFiles = @(
    "CMakeLists.txt",
    "mcux_include.json",
    ".vscode\mcuxpresso-tools.json",
    "cfg_tools\project_info.json",
    "cfg_tools\cfg_require.json"
)

foreach ($rel in $patchFiles) {
    $file = Join-Path $dst $rel
    if (-not (Test-Path $file)) { continue }

    $text = [IO.File]::ReadAllText($file, [Text.Encoding]::UTF8)

    $text = Replace-Path $text $sdkOld $sdkNew   # SDK path
    $text = Replace-Path $text $src    $dst       # project root path
    $text = $text.Replace($oldName, $Name)        # project name

    [IO.File]::WriteAllText($file, $text, [Text.Encoding]::UTF8)
    Write-Host "Patched  : $rel"
}

Write-Host ""
Write-Host "Done. Open $Dest in MCUXpresso to build."
