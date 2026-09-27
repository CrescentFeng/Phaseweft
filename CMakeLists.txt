cmake_minimum_required(VERSION 3.22)
project(Phaseweft VERSION 0.6.0 LANGUAGES C CXX)

option(MOTION_FIELD_BUILD_PLUGIN "Build JUCE VST3/AU/Standalone targets" ON)
option(MOTION_FIELD_ENABLE_ASAN "Enable AddressSanitizer for core tests" OFF)
option(MOTION_FIELD_FETCH_JUCE "Fetch the pinned JUCE release when no local checkout is provided" ON)
set(MOTION_FIELD_JUCE_DIR "" CACHE PATH "Optional path to a local JUCE checkout")
set(MOTION_FIELD_JUCE_VERSION "9.0.2" CACHE STRING "Pinned JUCE release used by FetchContent")
set(MOTION_FIELD_JUCE_URL
  "https://github.com/juce-framework/JUCE/archive/refs/tags/${MOTION_FIELD_JUCE_VERSION}.tar.gz"
  CACHE STRING "URL of the pinned JUCE source archive")
set(MOTION_FIELD_JUCE_SHA256
  "16d01c27e8327f3644306cc5c223aa6e21af1cc1ec62faa709fc0dced57d208a"
  CACHE STRING "SHA-256 of the pinned JUCE source archive")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(motion_field_core INTERFACE)
target_include_directories(motion_field_core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Source/DSP)

if(MSVC)
  target_compile_options(motion_field_core INTERFACE /W4 /permissive-)
else()
  target_compile_options(motion_field_core INTERFACE -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
endif()

if(MOTION_FIELD_ENABLE_ASAN AND NOT MSVC)
  target_compile_options(motion_field_core INTERFACE -fsanitize=address,undefined -fno-omit-frame-pointer)
  target_link_options(motion_field_core INTERFACE -fsanitize=address,undefined)
endif()

enable_testing()
add_executable(motion_field_tests tests/MotionFieldTests.cpp)
target_link_libraries(motion_field_tests PRIVATE motion_field_core)
add_test(NAME motion_field_core COMMAND motion_field_tests)

add_executable(motion_field_demo tests/RenderDemo.cpp)
target_link_libraries(motion_field_demo PRIVATE motion_field_core)

add_executable(motion_field_benchmark tests/Benchmark.cpp)
target_link_libraries(motion_field_benchmark PRIVATE motion_field_core)

add_executable(motion_field_preset_audit tests/PresetAudit.cpp)
target_link_libraries(motion_field_preset_audit PRIVATE motion_field_core)
add_test(NAME motion_field_presets COMMAND motion_field_preset_audit)

add_executable(motion_field_long_run tests/LongRunTests.cpp)
target_link_libraries(motion_field_long_run PRIVATE motion_field_core)
add_test(NAME motion_field_long_run COMMAND motion_field_long_run)
set_tests_properties(motion_field_long_run PROPERTIES LABELS "extended")

if(MOTION_FIELD_BUILD_PLUGIN)
  if(MOTION_FIELD_JUCE_DIR)
    if(NOT EXISTS "${MOTION_FIELD_JUCE_DIR}/CMakeLists.txt")
      message(FATAL_ERROR
        "MOTION_FIELD_JUCE_DIR does not contain a JUCE CMakeLists.txt: "
        "${MOTION_FIELD_JUCE_DIR}")
    endif()
    add_subdirectory("${MOTION_FIELD_JUCE_DIR}" "${CMAKE_BINARY_DIR}/JUCE" EXCLUDE_FROM_ALL)
  elseif(MOTION_FIELD_FETCH_JUCE)
    include(FetchContent)
    FetchContent_Declare(JUCE
      URL "${MOTION_FIELD_JUCE_URL}"
      URL_HASH "SHA256=${MOTION_FIELD_JUCE_SHA256}"
      DOWNLOAD_EXTRACT_TIMESTAMP FALSE)
    FetchContent_MakeAvailable(JUCE)
  else()
    message(FATAL_ERROR
      "Plug-in targets require JUCE. Set MOTION_FIELD_JUCE_DIR or enable "
      "MOTION_FIELD_FETCH_JUCE.")
  endif()
  add_subdirectory(Source/Plugin)
endif()
