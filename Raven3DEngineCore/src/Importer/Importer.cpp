//
// Created by wesley on 8/12/25.
//

#include <Raven3DEngineCore.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


using namespace Raven3DEngineCore::Importer;
using namespace Raven3DEngineCore::Scene;
using namespace Raven3DEngineCore::Rendering;

static RAVEN_INT LoadTexture(const std::string &path, const aiScene *ai_Scene, IRenderer &renderer) {
    RAVEN_INT textureID {};
    if (path[0] == '*') {
        const auto Tex = ai_Scene->mTextures[std::stoi(path.substr(1))];
        const int length = Tex->mHeight == 0 ? Tex->mWidth : Tex->mHeight * Tex->mWidth * sizeof(aiTexel);
        int height, width, compPerPixel;
        RAVEN_BYTE* TexData = stbi_load_from_memory(reinterpret_cast<RAVEN_BYTE*>(Tex->pcData), length, &width, &height, &compPerPixel, 0);
        textureID = renderer.LoadTexture(width, height, TexData, compPerPixel);
        stbi_image_free(TexData);
    } else {
        int height, width, compPerPixel;
        RAVEN_BYTE* TexData = stbi_load(path.c_str(), &width, &height, &compPerPixel, 0);
        textureID = renderer.LoadTexture(width, height, TexData, compPerPixel);
        stbi_image_free(TexData);
    }
    return textureID;
}

