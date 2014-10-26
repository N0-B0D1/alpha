
cmake_minimum_required(VERSION 2.6)


# Use relative paths in windows
if(WIN32)
  set(CMAKE_USE_RELATIVE_PATHS true)
  set(CMAKE_SUPPRESS_REGENERATION true)
endif()


project(ALPHA)


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(warnings "-Wall -Wextra -Werror")
    set(ALPHA_CXX_FLAGS "-std=c++0x -lX11 -lGL -lGLU")
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
if (WIN32)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/sdk" CACHE PATH "ALPHA install prefix" FORCE)
endif(WIN32)

set(ALPHA_LIBRARIES AlphaEngine)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(AlphaEngine)
add_subdirectory(Demos)
