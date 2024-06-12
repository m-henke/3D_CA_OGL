#ifndef PRESETLOADER_H
#define PRESETLOADER_H

#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <string>

/*
preset.txt guide
1st list is grid dimensions
2nd line is the rules (survival/birth/numStates/neighborType)
the rest of the lines are starting cells leave blank for random
*/

struct preset {
    std::vector<glm::vec3> startingCells;
    std::vector<bool> surviveRange;
    std::vector<bool> birthRange;
    glm::vec3 gridSize;
    char neighborType;
    int numStates;
};

class presetLoader {
    public:
        std::string showPresets();
        preset parsePreset(std::string presetPath);
};

#endif