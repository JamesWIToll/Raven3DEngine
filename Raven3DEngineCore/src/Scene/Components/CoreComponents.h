//
// Created by wesley on 8/10/25.
//

#ifndef RAVEN3DENGINECORE_CORECOMPONENTS_H
#define RAVEN3DENGINECORE_CORECOMPONENTS_H
#include "glm/gtx/quaternion.hpp"

namespace Raven3DEngineCore::Scene {

    enum class Entity_T : RAVEN_ENTITY_TYPE {};
    extern Entity_T NullEntity;

    struct TransformData {
        glm::mat4 localTransform {1.0f };
        glm::mat4 worldTransform {1.0f };

        void UpdateWorldTransform(const glm::mat4 &transformMat) {
            worldTransform = transformMat * localTransform;
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
        RAVEN_U_INT diffTex, specTex, ambTex, emisTex, normTex;
    };

    struct RenderData {
        MaterialData material;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uvs_0;
        std::vector<RAVEN_U_INT> indices;
        RAVEN_U_INT VAO, VBO, NBO, IBO, UV_0_BO;
        RAVEN_U_INT primitiveMode;
    };

    struct CameraData {
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

        [[nodiscard]] glm::mat4 GetProjectionMatrix(const RAVEN_INT &camWidth, const RAVEN_INT &camHeight) const {
            return glm::perspective(glm::radians(fov), static_cast<std::float_t>(camWidth)/static_cast<std::float_t>(camHeight), 0.1f, 200.0f);
        }

    };

    enum class LightType {
        Directional,
        Point
    };

    struct LightData {
        LightType type { LightType::Point };
        glm::vec3 direction {0.0, -1.0, 0.0}; //if directional
        glm::vec3 color {1.0f};
        RAVEN_FLOAT intensity {1.0f};
        RAVEN_FLOAT radius {10.0f}; //if point light
    };

}

#endif //RAVEN3DENGINECORE_CORECOMPONENTS_H