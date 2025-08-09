//
// Created by wesley on 8/8/25.
//

#ifndef GAMEPADCODE_H
#define GAMEPADCODE_H
#include "SDL3/SDL_gamepad.h"

namespace Raven3DEngineCore::Input::Gamepad {

    enum class GamepadButtonCode : int {
        BTN_INVALID = SDL_GAMEPAD_BUTTON_INVALID,
        BTN_DPAD_LEFT = SDL_GAMEPAD_BUTTON_DPAD_LEFT,
        BTN_DPAD_RIGHT = SDL_GAMEPAD_BUTTON_DPAD_RIGHT,
        BTN_DPAD_UP = SDL_GAMEPAD_BUTTON_DPAD_UP,
        BTN_DPAD_DOWN = SDL_GAMEPAD_BUTTON_DPAD_DOWN,
        BTN_START = SDL_GAMEPAD_BUTTON_START,
        BTN_BACK = SDL_GAMEPAD_BUTTON_BACK,
        BTN_SOUTH = SDL_GAMEPAD_BUTTON_SOUTH,
        BTN_NORTH = SDL_GAMEPAD_BUTTON_NORTH,
        BTN_EAST = SDL_GAMEPAD_BUTTON_EAST,
        BTN_WEST = SDL_GAMEPAD_BUTTON_WEST,
        BTN_GUIDE = SDL_GAMEPAD_BUTTON_GUIDE,
        BTN_LEFT_SHOULDER = SDL_GAMEPAD_BUTTON_LEFT_SHOULDER,
        BTN_RIGHT_SHOULDER = SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER,
    };

    enum class GamepadAxisCode : int {
        AXIS_INVALID = SDL_GAMEPAD_AXIS_INVALID,
        AXIS_LEFT_X = SDL_GAMEPAD_AXIS_LEFTX,
        AXIS_LEFT_Y = SDL_GAMEPAD_AXIS_LEFTY,
        AXIS_RIGHT_X = SDL_GAMEPAD_AXIS_RIGHTX,
        AXIS_RIGHT_Y = SDL_GAMEPAD_AXIS_RIGHTY,
        AXIS_LEFT_TRIGGER = SDL_GAMEPAD_AXIS_LEFT_TRIGGER,
        AXIS_RIGHT_TRIGGER = SDL_GAMEPAD_AXIS_RIGHT_TRIGGER,
        AXIS_COUNT = SDL_GAMEPAD_AXIS_COUNT,
    };

    static std::string getButtonName(const GamepadButtonCode button) {
        switch (button) {
            case GamepadButtonCode::BTN_BACK: return "Back Button";
            case GamepadButtonCode::BTN_NORTH: return "North Button";
            case GamepadButtonCode::BTN_EAST: return "East Button";
            case GamepadButtonCode::BTN_SOUTH: return "South Button";
            case GamepadButtonCode::BTN_WEST: return "West Button";
            case GamepadButtonCode::BTN_GUIDE: return "Guide Button";
            case GamepadButtonCode::BTN_DPAD_LEFT: return "Left DPAD Button";
            case GamepadButtonCode::BTN_DPAD_RIGHT: return "Right DPAD Button";
            case GamepadButtonCode::BTN_DPAD_UP: return "Up DPAD Button";
            case GamepadButtonCode::BTN_DPAD_DOWN: return "Down DPAD Button";
            case GamepadButtonCode::BTN_INVALID: return "Invalid Button";
            case GamepadButtonCode::BTN_LEFT_SHOULDER: return "Left Shoulder Button";
            case GamepadButtonCode::BTN_RIGHT_SHOULDER: return "Right Shoulder Button";
            case GamepadButtonCode::BTN_START: return "Start Button";
            default: return "Unknown Button";
        }
    }

    static std::string getAxisName(const GamepadAxisCode axis) {
        switch (axis) {
            case GamepadAxisCode::AXIS_LEFT_X: return "Left Axis X";
            case GamepadAxisCode::AXIS_LEFT_Y: return "Left Axis Y";
            case GamepadAxisCode::AXIS_RIGHT_X: return "Right Axis X";
            case GamepadAxisCode::AXIS_RIGHT_Y: return "Right Axis Y";
            case GamepadAxisCode::AXIS_COUNT: return "Axis Count";
            case GamepadAxisCode::AXIS_INVALID: return "Invalid Axis";
            case GamepadAxisCode::AXIS_LEFT_TRIGGER: return "Left Trigger Axis";
            case GamepadAxisCode::AXIS_RIGHT_TRIGGER: return "Right Trigger Axis";
            default: return "";
        }
    }

}

#endif //GAMEPADCODE_H
