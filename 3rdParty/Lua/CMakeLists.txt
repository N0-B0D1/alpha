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

PROJECT(Lua)

# define header files for the library
file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
source_group(Header FILES ${HEADER_FILES})

# define source files for the library
file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
source_group(Source FILES ${SOURCE_FILES})

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_library(Lua ${HEADER_FILES} ${SOURCE_FILES})

#install(FILES ${HEADER_FILES} DESTINATION include/lua)
install(TARGETS Lua DESTINATION lib/Release CONFIGURATIONS Release) # Release install
install(TARGETS Lua DESTINATION lib/Debug CONFIGURATIONS Debug) #Debug install
