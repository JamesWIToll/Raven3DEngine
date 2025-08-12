//
// Created by wesley on 8/11/25.
//

#ifndef RAVEN3DENGINECORE_CORECOMPONENTGETTERS_H
#define RAVEN3DENGINECORE_CORECOMPONENTGETTERS_H

namespace Raven3DEngineCore::Scene {
    static TransformData GetTransformData(const SceneManager &scene, const Entity_T &entity) {
        const auto data = scene.GetComponent<TransformData>(entity);
        return data;
    }

    static EntityMetaData GetEntityMetaData(const SceneManager &scene, const Entity_T &entity) {
        const auto data = scene.GetComponent<EntityMetaData>(entity);
        return data;
    }

    static RelationshipData GetRelationshipData(const SceneManager &scene, const Entity_T &entity) {
        const auto data = scene.GetComponent<RelationshipData>(entity);
        return data;
    }

    static Rendering::RenderData GetRenderData(const SceneManager &scene, const Entity_T &entity) {
        const auto data = scene.GetComponent<Rendering::RenderData>(entity);
        return data;
    }

    static Rendering::MaterialData GetMaterialData(const SceneManager &scene, const Entity_T &entity) {
        const auto data = scene.GetComponent<Rendering::MaterialData>(entity);
        return data;
    }

}

#endif