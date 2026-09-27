$ErrorActionPreference = "Stop"

$ProjectDir = Split-Path -Parent (Split-Path -Parent $MyInvocation.MyCommand.Path)
$BuildDir = if ($env:MOTION_FIELD_BUILD_DIR) { $env:MOTION_FIELD_BUILD_DIR } else { Join-Path $ProjectDir "build/release-windows" }

$CMakeArgs = @("-S", $ProjectDir, "-B", $BuildDir, "-DMOTION_FIELD_BUILD_PLUGIN=ON")
if ($env:MOTION_FIELD_JUCE_DIR) {
  $CMakeArgs += "-DMOTION_FIELD_JUCE_DIR=$env:MOTION_FIELD_JUCE_DIR"
}
cmake @CMakeArgs
cmake --build $BuildDir --config Release --parallel
ctest --test-dir $BuildDir --output-on-failure

Write-Host "Release build complete: $BuildDir"
