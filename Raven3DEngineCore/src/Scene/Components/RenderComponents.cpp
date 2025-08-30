//
// Created by wesley on 8/30/2025.
//
#include <Raven3DEngineCore.h>

Raven3DEngineCore::Rendering::TextureData::~TextureData() {
    Importer::FreeImage(data.data());
}
