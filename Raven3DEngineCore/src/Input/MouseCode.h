//
// Created by wesley on 8/6/25.
//

#ifndef MOUSECODE_H
#define MOUSECODE_H

namespace Raven3DEngineCore::Input::Mouse {

    enum class MouseCode {
        BUTTON_0 = SDL_BUTTON_LEFT,
        BUTTON_1 = SDL_BUTTON_RIGHT,
        BUTTON_2 = SDL_BUTTON_MIDDLE,
        BUTTON_3 = SDL_BUTTON_X1,
        BUTTON_4 = SDL_BUTTON_X2,
    };
}
#endif //MOUSECODE_H
