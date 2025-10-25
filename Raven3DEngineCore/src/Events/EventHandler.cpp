//
// Created by wesley on 10/25/2025.
//
#include <Raven3DEngineCore.h>

static RAVEN_U_LONG nextListenerID = 0;

RAVEN_U_LONG Raven3DEngineCore::Events::GetNextEventListenerID() {
    return nextListenerID++;
}