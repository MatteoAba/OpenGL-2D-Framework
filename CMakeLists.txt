cmake_minimum_required(VERSION 3.16)

# remove useless build target
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING INTERNAL FORCE)

# project name
project(ChessGL)

# compile options
if(MSVC)
    add_compile_options(/W3)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")
else()
    add_compile_options(-Wall)
endif()

# output options
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin-int)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# --------- external ----------

# OpenGL from system
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# GLFW from submodule
set(GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only")
add_subdirectory("vendor/glfw")

# Glad from directory
add_library(Glad STATIC "vendor/Glad/src/glad.c")
target_include_directories(Glad PUBLIC "vendor/Glad/include")

# stb_image from directory
add_library(stb_image STATIC "vendor/stb_image/stb_image.cpp")
target_include_directories(stb_image PUBLIC "vendor/stb_image")

# glm, spdlog and freetype from submodule
add_subdirectory("vendor/glm")
add_subdirectory("vendor/spdlog")
add_subdirectory("vendor/freetype")

# imgui from directory
add_library(imgui STATIC 
        "vendor/imgui/imconfig.h"
		"vendor/imgui/imgui.h"
		"vendor/imgui/imgui.cpp"
		"vendor/imgui/imgui_demo.cpp"
		"vendor/imgui/imgui_draw.cpp"
        "vendor/imgui/backends/imgui_impl_glfw.h"
        "vendor/imgui/backends/imgui_impl_glfw.cpp"
        "vendor/imgui/backends/imgui_impl_opengl3.h"
        "vendor/imgui/backends/imgui_impl_opengl3.cpp"
		"vendor/imgui/imgui_internal.h"
		"vendor/imgui/imgui_tables.cpp"
		"vendor/imgui/imgui_widgets.cpp"
		"vendor/imgui/imstb_truetype.h"
		"vendor/imgui/imstb_rectpack.h"
		"vendor/imgui/imstb_textedit.h"
)
target_include_directories(imgui PUBLIC "vendor/imgui" "vendor/imgui/backends" "vendor/glfw/include")

# -----------------------------

# ---------- project ----------

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
    src/Core/Timer.h

    # --- src/Layers/
    src/Layers/TestLayer.h
    src/Layers/TestLayer.cpp
    src/Layers/ImGuiLayer.h
    src/Layers/ImGuiLayer.cpp
    src/Layers/BatchRendererLayer.h
    src/Layers/BatchRendererLayer.cpp

    # --- src/Renderer/
    src/Renderer/Renderer.h
    src/Renderer/Renderer.cpp
    src/Renderer/Window.h
    src/Renderer/Window.cpp
    src/Renderer/Shader.h
    src/Renderer/Shader.cpp
    src/Renderer/Buffer.h
    src/Renderer/Buffer.cpp
    src/Renderer/VertexArray.h
    src/Renderer/VertexArray.cpp
    src/Renderer/Debug.h
    src/Renderer/Texture.h
    src/Renderer/Texture.cpp
    src/Renderer/Framebuffer.h
    src/Renderer/Framebuffer.cpp
    src/Renderer/OrthographicCamera.h
    src/Renderer/OrthographicCamera.cpp
    src/Renderer/Text.h
    src/Renderer/Text.cpp
	
    # --- assets/Shader/
    assets/Shader/Quads.vert
    assets/Shader/Quads.frag
    assets/Shader/Text.vert
    assets/Shader/Text.frag
    assets/Shader/TextureOnQuad.vert
    assets/Shader/TextureOnQuad.frag
    assets/Shader/Vertex2D.vert
    assets/Shader/Vertex2D.frag
    
    ${PREC_HEADER}
)

# recreate the files structure for visual studio
source_group(src "src")
source_group(src/Core "src/Core")
source_group(src/Layers "src/Layers")
source_group(src/Renderer "src/Renderer")
source_group(shader "assets/Shader")
 
add_executable(ChessGL ${PROJECT_SRC})
target_include_directories(ChessGL PRIVATE "vendor/Glad/include" "vendor/glm" "vendor/stb_image" "vendor/imgui" "vendor/freetype/include")      # cartelle da includere
target_link_libraries(ChessGL ${OPENGL_LIBRARIES} glfw Glad glm stb_image spdlog imgui freetype)	                                            # librerie da linkare
#target_precompile_headers(ChessGL PRIVATE ${PREC_HEADER})                                                                                      # precompiled headers

# copy resources (shader, media, ecc..) in the output folder
if (MSVC)
    file(COPY "assets" DESTINATION ${CMAKE_SOURCE_DIR}/bin/Debug)
    file(COPY "assets" DESTINATION ${CMAKE_SOURCE_DIR}/bin/Release)
    file(COPY "assets" DESTINATION ${CMAKE_SOURCE_DIR}/build)
else()
    file(COPY "assets" DESTINATION ${CMAKE_SOURCE_DIR}/bin)
endif()

# in Visual Studio setup project as Start Up Project
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
