//
// Created by wesley on 8/12/25.
//

#ifndef RAVEN3DENGINECORE_IMPORTER_H
#define RAVEN3DENGINECORE_IMPORTER_H

namespace Raven3DEngineCore::Importer {

    static glm::mat4 ConvertAssimpMatrix(const aiMatrix4x4 &matrix) {
        glm::mat4 mat;
        mat[0][0] = matrix.a1; mat[1][0] = matrix.a2; mat[2][0] = matrix.a3; mat[3][0] = matrix.a4;
        mat[0][1] = matrix.b1; mat[1][1] = matrix.b2; mat[2][1] = matrix.b3; mat[3][1] = matrix.b4;
        mat[0][2] = matrix.c1; mat[1][2] = matrix.c2; mat[2][2] = matrix.c3; mat[3][2] = matrix.c4;
        mat[0][3] = matrix.d1; mat[1][3] = matrix.d2; mat[2][3] = matrix.d3; mat[3][3] = matrix.d4;
        return mat;
    }

    extern bool Import3DFileToScene(const std::string &filePath, Scene::SceneManager &scene, Rendering::IRenderer &renderer, Scene::Entity_T parent = Scene::NullEntity);

}

#endif //RAVEN3DENGINECORE_IMPORTER_H