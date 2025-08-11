//
// Created by wesley on 8/10/25.
//

#ifndef RAVEN3DENGINECORE_CORECOMPONENTS_H
#define RAVEN3DENGINECORE_CORECOMPONENTS_H
#include "glm/gtx/quaternion.hpp"

namespace Raven3DEngineCore::Rendering {
    struct MaterialData {
        glm::vec3 diffuseColor;
        glm::vec3 specularColor;
        glm::vec3 ambientColor;
        glm::vec3 emissiveColor;
        bool wireframe;
        bool twoSided;
        float opacity;
        float shininess;
        float shininessStrength;
        GLuint diffTex, specTex, ambTex, emisTex, normTex;
    };

    struct RenderData {
        MaterialData material;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uvs_0;
        std::vector<unsigned int> indices;
        GLuint VAO, VBO, NBO, IBO, UV_0_BO;
        GLuint primitiveMode;
    };

}

namespace Raven3DEngineCore::Scene {
    struct TransformData {
        glm::vec3 translation;
        glm::quat rotation;
        glm::vec3 scale;

        glm::mat4 GetMatrix() const {
            auto result = glm::mat4(1.0f);
            result = glm::translate(result, translation);
            result = result * glm::toMat4(rotation);
            result = glm::scale(result, scale);
            return result;
        }
    };
}

#endif //RAVEN3DENGINECORE_CORECOMPONENTS_H