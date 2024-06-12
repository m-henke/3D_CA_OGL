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
}

void cellularAutomata::choosePreset() {
    presetLoader loader;
    std::string choice = loader.showPresets();
    curPreset = loader.parsePreset(choice);
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

glm::vec3 addVector(glm::vec3 v1, std::vector<int> v2) {
    int x, y, z;
    x = v1.x + v2[0];
    y = v1.y + v2[1];
    z = v1.z + v2[2];
    glm::vec3 ret(x, y, z);
    return ret;
}

void cellularAutomata::setNeighbors(cell cur) {
    if(cur.state != curPreset.numStates) return;
    for(glm::vec3 offset : neighborOffsets) {
        cell tempCell = {cur.pos + offset, curPreset.numStates};
        glm::vec3 tempPos = tempCell.pos;
        cur.pos = cur.pos - offset;
        if(!inBounds(tempPos, curPreset.gridSize)) continue;
        neighborGrid[toInt(tempPos.y)][toInt(tempPos.x)][toInt(tempPos.z)]++;
    }
}

void cellularAutomata::updateGrid() {
    for(cell Cell : liveCells) setNeighbors(Cell);
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
    //std::thread updateThread;
    //bool updating = false;
    //while(!WindowShouldClose()) {
    //    // auto drawStart = std::chrono::high_resolution_clock::now();
    //    if(!updating) {
    //        renderer.draw(liveCells, curPreset.gridSize);
    //    }else {
    //        renderer.updateCamOnly(curPreset.gridSize);
    //    }

    //    if(!updating) {
    //        // start thread
    //        updateThread = std::thread(&cellularAutomata::updateGrid, this);
    //        updating = true;
    //    }else {
    //        // check if joinable set not updating
    //        if(updateThread.joinable()) {
    //            updateThread.join();
    //            updating = false;
    //        }
    //    }
        // auto drawEnd = std::chrono::high_resolution_clock::now();

        // auto updateStart = std::chrono::high_resolution_clock::now();
        // updateGrid();
        // auto updateEnd = std::chrono::high_resolution_clock::now();

        // auto drawTime = std::chrono::duration_cast<std::chrono::milliseconds>(drawEnd - drawStart);
        // auto updateTime = std::chrono::duration_cast<std::chrono::milliseconds>(updateEnd - updateStart);

        // std::cout<<"number cells: "<<static_cast<int>(liveCells.size())<<' ';
        // std::cout<<"draw time: "<<drawTime.count()<<' ';
        // std::cout<<"update time: "<<updateTime.count()<<'\n';
    //}
}