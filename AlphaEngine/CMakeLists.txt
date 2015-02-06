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

PROJECT(AlphaEngine)

# start headers
file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Include/*.h")
source_group(Header FILES ${HEADER_FILES})

# start sources
file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp")
source_group(Source FILES ${SOURCE_FILES})

install(FILES ${HEADER_FILES} DESTINATION include/AlphaEngine)


function (include_alpha_source sub_dir)

    # better than nothing for now
    # XXX should figure out how to replace '/' with '\\', in windows
    # and if that will even work for source_group's
    set(SUB_GROUP ${sub_dir})
    string(REGEX REPLACE "/" "_" SUB_GROUP ${SUB_GROUP})
    
    # add header files
    file(GLOB SUB_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Include/${sub_dir}/*.h")
    file(GLOB SUB_PLAT_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Include/${ALPHA_BUILD_TARGET_DIR}/${sub_dir}/*.h")
    list(APPEND SUB_HEADER_FILES ${SUB_PLAT_HEADER_FILES})
    source_group(Header\\${SUB_GROUP} FILES ${SUB_HEADER_FILES})
    
    # add sub group to final list of header files for building
    set(HEADER_FILES ${HEADER_FILES} ${SUB_HEADER_FILES} PARENT_SCOPE)

    # add source files
    file(GLOB SUB_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Source/${sub_dir}/*.cpp")
    file(GLOB SUB_PLAT_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Source/${ALPHA_BUILD_TARGET_DIR}/${sub_dir}/*.cpp")
    list(APPEND SUB_SOURCE_FILES ${SUB_PLAT_SOURCE_FILES})
    source_group(Source\\${SUB_GROUP} FILES ${SUB_SOURCE_FILES})
    
    # add sub group source files to final list of source files for building
    set(SOURCE_FILES ${SOURCE_FILES} ${SUB_SOURCE_FILES} PARENT_SCOPE)
    
    # install any header files for exporting the final SDK
    install(FILES ${SUB_HEADER_FILES} DESTINATION include/AlphaEngine/${sub_dir})
    
endfunction ()


include_alpha_source("Logic")
include_alpha_source("Toolbox")
include_alpha_source("Graphics")
include_alpha_source("Assets")
include_alpha_source("Events")
include_alpha_source("Entities")
include_alpha_source("Scripting")
include_alpha_source("Math")
include_alpha_source("FSA")


# make engine project
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Include/${ALPHA_BUILD_TARGET_DIR})

#include 3rdParty dependencies
include_directories(${CMAKE_SOURCE_DIR}/3rdParty/Lua/src)

add_library(AlphaEngine ${HEADER_FILES} ${SOURCE_FILES})

#install(TARGETS AlphaEngine DESTINATION lib)
install(TARGETS AlphaEngine DESTINATION lib/Release CONFIGURATIONS Release)
install(TARGETS AlphaEngine DESTINATION lib/Debug CONFIGURATIONS Debug)
