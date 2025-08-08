//
// Created by wesley on 8/2/25.
//


#include "../Raven3DEngineCore/Include/Raven3DEngineCore.h"

int main() {
    const auto app = Raven3DEngineCore::RavenApp("Raven 3D app", 800, 600);
    app.run();
    return 0;
}
