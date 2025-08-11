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

#include <SDL3/SDL.h>
#include <SDL3/SDL_gamepad.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "glm/detail/type_quat.hpp"
#include <entt/entt.hpp>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#endif //PCH_H
