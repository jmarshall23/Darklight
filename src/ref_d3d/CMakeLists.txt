cmake_minimum_required(VERSION 3.10)

# Project name
project(ref_d3d)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define configurations
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;DebugAlpha;ReleaseAlpha")

# Define the DLL target
add_library(ref_d3d SHARED "d3d_main.cpp" "tinydx.cpp" "d3d_2d.cpp" "d3d_frame.cpp" "d3d_image.cpp" "d3d_model.cpp" "window.cpp")

# Link with necessary libraries
target_link_libraries(ref_d3d 
    d3d12
    winmm
    gdi32
    user32
)

# Including the .def file for symbol exports
target_link_options(ref_d3d PRIVATE 
   /DEF:${CMAKE_CURRENT_SOURCE_DIR}/ref_d3d.def
)

# Compiler options and definitions for different configurations
target_compile_definitions(ref_d3d PRIVATE
    $<$<CONFIG:Debug>:WIN32;_DEBUG;_WINDOWS>
    $<$<CONFIG:Release>:WIN32;NDEBUG;_WINDOWS>
)

target_compile_options(ref_d3d PRIVATE
    $<$<CONFIG:Debug>:/MTd;/W3;/Gm;/GX;/Zi;/Od>
    $<$<CONFIG:Release>:/MT;/W3;/GX;/O2>
)

# Set the output directory for the DLL and import library
set_target_properties(ref_d3d PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../../../../
)
