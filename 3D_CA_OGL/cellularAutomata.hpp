#ifndef CELLULARAUTOMATA_H
#define CELLULARAUTOMATA_H

#include "renderer.hpp"
#include "presetLoader.hpp"
#include "cell.hpp"

class cellularAutomata {
    private:
        std::vector<glm::vec3> neighborOffsets;
        std::vector<cell> liveCells;
        Renderer* renderer;
        preset curPreset;
        int ***neighborGrid;
        int ***grid;

        void createGrids();
        void reviveCell(glm::vec3 pos, int numStates);
        void setStartingCells();
        void setNeighbors(cell cur);
        void updateGrid();
    public:
        cellularAutomata();
        ~cellularAutomata();
        void choosePreset();
        void startRenderer(const unsigned int w, const unsigned int h, const char* title);
        void run();
};

#endif