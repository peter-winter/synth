cmake_minimum_required(VERSION 3.14)
project(synth_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ------------------------------------------------------------------
# miniaudio
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

# ------------------------------------------------------------------
# Auto-collect all example sources except main.cpp
# ------------------------------------------------------------------
file(GLOB EXAMPLE_SOURCES 
    CONFIGURE_DEPENDS
    "examples/*.cpp"
)

# Remove the dispatcher from the list
list(FILTER EXAMPLE_SOURCES EXCLUDE REGEX "examples/main.cpp$")

# ------------------------------------------------------------------
# Single unified binary
# ------------------------------------------------------------------
add_executable(synth_demo
    examples/main.cpp
    ${EXAMPLE_SOURCES}
)

target_include_directories(synth_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(synth_demo PRIVATE miniaudio_lib pthread dl)


