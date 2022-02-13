cmake_minimum_required(VERSION 3.16)

# rimozione build target inutili
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING INTERNAL FORCE)

# nome progetto
project(ChessGL)

# opzioni di compilazione
if(MSVC)
    add_compile_options(/W3)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")
else()
    add_compile_options(-W -Wall)
endif()

# opzioni output
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin-int)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# ---------- esterni -----------

# OpenGL da sistema
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# GLFW da submodule
set(GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only")
add_subdirectory("vendor/GLFW")

# Glad da cartella
add_library(Glad STATIC "vendor/Glad/src/glad.c")
target_include_directories(Glad PUBLIC "vendor/Glad/include")

# stb_image da cartella
add_library(stb_image STATIC "vendor/stb_image/stb_image.cpp")
target_include_directories(stb_image PUBLIC "vendor/stb_image")

# glm e spdlog da submodule
add_subdirectory("vendor/glm")
add_subdirectory("vendor/spdlog")

# ------------------------------

# ---------- progetto ----------

set(PREC_HEADER
	src/cglph.h
)

set(PROJECT_SRC
    # --- src/
	src/Main.cpp
    
    # --- src/Core/
    src/Core/Application.h
    src/Core/Application.cpp
    src/Core/Log.h
    src/Core/Log.cpp
    src/Core/Layer.h
    src/Core/LayerStack.h
    src/Core/LayerStack.cpp
    src/Core/Event.h
    src/Core/Event.cpp
    src/Core/Keycodes.h

    # --- src/Renderer/
    src/Renderer/Renderer.h
    src/Renderer/Renderer.cpp
    src/Renderer/Window.h
    src/Renderer/Window.cpp
    src/Renderer/Shader.h
    src/Renderer/Shader.cpp
	
    # --- assets/Shader/
    assets/Shader/triangolo.vert
    assets/Shader/triangolo.frag
    
    ${PREC_HEADER}
)

# ricreo la struttura dei file per visual studio
source_group(src "src")
source_group(src/Core "src/Core")
source_group(src/Renderer "src/Renderer")
source_group(shader "assets/Shader")
 
add_executable(ChessGL ${PROJECT_SRC})
target_include_directories(ChessGL PRIVATE "vendor/Glad/include" "vendor/glm" "vendor/stb_image")       			    # cartelle da includere
target_link_libraries(ChessGL ${OPENGL_LIBRARIES} glfw Glad glm stb_image spdlog)	                                    # librerie da linkare
#target_precompile_headers(ChessGL PRIVATE ${PREC_HEADER})                                                               # precompiled headers

# in Visual Studio setto il progetto come Start Up Project
if (MSVC)
    if (${CMAKE_VERSION} VERSION_LESS "3.6.0") 
        message("\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set 'ChessGL' as StartUp Project in Visual Studio.\n")
    else()
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ChessGL)
    endif()
endif()

# ------------------------------

# ---------- platform ----------

if (WIN32)
    add_definitions(-DWIN)
elseif (UNIX)
    add_definitions(-DLIN)
else()
    add_definitions(-DUNSUPPORTED)
endif()

# ------------------------------