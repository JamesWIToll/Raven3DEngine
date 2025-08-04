//
// Created by wesley on 8/4/25.
//

#include <stdexcept>
#include <string>
#include "./IRenderer.h"
#include "GL/glew.h"

using namespace RavEngineCore::Rendering;

void OpenGLRenderer::InitializeWindow(const std::string &name, const int &pixelWidth, const int &pixelHeight, const glm::vec3 &clearColor) {
    glewInit();

    window = SDL_CreateWindow(name.c_str(), pixelWidth, pixelHeight, SDL_WINDOW_OPENGL);
    context = SDL_GL_CreateContext(window);
    if (!SDL_GL_MakeCurrent(window, context)) {
        throw std::runtime_error("SDL Failed to create OpenGL context");
    }

    glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
}

void OpenGLRenderer::RenderFrame() {
    if (window == nullptr || context == nullptr) {
        throw std::runtime_error("No Valid OpenGL Window/Context to render frame");
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    SDL_GL_SwapWindow(window);
}
