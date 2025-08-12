//
// Created by wesley on 8/12/25.
//

#include <Raven3DEngineCore.h>

using namespace Raven3DEngineCore::Events;

std::chrono::system_clock::time_point AppUpdateEvent::_lastUpdateTime = std::chrono::system_clock::now();
std::chrono::system_clock::time_point AppRenderEvent::_lastRenderTime = std::chrono::system_clock::now();
std::chrono::system_clock::time_point AppPostRenderEvent::_lastPostRenderTime = std::chrono::system_clock::now();
std::chrono::system_clock::time_point AppPostUpdateEvent::_lastPostUpdateTime = std::chrono::system_clock::now();