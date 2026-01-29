#pragma once
#include <string>
#include <vector>

struct GCommand {
    char code;       // 'G' or 'M'
    int number;      // e.g., 1 in G1cd 
    double x = 0.0;  // X coordinate
    double y = 0.0;  // Y coordinate
    double z = 0.0;  // Z coordinate
    double feed = 0; // Feed rate
};

class GCodeInterpreter {
public:
    std::vector<GCommand> parseFile(const std::string& filename);

private:
    GCommand parseLine(const std::string& line);
};
