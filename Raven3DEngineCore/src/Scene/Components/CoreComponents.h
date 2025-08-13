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
        std::vector<glm::vec2> uvs_1;
        std::vector<glm::vec2> uvs_2;
        std::vector<glm::vec2> uvs_3;
        std::vector<RAVEN_U_INT> indices;
        RAVEN_INT VAO, VBO, NBO, IBO, UV_0_BO, UV_1_BO, UV_2_BO, UV_3_BO;
        RAVEN_INT primitiveMode;
    };

}

namespace Raven3DEngineCore::Scene {

    enum class Entity_T : RAVEN_ENTITY_TYPE {};
    extern Entity_T NullEntity;

    struct Transform {
        glm::vec3 translation;
        glm::quat rotation;
        glm::vec3 scale;
    };

    static Transform GetTransformFromMat(const glm::mat4 &mat) {
        return Transform{
            .translation = glm::vec3(mat * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
            .rotation = glm::quat_cast(mat),
            .scale = glm::vec3(glm::vec3(mat * glm::vec4(1.0f, 1.0f, 1.0f, 0.0f)))
        };
    }

    struct TransformData {
        Transform localTransform;
        Transform worldTransform;

        void SetLocalTransform(const Transform &transform) {
            this->localTransform = transform;
            this->localTransform.scale = transform.scale;
            this->localTransform.rotation = transform.rotation;
        }

        void TransformByMat(const glm::mat4 &transformMat) {
            this->worldTransform.translation = glm::vec3(transformMat * glm::vec4(localTransform.translation, 1.0f));
            this->worldTransform.rotation = glm::quat_cast(transformMat * glm::mat4(localTransform.rotation));
            this->worldTransform.scale = glm::vec3(transformMat * glm::vec4(localTransform.scale, 1.0f));
        }

        [[nodiscard]] glm::mat4 GetMatrix() const {
            auto result = glm::mat4(1.0f);
            result = glm::translate(result, worldTransform.translation);
            result = glm::toMat4(worldTransform.rotation) * result;
            result = glm::scale(result, worldTransform.scale);
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