//
// Created by wesley on 8/10/25.
//

#ifndef RAVEN3DENGINECORE_SCENEMANAGER_H
#define RAVEN3DENGINECORE_SCENEMANAGER_H

namespace Raven3DEngineCore::Scene {


    class SceneManager final : public Events::EventNotifier {
        entt::basic_registry<Entity_T> _registry;
        Entity_T _root = NullEntity;
        Window::IRenderWindow* _window = nullptr;
    public:
        explicit SceneManager(Window::IRenderWindow *window);
        ~SceneManager() override;

        void Initialize();

        void Update();
        void ProcessRenderables(Rendering::IRenderer *renderer);

        Entity_T CreateEntity(const EntityMetaData& data, Entity_T parent = NullEntity);

        bool DestroyEntity(Entity_T entity);

        Entity_T GetParentEntity(Entity_T entity);

        bool ReparentEntity(Entity_T entity, Entity_T parent);
        bool AddChildEntity(Entity_T entity, Entity_T child);


        template<typename T_Comp1, typename... T_Comps>
        bool ConnectComponents(Entity_T entity, T_Comp1 firstComponent, T_Comps... restComponents) {
            _registry.emplace<T_Comp1>(entity, firstComponent);
            if constexpr (sizeof...(restComponents) > 0) {
                return ConnectComponents(entity, restComponents...);
            }
            return true;
        }

        template<typename T_Comp1, typename... T_Comps>
        bool DisconnectComponents(const Entity_T entity) {
            _registry.remove<T_Comp1, T_Comps...>(entity);
            return true;
        }

        template<typename T_Comp>
        bool HasComponent(const Entity_T entity) {
            if (auto ptr = _registry.try_get<T_Comp>(entity); ptr != nullptr) {
                return true;
            }
            return false;
        }


        template<typename T_Comp>
        T_Comp *GetComponent(const Entity_T entity) {
            if (auto ptr = _registry.try_get<T_Comp>(entity); ptr != nullptr) {
                return ptr;
            }
            RAVEN_LOG_ERROR("Entity {} does not have component {}", static_cast<RAVEN_ENTITY_TYPE>(entity), typeid(T_Comp).name());
            return nullptr;
        }

        template<typename... T_Comps>
        std::tuple<T_Comps *...> GetComponents(Entity_T entity) {
            std::tuple<T_Comps...> components;
            std::string errorTypeNames;
            for ([[maybe_unused]] const auto type : {typeid(T_Comps)...}) {
                auto ptr = _registry.try_get<type>(entity);
                if (ptr != nullptr) {
                    components.emplace_back(*ptr);
                } else {
                    errorTypeNames += typeid(type).name();
                    errorTypeNames += ", ";
                }
            }
            if (!errorTypeNames.empty()) {
                RAVEN_LOG_ERROR("Entity {} does not have components: {}", static_cast<RAVEN_ENTITY_TYPE>(entity), errorTypeNames);
                return {};
            }
            return components;
        }
        [[nodiscard]] Entity_T GetRootEntity() const {return _root;}


        void PrintSceneGraph() const;
    };
}



#endif //RAVEN3DENGINECORE_SCENEMANAGER_H