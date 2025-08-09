//
// Created by wesley on 8/6/25.
//

#ifndef KEYCODE_H
#define KEYCODE_H

namespace Raven3DEngineCore::Input::Key {

    enum class KeyCode : unsigned int {
        ONE = SDL_SCANCODE_1,
        TWO = SDL_SCANCODE_2,
        THREE = SDL_SCANCODE_3,
        FOUR = SDL_SCANCODE_4,
        FIVE = SDL_SCANCODE_5,
        SIX = SDL_SCANCODE_6,
        SEVEN = SDL_SCANCODE_7,
        EIGHT = SDL_SCANCODE_8,
        NINE = SDL_SCANCODE_9,
        ZERO = SDL_SCANCODE_0,
        A = SDL_SCANCODE_A,
        B = SDL_SCANCODE_B,
        C = SDL_SCANCODE_C,
        D = SDL_SCANCODE_D,
        E = SDL_SCANCODE_E,
        F = SDL_SCANCODE_F,
        G = SDL_SCANCODE_G,
        H = SDL_SCANCODE_H,
        I = SDL_SCANCODE_I,
        J = SDL_SCANCODE_J,
        K = SDL_SCANCODE_K,
        L = SDL_SCANCODE_L,
        M = SDL_SCANCODE_M,
        N = SDL_SCANCODE_N,
        O = SDL_SCANCODE_O,
        P = SDL_SCANCODE_P,
        Q = SDL_SCANCODE_Q,
        R = SDL_SCANCODE_R,
        S = SDL_SCANCODE_S,
        T = SDL_SCANCODE_T,
        U = SDL_SCANCODE_U,
        V = SDL_SCANCODE_V,
        W = SDL_SCANCODE_W,
        X = SDL_SCANCODE_X,
        Y = SDL_SCANCODE_Y,
        Z = SDL_SCANCODE_Z,
        KP_0 = SDL_SCANCODE_KP_0,
        KP_1 = SDL_SCANCODE_KP_1,
        KP_2 = SDL_SCANCODE_KP_2,
        KP_3 = SDL_SCANCODE_KP_3,
        KP_4 = SDL_SCANCODE_KP_4,
        KP_5 = SDL_SCANCODE_KP_5,
        KP_6 = SDL_SCANCODE_KP_6,
        KP_7 = SDL_SCANCODE_KP_7,
        KP_8 = SDL_SCANCODE_KP_8,
        KP_9 = SDL_SCANCODE_KP_9,
        KP_DECIMAL = SDL_SCANCODE_KP_DECIMAL,
        KP_DIVIDE = SDL_SCANCODE_KP_DIVIDE,
        KP_MULTIPLY = SDL_SCANCODE_KP_MULTIPLY,
        KP_SUBTRACT = SDL_SCANCODE_KP_MINUS,
        KP_ADD = SDL_SCANCODE_KP_PLUS,
        KP_ENTER = SDL_SCANCODE_KP_ENTER,
        KP_EQUAL = SDL_SCANCODE_KP_EQUALS,
        LEFT = SDL_SCANCODE_LEFT,
        RIGHT = SDL_SCANCODE_RIGHT,
        UP = SDL_SCANCODE_UP,
        DOWN = SDL_SCANCODE_DOWN,
        HOME = SDL_SCANCODE_HOME,
        END = SDL_SCANCODE_END,
        PAGE_UP = SDL_SCANCODE_PAGEUP,
        PAGE_DOWN = SDL_SCANCODE_PAGEDOWN,
        INSERT = SDL_SCANCODE_INSERT,
        DELETE = SDL_SCANCODE_DELETE,
        BACKSPACE = SDL_SCANCODE_BACKSPACE,
        TAB = SDL_SCANCODE_TAB,
        ESCAPE = SDL_SCANCODE_ESCAPE,
        SLASH  = SDL_SCANCODE_SLASH,
        BACK_SLASH = SDL_SCANCODE_BACKSLASH,
        BRACKET_LEFT = SDL_SCANCODE_LEFTBRACKET,
        BRACKET_RIGHT = SDL_SCANCODE_RIGHTBRACKET,
        APOSTROPHE = SDL_SCANCODE_APOSTROPHE,
        COMMA = SDL_SCANCODE_COMMA,
        PERIOD = SDL_SCANCODE_PERIOD,
        GRAVE = SDL_SCANCODE_GRAVE,
        EQUAL = SDL_SCANCODE_EQUALS,
        SUBTRACT = SDL_SCANCODE_MINUS,
        LEFT_CTRL = SDL_SCANCODE_LCTRL,
        LEFT_SHIFT = SDL_SCANCODE_LSHIFT,
        LEFT_ALT = SDL_SCANCODE_LALT,
        RIGHT_CTRL = SDL_SCANCODE_RCTRL,
        RIGHT_SHIFT = SDL_SCANCODE_RSHIFT,
        RIGHT_ALT = SDL_SCANCODE_RALT,
        SEMICOLON = SDL_SCANCODE_SEMICOLON,
        F1 = SDL_SCANCODE_F1,
        F2 = SDL_SCANCODE_F2,
        F3 = SDL_SCANCODE_F3,
        F4 = SDL_SCANCODE_F4,
        F5 = SDL_SCANCODE_F5,
        F6 = SDL_SCANCODE_F6,
        F7 = SDL_SCANCODE_F7,
        F8 = SDL_SCANCODE_F8,
        F9 = SDL_SCANCODE_F9,
        F10 = SDL_SCANCODE_F10,
        F11 = SDL_SCANCODE_F11,
        F12 = SDL_SCANCODE_F12,
    };

