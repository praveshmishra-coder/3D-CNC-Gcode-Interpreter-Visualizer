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
        // Remove everything after ';' (comment)
        size_t commentPos = line.find(';');
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }

        // Remove leading/trailing spaces
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        // Skip empty lines
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
        if (word.empty()) continue;

        char type = word[0];
        std::string value = word.substr(1);

        // Make uppercase for safety
        type = std::toupper(type);

        try {
            switch (type) {
                case 'G':
                    cmd.code = 'G';
                    cmd.number = std::stoi(value);
                    break;
                case 'M':
                    cmd.code = 'M';
                    cmd.number = std::stoi(value);
                    break;
                case 'X':
                    cmd.x = std::stod(value);
                    break;
                case 'Y':
                    cmd.y = std::stod(value);
                    break;
                case 'Z':
                    cmd.z = std::stod(value);
                    break;
                case 'F':
                    cmd.feed = std::stod(value);
                    break;
                default:
                    // ignore unknown words
                    break;
            }
        } catch (...) {
            std::cerr << "Warning: Could not parse '" << word << "' in line: " << line << std::endl;
        }
    }

    return cmd;
}
