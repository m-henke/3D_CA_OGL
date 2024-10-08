#include "cellularAutomata.hpp"
#include "neighborTypes.hpp"
#include <thread>
#include <chrono>

cellularAutomata::cellularAutomata() {
    srand(time(0));
}

cellularAutomata::~cellularAutomata() {
    // delete grids
    for(int i = 0; i < curPreset.gridSize.y; i++) {
            for(int j = 0; j < curPreset.gridSize.x; j++) {
                delete grid[i][j];
                delete neighborGrid[i][j];
            }
            delete grid[i];
            delete neighborGrid[i];
    }
    delete grid;
    delete neighborGrid;
    renderer->~Renderer();
    delete renderer;
}

void cellularAutomata::choosePreset() {
    presetLoader loader;
    std::string choice = loader.showPresets();
    curPreset = loader.parsePreset(choice);
}

void cellularAutomata::startRenderer(const unsigned int w, const unsigned int h, const char* title) {
    renderer = new Renderer(w, h, title);
}

void cellularAutomata::createGrids() {
    grid = new int**[curPreset.gridSize.y];
    neighborGrid = new int**[curPreset.gridSize.y];
    for(int i = 0; i < curPreset.gridSize.y; i++) {
        grid[i] = new int*[curPreset.gridSize.x];
        neighborGrid[i] = new int*[curPreset.gridSize.x];
        for(int j = 0; j < curPreset.gridSize.x; j++) {
            grid[i][j] = new int[curPreset.gridSize.z];
            neighborGrid[i][j] = new int[curPreset.gridSize.z];
            for(int k = 0; k < curPreset.gridSize.z; k++) {
                grid[i][j][k] = 0;
                neighborGrid[i][j][k] = 0;
            }
        }
    }
}

int toInt(float x) {
    return static_cast<int>(x);
}

void cellularAutomata::reviveCell(glm::vec3 pos, int numStates) {
    grid[toInt(pos.y)][toInt(pos.x)][toInt(pos.z)] = numStates;
    cell newCell;
    newCell.pos = pos;
    newCell.state = numStates;
    liveCells.push_back(newCell);
}

glm::vec3 toVec3(int i, int j, int k) {
    glm::vec3 ret(j, i, k);
    return ret;
}

void cellularAutomata::setStartingCells() {
    // revive starting cells
    if(static_cast<int>(curPreset.startingCells.size()) > 0) {
        for(glm::vec3 pos : curPreset.startingCells) {
            reviveCell(pos, curPreset.numStates);
        }
    // if no cells revive random ones
    }else {
        for(int i = 0; i < curPreset.gridSize.y; i++) 
            for(int j = 0; j < curPreset.gridSize.x; j++) 
                for(int k = 0; k < curPreset.gridSize.z; k++) 
                    if(rand() % 100 == 1) reviveCell(toVec3(i, j, k), curPreset.numStates);
    }
}

bool inBounds(glm::vec3 pos, glm::vec3 gridSize) {
    if(pos.x < 0 || pos.y < 0 || pos.z < 0) return false;
    if(pos.x >= gridSize.x || pos.y >= gridSize.y || pos.z >= gridSize.z) return false;
    return true;
}

void cellularAutomata::setNeighbors(cell cur) {
    if(cur.state != curPreset.numStates) return;
    for (int i = 0; i < static_cast<int>(neighborOffsets.size()); i++) {
        glm::vec3 tempPos = cur.pos + neighborOffsets[i];
        if(!inBounds(tempPos, curPreset.gridSize)) continue;
        neighborGrid[toInt(tempPos.y)][toInt(tempPos.x)][toInt(tempPos.z)]++;
    }
}

void cellularAutomata::updateGrid() {
    for (int i = 0; i < static_cast<int>(liveCells.size()); i++) setNeighbors(liveCells[i]);
    liveCells.clear();
    for(int i = 0; i < curPreset.gridSize.y; i++) {
        for(int j = 0; j < curPreset.gridSize.x; j++) {
            for(int k = 0; k < curPreset.gridSize.z; k++) {
                if(grid[i][j][k] == 0) {
                    if(curPreset.birthRange[neighborGrid[i][j][k]]) 
                        reviveCell(toVec3(i, j, k), curPreset.numStates);
                }else if(grid[i][j][k] == curPreset.numStates) {
                    if(curPreset.surviveRange[neighborGrid[i][j][k]]) 
                        reviveCell(toVec3(i, j, k), curPreset.numStates);
                }else {
                    grid[i][j][k]--;
                    if(grid[i][j][k] > 0) 
                        reviveCell(toVec3(i, j, k), grid[i][j][k]);
                }
                neighborGrid[i][j][k] = 0;
            }
        }
    }
}

void cellularAutomata::run() {
    createGrids();
    setStartingCells();
    neighborOffsets = curPreset.neighborType == 'M' ? MOORE : VON_NEUMANN;

    while (!glfwWindowShouldClose(renderer->window)) {        
        // render
        auto rs = std::chrono::high_resolution_clock::now();
        renderer->draw(liveCells);
        auto re = std::chrono::high_resolution_clock::now();

        // update
        auto us = std::chrono::high_resolution_clock::now();
        updateGrid();
        auto ue = std::chrono::high_resolution_clock::now();

        auto rt = std::chrono::duration_cast<std::chrono::milliseconds>(re - rs);
        auto ut = std::chrono::duration_cast<std::chrono::milliseconds>(ue - us);

        //std::cout << "Render Time: " << rt.count() << " Update Time: " << ut.count() << " Live Cells: " << static_cast<int>(liveCells.size()) << '\n';
        
        glfwPollEvents();
    }
}