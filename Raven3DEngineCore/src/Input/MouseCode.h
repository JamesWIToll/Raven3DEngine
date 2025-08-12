//
// Created by wesley on 8/6/25.
//

#ifndef MOUSECODE_H
#define MOUSECODE_H

namespace Raven3DEngineCore::Input::Mouse {

    enum class MouseCode : RAVEN_U_INT {
        BUTTON_LEFT = SDL_BUTTON_LEFT,
        BUTTON_RIGHT = SDL_BUTTON_RIGHT,
        BUTTON_MIDDLE = SDL_BUTTON_MIDDLE,
        BUTTON_X1 = SDL_BUTTON_X1,
        BUTTON_X2 = SDL_BUTTON_X2,
    };

    static std::string getMouseCodeName(const MouseCode code) {
        switch (code) {
            case MouseCode::BUTTON_LEFT:
                return "Left Click";
                break;
            case MouseCode::BUTTON_RIGHT:
                return "Right Click";
                break;
            case MouseCode::BUTTON_MIDDLE:
                return "Middle Click";
                break;
            case MouseCode::BUTTON_X1:
                return "X1 Click";
                break;
            case MouseCode::BUTTON_X2:
                return "X2 Click";
                break;
        }
        return "Unknown Mouse Button";
    }
}
#endif //MOUSECODE_H
