//
// Created by wesley on 8/10/25.
//

#ifndef RAVEN3DENGINECORE_SCENEMANAGER_H
#define RAVEN3DENGINECORE_SCENEMANAGER_H

namespace Raven3DEngineCore::Scene {
    class SceneManager {
        entt::basic_registry<Entity_T> _registry;
        Entity_T _root = entt::null;
    public:
        SceneManager();
        ~SceneManager();

        void Update();
        void ProcessRenderables(Rendering::IRenderer *renderer);

        Entity_T CreateEntity(const EntityMetaData& data, Entity_T parent = entt::null);

        bool DestroyEntity(Entity_T entity);

        Entity_T GetParentEntity(Entity_T entity);

        bool ReparentEntity(Entity_T entity, Entity_T parent);
        bool AddChildEntity(Entity_T entity, Entity_T child);


        template<typename T_Comp1, typename... T_Comps>
        bool ConnectComponents(Entity_T entity, T_Comp1 firstComponent, T_Comps... restComponents);

        template<typename T_Comp1, typename... T_Comps>
        bool DisconnectComponents(Entity_T entity);

        template<typename T_Comp>
        T_Comp GetComponent(Entity_T entity) const;

        template<typename... T_Comps>
        std::tuple<T_Comps...> GetComponents(Entity_T entity) const;


        [[nodiscard]] Entity_T GetRootEntity() const {return _root;}

    };
}



#endif //RAVEN3DENGINECORE_SCENEMANAGER_H