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

RAVEN_U_INT OpenGLRenderer::LoadTexture(const std::int32_t width, const std::int32_t height, std::uint8_t *data, const std::int32_t numComps) {
    RAVEN_U_INT textureID {};
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (numComps == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    return textureID;
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