    static std::string getKeyName(const KeyCode code) {
        switch (code) {
            case KeyCode::SEMICOLON: return "Semicolon";
            case KeyCode::RIGHT_ALT: return "Right Alt";
            case KeyCode::LEFT_ALT: return "Left Alt";
            case KeyCode::RIGHT_CTRL: return "Right Ctrl";
            case KeyCode::LEFT_CTRL: return "Left Ctrl";
            case KeyCode::LEFT_SHIFT: return "Left Shift";
            case KeyCode::RIGHT_SHIFT: return "Right Shift";
            case KeyCode::ONE: return "ONE";
            case KeyCode::TWO: return "TWO";
            case KeyCode::THREE: return "THREE";
            case KeyCode::FOUR: return "FOUR";
            case KeyCode::FIVE: return "FIVE";
            case KeyCode::SIX: return "SIX";
            case KeyCode::SEVEN: return "SEVEN";
            case KeyCode::EIGHT: return "EIGHT";
            case KeyCode::NINE: return "NINE";
            case KeyCode::ZERO: return "ZERO";
            case KeyCode::ESCAPE: return "ESCAPE";
            case KeyCode::F1: return "F1";
            case KeyCode::F2: return "F2";
            case KeyCode::F3: return "F3";
            case KeyCode::F4: return "F4";
            case KeyCode::F5: return "F5";
            case KeyCode::F6: return "F6";
            case KeyCode::F7: return "F7";
            case KeyCode::F8: return "F8";
            case KeyCode::F9: return "F9";
            case KeyCode::F10: return "F10";
            case KeyCode::F11: return "F11";
            case KeyCode::F12: return "F12";
            case KeyCode::GRAVE: return "GRAVE";
            case KeyCode::EQUAL: return "EQUAL";
            case KeyCode::SUBTRACT: return "SUBTRACT";
            case KeyCode::LEFT: return "LEFT";
            case KeyCode::RIGHT: return "RIGHT";
            case KeyCode::UP: return "UP";
            case KeyCode::DOWN: return "DOWN";
            case KeyCode::HOME: return "HOME";
            case KeyCode::END: return "END";
            case KeyCode::PAGE_UP: return "PAGE_UP";
            case KeyCode::PAGE_DOWN: return "PAGE_DOWN";
            case KeyCode::INSERT: return "INSERT";
            case KeyCode::DELETE: return "DELETE";
            case KeyCode::BACKSPACE: return "BACKSPACE";
            case KeyCode::TAB: return "TAB";
            case KeyCode::SLASH: return "SLASH";
            case KeyCode::BACK_SLASH: return "BACK_SLASH";
            case KeyCode::BRACKET_LEFT: return "BRACKET_LEFT";
            case KeyCode::BRACKET_RIGHT: return "BRACKET_RIGHT";
            case KeyCode::APOSTROPHE: return "APOSTROPHE";
            case KeyCode::COMMA: return "COMMA";
            case KeyCode::PERIOD: return "PERIOD";
            case KeyCode::A: return "A";
            case KeyCode::B: return "B";
            case KeyCode::C: return "C";
            case KeyCode::D: return "D";
            case KeyCode::E: return "E";
            case KeyCode::F: return "F";
            case KeyCode::G: return "G";
            case KeyCode::H: return "H";
            case KeyCode::I: return "I";
            case KeyCode::J: return "J";
            case KeyCode::K: return "K";
            case KeyCode::L: return "L";
            case KeyCode::M: return "M";
            case KeyCode::N: return "N";
            case KeyCode::O: return "O";
            case KeyCode::P: return "P";
            case KeyCode::Q: return "Q";
            case KeyCode::R: return "R";
            case KeyCode::S: return "S";
            case KeyCode::T: return "T";
            case KeyCode::U: return "U";
            case KeyCode::V: return "V";
            case KeyCode::W: return "W";
            case KeyCode::X: return "X";
            case KeyCode::Y: return "Y";
            case KeyCode::Z: return "Z";
            case KeyCode::KP_0: return "KP_0";
            case KeyCode::KP_1: return "KP_1";
            case KeyCode::KP_2: return "KP_2";
            case KeyCode::KP_3: return "KP_3";
            case KeyCode::KP_4: return "KP_4";
            case KeyCode::KP_5: return "KP_5";
            case KeyCode::KP_6: return "KP_6";
            case KeyCode::KP_7: return "KP_7";
            case KeyCode::KP_8: return "KP_8";
            case KeyCode::KP_9: return "KP_9";
            case KeyCode::KP_DECIMAL: return "KP_DECIMAL";
            case KeyCode::KP_DIVIDE: return "KP_DIVIDE";
            case KeyCode::KP_MULTIPLY: return "KP_MULTIPLY";
            case KeyCode::KP_SUBTRACT: return "KP_SUBTRACT";
            case KeyCode::KP_ADD: return "KP_ADD";
            case KeyCode::KP_ENTER: return "KP_ENTER";
            case KeyCode::KP_EQUAL: return "KP_EQUAL";
            default: return "";
        }
    }

}

#endif //KEYCODE_H
