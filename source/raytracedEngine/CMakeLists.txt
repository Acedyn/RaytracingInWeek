# Define source files
file(GLOB SOURCES
    "*.cpp")

# Define header files
file(GLOB HEADERS
    "*.h")

# Create library for raytracedEngine
add_executable(raytracedEngine
    ${SOURCES}
    ${HEADERS})

if(WIN32)
    set_target_properties(raytracedEngine
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/binaries/raytracedEngine/vs2019")
endif(WIN32)

if(MINGW)
    set_target_properties(raytracedEngine
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/binaries/raytracedEngine/mingw")
endif(MINGW)

if(UNIX)
    set_target_properties(raytracedEngine
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/binaries/raytracedEngine/unix")
endif(UNIX)

