//
// Created by wesley on 8/4/25.
//
#include <Raven3DEngineCore.h>

using namespace Raven3DEngineCore::Rendering;

void OpenGLRenderer::Initialize(const glm::vec3 &clearColor) {
    glewInit();
    glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
}

void OpenGLRenderer::RenderFrame() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
