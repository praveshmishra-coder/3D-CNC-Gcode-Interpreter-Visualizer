#include "GCodeInterpreter.h"
#include "OpenGLVisualizer.h"
#include <iostream>
#include <future>

int main() {

    GCodeInterpreter parser;

    // 🔹 Parse G-code in background (C++17 concurrency)
    auto futureCommands = std::async(std::launch::async, [&parser] {
        return parser.parseFile("../gcode_files/cube.gcode");
    });

    std::cout << "Parsing G-code asynchronously...\n";

    // 🔹 Main thread is free here
    // (OpenGL setup, splash screen, etc. can go here)

    // 🔹 Wait for parsing to finish only when needed
    auto commands = futureCommands.get();

    std::cout << "Commands loaded: " << commands.size() << "\n";

    OpenGLVisualizer viewer;
    viewer.drawPath3D(commands);

    return 0;
}
