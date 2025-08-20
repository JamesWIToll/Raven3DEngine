//
// Created by wesley on 8/4/25.
//

#ifndef IRENDERER_H
#define IRENDERER_H

#define MAX_LIGHTS 50

namespace Raven3DEngineCore::Rendering {

    enum class RenderAPI : RAVEN_U_SHORT{
        OPENGL,
    };

    class IRenderer : public Events::EventNotifier {
    protected:
        virtual void RenderMesh(RenderData &renderData, Scene::TransformData &transformData) = 0;
    public:
        ~IRenderer() override = default;

        virtual void ReleaseRenderData(RenderData *data) = 0;

        virtual void QueueForRender(RenderData *data, Scene::TransformData *transform) = 0;
        virtual void AddLight(LightData *data) = 0;
        virtual void SetActiveCam(CameraData *data, Scene::TransformData *camTransform) = 0;
        virtual RAVEN_U_INT LoadTexture(RAVEN_INT width, RAVEN_INT height, RAVEN_BYTE* data, RAVEN_INT numComps) = 0;

        virtual void Initialize(const glm::vec3 &clearColor, const RAVEN_INT &width, const RAVEN_INT &height) = 0;
        virtual void RenderFrame() = 0;
    };


    class OpenGLRenderer final : public IRenderer {
        void RenderMesh(RenderData &renderData, Scene::TransformData &transformData) override;

        std::vector<std::pair<RenderData*, Scene::TransformData*>> _renderData {};
        std::vector<std::pair<RenderData*, Scene::TransformData*>> _transparentRenderData {};
        LightData *_lights[MAX_LIGHTS] {};
        RAVEN_INT _numLights {};

        CameraData* _cameraData {};
        Scene::TransformData* _camTransform {};

        GLShader _mainShader {};

        RAVEN_INT _width {}, _height {};
        glm::vec3 _clearColor {};

        static void LoadBuffers(RenderData *data) {
            GLuint VAO, VBO, NBO, IBO, UV_0_BO, TBO;
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
        }

    public:
        OpenGLRenderer() = default;
        ~OpenGLRenderer() override {
            RAVEN_LOG_INFO("OpenGL Renderer Shut Down");
        }

        void ReleaseRenderData(RenderData *data) override;

        void QueueForRender(RenderData *data, Scene::TransformData *transform) override;
        void AddLight(LightData *data) override;
        void SetActiveCam(CameraData *data, Scene::TransformData *camTransform) override;
        RAVEN_U_INT LoadTexture(RAVEN_INT width, RAVEN_INT height, RAVEN_BYTE* data, RAVEN_INT numComps) override;
        void Initialize(const glm::vec3 & clearColor, const RAVEN_INT &width, const RAVEN_INT &height) override;
        void RenderFrame() override;
    };
}



#endif //IRENDERER_H
