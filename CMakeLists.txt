cmake_minimum_required(VERSION 3.31)
project(Raven3DEngineCore)
include(ExternalProject)

set(CMAKE_CXX_STANDARD 20)
set(OpenGL_GL_PREFERENCE GLVND)

if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif ()

add_subdirectory(ThirdParty/assimp)
add_subdirectory(ThirdParty/entt)
add_subdirectory(ThirdParty/glew/build/cmake)
add_subdirectory(ThirdParty/glm)
add_subdirectory(ThirdParty/SDL/SDL3)
add_subdirectory(ThirdParty/lua)

add_library(Raven3DEngineCore SHARED
        Raven3DEngineCore/src/Logging/Log.cpp
        Raven3DEngineCore/src/Input/InputDeviceRegistry.cpp
        Raven3DEngineCore/src/Window/SDLWindow.cpp
        Raven3DEngineCore/src/Rendering/OpenGLRenderer.cpp
        Raven3DEngineCore/src/RavenApp.cpp
        Raven3DEngineCore/src/Scene/SceneManager.cpp
        Raven3DEngineCore/src/Scripting/LuaScriptManager.cpp
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Raven3DEngineCore PRIVATE RAVEN_CORE_DEBUG)
endif ()

target_include_directories(Raven3DEngineCore PUBLIC
        ThirdParty/assimp/include
        ThirdParty/entt/src/
        ThirdParty/glew/include
        ThirdParty/glm/
        ThirdParty/SDL/SDL3/include
        Raven3DEngineCore/Include
        Raven3DEngineCore/src
        ThirdParty/lua
)

target_precompile_headers(Raven3DEngineCore PUBLIC
    Raven3DEngineCore/Include/pch.h
)


target_link_libraries(Raven3DEngineCore PRIVATE
        glew
        SDL3::SDL3
        assimp::assimp
        LuaLib
)


add_executable(EngineTest
        EngineTest/main.cpp
)

target_link_libraries(EngineTest PUBLIC Raven3DEngineCore)

set_target_properties(Raven3DEngineCore PROPERTIES RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR}/bin)
set_target_properties(LuaLib PROPERTIES RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR}/bin)
set_target_properties(EngineTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR}/bin)
set_target_properties(SDL3-shared PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set_target_properties(assimp PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
