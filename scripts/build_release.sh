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

print "Release build complete: ${build_dir}"
