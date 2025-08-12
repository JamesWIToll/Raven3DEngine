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
        RAVEN_FLOAT opacity {1.0f};
        RAVEN_FLOAT shininess;
        RAVEN_FLOAT shininessStrength;
        RAVEN_INT diffTex, specTex, ambTex, emisTex, normTex;
    };

    struct RenderData {
        MaterialData material;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uvs_0;
        std::vector<RAVEN_U_INT> indices;
        RAVEN_INT VAO, VBO, NBO, IBO, UV_0_BO;
        RAVEN_INT primitiveMode;
    };

}

namespace Raven3DEngineCore::Scene {

    enum class Entity_T : RAVEN_ENTITY_TYPE {};

    struct TransformData {
        glm::vec3 translation;
        glm::quat rotation;
        glm::vec3 scale;

        void Transform(const glm::mat4 &transformMat) {
            this->translation = glm::vec3(transformMat * glm::vec4(translation, 1.0f));
            this->rotation = glm::quat_cast(transformMat * glm::mat4(rotation));
            this->scale = glm::vec3(transformMat * glm::vec4(scale, 1.0f));
        }

        [[nodiscard]] glm::mat4 GetMatrix() const {
            auto result = glm::mat4(1.0f);
            result = glm::translate(result, translation);
            result = glm::toMat4(rotation) * result;
            result = glm::scale(result, scale);
            return result;
        }
    };

    struct RelationshipData {
        Entity_T root;
        Entity_T parent;
        std::vector<Entity_T> children;
    };

    struct EntityMetaData {
        Entity_T id;
        std::string name;
    };
}

#endif //RAVEN3DENGINECORE_CORECOMPONENTS_H