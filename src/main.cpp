#include "GCodeInterpreter.h"
#include "OpenGLVisualizer.h"
#include <iostream>

int main() {

    GCodeInterpreter parser;
    auto commands = parser.parseFile("../gcode_files/cube.gcode");

    std::cout << "Commands loaded: " << commands.size() << "\n";

    OpenGLVisualizer viewer;
    viewer.drawPath3D(commands);

    return 0;
}