bool Import3DFileToScene(const std::string &filePath, SceneManager &scene, IRenderer &renderer, Entity_T parent = NullEntity) {
    Assimp::Importer importer;

    if (parent == NullEntity) {
        parent = scene.GetRootEntity();
    }

    const auto ai_Scene = importer.ReadFile(filePath,
        aiProcess_JoinIdenticalVertices |
        aiProcess_Triangulate |
        aiProcess_EmbedTextures
    );

    if (!ai_Scene) {
        RAVEN_LOG_ERROR("Failed to load 3D model: {}", filePath);
        return false;
    }

    std::function<Entity_T(const aiNode*)> loadNode;
    loadNode = [&](const aiNode *current) -> Entity_T{
        if (current == nullptr) {
            return NullEntity;
        }

        const auto mat = current->mTransformation;
        const auto glmMat = ConvertAssimpMatrix(mat);

        const Entity_T entity = scene.CreateEntity(EntityMetaData{
            .name = current->mName.C_Str()
        }, parent);
        scene.GetComponent<TransformData>(entity)->SetLocalTransform(GetTransformFromMat(glmMat));

        for (RAVEN_INT i=0; i < current->mNumMeshes; i++) {
            const auto mesh = ai_Scene->mMeshes[current->mMeshes[i]];
            const auto meshEntity = scene.CreateEntity(EntityMetaData{
                .name = mesh->mName.C_Str()
            }, entity);

            Raven3DEngineCore::Rendering::RenderData renderData {};

            for (RAVEN_INT v = 0; v < mesh->mNumVertices; v++) {
                renderData.vertices.push_back(glm::vec3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z));
                renderData.normals.push_back(glm::vec3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z));
                renderData.uvs_0.push_back(glm::vec2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y));
                renderData.uvs_1.push_back(glm::vec2(mesh->mTextureCoords[1][v].x, mesh->mTextureCoords[1][v].y));
                renderData.uvs_2.push_back(glm::vec2(mesh->mTextureCoords[2][v].x, mesh->mTextureCoords[2][v].y));
                renderData.uvs_3.push_back(glm::vec2(mesh->mTextureCoords[3][v].x, mesh->mTextureCoords[3][v].y));
            }

            for (RAVEN_INT f = 0; f < mesh->mNumFaces; f++) {
                const auto face = mesh->mFaces[f];
                for (RAVEN_INT j = 0; j < face.mNumIndices; j++) {
                    renderData.indices.push_back(face.mIndices[j]);
                }
            }

            MaterialData materialData {};


            const auto material = ai_Scene->mMaterials[mesh->mMaterialIndex];

            aiColor3D diffuseColor; material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
            aiColor3D specularColor; material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
            aiColor3D ambientColor; material->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
            aiColor3D emissiveColor; material->Get(AI_MATKEY_COLOR_EMISSIVE, emissiveColor);
            bool wireframe; material->Get(AI_MATKEY_ENABLE_WIREFRAME, wireframe);
            RAVEN_FLOAT shininess; material->Get(AI_MATKEY_SHININESS, shininess);
            RAVEN_FLOAT shininessStrength; material->Get(AI_MATKEY_SHININESS_STRENGTH, shininessStrength);
            RAVEN_FLOAT opacity; material->Get(AI_MATKEY_OPACITY, opacity);

            RAVEN_INT diffTexIdx = -1; material->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), diffTexIdx);
            aiString diffTexturePath; material->GetTexture(aiTextureType_DIFFUSE, diffTexIdx, &diffTexturePath);
            auto diffTexID = LoadTexture(std::string(diffTexturePath.C_Str()), ai_Scene, renderer);


            RAVEN_INT specTexIdx = -1; material->Get(AI_MATKEY_TEXTURE_SPECULAR(0), specTexIdx);
            aiString specTexturePath; material->GetTexture(aiTextureType_SPECULAR, specTexIdx, &specTexturePath);
            auto specTexID = LoadTexture(std::string(specTexturePath.C_Str()), ai_Scene, renderer);

            RAVEN_INT ambTexIdx = -1; material->Get(AI_MATKEY_TEXTURE_AMBIENT(0), ambTexIdx);
            aiString ambTexturePath; material->GetTexture(aiTextureType_AMBIENT, 0, &ambTexturePath);
            auto ambTexID = LoadTexture(std::string(ambTexturePath.C_Str()), ai_Scene, renderer);

            RAVEN_INT emiTexIdx = -1; material->Get(AI_MATKEY_TEXTURE_EMISSIVE(0), emiTexIdx);
            aiString emisTexturePath; material->GetTexture(aiTextureType_EMISSIVE, 0, &emisTexturePath);
            auto emiTexID = LoadTexture(std::string(emisTexturePath.C_Str()), ai_Scene, renderer);

            RAVEN_INT normTexIdx = -1; material->Get(AI_MATKEY_TEXTURE_NORMALS(0), normTexIdx);
            aiString normTexturePath; material->GetTexture(aiTextureType_NORMALS, 0, &normTexturePath);
            auto normTexID = LoadTexture(std::string(normTexturePath.C_Str()), ai_Scene, renderer);


            materialData.diffuseColor = glm::vec3(diffuseColor.r, diffuseColor.g, diffuseColor.b);
            materialData.specularColor = glm::vec3(specularColor.r, specularColor.g, specularColor.b);
            materialData.ambientColor = glm::vec3(ambientColor.r, ambientColor.g, ambientColor.b);
            materialData.emissiveColor = glm::vec3(emissiveColor.r, emissiveColor.g, emissiveColor.b);
            materialData.wireframe = wireframe;
            materialData.shininess = shininess;
            materialData.shininessStrength = shininessStrength;
            materialData.opacity = opacity;
            materialData.diffTex = diffTexID;
            materialData.specTex = specTexID;
            materialData.ambTex = ambTexID;
            materialData.emisTex = emiTexID;
            materialData.normTex = normTexID;

            renderData.material = materialData;
            scene.ConnectComponents<RenderData>(meshEntity, renderData);

            scene.AddChildEntity(entity, meshEntity);
        }

        for (RAVEN_INT i=0; i < current->mNumChildren; i++) {
            const auto childEntity = loadNode(current->mChildren[i]);
            scene.AddChildEntity(entity, childEntity);
        }

        return entity;
    };

    loadNode(ai_Scene->mRootNode);

    return true;
}