#!/bin/zsh
set -euo pipefail

project_dir="${0:A:h:h}"
build_dir="${MOTION_FIELD_BUILD_DIR:-${project_dir}/build/release-macos-universal}"
pluginval_bin="${1:-${PLUGINVAL_PATH:-}}"
validation_dir="${project_dir}/release/validation"
vst3_path="${project_dir}/artifacts/plugins/Phaseweft.vst3"
component_path="${project_dir}/artifacts/plugins/Phaseweft.component"
installed_component="${HOME}/Library/Audio/Plug-Ins/Components/Phaseweft.component"

mkdir -p "${validation_dir}"
ctest --test-dir "${build_dir}" --output-on-failure
codesign --verify --deep --strict --verbose=2 "${vst3_path}"
codesign --verify --deep --strict --verbose=2 "${component_path}"

if [[ -z "${pluginval_bin}" || ! -x "${pluginval_bin}" ]]; then
  print -u2 "Pass pluginval as argument 1 or set PLUGINVAL_PATH."
  exit 2
fi

"${pluginval_bin}" --validate-in-process \
  --strictness-level 10 \
  --timeout-ms 120000 \
  --sample-rates 44100,48000,96000,192000 \
  --block-sizes 1,16,64,127,512,1024 \
  --output-dir "${validation_dir}" \
  "${vst3_path}"

if [[ -d "${installed_component}" ]]; then
  /usr/bin/auval -v aufx Pwf1 Crsc
else
  print "AU validation skipped: install the release candidate at ${installed_component} first."
fi
