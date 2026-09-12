#include <Raven3DEngineCore.h>

using namespace Raven3DEngineCore::Importer;
using namespace Raven3DEngineCore::Rendering;

RAVEN_U_INT TextureManager::RegisterTexture(const TextureData& tex) {
    auto id = _nextTextureId++;
    _textures.emplace(id, tex);
    return id;
}

bool TextureManager::UnregisterTexture(const RAVEN_U_INT id) {
    if (_textures.contains(id)) {
        _textures.erase(id);
        return true;
    }
    return false;
}

TextureData* TextureManager::getTexData(const RAVEN_U_INT id) {
    if (!_textures.contains(id)) {
        return nullptr;
    }
    return &_textures[id];
}

