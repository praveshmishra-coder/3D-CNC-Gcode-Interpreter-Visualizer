#include "../include/GCodeInterpreter.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>

std::vector<GCommand> GCodeInterpreter::parseFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return {};
    }

    std::vector<GCommand> commands;
    std::string line;

    while (std::getline(file, line)) {
        size_t commentPos = line.find(';');
        if (commentPos != std::string::npos)
            line = line.substr(0, commentPos);

        if (line.empty()) continue;

        GCommand cmd = parseLine(line);
        commands.push_back(cmd);
    }

    return commands;
}

GCommand GCodeInterpreter::parseLine(const std::string& line) {
    GCommand cmd;
    std::istringstream ss(line);
    std::string word;

    while (ss >> word) {
        char type = std::toupper(word[0]);
        std::string value = word.substr(1);

        try {
            switch (type) {
                case 'G': cmd.code = 'G'; cmd.number = std::stoi(value); break;
                case 'M': cmd.code = 'M'; cmd.number = std::stoi(value); break;

                case 'X': cmd.x = std::stod(value); break;
                case 'Y': cmd.y = std::stod(value); break;
                case 'Z': cmd.z = std::stod(value); break;

                case 'I': cmd.i = std::stod(value); break;
                case 'J': cmd.j = std::stod(value); break;

                case 'F': cmd.feed = std::stod(value); break;
            }
        } catch (...) {
            std::cerr << "Parse warning: " << word << std::endl;
        }
    }

    return cmd;
}
