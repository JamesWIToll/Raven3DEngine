//
// Created by wesley on 8/22/2025.
//

#ifndef RAVEN3DENGINECORE_RENDERCOMPONENTS_H
#define RAVEN3DENGINECORE_RENDERCOMPONENTS_H


namespace Raven3DEngineCore::Rendering {

    struct TextureData {
        ~TextureData();

        RAVEN_U_INT id {0};
        RAVEN_INT width, height;
        std::vector<RAVEN_BYTE> data;
        RAVEN_U_INT numChannels;
    };

    struct MaterialData3D {
        glm::vec3 diffuseColor;
        glm::vec3 specularColor;
        glm::vec3 ambientColor;
        glm::vec3 emissiveColor;
        bool wireframe;
        bool twoSided;
        RAVEN_FLOAT opacity {1.0f};
        RAVEN_FLOAT shininess;
        RAVEN_FLOAT shininessStrength;
        RAVEN_FLOAT metallicFactor;
        RAVEN_FLOAT roughnessFactor;
        TextureData *diffTex, *specTex, *ambTex, *emisTex, *normTex;
    };

    struct RenderData3D {
        MaterialData3D material;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec3> tangents;
        std::vector<glm::vec2> uvs_0;
        std::vector<RAVEN_U_INT> indices;
        RAVEN_U_INT VAO, VBO, NBO, IBO, TBO, UV_0_BO;
        RAVEN_U_INT primitiveMode;
    };

    struct CameraData3D {
        RAVEN_FLOAT fov { 40.0f };
        glm::vec3 position {0, 0, 0};
        glm::vec3 center {0, 0, -1};
        glm::vec3 up {0, 1, 0};
        bool current = false;

        void UpdateVectors(const glm::mat4 &transform) {
            position = glm::vec3(transform[3]);
            center = glm::normalize(glm::quat_cast(transform) * glm::vec3{0, 0, -1});
            up =  glm::vec3{0, 1, 0};
        }

        [[nodiscard]] glm::mat4 GetViewMatrix() const {
            return glm::lookAt(position, position + center, up);
        }

        [[nodiscard]] glm::mat4 GetPerspectiveMatrix(const RAVEN_INT &camWidth, const RAVEN_INT &camHeight) const {
            return glm::perspective(glm::radians(fov), static_cast<std::float_t>(camWidth)/static_cast<std::float_t>(camHeight), 0.1f, 200.0f);
        }

    };

    enum class LightType : RAVEN_U_INT {
        Directional = 0,
        Point = 1
    };

    struct LightData3D {
        LightType type { LightType::Point };
        glm::vec3 posDir {0.0, 0.0, 0.0};
        glm::vec3 color {1.0f};
        RAVEN_FLOAT intensity {1.0f};
        RAVEN_FLOAT radius {10.0f}; //if point light
    };

    struct FontData {
        std::string fontFilePath;
        RAVEN_INT charWidth, charHeight;
        RAVEN_BYTE *fontRawData;
        RAVEN_U_INT fontTextureID;
    };

    struct FontStyleData {
        glm::vec3 color;
        FontData regularFont;
        FontData boldFont;
        FontData italicFont;
        FontData boldItalicFont;
    };

    struct TextData {
        std::string text;
        FontStyleData fontStyle;
    };

}

#endif //RAVEN3DENGINECORE_RENDERCOMPONENTS_H