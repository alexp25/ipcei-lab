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
if (-not (Test-Path $src))    { throw "Template not found: $src" }
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

# Detect the main source file name used in the template root.
# Templates cloned from SDK demos often keep the demo's source name
# (e.g. "led_blinky") even when the project folder has a different name.
$boardFileNames = @('board', 'clock_config', 'pin_mux', 'peripherals', 'hardware_init')
$templateMainC  = Get-ChildItem $src -MaxDepth 1 -Filter "*.c" |
    Where-Object { $_.BaseName -notin $boardFileNames } |
    Select-Object -First 1
$srcSourceName = if ($templateMainC) { $templateMainC.BaseName } else { $oldName }

Write-Host ""
Write-Host "Template : $Template"
Write-Host "Dest     : $Dest"
Write-Host "Name     : $Name"
Write-Host "SDK old  : $sdkOld"
Write-Host "SDK new  : src\sdks\mcuxsdk"
if ($srcSourceName -ne $oldName) {
    Write-Host "Source   : $srcSourceName -> $Name (template source file name differs from folder name)"
}
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

# ── 3. Rename source files and directories to the new project name ─────────────
# We rename both the template folder name AND the source file name (if different).
$namesToRename = [System.Collections.Generic.List[string]]::new()
if ($srcSourceName -ne $Name) { $namesToRename.Add($srcSourceName) }
if ($oldName -ne $Name -and $oldName -ne $srcSourceName) { $namesToRename.Add($oldName) }

foreach ($from in $namesToRename) {
    # Root .c  (e.g. led_blinky.c -> uart_demo.c)
    $f = Join-Path $dst "$from.c"
    if (Test-Path $f) { Rename-Item $f "$Name.c"; Write-Host "Renamed  : $from.c -> $Name.c" }

    # Root .mex
    $f = Join-Path $dst "$from.mex"
    if (Test-Path $f) { Rename-Item $f "$Name.mex"; Write-Host "Renamed  : $from.mex -> $Name.mex" }

    # Root source subdirectory (e.g. led_blinky\ -> uart_demo\)
    $f = Join-Path $dst $from
    if (Test-Path $f -PathType Container) { Rename-Item $f $Name; Write-Host "Renamed  : $from\ -> $Name\" }

    # Board-specific source subdirectory  (e.g. frdmmcxa153\led_blinky\)
    $f = Join-Path $dst "frdmmcxa153\$from"
    if (Test-Path $f -PathType Container) { Rename-Item $f $Name; Write-Host "Renamed  : frdmmcxa153\$from\ -> frdmmcxa153\$Name\" }

    # Board-specific .mex  (e.g. frdmmcxa153\led_blinky.mex)
    $f = Join-Path $dst "frdmmcxa153\$from.mex"
    if (Test-Path $f) { Rename-Item $f "$Name.mex"; Write-Host "Renamed  : frdmmcxa153\$from.mex -> frdmmcxa153\$Name.mex" }
}

# ── 4. Patch config files ─────────────────────────────────────────────────────
# Helper: replace a path string handling both C: and c: drive-letter casing,
# and both forward-slash and double-backslash (JSON) variants.
function Replace-Path([string]$text, [string]$oldAbs, [string]$newAbs) {
    foreach ($drive in @('C:', 'c:')) {
        $oldFwd  = $drive + $oldAbs.Substring(2).Replace('\', '/')
        $newFwd  = $drive + $newAbs.Substring(2).Replace('\', '/')
        $oldBack = $drive + $oldAbs.Substring(2).Replace('\', '\\')
        $newBack = $drive + $newAbs.Substring(2).Replace('\', '\\')

        $text = $text.Replace($oldFwd,  $newFwd)
        $text = $text.Replace($oldBack, $newBack)
    }
    return $text
}

$patchFiles = @(
    "CMakeLists.txt",
    "CMakePresets.json",
    "example.yml",
    "mcux_include.json",
    ".vscode\launch.json",
    ".vscode\mcuxpresso-tools.json",
    "frdmmcxa153\board_files.cmake",
    "cfg_tools\project_info.json",
    "cfg_tools\cfg_require.json",
    "$Name.mex",
    "frdmmcxa153\$Name.mex"
)

foreach ($rel in $patchFiles) {
    $file = Join-Path $dst $rel
    if (-not (Test-Path $file)) { continue }

    $text = [IO.File]::ReadAllText($file, [Text.Encoding]::UTF8)

    $text = Replace-Path $text $sdkOld $sdkNew   # SDK path
    $text = Replace-Path $text $src    $dst       # project root path

    # Source file name -> new name (before template folder name, to avoid double-replace)
    if ($srcSourceName -ne $Name) { $text = $text.Replace($srcSourceName, $Name) }
    # Template folder name -> new name
    if ($oldName -ne $Name)       { $text = $text.Replace($oldName, $Name) }

    # Remove any residual "demo_apps/<name>" board-port-path segment from CMakeLists
    $text = $text -replace "demo_apps/$([regex]::Escape($Name))", $Name

    # CMakePresets.json ships with board="" which prevents CMake configure from
    # finding CMAKE_PROJECT_NAME; fill in the board ID that the rest of the
    # project already assumes (frdmmcxa153).
    if ($rel -eq "CMakePresets.json") {
        $text = $text -replace '"board":\s*""', '"board": "frdmmcxa153"'
    }

    # launch.json ships with elf="" which causes GDB to disconnect immediately.
    if ($rel -eq ".vscode\launch.json") {
        $text = $text -replace '"elf":\s*""', "`"elf`": `"`${workspaceFolder}/debug/$Name.elf`""
    }

    [IO.File]::WriteAllText($file, $text, [Text.Encoding]::UTF8)
    Write-Host "Patched  : $rel"
}

# ── 5. Clear stale project_links in .mex files ────────────────────────────────
# Templates may carry links to unrelated projects; remove them so MCUXpresso
# doesn't try to resolve paths from a different workspace.
foreach ($rel in @("$Name.mex", "frdmmcxa153\$Name.mex")) {
    $file = Join-Path $dst $rel
    if (-not (Test-Path $file)) { continue }

    $text = [IO.File]::ReadAllText($file, [Text.Encoding]::UTF8)
    $before = $text
    $text = $text -replace '\s*<project_link\b[^/]*/>', ''
    if ($text -ne $before) { Write-Host "Cleared  : project_links in $rel" }
    [IO.File]::WriteAllText($file, $text, [Text.Encoding]::UTF8)
}

Write-Host ""
Write-Host "Done. Open $Dest in MCUXpresso to build."
