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

# for for adding subdirectory source files easily
# helps with grouping into directories for IDE's like Visual Studio
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

# build Basic demo
# Basic demo showcases the minimal need to set up ALPHA
add_subdirectory(DemoBasic)
set_target_properties(DemoBasic PROPERTIES FOLDER Demos)
add_subdirectory(Demo-0001-Blank)
set_target_properties(Demo-0001-Blank PROPERTIES FOLDER Demos)
add_subdirectory(Demo-0002-Lights)
set_target_properties(Demo-0002-Lights PROPERTIES FOLDER Demos)

# make content files available for debugging
if (MSVC)
	# XXX Not sure if this is correct to do ...
	# copy fmod.dll to bin directory, so it can be loaded, only do this for windows
    file(GLOB FMOD_DLL "${FMOD_LOWLEVELAPI_DIRECTORY}/lib/fmodL.dll")
    file(COPY ${FMOD_DLL} DESTINATION "${CMAKE_BINARY_DIR}/bin/Debug/")

    set(WORKING_DIR /bin/Debug/Content)
    set(SHADER_EXT hlsl)
else ()
    set(WORKING_DIR /bin/Content)
    set(SHADER_EXT glsl)
endif (MSVC)

# copy Entities
file(GLOB ENTITY_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Content/Entities/*.lua")
file(COPY ${ENTITY_FILES} DESTINATION "${CMAKE_BINARY_DIR}${WORKING_DIR}/Entities/")

# copy Media files
file(GLOB MEDIA_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Content/Media/*.wav")
file(COPY ${MEDIA_FILES} DESTINATION "${CMAKE_BINARY_DIR}${WORKING_DIR}/Media/")
file(GLOB MEDIA_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Content/Media/*.ogg")
file(COPY ${MEDIA_FILES} DESTINATION "${CMAKE_BINARY_DIR}${WORKING_DIR}/Media/")
file(GLOB MEDIA_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Content/Media/*.mp3")
file(COPY ${MEDIA_FILES} DESTINATION "${CMAKE_BINARY_DIR}${WORKING_DIR}/Media/")
	
# copy model files
file(GLOB MEDIA_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Content/Models/*.am")
file(COPY ${MEDIA_FILES} DESTINATION "${CMAKE_BINARY_DIR}${WORKING_DIR}/Models/")

# copy shader content files for GLSL
file(GLOB CONTENT_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Content/Shaders/*.${SHADER_EXT}")
file(COPY ${CONTENT_FILES} DESTINATION "${CMAKE_BINARY_DIR}${WORKING_DIR}/Shaders/")
