cmake_minimum_required(VERSION 3.31)
project(Raven3DEngineCore)

set(CMAKE_CXX_STANDARD 20)
set(OpenGL_GL_PREFERENCE GLVND)

add_subdirectory(ThirdParty/assimp)
add_subdirectory(ThirdParty/entt)
add_subdirectory(ThirdParty/glew/build/cmake)
add_subdirectory(ThirdParty/glm)
add_subdirectory(ThirdParty/SDL/SDL3)


add_library(Raven3DEngineCore SHARED
        Raven3DEngineCore/src/Logging/Log.cpp
        Raven3DEngineCore/src/Window/SDLWindow.cpp
        Raven3DEngineCore/src/Rendering/OpenGLRenderer.cpp
        Raven3DEngineCore/src/RavenApp.cpp
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
)

target_precompile_headers(Raven3DEngineCore PUBLIC
    Raven3DEngineCore/Include/pch.h
)

target_link_libraries(Raven3DEngineCore PRIVATE
    glew
    SDL3::SDL3
    assimp::assimp
)

add_executable(EngineTest
        EngineTest/main.cpp
)

target_link_libraries(EngineTest PRIVATE Raven3DEngineCore)
