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

PROJECT(Demo-0002-Lights)

# define header files for the library
file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Include/*.h")
source_group(Header FILES ${HEADER_FILES})

# define source files for the library
file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp")
source_group(Source FILES ${SOURCE_FILES})

include_demo_source("Logic")

# include engine source, and project source for demo
include_directories(${CMAKE_SOURCE_DIR}/AlphaEngine/Include)
include_directories(${CMAKE_SOURCE_DIR}/AlphaEngine/Include/${ALPHA_BUILD_TARGET_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Include)

# create demo as an executable
add_executable(Demo-0002-Lights WIN32 ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(Demo-0002-Lights ${ALPHA_LIBRARIES})

# Windows link and project settings for Visual Studio
if (MSVC)
    set(CMAKE_CREATE_WIN32_EXE "/SUBSYSTEM:WINDOWS /ENTRY:\"mainCRTStartup\"")
endif (MSVC)

# install demo to sdk folder
install(TARGETS Demo-0002-Lights DESTINATION bin)

