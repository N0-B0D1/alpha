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

PROJECT(DemoBasic)

# define header files for the library
file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Include/*.h")
source_group(Header FILES ${HEADER_FILES})

# define source files for the library
file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp")
source_group(Source FILES ${SOURCE_FILES})

# for for adding subdirectory source files easily
# helps with grouping into directories for IDE's liek Visual Studio
function (include_demo_source sub_dir)
    # better than nothing for now
    # XXX should figure out how to replace '/' with '\\', in windows
    # and if that will even work for source_group's
    set(SUB_GROUP ${sub_dir})
    string(REGEX REPLACE "/" "_" SUB_GROUP ${SUB_GROUP})
    
    # add header files
    file(GLOB SUB_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Include/${sub_dir}/*.h")
    source_group(Header\\${SUB_GROUP} FILES ${SUB_HEADER_FILES})
    
    # add sub group to final list of header files for building
    set(HEADER_FILES ${HEADER_FILES} ${SUB_HEADER_FILES} PARENT_SCOPE)

    # add source files
    file(GLOB SUB_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Source/${sub_dir}/*.cpp")
    source_group(Source\\${SUB_GROUP} FILES ${SUB_SOURCE_FILES})
    
    # add sub group source files to final list of source files for building
    set(SOURCE_FILES ${SOURCE_FILES} ${SUB_SOURCE_FILES} PARENT_SCOPE)
endfunction ()

include_demo_source("HID")
include_demo_source("Logic")

# include engine source, and project source for demo
include_directories(${CMAKE_SOURCE_DIR}/AlphaEngine/Include)
include_directories(${CMAKE_SOURCE_DIR}/AlphaEngine/Include/${ALPHA_BUILD_TARGET_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Include)

# create demo as an executable
add_executable(DemoBasic WIN32 ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(DemoBasic ${ALPHA_LIBRARIES})

# Windows link and project settings for Visual Studio
if (MSVC)
    set(CMAKE_CREATE_WIN32_EXE "/SUBSYSTEM:WINDOWS /ENTRY:\"mainCRTStartup\"")
endif (MSVC)

# install demo to sdk folder
install(TARGETS DemoBasic DESTINATION bin)

# make content files available for debugging
if (MSVC)
	#configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Content/Entities/test.lua ${CMAKE_BINARY_DIR}/bin/Debug/Content/Entities/test.lua)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Content/Shaders/Basic_PS.hlsl ${CMAKE_BINARY_DIR}/bin/Debug/Content/Shaders/Basic_PS.hlsl)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Content/Shaders/Basic_VS.hlsl ${CMAKE_BINARY_DIR}/bin/Debug/Content/Shaders/Basic_VS.hlsl)
	
    # copy Entities
    file(GLOB ENTITY_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Content/Entities/*.lua")
    file(COPY ${ENTITY_FILES} DESTINATION "${CMAKE_BINARY_DIR}/bin/Debug/Content/Entities/")
	
    # copy Media files
    file(GLOB MEDIA_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Content/Media/*.wav")
    file(COPY ${MEDIA_FILES} DESTINATION "${CMAKE_BINARY_DIR}/bin/Debug/Content/Media/")
    file(GLOB MEDIA_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Content/Media/*.ogg")
    file(COPY ${MEDIA_FILES} DESTINATION "${CMAKE_BINARY_DIR}/bin/Debug/Content/Media/")
    file(GLOB MEDIA_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Content/Media/*.mp3")
    file(COPY ${MEDIA_FILES} DESTINATION "${CMAKE_BINARY_DIR}/bin/Debug/Content/Media/")
	
	# copy model files
    file(GLOB MEDIA_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Content/Models/*.am")
    file(COPY ${MEDIA_FILES} DESTINATION "${CMAKE_BINARY_DIR}/bin/Debug/Content/Models/")
	
	# XXX Not sure if this is correct to do ...
	# copy fmod.dll to bin directory, so it can be loaded, only do this for windows
    file(GLOB FMOD_DLL "${FMOD_LOWLEVELAPI_DIRECTORY}/lib/fmodL.dll")
    file(COPY ${FMOD_DLL} DESTINATION "${CMAKE_BINARY_DIR}/bin/Debug/")
else ()
    # copy Entities
    file(GLOB ENTITY_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Content/Entities/*.lua")
    file(COPY ${ENTITY_FILES} DESTINATION "${CMAKE_BINARY_DIR}/bin/Content/Entities/")
	
    # copy Media files
    file(GLOB MEDIA_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Content/Media/*.wav")
    file(COPY ${MEDIA_FILES} DESTINATION "${CMAKE_BINARY_DIR}/bin/Content/Media/")
    file(GLOB MEDIA_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Content/Media/*.ogg")
    file(COPY ${MEDIA_FILES} DESTINATION "${CMAKE_BINARY_DIR}/bin/Content/Media/")
    file(GLOB MEDIA_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Content/Media/*.mp3")
    file(COPY ${MEDIA_FILES} DESTINATION "${CMAKE_BINARY_DIR}/bin/Content/Media/")

    # copy shader content files for GLSL
    file(GLOB CONTENT_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Content/Shaders/*.glsl")
    file(COPY ${CONTENT_FILES} DESTINATION "${CMAKE_BINARY_DIR}/bin/Content/Shaders/")
endif (MSVC)

