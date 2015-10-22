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

# Lua
add_subdirectory(Lua)
set_target_properties(Lua PROPERTIES FOLDER 3rdParty)

# Assimp
MARK_AS_ADVANCED(ASSIMP_BIN_INSTALL_DIR)
MARK_AS_ADVANCED(ASSIMP_BUILD_ASSIMP_TOOLS)
MARK_AS_ADVANCED(ASSIMP_BUILD_SAMPLES)
MARK_AS_ADVANCED(ASSIMP_BUILD_STATIC_LIB)
MARK_AS_ADVANCED(ASSIMP_DEBUG_POSTFIX)
MARK_AS_ADVANCED(ASSIMP_ENABLE_BOOST_WORKAROUND)
MARK_AS_ADVANCED(ASSIMP_INCLUDE_INSTALL_DIR)
MARK_AS_ADVANCED(ASSIMP_LIBRARY_SUFFIX)
MARK_AS_ADVANCED(ASSIMP_LIB_INSTALL_DIR)
MARK_AS_ADVANCED(ASSIMP_NO_EXPORT)
MARK_AS_ADVANCED(ASSIMP_OPT_BUILD_PACKAGES)
MARK_AS_ADVANCED(ASSIMP_PACKAGE_VERSION)
MARK_AS_ADVANCED(BUILD_SHARED_LIBS)
MARK_AS_ADVANCED(AMD64)
MARK_AS_ADVANCED(ASM686)

add_subdirectory(Assimp)

# force a specific set of assimp build options
# these are needed to build alpha properly.
SET(ASSIMP_BIN_INSTALL_DIR "bin" CACHE STRING "" FORCE)
SET(ASSIMP_BUILD_ASSIMP_TOOLS false CACHE BOOL "" FORCE)
SET(ASSIMP_BUILD_SAMPLES FALSE CACHE BOOL "" FORCE)
SET(ASSIMP_BUILD_STATIC_LIB TRUE CACHE BOOL "" FORCE)
SET(ASSIMP_DEBUG_POSTFIX "d" CACHE STRING "" FORCE)
SET(ASSIMP_ENABLE_BOOST_WORKAROUND TRUE CACHE BOOL "" FORCE)
SET(ASSIMP_INCLUDE_INSTALL_DIR "include" CACHE STRING "" FORCE)
SET(ASSIMP_LIBRARY_SUFFIX "" CACHE STRING "" FORCE)
SET(ASSIMP_LIB_INSTALL_DIR "lib" CACHE STRING "" FORCE)
SET(ASSIMP_NO_EXPORT FALSE CACHE BOOL "" FORCE)
SET(ASSIMP_OPT_BUILD_PACKAGES FALSE CACHE BOOL "" FORCE)
SET(ASSIMP_PACKAGE_VERSION 0 CACHE INT "" FORCE)
SET(BUILD_SHARED_LIBS FALSE CACHE BOOL "" FORCE)

set_target_properties(assimp PROPERTIES FOLDER 3rdParty)
if (TARGET zlibstatic)
    set_target_properties(zlibstatic PROPERTIES FOLDER 3rdParty)
endif (TARGET zlibstatic)
set_target_properties(uninstall PROPERTIES FOLDER 3rdParty)

# SDL2
add_subdirectory(SDL2)
set_target_properties(SDL2 PROPERTIES FOLDER 3rdParty)
set_target_properties(SDL2main PROPERTIES FOLDER 3rdParty)
