# =============================================================================
# 2D Game Engine - CMake Build Configuration
# =============================================================================
#
# This CMakeLists.txt builds the existing 2D game engine project without
# altering any behavior. It supports Debug and Release configurations and
# handles all current source files, assets, and third-party dependencies.
#
# ASSUMPTIONS:
# - MinGW-w64 or compatible GCC toolchain on Windows
# - System-installed SDL2 libraries on Linux/macOS (via pkg-config)
# - All vendor libraries are present in vendor/ directory
# - Pre-built static libraries (.a) are in lib/ directory (Windows)
# - Pre-built DLLs are in bin/ directory (Windows runtime)
#
# BUILD INSTRUCTIONS:
#   mkdir build && cd build
#   cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug  # Windows MinGW
#   cmake .. -DCMAKE_BUILD_TYPE=Release                      # Linux/macOS
#   cmake --build .
#
# OUTPUT:
#   The executable is placed in the bin/ directory alongside DLLs and assets.
#
# =============================================================================

cmake_minimum_required(VERSION 3.20)

project(2DGameEngine
    VERSION 1.0.0
    DESCRIPTION "A high-performance 2D game engine with ECS architecture"
    LANGUAGES CXX C
)

# =============================================================================
# C++ Standard Configuration
# =============================================================================
# The engine requires C++23 for ranges, views, and other modern features.
# This is a hard requirement - older standards will not compile.

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# =============================================================================
# Build Type Configuration
# =============================================================================
# Default to Debug if not specified. Supports Debug and Release.

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

# =============================================================================
# Compiler Flags
# =============================================================================
# Match the existing Makefile behavior:
# - Debug: -g -DDEBUG
# - Release: -O2 -DNDEBUG

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Common flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")
    
    # Debug flags
    set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")
    set(CMAKE_C_FLAGS_DEBUG "-g -DDEBUG")
    
    # Release flags
    set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
    set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
elseif(MSVC)
    # MSVC equivalent flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /EHsc")
    set(CMAKE_CXX_FLAGS_DEBUG "/Od /Zi /DDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG")
endif()

# =============================================================================
# Output Directories
# =============================================================================
# Place the executable in bin/ to match existing project structure.
# This ensures the executable can find assets at ../assets/

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)

# =============================================================================
# Source Files
# =============================================================================
# Explicitly list all source files to match existing Makefile behavior.
# This provides full control and avoids accidental inclusion of new files.

# Project C++ sources
set(PROJECT_SOURCES
    ${CMAKE_SOURCE_DIR}/src/Main.cpp
    ${CMAKE_SOURCE_DIR}/src/Game.cpp
    ${CMAKE_SOURCE_DIR}/src/Logger.cpp
    ${CMAKE_SOURCE_DIR}/src/LevelLoader.cpp
    ${CMAKE_SOURCE_DIR}/src/ECS/ECS.cpp
    ${CMAKE_SOURCE_DIR}/src/AssetStore/AssetStore.cpp
)

# SDL2Ext C sources (SDL2_gfx primitives)
set(SDL2EXT_SOURCES
    ${CMAKE_SOURCE_DIR}/src/SDL2Ext/SDL2_gfxPrimitives.c
    ${CMAKE_SOURCE_DIR}/src/SDL2Ext/SDL2_imageFilter.c
    ${CMAKE_SOURCE_DIR}/src/SDL2Ext/SDL2_rotozoom.c
)

# Dear ImGui sources
set(IMGUI_SOURCES
    ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_demo.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_impl_sdl2.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_impl_sdlrenderer2.cpp
)

# pugixml source
set(PUGIXML_SOURCES
    ${CMAKE_SOURCE_DIR}/vendor/pugixml/pugixml.cpp
)

# Combine all sources
set(ALL_SOURCES
    ${PROJECT_SOURCES}
    ${SDL2EXT_SOURCES}
    ${IMGUI_SOURCES}
    ${PUGIXML_SOURCES}
)

# =============================================================================
# Executable Target
# =============================================================================

add_executable(game ${ALL_SOURCES})

# =============================================================================
# Include Directories
# =============================================================================
# Add all include paths matching the existing Makefile configuration.

target_include_directories(game PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/vendor/glm
    ${CMAKE_SOURCE_DIR}/vendor/imgui
    ${CMAKE_SOURCE_DIR}/vendor/imgui/backends
    ${CMAKE_SOURCE_DIR}/vendor/lua
    ${CMAKE_SOURCE_DIR}/vendor/sol2/include
    ${CMAKE_SOURCE_DIR}/vendor/pugixml
)

# =============================================================================
# Platform-Specific Configuration
# =============================================================================

