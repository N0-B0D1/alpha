
PROJECT(AlphaEngine)

# define header files for the library
file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Include/AppLayer/*.h")
source_group(Include\\AppLayer FILES ${HEADER_FILES})

# define source files for the library
file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Source/AppLayer/*.cpp")
source_group(Source\\AppLayer FILES ${SOURCE_FILES})

# make engine project
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Include)
add_library(AlphaEngine ${HEADER_FILES} ${SOURCE_FILES})

# install to sdk folder
install(FILES ${HEADER_FILES} DESTINATION include/AlphaEngine)
install(TARGETS AlphaEngine DESTINATION lib)