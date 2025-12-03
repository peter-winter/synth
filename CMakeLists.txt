cmake_minimum_required(VERSION 3.14)
project(synth_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ------------------------------------------------------------------
# Fetch miniaudio
# ------------------------------------------------------------------
include(FetchContent)

FetchContent_Declare(
    miniaudio
    GIT_REPOSITORY https://github.com/mackron/miniaudio.git
    GIT_TAG        0.11.21
    GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(miniaudio)   # ← this populates ${miniaudio_SOURCE_DIR}

# miniaudio is header-only → create an INTERFACE library
add_library(miniaudio_lib INTERFACE)
target_include_directories(miniaudio_lib INTERFACE ${miniaudio_SOURCE_DIR})

# ------------------------------------------------------------------
# Global compile definitions (disable unused miniaudio parts)
# ------------------------------------------------------------------
add_compile_definitions(
    MA_NO_DECODING
    MA_NO_ENCODING
    MA_NO_GENERATION
)

# ------------------------------------------------------------------
# Make src/ visible to all targets (so #include "factory.hpp" or similar works)
# ------------------------------------------------------------------
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# ------------------------------------------------------------------
# Helper to add examples
# ------------------------------------------------------------------
function(add_example name)
    add_executable(${name} examples/${name}.cpp)
    target_link_libraries(${name} PRIVATE miniaudio_lib pthread dl)
endfunction()

# ------------------------------------------------------------------
# Build the three examples
# ------------------------------------------------------------------
add_example(mono_sine)
add_example(stereo_square_copier)
add_example(detuned_dminor_stereo)
add_example(sine_volume_square)