if(WIN32)
    # =========================================================================
    # Windows Configuration (MinGW)
    # =========================================================================
    # ASSUMPTION: Using MinGW-w64 with pre-built SDL2 libraries in vendor/
    # and import libraries (.a) in lib/
    
    message(STATUS "Configuring for Windows (MinGW)")
    
    # Add SDL2 include directory from vendor
    target_include_directories(game PRIVATE
        ${CMAKE_SOURCE_DIR}/vendor/SDL2/include
    )
    
    # Add library search path
    target_link_directories(game PRIVATE
        ${CMAKE_SOURCE_DIR}/lib
    )
    
    # Link libraries in correct order for MinGW
    # ASSUMPTION: Libraries are named exactly as shown (mingw32, SDL2main, etc.)
    target_link_libraries(game PRIVATE
        mingw32
        SDL2main
        SDL2
        SDL2_image
        SDL2_mixer
        SDL2_ttf
        lua53
    )
    
    # Set Windows subsystem to avoid console window in Release
    # Comment out to keep console for logging:
    # set_target_properties(game PROPERTIES WIN32_EXECUTABLE TRUE)

elseif(UNIX AND NOT APPLE)
    # =========================================================================
    # Linux Configuration
    # =========================================================================
    # ASSUMPTION: SDL2 and Lua installed via package manager (apt, dnf, etc.)
    # Uses pkg-config to find libraries.
    
    message(STATUS "Configuring for Linux")
    
    find_package(PkgConfig REQUIRED)
    
    # Find SDL2 libraries
    pkg_check_modules(SDL2 REQUIRED sdl2)
    pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
    pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)
    pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
    
    # Find Lua 5.3
    # ASSUMPTION: Lua 5.3 is installed as lua5.3 or lua53
    pkg_check_modules(LUA QUIET lua-5.3)
    if(NOT LUA_FOUND)
        pkg_check_modules(LUA QUIET lua5.3)
    endif()
    if(NOT LUA_FOUND)
        pkg_check_modules(LUA REQUIRED lua53)
    endif()
    
    # Add include directories
    target_include_directories(game PRIVATE
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_MIXER_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
        ${LUA_INCLUDE_DIRS}
    )
    
    # Link libraries
    target_link_libraries(game PRIVATE
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_MIXER_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        ${LUA_LIBRARIES}
    )
    
    # Add compiler flags from pkg-config
    target_compile_options(game PRIVATE
        ${SDL2_CFLAGS_OTHER}
    )

elseif(APPLE)
    # =========================================================================
    # macOS Configuration
    # =========================================================================
    # ASSUMPTION: SDL2 and Lua installed via Homebrew
    
    message(STATUS "Configuring for macOS")
    
    find_package(PkgConfig REQUIRED)
    
    # Find SDL2 libraries
    pkg_check_modules(SDL2 REQUIRED sdl2)
    pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
    pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)
    pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
    
    # Find Lua (Homebrew installs as lua@5.3)
    pkg_check_modules(LUA QUIET lua-5.3)
    if(NOT LUA_FOUND)
        pkg_check_modules(LUA QUIET lua@5.3)
    endif()
    if(NOT LUA_FOUND)
        # Fallback to system lua
        pkg_check_modules(LUA REQUIRED lua)
    endif()
    
    # Add include directories
    target_include_directories(game PRIVATE
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_MIXER_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
        ${LUA_INCLUDE_DIRS}
    )
    
    # Link libraries
    target_link_libraries(game PRIVATE
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_MIXER_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        ${LUA_LIBRARIES}
    )
    
    # Add compiler flags from pkg-config
    target_compile_options(game PRIVATE
        ${SDL2_CFLAGS_OTHER}
    )
endif()

# =============================================================================
# Post-Build: Copy DLLs (Windows Only)
# =============================================================================
# On Windows, ensure all DLLs are in the bin/ directory.
# ASSUMPTION: DLLs are already present in bin/ from repository.
# This step copies any missing DLLs from vendor/SDL2/lib if they exist.

if(WIN32)
    # Check if vendor SDL2 DLLs exist and copy them
    set(SDL2_DLL_DIR "${CMAKE_SOURCE_DIR}/vendor/SDL2/lib")
    if(EXISTS ${SDL2_DLL_DIR})
        file(GLOB SDL2_DLLS "${SDL2_DLL_DIR}/*.dll")
        foreach(DLL ${SDL2_DLLS})
            get_filename_component(DLL_NAME ${DLL} NAME)
            if(NOT EXISTS "${CMAKE_SOURCE_DIR}/bin/${DLL_NAME}")
                file(COPY ${DLL} DESTINATION ${CMAKE_SOURCE_DIR}/bin)
                message(STATUS "Copied ${DLL_NAME} to bin/")
            endif()
        endforeach()
    endif()
endif()

# =============================================================================
# Custom Targets
# =============================================================================

# Run target - builds and executes the game
add_custom_target(run
    COMMAND game
    DEPENDS game
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    COMMENT "Running the game..."
)

# =============================================================================
# Installation (Optional)
# =============================================================================
# Basic install target for packaging purposes.
# Installs executable and assets to CMAKE_INSTALL_PREFIX.

install(TARGETS game
    RUNTIME DESTINATION bin
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets
    DESTINATION .
)

# =============================================================================
# Summary
# =============================================================================

message(STATUS "")
message(STATUS "=== 2D Game Engine Build Configuration ===")
message(STATUS "Build Type:      ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard:    C++${CMAKE_CXX_STANDARD}")
message(STATUS "Output Directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "")
message(STATUS "To build: cmake --build .")
message(STATUS "To run:   cmake --build . --target run")
message(STATUS "")
