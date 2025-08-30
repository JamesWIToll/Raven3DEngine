//
// Created by wesley on 8/4/25.
//
#include <Raven3DEngineCore.h>

using namespace Raven3DEngineCore::Rendering;

static void PollGLErrors() {
    GLenum error = glGetError();
    while (error != GL_NO_ERROR) {
        auto glVersionString = reinterpret_cast<const char*>( glGetString(GL_VERSION));
        if (glVersionString == nullptr)  glVersionString = "UNKNOWN";
        const auto errorString = reinterpret_cast<const char *>(glewGetErrorString(error));
        RAVEN_LOG_ERROR("OPENGL V: {} - ERROR: {} - {}", glVersionString, error, errorString);
        error = glGetError();
    }
}

void OpenGLRenderer::LoadBuffers(RenderData3D *data) {
    GLuint VAO, VBO, NBO, IBO, UV_0_BO, TBO;
    if (data->VAO != 0)         glDeleteVertexArrays(1, &data->VAO);
    if (data->VBO != 0)         glDeleteBuffers(1, &data->VBO);
    if (data->NBO != 0)         glDeleteBuffers(1, &data->NBO);
    if (data->IBO != 0)         glDeleteBuffers(1, &data->IBO);
    if (data->UV_0_BO != 0)     glDeleteBuffers(1, &data->UV_0_BO);
    if (data->TBO != 0)         glDeleteBuffers(1, &data->TBO);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &NBO);
    glGenBuffers(1, &IBO);
    glGenBuffers(1, &TBO);
    glGenBuffers(1, &UV_0_BO);


    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, data->vertices.size() * sizeof(glm::vec3), data->vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, data->normals.size() * sizeof(glm::vec3), data->normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glBufferData(GL_ARRAY_BUFFER, data->tangents.size() * sizeof(glm::vec3), data->tangents.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, UV_0_BO);
    glBufferData(GL_ARRAY_BUFFER, data->uvs_0.size() * sizeof(glm::vec2), data->uvs_0.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->indices.size() * sizeof(RAVEN_U_INT), data->indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    data->VAO = VAO;
    data->VBO = VBO;
    data->NBO = NBO;
    data->IBO = IBO;
    data->UV_0_BO = UV_0_BO;
    data->TBO = TBO;
    PollGLErrors();
}

void OpenGLRenderer::LoadTextures(MaterialData3D *data) {
    if (data->ambTex  != nullptr && data->ambTex->id  == 0) LoadTexture(data->ambTex);
    if (data->diffTex != nullptr && data->diffTex->id == 0) LoadTexture(data->diffTex);
    if (data->specTex != nullptr && data->specTex->id == 0) LoadTexture(data->specTex);
    if (data->emisTex != nullptr && data->emisTex->id == 0) LoadTexture(data->emisTex);
    if (data->normTex != nullptr && data->normTex->id == 0) LoadTexture(data->normTex);
}

void OpenGLRenderer::ReleaseRenderData(RenderData3D *data) {
    glDeleteVertexArrays(1, &data->VAO);

    glDeleteBuffers(1, &data->VBO);
    glDeleteBuffers(1, &data->IBO);
    glDeleteBuffers(1, &data->NBO);
    glDeleteBuffers(1, &data->TBO);
    glDeleteBuffers(1, &data->UV_0_BO);

    glDeleteTextures(1, &data->material.diffTex->id);
    glDeleteTextures(1, &data->material.specTex->id);
    glDeleteTextures(1, &data->material.ambTex->id);
    glDeleteTextures(1, &data->material.normTex->id);
    glDeleteTextures(1, &data->material.emisTex->id);

    data->VAO                   = 0;
    data->VBO                   = 0;
    data->NBO                   = 0;
    data->IBO                   = 0;
    data->UV_0_BO               = 0;
    data->TBO                   = 0;
    data->material.diffTex->id  = 0;
    data->material.specTex->id  = 0;
    data->material.ambTex->id   = 0;
    data->material.normTex->id  = 0;
    data->material.emisTex->id  = 0;

    PollGLErrors();
}

void OpenGLRenderer::QueueForRender(RenderData3D *data, Scene::TransformData3D *transform) {
    if (data->VAO == 0 || data->VBO == 0 || data->NBO == 0 || data->UV_0_BO == 0 || data->IBO == 0 || data->TBO == 0) {
        LoadBuffers(data);
    }

    LoadTextures(&data->material);

    if (data->material.opacity < 1.0f) {
        _transparentRenderData.emplace_back(data, transform);
    } else {
        _renderData.emplace_back(data, transform);
    }
}

void OpenGLRenderer::AddLight(LightData3D *data) {
    if (_numLights >= MAX_LIGHTS) {
        RAVEN_LOG_ERROR("Cannot add more lights to scene");
        return;
    }
    _lights[_numLights] = data;
    _numLights++;
}

