#pragma once
#include <vector>
#include "GCodeInterpreter.h"

class OpenGLVisualizer {
public:
    void drawPath3D(const std::vector<GCommand>& commands);
};
