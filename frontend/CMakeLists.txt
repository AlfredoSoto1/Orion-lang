project(frontend)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define all source files
file(GLOB_RECURSE FRONTEND_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Create the game executable
add_executable(frontend ${FRONTEND_SOURCES})

# Include directories for the target
target_include_directories(frontend 
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# Link additional libraries
target_link_libraries(frontend
    PRIVATE backend_static
)

# Set specific flags for each configuration directly
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(frontend PRIVATE /Zi /Od /MDd)

elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(frontend PRIVATE /MD /O2 /GS- /DNDEBUG)

elseif(CMAKE_BUILD_TYPE STREQUAL "Distribution")
    target_compile_options(frontend PRIVATE /O2 /DNDEBUG /OPT:REF /INCREMENTAL:NO /MD)
endif()

# Apply linker flags for headless distribution builds
if(HEADLESS_DIST)
    set_target_properties(frontend PROPERTIES 
        LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

