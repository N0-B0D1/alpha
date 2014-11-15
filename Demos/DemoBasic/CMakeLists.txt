
PROJECT(DemoBasic)

# define header files for the library
file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Include/*.h")
source_group(Header FILES ${HEADER_FILES})
file(GLOB LOGIC_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Include/Logic/*.h")
source_group(Header\\Logic FILES ${LOGIC_HEADER_FILES})
#list(APPEND HEADER_FILES ${LOGIC_HEADER_FILES})

# define source files for the library
file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp")
source_group(Source FILES ${SOURCE_FILES})
file(GLOB LOGIC_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Source/Logic/*.cpp")
source_group(Source\\Logic FILES ${LOGIC_SOURCE_FILES})
#list(APPEND SOURCE_FILES ${LOGIC_SOURCE_FILES})

# make basic demo project
include_directories(${CMAKE_SOURCE_DIR}/AlphaEngine/Include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Include)

add_executable(
    DemoBasic WIN32 
    ${HEADER_FILES}
    ${LOGIC_HEADER_FILES}
    ${SOURCE_FILES}
    ${LOGIC_SOURCE_FILES}
)
target_link_libraries(DemoBasic ${ALPHA_LIBRARIES})

# Windows link and project settings for Visual Studio
if (WIN32)
    target_link_libraries(DemoBasic d3d11.lib d3dcompiler.lib dxguid.lib winmm.lib)
endif (WIN32)
if (MSVC)
    set(CMAKE_CREATE_WIN32_EXE "/SUBSYSTEM:WINDOWS /ENTRY:\"mainCRTStartup\"")
endif (MSVC)

# install demo to sdk folder
install(TARGETS DemoBasic DESTINATION bin)

