//
// Created by wesley on 8/10/25.
//

#ifndef RAVEN3DENGINECORE_CORECOMPONENTS_H
#define RAVEN3DENGINECORE_CORECOMPONENTS_H

namespace Raven3DEngineCore::Scene {

    enum class Entity_T : RAVEN_ENTITY_TYPE {};
    extern Entity_T NullEntity;

    struct TransformData3D {
        glm::mat4 localTransform {1.0f };
        glm::mat4 worldTransform {1.0f };

        void UpdateWorldTransform(const glm::mat4 &transformMat) {
            worldTransform = transformMat * localTransform;
        }
    };

    struct TransformData2D {
        glm::mat3 localTransform {1.0f };
        glm::mat3 worldTransform {1.0f };

        void UpdateWorldTransform(const glm::mat3 &transformMat) {
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

#endif //RAVEN3DENGINECORE_CORECOMPONENTS_H