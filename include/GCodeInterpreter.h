#pragma once
#include <string>
#include <vector>

struct GCommand {
    char code = 0;
    int number = -1;

    double x = 0;
    double y = 0;
    double z = 0;

    double i = 0;
    double j = 0;

    double feed = 0;
};

class GCodeInterpreter {
public:
    std::vector<GCommand> parseFile(const std::string& filename);
    GCommand parseLine(const std::string& line);
};
