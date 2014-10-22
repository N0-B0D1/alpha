
PROJECT(AlphaEngine)


# define header files for the library
file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Include/*.h")
source_group(Header FILES ${HEADER_FILES})
file(GLOB LOGIC_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Include/Logic/*.h")
source_group(Header\\Logic FILES ${LOGIC_HEADER_FILES})
file(GLOB TOOLBOX_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Include/Toolbox/*.h")
source_group(Header\\Toolbox FILES ${TOOLBOX_HEADER_FILES})
file(GLOB GRAPHICS_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Include/Graphics/*.h")
if (WIN32)
    file(GLOB GRAPHICS_PLAT_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Include/Win32/Graphics/*.h")
    list(APPEND GRAPHICS_HEADER_FILES ${GRAPHICS_PLAT_HEADER_FILES})
endif (WIN32)
source_group(Header\\Graphics FILES ${GRAPHICS_HEADER_FILES})
file(GLOB ASSETS_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Include/Assets/*.h")
source_group(Header\\Assets FILES ${ASSETS_HEADER_FILES})


# define source files for the library
file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp")
source_group(Source FILES ${SOURCE_FILES})
file(GLOB LOGIC_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Source/Logic/*.cpp")
source_group(Source\\Logic FILES ${LOGIC_SOURCE_FILES})
file(GLOB TOOLBOX_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Source/Toolbox/*.cpp")
source_group(Source\\Toolbox FILES ${TOOLBOX_SOURCE_FILES})
file(GLOB GRAPHICS_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Source/Graphics/*.cpp")
if (WIN32)
    file(GLOB GRAPHICS_PLAT_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Source/Win32/Graphics/*.cpp")
    list(APPEND GRAPHICS_SOURCE_FILES ${GRAPHICS_PLAT_SOURCE_FILES})
endif (WIN32)
source_group(Source\\Graphics FILES ${GRAPHICS_SOURCE_FILES})
file(GLOB ASSETS_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Source/Assets/*.cpp")
source_group(Source\\Assets FILES ${ASSETS_SOURCE_FILES})


# make engine project
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Include)
if (WIN32)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Include/Win32)
endif (WIN32)
add_library(
        AlphaEngine 
        ${HEADER_FILES} 
        ${LOGIC_HEADER_FILES}
        ${TOOLBOX_HEADER_FILES}
        ${GRAPHICS_HEADER_FILES}
        ${ASSETS_HEADER_FILES}
        ${SOURCE_FILES}
        ${LOGIC_SOURCE_FILES}
        ${TOOLBOX_SOURCE_FILES}
        ${GRAPHICS_SOURCE_FILES}
        ${ASSETS_SOURCE_FILES}
    )
    

# install to sdk folder
install(FILES ${HEADER_FILES} DESTINATION include/AlphaEngine)
install(FILES ${LOGIC_HEADER_FILES} DESTINATION include/AlphaEngine/Logic)
install(FILES ${TOOLBOX_HEADER_FILES} DESTINATION include/AlphaEngine/Toolbox)
install(FILES ${GRAPHICS_HEADER_FILES} DESTINATION include/AlphaEngine/Graphics)
install(FILES ${ASSETS_HEADER_FILES} DESTINATION include/AlphaEngine/Assets)
install(TARGETS AlphaEngine DESTINATION lib)