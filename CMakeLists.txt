
cmake_minimum_required(VERSION 2.6)


# Use relative paths in windows
if(WIN32)
  set(CMAKE_USE_RELATIVE_PATHS true)
  set(CMAKE_SUPPRESS_REGENERATION true)
endif()


project(ALPHA)

# build 3rdParty dependencies
add_subdirectory(3rdParty)

# set build target variables
if (WIN32)
    set(ALPHA_BUILD_TARGET_DIR "Win32")
elseif (UNIX)
    set(ALPHA_BUILD_TARGET_DIR "Unix")
endif ()


set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DALPHA_DEBUG")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(warnings "-Wall -Wextra -Werror")
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        set(ALPHA_CXX_FLAGS "-g -std=c++0x -lX11 -lGL -lGLU")
    else()
        set(ALPHA_CXX_FLAGS "-std=c++0x -lX11 -lGL -lGLU")
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(warnings "/W4 /WX /EHsc")
    set(ALPHA_CXX_FLAGS "")
endif()
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} ${warnings}")
set(CMAKE_CXX_FLAGS "${ALPHA_CXX_FLAGS} ${CMAKE_CXX_FLAGS} ${warnings}")

# specify output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

# set install location to relative directory
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/sdk" CACHE PATH "ALPHA install prefix")

set(ALPHA_LIBRARIES AlphaEngine lua)

#include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# build Engine
add_subdirectory(AlphaEngine)

# build Demos
add_subdirectory(Demos)
