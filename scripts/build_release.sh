#!/bin/zsh
set -euo pipefail

project_dir="${0:A:h:h}"
build_dir="${MOTION_FIELD_BUILD_DIR:-${project_dir}/build/release-macos-universal}"
archs="${MOTION_FIELD_MACOS_ARCHS:-arm64;x86_64}"

cmake_args=(-S "${project_dir}" -B "${build_dir}" -G "Unix Makefiles" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_OSX_ARCHITECTURES="${archs}" \
  -DMOTION_FIELD_BUILD_PLUGIN=ON)
if [[ -n "${MOTION_FIELD_JUCE_DIR:-}" ]]; then
  cmake_args+=("-DMOTION_FIELD_JUCE_DIR=${MOTION_FIELD_JUCE_DIR}")
fi
cmake "${cmake_args[@]}"
cmake --build "${build_dir}" --config Release --parallel
ctest --test-dir "${build_dir}" --output-on-failure

if [[ "${MOTION_FIELD_ADHOC_SIGN:-1}" == "1" ]]; then
  bundles=(
    "${build_dir}/Source/Plugin/MotionField_artefacts/Release/Standalone/Phaseweft.app"
    "${build_dir}/Source/Plugin/MotionField_artefacts/Release/VST3/Phaseweft.vst3"
    "${build_dir}/Source/Plugin/MotionField_artefacts/Release/AU/Phaseweft.component"
  )
  for bundle in "${bundles[@]}"; do
    if [[ -d "${bundle}" ]]; then
      /usr/bin/codesign --force --deep --sign - --options runtime "${bundle}"
      /usr/bin/codesign --verify --deep --strict "${bundle}"
    fi
  done
fi

print "Release build complete: ${build_dir}"
