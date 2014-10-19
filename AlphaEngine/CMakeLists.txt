
PROJECT(AlphaEngine)


# define header files for the library
file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Include/*.h")
source_group(Header FILES ${HEADER_FILES})
file(GLOB LOGIC_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Include/Logic/*.h")
source_group(Header\\Logic FILES ${LOGIC_HEADER_FILES})
file(GLOB TOOLBOX_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Include/Toolbox/*.h")
source_group(Header\\Toolbox FILES ${TOOLBOX_HEADER_FILES})
file(GLOB GRAPHICS_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Include/Graphics/*.h")
source_group(Header\\Graphics FILES ${GRAPHICS_HEADER_FILES})


# define source files for the library
file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp")
source_group(Source FILES ${SOURCE_FILES})
file(GLOB LOGIC_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Source/Logic/*.cpp")
source_group(Source\\Logic FILES ${LOGIC_SOURCE_FILES})
file(GLOB TOOLBOX_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Source/Toolbox/*.cpp")
source_group(Source\\Toolbox FILES ${TOOLBOX_SOURCE_FILES})
file(GLOB GRAPHICS_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Source/Graphics/*.cpp")
source_group(Source\\Graphics FILES ${GRAPHICS_SOURCE_FILES})


# make engine project
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Include)
add_library(
        AlphaEngine 
        ${HEADER_FILES} 
        ${LOGIC_HEADER_FILES}
        ${TOOLBOX_HEADER_FILES}
        ${GRAPHICS_HEADER_FILES}
        ${SOURCE_FILES}
        ${LOGIC_SOURCE_FILES}
        ${TOOLBOX_SOURCE_FILES}
        ${GRAPHICS_SOURCE_FILES}
    )


# install to sdk folder
install(FILES ${HEADER_FILES} DESTINATION include/AlphaEngine)
install(FILES ${LOGIC_HEADER_FILES} DESTINATION include/AlphaEngine/Logic)
install(FILES ${TOOLBOX_HEADER_FILES} DESTINATION include/AlphaEngine/Toolbox)
install(FILES ${GRAPHICS_HEADER_FILES} DESTINATION include/AlphaEngine/Graphics)
install(TARGETS AlphaEngine DESTINATION lib)