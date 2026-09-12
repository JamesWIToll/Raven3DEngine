//
// Created by wesley on 8/30/2025.
//

#ifndef RAVEN3DENGINECORE_TEXTUREMANAGER_H
#define RAVEN3DENGINECORE_TEXTUREMANAGER_H
#include <RavenForward.h>

namespace Raven3DEngineCore::Importer {
    class TextureManager {
        std::map<RAVEN_U_INT, Rendering::TextureData> _textures {};
        RAVEN_U_INT _nextTextureId = 1;
    public:
        TextureManager() = default;
        ~TextureManager() = default;

        [[nodiscard]] Rendering::TextureData* getTexData(const RAVEN_U_INT id);

        RAVEN_U_INT RegisterTexture(const Rendering::TextureData& tex);


        bool UnregisterTexture(const RAVEN_U_INT id); 
    };

    extern TextureManager globalTextureManager;
}


#endif //RAVEN3DENGINECORE_TEXTUREMANAGER_H
