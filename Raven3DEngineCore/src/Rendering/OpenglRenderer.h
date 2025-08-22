//
// Created by wesley on 8/22/2025.
//

#ifndef RAVEN3DENGINECORE_OPENGLRENDERER_H
#define RAVEN3DENGINECORE_OPENGLRENDERER_H


namespace Raven3DEngineCore::Rendering {
    class OpenGLRenderer final : public IRenderer {
        void RenderMesh(RenderData &renderData, Scene::TransformData &transformData) override;

        std::vector<std::pair<RenderData*, Scene::TransformData*>> _renderData {};
        std::vector<std::pair<RenderData*, Scene::TransformData*>> _transparentRenderData {};
        LightData *_lights[MAX_LIGHTS] {};
        RAVEN_INT _numLights {};

        CameraData* _cameraData {};
        Scene::TransformData* _camTransform {};

        GLShader _mainShader {};

        static void LoadBuffers(RenderData *data);

    public:
        explicit OpenGLRenderer(const RAVEN_U_INT viewportID) : IRenderer(viewportID)  {}
        ~OpenGLRenderer() override {
            RAVEN_LOG_INFO("OpenGL Renderer Shut Down");
        }

        void ReleaseRenderData(RenderData *data) override;

        void QueueForRender(RenderData *data, Scene::TransformData *transform) override;
        void AddLight(LightData *data) override;
        void SetActiveCam(CameraData *data, Scene::TransformData *camTransform) override;
        RAVEN_U_INT LoadTexture(RAVEN_INT width, RAVEN_INT height, RAVEN_BYTE* data, RAVEN_INT numComps) override;
        void Initialize() override;
        void RenderFrame() override;
    };
}

#endif //RAVEN3DENGINECORE_OPENGLRENDERER_H