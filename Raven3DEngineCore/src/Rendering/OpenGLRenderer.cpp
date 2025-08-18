//
// Created by wesley on 8/4/25.
//
#include <Raven3DEngineCore.h>

using namespace Raven3DEngineCore::Rendering;

void OpenGLRenderer::QueueForRender(RenderData *data, Scene::TransformData *transform) {
    if (data->VAO == 0 || data->VBO == 0 || data->NBO == 0 || data->UV_0_BO == 0 || data->IBO == 0) {
        LoadBuffers(data);
    }

    if (data->material.opacity < 1.0f) {
        _transparentRenderData.emplace_back(data, transform);
    } else {
        _renderData.emplace_back(data, transform);
    }
}

void OpenGLRenderer::SetActiveCam(CameraData *data, Scene::TransformData *camTransform) {
    _cameraData = data;
    _camTransform = camTransform;
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


void OpenGLRenderer::Initialize(const glm::vec3 &clearColor, const RAVEN_INT &width, const RAVEN_INT &height) {
    _clearColor = clearColor;
    _width = width;
    _height = height;

    _eventHandler->RegisterEventListener(Events::EventType::AppRender, [this] (const Events::Event &) { RenderFrame(); });

    _renderData.clear();
    glewInit();

    _mainShader.Initialize(std::string(RAVEN_RESOURCE_PATH) + "Shaders/main.vert", std::string(RAVEN_RESOURCE_PATH) + "Shaders/main.frag");

    RAVEN_LOG_INFO("OpenGL Renderer Initialized");
}

void OpenGLRenderer::RenderFrame() {

    _mainShader.use();
    glClearColor(_clearColor.x, _clearColor.y, _clearColor.z, 1.0f);
    glViewport(0, 0, _width, _height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    const glm::mat4 projectionMat = _cameraData->GetProjectionMatrix(_width, _height);
    const glm::mat4 viewMat = _cameraData->GetViewMatrix();

    _mainShader.setMat4("uProjection", projectionMat);
    _mainShader.setMat4("uView", viewMat );

    for (int i = 0; i < _renderData.size(); i++) {
        const auto renderData = _renderData[i].first;
        const auto transform = _renderData[i].second;

        _mainShader.setMat4("uModel", transform->worldTransform);
        _mainShader.setVec3("uMaterial.diffuseColor", renderData->material.diffuseColor);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, renderData->material.diffTex);
        _mainShader.setInt("uMaterial.diffuseTex", 0);

        glBindVertexArray(renderData->VAO);
        constexpr auto mode = GL_TRIANGLES;
        //if (renderData->material.wireframe) { mode = GL_LINES;}

        glDrawElements(mode, renderData->indices.size(), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }
    _renderData.clear();

    const auto camPos = _cameraData->position;
    std::ranges::sort(_transparentRenderData, [&](const auto &a, const auto &b) -> bool {
        const auto camDistA = glm::distance(camPos, glm::vec3(a.second->worldTransform[3]));
        const auto camDistB = glm::distance(camPos, glm::vec3(b.second->worldTransform[3]));
        return camDistA > camDistB;
    });

    // glEnable(GL_BLEND);
    // for (int i = 0; i < _transparentRenderData.size(); i++) {
    //     const auto renderData = _transparentRenderData[i].first;
    //     const auto transform = _transparentRenderData[i].second;
    //     _mainShader.setMat4("uModel", transform->GetMatrix());
    //     _mainShader.setVec3("uMaterial.diffuseColor", renderData->material.diffuseColor);
    //     glActiveTexture(GL_TEXTURE0);
    //     glBindTexture(GL_TEXTURE_2D, renderData->material.diffTex);
    //     _mainShader.setInt("uMaterial.diffuseTex", 0);
    //
    //     glBindVertexArray(renderData->VAO);
    //     constexpr auto mode = GL_TRIANGLES;
    //     //if (renderData->material.wireframe) { mode = GL_LINES;}
    //     glDrawElements(mode, renderData->indices.size(), GL_UNSIGNED_INT, nullptr);
    //     glBindVertexArray(0);
    // }
    // glDisable(GL_BLEND);

    _transparentRenderData.clear();

}
