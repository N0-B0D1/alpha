
PROJECT(DemoBasic)

# define header files for the library
file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Include/*.h")
#source_group(Include\\AppLayer FILES ${HEADER_FILES})

# define source files for the library
file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp")
#source_group(Source\\AppLayer FILES ${SOURCE_FILES})

# make basic demo project
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Include)
add_executable(DemoBasic WIN32 ${HEADER_FILES} ${SOURCE_FILES})
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