void OpenGLRenderer::SetActiveCam(CameraData3D *data, Scene::TransformData3D *camTransform) {
    _cameraData = data;
    _camTransform = camTransform;
}

void OpenGLRenderer::LoadTexture(TextureData * texture) {
    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (texture->numChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->data.data());
    } else if (texture->numChannels == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->width, texture->height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->data.data());
    } else if (texture->numChannels == 1) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R, texture->width, texture->height, 0, GL_R, GL_UNSIGNED_BYTE, texture->data.data());
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    PollGLErrors();
}


void OpenGLRenderer::Initialize() {
    Viewports::globalViewportManager->GetViewport(_viewportID)->window->MakeCurrent();

    glewInit();

    _eventHandler->RegisterEventListener(Events::EventType::AppRender, [this] (const Events::Event &) { RenderFrame(); });

    _renderData.clear();

    _mainShader.Initialize(std::string(RAVEN_RESOURCE_PATH) + "Shaders/main.vert", std::string(RAVEN_RESOURCE_PATH) + "Shaders/main.frag");

    PollGLErrors();
    const auto glVersionString = reinterpret_cast<const char*>( glGetString(GL_VERSION));

    RAVEN_LOG_INFO("OpenGL (version: {}) Renderer Initialized", glVersionString);
}

void OpenGLRenderer::RenderMesh(RenderData3D &renderData, Scene::TransformData3D &transformData) {
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
    glBindTexture(GL_TEXTURE_2D, renderData.material.diffTex->id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, renderData.material.specTex->id);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, renderData.material.ambTex->id);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, renderData.material.normTex->id);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, renderData.material.emisTex->id);
    _mainShader.setInt("uMaterial.diffTex", 0);
    _mainShader.setInt("uMaterial.specTex", 1);
    _mainShader.setInt("uMaterial.ambTex",  2);
    _mainShader.setInt("uMaterial.normTex", 3);
    _mainShader.setInt("uMaterial.emisTex", 4);
    _mainShader.setBool("uMaterial.useDiffTex", renderData.material.diffTex->id != 0);
    _mainShader.setBool("uMaterial.useSpecTex", renderData.material.specTex->id != 0);
    _mainShader.setBool("uMaterial.useAmbTex",  renderData.material.ambTex->id  != 0);
    _mainShader.setBool("uMaterial.useNormTex", renderData.material.normTex->id != 0);
    _mainShader.setBool("uMaterial.useEmisTex", renderData.material.emisTex->id != 0);
    glBindVertexArray(renderData.VAO);
    auto mode = GL_TRIANGLES;
    if (renderData.material.wireframe) { mode = GL_LINES;}

    glDrawElements(mode, renderData.indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    PollGLErrors();
}

void OpenGLRenderer::RenderFrame() {
    const auto vp = Viewports::globalViewportManager->GetViewport(_viewportID);
    if (vp == nullptr || vp->renderer == nullptr || vp->window == nullptr) {
        return;
    }
    vp->window->MakeCurrent();

    _mainShader.use();

    glEnable(GL_DEPTH_TEST);
    PollGLErrors();
    glEnable(GL_SCISSOR_TEST);
    PollGLErrors();

    glViewport(vp->x_offset, vp->y_offset, vp->width, vp->height);
    PollGLErrors();
    glScissor(vp->x_offset, vp->y_offset, vp->width, vp->height);
    PollGLErrors();
    glClearColor(vp->borderColor[0], vp->borderColor[1], vp->borderColor[2], vp->borderColor[3]);
    PollGLErrors();
    glClear(GL_COLOR_BUFFER_BIT);
    PollGLErrors();

    glScissor(vp->x_offset + vp->borderWidth, vp->y_offset + vp->borderWidth, vp->width - vp->borderWidth*2, vp->height - vp->borderWidth*2);
    PollGLErrors();
    glClearColor(vp->clearColor[0], vp->clearColor[1], vp->clearColor[2], vp->clearColor[3]);
    PollGLErrors();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    PollGLErrors();


    if (_cameraData != nullptr && _camTransform != nullptr) {
        const glm::mat4 projectionMat = _cameraData->GetPerspectiveMatrix(vp->width, vp->height);
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
            _mainShader.setInt("uLights[" + std::to_string(i) + "].type", static_cast<RAVEN_INT>(light->type));
        }
        _mainShader.setInt("uNumLights", _numLights);

        for (auto &[rData, tData] : _renderData) {
            const auto renderData = rData;
            const auto transform = tData;
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
        for (auto &[rData, tData] : _transparentRenderData) {
            const auto renderData = rData;
            renderData->material.diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
            const auto transform = tData;
            RenderMesh(*renderData, *transform);
        }
        glDisable(GL_BLEND);
    }

    _numLights = 0;
    _transparentRenderData.clear();

    PollGLErrors();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_SCISSOR_TEST);
}
