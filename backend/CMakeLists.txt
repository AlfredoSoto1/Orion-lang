project(backend)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define all source files
file(GLOB_RECURSE BACKEND_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Create static library from the source files
add_library(backend_static STATIC ${BACKEND_SOURCES})

# Include directories for the target
target_include_directories(backend_static 
  PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# Set specific flags for each configuration directly
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(backend_static PRIVATE /Zi /Od /MDd)

elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  target_compile_options(backend_static PRIVATE /MD /O2 /GS- /DNDEBUG)

elseif(CMAKE_BUILD_TYPE STREQUAL "Distribution")
  target_compile_options(backend_static PRIVATE /O2 /DNDEBUG /OPT:REF /INCREMENTAL:NO /MD)
endif()

# Apply linker flags for headless distribution builds (if applicable to backend_static)
if (HEADLESS_DIST)
  set_target_properties(backend_static PROPERTIES 
    LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()