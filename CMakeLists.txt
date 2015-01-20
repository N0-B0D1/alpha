
cmake_minimum_required(VERSION 2.6)


# Use relative paths in windows
if(WIN32)
  set(CMAKE_USE_RELATIVE_PATHS true)
  set(CMAKE_SUPPRESS_REGENERATION true)
endif()

project(ALPHA)


# set build target variables
if (WIN32)
    set(ALPHA_BUILD_TARGET_DIR "Win32")
elseif (UNIX)
    set(ALPHA_BUILD_TARGET_DIR "Unix")
endif ()

# specify output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

# set install location to relative directory
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/sdk" CACHE PATH "ALPHA install prefix")

# build 3rdParty libraries
# do these before we set the compiler flags, so that they build cleanly.
# LUA for instance, complains about a lot of type conversions.
add_subdirectory(3rdParty)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DALPHA_DEBUG")

# set compiler specific flags
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(warnings "-Wall -Wextra -Werror")
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        set(ALPHA_CXX_FLAGS "-g -std=c++11 -lX11 -lGL -lGLU")
    else()
        set(ALPHA_CXX_FLAGS "-std=c++11 -lX11 -lGL -lGLU")
    endif()

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(warnings "-Wall -Wextra -Werror")
    FIND_LIBRARY(X11_LIB X11)
    FIND_LIBRARY(GL_LIB GL)
    FIND_LIBRARY(GLUT_LIB GLU)
	set(PLATFORM_LIBS ${X11_LIB} ${GL_LIB} ${GLUT_LIB})
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        set(ALPHA_CXX_FLAGS "-g -std=c++11")
    else()
        set(ALPHA_CXX_FLAGS "-std=c++11")
    endif()

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	set(PLATFORM_LIBS d3d11.lib d3dcompiler.lib dxguid.lib winmm.lib)
	# 4996 - disables warning for unsafe functions such as fopen.
    set(warnings "/W4 /WX /EHsc /wd4996")
    set(ALPHA_CXX_FLAGS "")

endif()

# apply flags to global settings
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} ${warnings}")
set(CMAKE_CXX_FLAGS "${ALPHA_CXX_FLAGS} ${CMAKE_CXX_FLAGS} ${warnings}")

#set(ALPHA_LIBRARIES AlphaEngine lua)
set(ALPHA_LIBRARIES AlphaEngine Lua ${PLATFORM_LIBS})

# build Engine
add_subdirectory(AlphaEngine)

# build Demos
add_subdirectory(Demos)
