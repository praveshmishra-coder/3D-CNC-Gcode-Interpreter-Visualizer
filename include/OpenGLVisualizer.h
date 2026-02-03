#pragma once

#include <GL/glew.h>      // MUST be first
#include <GLFW/glfw3.h>
#include <vector>
#include "GCodeInterpreter.h"

class OpenGLVisualizer {
public:
    void drawPath3D(const std::vector<GCommand>& commands);
};
