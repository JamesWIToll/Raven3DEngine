//
// Created by wesley on 8/12/25.
//

#include <Raven3DEngineCore.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace Raven3DEngineCore::Importer;
using namespace Raven3DEngineCore::Scene;
using namespace Raven3DEngineCore::Rendering;


void Raven3DEngineCore::Importer::FreeImage(RAVEN_BYTE* data) {
    stbi_image_free(data);
}


Entity_T AssimpImporter::ImportFile(const std::string &filePath, Entity_T parent) {
    Assimp::Importer importer;

    const auto ai_Scene = importer.ReadFile(filePath,
        aiProcess_Triangulate |
        aiProcess_FlipUVs |
        aiProcess_EmbedTextures |
        aiProcess_CalcTangentSpace
    );

    if (!ai_Scene) {
        RAVEN_LOG_ERROR("Failed to load 3D model: {}", filePath);
        return NullEntity;
    }

    if (parent == NullEntity) {
        parent = _scene->CreateEntity(EntityMetaData{
            .name = "Imported Scene ("  + std::string(ai_Scene->mName.C_Str()) + ")"
        });
        _scene->ConnectComponents<TransformData3D>(parent, TransformData3D{
            .localTransform = { 1.0f }
        });
    }

    const std::function LoadTexture = [&](const std::string &texPath) -> TextureData* {
            auto* texture = new TextureData();
            if (texPath[0] == '*') {
                const auto Tex = ai_Scene->mTextures[std::stoi(texPath.substr(1))];
                const int length = Tex->mHeight == 0 ? Tex->mWidth * sizeof(aiTexel) : Tex->mHeight * Tex->mWidth * sizeof(aiTexel);
                int height, width, compPerPixel;
                const RAVEN_BYTE* data = stbi_load_from_memory(reinterpret_cast<RAVEN_BYTE*>(Tex->pcData), length, &width, &height, &compPerPixel, 0);
                texture->data = std::vector<RAVEN_BYTE>(data, data + width * height * compPerPixel);
                texture->width = width;
                texture->height = height;
                texture->numChannels = compPerPixel;
                _renderer->LoadTexture(texture);
            } else {
                int height, width, compPerPixel;
                const RAVEN_BYTE* data = stbi_load(texPath.c_str(), &width, &height, &compPerPixel, 0);
                const auto length = width * height * compPerPixel;
                texture->data = std::vector<RAVEN_BYTE>(data, data + length);
                texture->width = width;
                texture->height = height;
                texture->numChannels = compPerPixel;
                _renderer->LoadTexture(texture);
            }
            return texture;
    };

    std::function<Entity_T(const aiNode*, Entity_T currParent)> loadNode;
    loadNode = [&](const aiNode *current, Entity_T currParent) -> Entity_T{
        if (current == nullptr) {
            return NullEntity;
        }
        if (currParent == NullEntity) {
            currParent = parent;
        }

        const auto mat = current->mTransformation;
        const auto glmMat = ConvertAssimpMatrix(mat);

        const Entity_T entity = _scene->CreateEntity(EntityMetaData{
            .name = current->mName.C_Str()
        }, currParent);


        if (_scene->HasComponent<TransformData3D>(entity)) {
            _scene->GetComponent<TransformData3D>(entity)->localTransform = glmMat;
        } else {
            _scene->ConnectComponents<TransformData3D>(entity, TransformData3D{
                .localTransform = glmMat
            });
        }

        for (RAVEN_INT i=0; i < current->mNumMeshes; i++) {
            const auto mesh = ai_Scene->mMeshes[current->mMeshes[i]];
            const auto meshEntity = _scene->CreateEntity(EntityMetaData{
                .name = mesh->mName.C_Str()
            }, entity);

            RenderData3D renderData {};

            for (RAVEN_INT v = 0; v < mesh->mNumVertices; v++) {
                renderData.vertices.emplace_back(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
                renderData.normals.emplace_back(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
                renderData.uvs_0.emplace_back(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
                renderData.tangents.emplace_back(mesh->mTangents[v].x, mesh->mTangents[v].y, mesh->mTangents[v].z);
            }

            for (RAVEN_INT f = 0; f < mesh->mNumFaces; f++) {
                const auto face = mesh->mFaces[f];
                for (RAVEN_INT j = 0; j < face.mNumIndices; j++) {
                    renderData.indices.emplace_back(face.mIndices[j]);
                }
            }

            MaterialData3D materialData {};


            const auto material = ai_Scene->mMaterials[mesh->mMaterialIndex];

            aiColor3D diffuseColor; material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
            aiColor3D specularColor; material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
            aiColor3D ambientColor; material->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
            aiColor3D emissiveColor; material->Get(AI_MATKEY_COLOR_EMISSIVE, emissiveColor);
            bool wireframe; material->Get(AI_MATKEY_ENABLE_WIREFRAME, wireframe);
            RAVEN_FLOAT shininess; material->Get(AI_MATKEY_SHININESS, shininess);
            RAVEN_FLOAT shininessStrength; material->Get(AI_MATKEY_SHININESS_STRENGTH, shininessStrength);
            RAVEN_FLOAT opacity; material->Get(AI_MATKEY_OPACITY, opacity);
            RAVEN_FLOAT metallicFactor; material->Get(AI_MATKEY_METALLIC_FACTOR, metallicFactor);
            RAVEN_FLOAT roughnessFactor; material->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughnessFactor);

            RAVEN_INT diffTexIdx = -1; material->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), diffTexIdx);
            TextureData* diffTex;
            if (diffTexIdx > -1) {
                aiString diffTexturePath; material->GetTexture(aiTextureType_DIFFUSE, diffTexIdx, &diffTexturePath);
                diffTex = LoadTexture(std::string(diffTexturePath.C_Str()));
            }


            RAVEN_INT specTexIdx = -1; material->Get(AI_MATKEY_TEXTURE_SPECULAR(0), specTexIdx);
            TextureData* specTex;
            if (specTexIdx > -1) {
                aiString specTexturePath; material->GetTexture(aiTextureType_SPECULAR, specTexIdx, &specTexturePath);
                specTex = LoadTexture(std::string(specTexturePath.C_Str()));
            }


            RAVEN_INT ambTexIdx = -1; material->Get(AI_MATKEY_TEXTURE_AMBIENT(0), ambTexIdx);
            TextureData* ambTex;
            if (ambTexIdx > -1) {
                aiString ambTexturePath; material->GetTexture(aiTextureType_AMBIENT, 0, &ambTexturePath);
                ambTex = LoadTexture(std::string(ambTexturePath.C_Str()));
            }


            RAVEN_INT emiTexIdx = -1; material->Get(AI_MATKEY_TEXTURE_EMISSIVE(0), emiTexIdx);
            TextureData* emiTex;
            if (emiTexIdx > -1) {
                aiString emisTexturePath; material->GetTexture(aiTextureType_EMISSIVE, 0, &emisTexturePath);
                emiTex = LoadTexture(std::string(emisTexturePath.C_Str()));
            }


            RAVEN_INT normTexIdx = -1; material->Get(AI_MATKEY_TEXTURE_NORMALS(0), normTexIdx);
            TextureData* normTex;
            if (normTexIdx > -1) {
                aiString normTexturePath; material->GetTexture(aiTextureType_NORMALS, 0, &normTexturePath);
                normTex = LoadTexture(std::string(normTexturePath.C_Str()));
            }



            materialData.diffuseColor = glm::vec3(diffuseColor.r, diffuseColor.g, diffuseColor.b);
            materialData.specularColor = glm::vec3(specularColor.r, specularColor.g, specularColor.b);
            materialData.ambientColor = glm::vec3(ambientColor.r, ambientColor.g, ambientColor.b);
            materialData.emissiveColor = glm::vec3(emissiveColor.r, emissiveColor.g, emissiveColor.b);
            materialData.wireframe = wireframe;
            materialData.shininess = shininess;
            materialData.shininessStrength = shininessStrength;
            materialData.opacity = opacity;
            materialData.diffTex = diffTex;
            materialData.specTex = specTex;
            materialData.ambTex = ambTex;
            materialData.emisTex = emiTex;
            materialData.normTex = normTex;
            materialData.metallicFactor = metallicFactor;
            materialData.roughnessFactor = roughnessFactor;

            renderData.material = materialData;
            _scene->ConnectComponents<RenderData3D, TransformData3D>(meshEntity, renderData, TransformData3D{});
        }

        for (RAVEN_INT i=0; i < current->mNumChildren; i++) {
            loadNode(current->mChildren[i], entity);
        }

        return entity;
    };

    loadNode(ai_Scene->mRootNode, NullEntity);

    RAVEN_LOG_DEBUG("Successfully imported 3D model: {}", filePath);

    return parent;
}