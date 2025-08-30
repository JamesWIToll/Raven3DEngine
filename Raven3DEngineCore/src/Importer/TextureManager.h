//
// Created by wesley on 8/30/2025.
//

#ifndef RAVEN3DENGINECORE_TEXTUREMANAGER_H
#define RAVEN3DENGINECORE_TEXTUREMANAGER_H


namespace Raven3DEngineCore::Importer {
    class TextureManager {
        std::map<RAVEN_U_INT, Rendering::TextureData> _textures {};
        RAVEN_U_INT _nextTextureId = 1;
    public:
        TextureManager() = default;
        ~TextureManager() = default;

        [[nodiscard]] Rendering::TextureData* getTexData(const RAVEN_U_INT id) {
            if (!_textures.contains(id)) {
                return nullptr;
            }
            return &_textures[id];
        }

        RAVEN_U_INT RegisterTexture(const Rendering::TextureData& tex) {
            auto id = _nextTextureId++;
            _textures.emplace(id, tex);
            return id;
        }

        bool UnregisterTexture(const RAVEN_U_INT id) {
            if (_textures.contains(id)) {
                _textures.erase(id);
                return true;
            }
            return false;
        }
    };

    extern TextureManager globalTextureManager;
}


#endif //RAVEN3DENGINECORE_TEXTUREMANAGER_H