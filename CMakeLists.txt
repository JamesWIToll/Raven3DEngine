cmake_minimum_required(VERSION 3.31)
project(RavEngineCore)

set(CMAKE_CXX_STANDARD 20)
set(OpenGL_GL_PREFERENCE GLVND)

add_subdirectory(ThirdParty/assimp)
add_subdirectory(ThirdParty/entt)
add_subdirectory(ThirdParty/glew/build/cmake)
add_subdirectory(ThirdParty/glm)
add_subdirectory(ThirdParty/SDL/SDL3)



add_library(RavEngineCore SHARED
        RavEngineCore/Rendering/OpenGLRenderer.cpp
        RavEngineCore/Rendering/IRenderer.h
        RavEngineCore/RavenApp.cpp
        RavEngineCore/RavenApp.h
)

target_include_directories(RavEngineCore PUBLIC
        ThirdParty/assimp/include
        ThirdParty/entt/src/
        ThirdParty/glew/include
        ThirdParty/glm/
        ThirdParty/SDL/SDL3/include
)

target_link_libraries(RavEngineCore PRIVATE
    glew
    SDL3::SDL3
    assimp::assimp
)

add_executable(RavEngineTest
        RavEngineTest/main.cpp
)

target_link_libraries(RavEngineTest PRIVATE RavEngineCore)
