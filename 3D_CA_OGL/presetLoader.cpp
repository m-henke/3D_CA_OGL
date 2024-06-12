#include "presetLoader.hpp"
//#include <dirent.h>
#include <filesystem>
#include <fstream>

std::string presetLoader::showPresets() {
    int curPreset = 1;
    std::vector<std::string> paths;
    // print preset files
    for (const auto& entry : std::filesystem::directory_iterator("presets/")) {
        std::cout << curPreset++ << ") " << entry.path() << '\n';
        paths.push_back(entry.path().string());
    }
    //// get user input
    std::cout<<"> ";
    std::cin>>curPreset;
    // if they input out of range
    if(curPreset > static_cast<int>(paths.size())) {
        std::cout<<"unexpected input\nsetting to default of 1\n";
        curPreset = 1;
    }
    return paths[curPreset - 1];
}

glm::vec3 getLineNumbers(std::string line) {
    float x, y, z;
    size_t pos = 0;
    pos = line.find(' ', 0);
    x = stoi(line.substr(0, pos));
    line.erase(0, pos + 1);
    pos = line.find(' ', 0);
    y = stoi(line.substr(0, pos));
    line.erase(0, pos + 1);
    z = stoi(line);
    glm::vec3 ret(x, y, z);
    return ret;
}

void setRange(std::string line, std::vector<bool> &range) {
    size_t pos = 0;
    size_t pos2 = 0;
    int num1, num2;
    std::string temp;
    bool runLastTime = true;
    while((pos = line.find(',', 0)) != std::string::npos || runLastTime) {
        // make the loop run one more time
        if(pos == std::string::npos) {
            runLastTime = false;
            temp = line;
        }else {
            temp = line.substr(0, pos);
        }
        // if its a range of numbers
        if((pos2 = temp.find('-', 0)) != std::string::npos) {
            num1 = stoi(temp.substr(0, pos2));
            num2 = stoi(temp.substr(pos2 + 1, temp.length())) + 1;
        }else {
            num1 = stoi(temp);
            num2 = stoi(temp);
        }
        // add range to bool vector
        int numToAdd = static_cast<int>(num2 - range.size());
        if(num1 == num2) numToAdd++;
        int startingNum = static_cast<int>(range.size());
        for(int i = 0; i < numToAdd; i++) {
            range.push_back((i + startingNum >= num1 && i + startingNum<= num2) ? true : false);
        }
        line.erase(0, pos + 1);
    }
}

void fillRange(char neighborType, std::vector<bool> &range) {
    // n = 6, m = 26
    if(neighborType == 'N' && static_cast<int>(range.size()) < 6) {
        int numToAdd = static_cast<int>(6 - range.size());
        for(int i = 0; i < numToAdd; i++)
            range.push_back(false);
    }else if(neighborType == 'M' && static_cast<int>(range.size()) < 26) {
        int numToAdd = static_cast<int>(26 - range.size());
        for(int i = 0; i < numToAdd; i++)
            range.push_back(false);
    }
}

void setRules(std::string line, preset &ret) {
    size_t pos = 0;
    pos = line.find('/', 0);
    setRange(line.substr(0, pos), ret.surviveRange);
    line.erase(0, pos + 1);
    pos = line.find('/', 0);
    setRange(line.substr(0, pos), ret.birthRange);
    line.erase(0, pos + 1);
    pos = line.find('/', 0);
    ret.numStates = stoi(line.substr(0, pos)) - 1;
    ret.neighborType = line[pos + 1];
    fillRange(ret.neighborType, ret.surviveRange);
    fillRange(ret.neighborType, ret.birthRange);
}

preset presetLoader::parsePreset(std::string presetPath) {
    std::ifstream file(presetPath);
    std::string line;
    preset ret;
    // get grid size
    std::getline(file, line);
    ret.gridSize = getLineNumbers(line);
    // get rules
    std::getline(file, line);
    setRules(line, ret);
    // get starting cells
    while(std::getline(file, line)) {
        ret.startingCells.push_back(getLineNumbers(line));
    }
    file.close();
    return ret;
}