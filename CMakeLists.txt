cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

option(LIB_OPT "Compile shared libraries (ON) or static (OFF)" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

project(csvpp)
add_subdirectory(src)

# option(BUILD_EFG_SAMPLES "Build the samples showing how to use EFG" ON)
# if(BUILD_EFG_SAMPLES)
# 	project(EFG-Samples)
#     add_subdirectory(samples)
# endif()

option(BUILD_CSV_READER_TESTS "" OFF)
if(BUILD_CSV_READER_TESTS)
    add_subdirectory(tests)
endif()
