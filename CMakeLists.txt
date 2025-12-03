cmake_minimum_required(VERSION 3.14)
project(synth_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ------------------------------------------------------------------
# miniaudio (header-only)
# ------------------------------------------------------------------
include(FetchContent)
FetchContent_Declare(
    miniaudio
    GIT_REPOSITORY https://github.com/mackron/miniaudio.git
    GIT_TAG        0.11.21
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(miniaudio)

add_library(miniaudio_lib INTERFACE)
target_include_directories(miniaudio_lib INTERFACE ${miniaudio_SOURCE_DIR})

add_compile_definitions(
    MA_NO_DECODING
    MA_NO_ENCODING
    MA_NO_GENERATION
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# ------------------------------------------------------------------
# Automatically find and build ALL examples
# ------------------------------------------------------------------
file(GLOB EXAMPLE_SOURCES
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "examples/*.cpp"
)

foreach(example_path ${EXAMPLE_SOURCES})
    get_filename_component(example_name ${example_path} NAME_WE)  # strip .cpp
    add_executable(${example_name} ${example_path})
    target_link_libraries(${example_name} PRIVATE miniaudio_lib pthread dl)
    message(STATUS "Added example: ${example_name}")
endforeach()
