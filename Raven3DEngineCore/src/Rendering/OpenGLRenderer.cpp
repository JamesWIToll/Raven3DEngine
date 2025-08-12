//
// Created by wesley on 8/4/25.
//
#include <Raven3DEngineCore.h>

using namespace Raven3DEngineCore::Rendering;

void OpenGLRenderer::QueueForRender(RenderData *data, Scene::TransformData *transform) {
    if (data->material.opacity < 1.0f) {
        _transparentRenderData.emplace_back(data, transform);
    } else {
        _renderData.emplace_back(data, transform);
    }
}


void OpenGLRenderer::Initialize(const glm::vec3 &clearColor) {
    _eventHandler->RegisterEventListener(Events::EventType::AppRender, [this] (const Events::Event &) { RenderFrame(); });

    _renderData.clear();
    glewInit();
    glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
    RAVEN_LOG_INFO("OpenGL Renderer Initialized");
}

void OpenGLRenderer::RenderFrame() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
