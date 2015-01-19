
project(lua CXX)

# define header files for the library
file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
source_group(Header FILES ${HEADER_FILES})

# define source files for the library
file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
source_group(Source FILES ${SOURCE_FILES})

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_library(lua ${HEADER_FILES} ${SOURCE_FILES})

#install(FILES ${HEADER_FILES} DESTINATION include/lua)
install(TARGETS lua DESTINATION lib/Release CONFIGURATIONS Release) # Release install
install(TARGETS lua DESTINATION lib/Debug CONFIGURATIONS Debug) #Debug install