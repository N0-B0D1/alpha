# Copyright 2014-2015 Jason R. Wendlandt
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


cmake_minimum_required(VERSION 2.6)

# Use relative paths in windows
if(WIN32)
  set(CMAKE_USE_RELATIVE_PATHS true)
  set(CMAKE_SUPPRESS_REGENERATION true)
endif()

project(ALPHA)

# use solution folders for IDE's
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

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

# set compiler specific flags for our projects
# since we want more strict compilation (treat warnings as errors) which seemingly most
# 3rdParty libs don't care about, we do this after the 3rdParty folder is setup.
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(warnings "-Wall -Wextra -Werror")
	
    FIND_LIBRARY(ALPHA_LIB_X11 X11)
    FIND_LIBRARY(ALPHA_LIB_GL GL)
    FIND_LIBRARY(ALPHA_LIB_GLEW GLEW)
    FIND_LIBRARY(ALPHA_LIB_UUID uuid)
    FIND_PACKAGE(Threads)
	
    set(PLATFORM_LIBS ${ALPHA_LIB_X11} ${ALPHA_LIB_GL} ${ALPHA_LIB_GLEW} ${ALPHA_LIB_UUID} ${CMAKE_THREAD_LIBS_INIT})
	
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        set(ALPHA_CXX_FLAGS "-g -std=c++11")
    else()
        set(ALPHA_CXX_FLAGS "-std=c++11")
    endif()

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	# 4996 - disables warning for unsafe functions such as fopen.
    set(warnings "/W4 /WX /EHsc /wd4996")
	
    link_directories($ENV{DXSDK_DIR}\\lib\\x86)
	
	set(PLATFORM_LIBS d3d11.lib d3dcompiler.lib dxguid.lib winmm.lib rpcrt4.lib)
    set(ALPHA_CXX_FLAGS "")

endif()

# apply flags to global settings
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} ${warnings}")
set(CMAKE_CXX_FLAGS "${ALPHA_CXX_FLAGS} ${CMAKE_CXX_FLAGS} ${warnings}")

#set(ALPHA_LIBRARIES AlphaEngine lua)
set(ALPHA_LIBRARIES AlphaEngine Lua SDL2 ${PLATFORM_LIBS})

# build Engine
add_subdirectory(AlphaEngine)

# build Demos
add_subdirectory(Demos)

# build Engine Tools
add_subdirectory(Tools)
