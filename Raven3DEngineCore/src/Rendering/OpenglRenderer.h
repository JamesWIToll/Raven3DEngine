//
// Created by wesley on 8/22/2025.
//

#ifndef RAVEN3DENGINECORE_OPENGLRENDERER_H
#define RAVEN3DENGINECORE_OPENGLRENDERER_H


namespace Raven3DEngineCore::Rendering {
    class OpenGLRenderer final : public IRenderer {
        void RenderMesh(RenderData3D &renderData, Scene::TransformData3D &transformData) override;

        std::vector<std::pair<RenderData3D*, Scene::TransformData3D*>> _renderData {};
        std::vector<std::pair<RenderData3D*, Scene::TransformData3D*>> _transparentRenderData {};
        LightData3D *_lights[MAX_LIGHTS] {};
        RAVEN_INT _numLights {};

        CameraData3D* _cameraData {};
        Scene::TransformData3D* _camTransform {};

        GLShader _mainShader {};

        static void LoadBuffers(RenderData3D *data);
        void LoadTextures(MaterialData3D *data);

    public:
        explicit OpenGLRenderer(const RAVEN_U_INT viewportID) : IRenderer(viewportID) {}
        ~OpenGLRenderer() override {
            RAVEN_LOG_INFO("OpenGL Renderer Shut Down");
        }

        void ReleaseRenderData(RenderData3D *data) override;

        void QueueForRender(RenderData3D *data, Scene::TransformData3D *transform) override;
        void AddLight(LightData3D *data) override;
        void SetActiveCam(CameraData3D *data, Scene::TransformData3D *camTransform) override;
        void LoadTexture(TextureData *texture) override;
        void Initialize() override;
        void RenderFrame() override;
    };
}

#endif //RAVEN3DENGINECORE_OPENGLRENDERER_H