//
// Created by wesley on 8/7/25.
//

#ifndef PCH_H
#define PCH_H

#define GLM_ENABLE_EXPERIMENTAL

#include <string>
#include <vector>
#include <functional>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <format>
#include <cmath>

#include <SDL3/SDL.h>
#include <SDL3/SDL_gamepad.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "glm/detail/type_quat.hpp"
#include <entt/entt.hpp>

#include <assimp/config.h>
#ifndef AI_CONFIG_CHECK_IDENTITY_MATRIX_EPSILON_DEFAULT
#define AI_CONFIG_CHECK_IDENTITY_MATRIX_EPSILON_DEFAULT 10e-3f
#endif
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "assimp/cimport.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#endif //PCH_H
