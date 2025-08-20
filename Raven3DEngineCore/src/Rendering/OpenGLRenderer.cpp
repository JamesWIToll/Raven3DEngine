//
// Created by wesley on 8/4/25.
//
#include <Raven3DEngineCore.h>

using namespace Raven3DEngineCore::Rendering;

static void PollGLErrors() {
    GLenum error = glGetError();
    while (error != GL_NO_ERROR) {
        const auto errorString = reinterpret_cast<const char *>(glewGetErrorString(error));
        RAVEN_LOG_ERROR("OPENGL ERROR: {}", errorString);
        error = glGetError();
    }
}

void OpenGLRenderer::ReleaseRenderData(RenderData *data) {
    glDeleteVertexArrays(1, &data->VAO);
    glDeleteBuffers(1, &data->VBO);
    glDeleteBuffers(1, &data->IBO);
    glDeleteBuffers(1, &data->NBO);
    glDeleteBuffers(1, &data->IBO);
    glDeleteBuffers(1, &data->TBO);

    glDeleteTextures(1, &data->material.diffTex);
    glDeleteTextures(1, &data->material.specTex);
    glDeleteTextures(1, &data->material.ambTex);
    glDeleteTextures(1, &data->material.normTex);
    glDeleteTextures(1, &data->material.emisTex);
}

void OpenGLRenderer::QueueForRender(RenderData *data, Scene::TransformData *transform) {
    if (data->VAO == 0 || data->VBO == 0 || data->NBO == 0 || data->UV_0_BO == 0 || data->IBO == 0 || data->TBO == 0) {
        LoadBuffers(data);
    }

    if (data->material.opacity < 1.0f) {
        _transparentRenderData.emplace_back(data, transform);
    } else {
        _renderData.emplace_back(data, transform);
    }
}

void OpenGLRenderer::AddLight(LightData *data) {
    if (_numLights >= MAX_LIGHTS) {
        RAVEN_LOG_ERROR("Cannot add more lights to scene");
        return;
    }
    _lights[_numLights] = data;
    _numLights++;
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
    PollGLErrors();
    return textureID;
}


void OpenGLRenderer::Initialize(const glm::vec3 &clearColor, const RAVEN_INT &width, const RAVEN_INT &height) {
    glewInit();

    _clearColor = clearColor;
    _width = width;
    _height = height;

    _eventHandler->RegisterEventListener(Events::EventType::AppRender, [this] (const Events::Event &) { RenderFrame(); });

    _renderData.clear();

    _mainShader.Initialize(std::string(RAVEN_RESOURCE_PATH) + "Shaders/main.vert", std::string(RAVEN_RESOURCE_PATH) + "Shaders/main.frag");

    PollGLErrors();

    RAVEN_LOG_INFO("OpenGL Renderer Initialized");
}

void OpenGLRenderer::RenderMesh(RenderData &renderData, Scene::TransformData &transformData) {
    _mainShader.setMat4("uModel", transformData.worldTransform);
    _mainShader.setVec3("uMaterial.diffuseColor", renderData.material.diffuseColor);
    _mainShader.setVec3("uMaterial.specularColor", renderData.material.specularColor);
    _mainShader.setFloat("uMaterial.shininess", renderData.material.shininess);
    _mainShader.setFloat("uMaterial.shininessStrength", renderData.material.shininessStrength);
    _mainShader.setFloat("uMaterial.opacity", renderData.material.opacity);
    _mainShader.setVec3("uMaterial.emissiveColor", renderData.material.emissiveColor);
    _mainShader.setFloat("uMaterial.metallicFactor", renderData.material.metallicFactor);
    _mainShader.setFloat("uMaterial.roughnessFactor", renderData.material.roughnessFactor);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderData.material.diffTex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, renderData.material.specTex);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, renderData.material.ambTex);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, renderData.material.normTex);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, renderData.material.emisTex);
    _mainShader.setInt("uMaterial.diffTex", 0);
    _mainShader.setInt("uMaterial.specTex", 1);
    _mainShader.setInt("uMaterial.ambTex", 2);
    _mainShader.setInt("uMaterial.normTex", 3);
    _mainShader.setInt("uMaterial.emisTex", 4);
    _mainShader.setBool("uMaterial.useDiffTex", renderData.material.diffTex != 0);
    _mainShader.setBool("uMaterial.useSpecTex", renderData.material.specTex != 0);
    _mainShader.setBool("uMaterial.useAmbTex", renderData.material.ambTex != 0);
    _mainShader.setBool("uMaterial.useNormTex", renderData.material.normTex != 0);
    _mainShader.setBool("uMaterial.useEmisTex", renderData.material.emisTex != 0);
    glBindVertexArray(renderData.VAO);
    constexpr auto mode = GL_TRIANGLES;
    //if (renderData.material.wireframe) { mode = GL_LINES;}

    glDrawElements(mode, renderData.indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    PollGLErrors();
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

    _mainShader.setVec3("uCamPos", _camTransform->worldTransform[3]);

    for (int i = 0; i < _numLights; i++) {
        const auto light = _lights[i];
        _mainShader.setVec3("uLights[" + std::to_string(i) + "].posDir", light->posDir);
        _mainShader.setVec3("uLights[" + std::to_string(i) + "].color", light->color);
        _mainShader.setFloat("uLights[" + std::to_string(i) + "].radius", light->radius);
        _mainShader.setFloat("uLights[" + std::to_string(i) + "].intensity", light->intensity);
        _mainShader.setInt("uLights[" + std::to_string(i) + "].type", static_cast<RAVEN_U_INT>(light->type));
    }
    _mainShader.setInt("uNumLights", _numLights);

    for (int i = 0; i < _renderData.size(); i++) {
        const auto renderData = _renderData[i].first;
        const auto transform = _renderData[i].second;
        RenderMesh(*renderData, *transform);
    }
    _renderData.clear();

    const auto camPos = _cameraData->position;
    std::ranges::sort(_transparentRenderData, [&](const auto &a, const auto &b) -> bool {
        const auto camDistA = glm::distance(camPos, glm::vec3(a.second->worldTransform[3]));
        const auto camDistB = glm::distance(camPos, glm::vec3(b.second->worldTransform[3]));
        return camDistA > camDistB;
    });

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (int i = 0; i < _transparentRenderData.size(); i++) {
        const auto renderData = _transparentRenderData[i].first;
        renderData->material.diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
        const auto transform = _transparentRenderData[i].second;
        RenderMesh(*renderData, *transform);
    }
    glDisable(GL_BLEND);

    _numLights = 0;
    _transparentRenderData.clear();

    PollGLErrors();
}
