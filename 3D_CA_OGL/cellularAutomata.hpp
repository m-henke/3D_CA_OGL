#ifndef CELLULARAUTOMATA_H
#define CELLULARAUTOMATA_H

#include "presetLoader.hpp"
#include "cell.hpp"

class cellularAutomata {
    private:
        std::vector<glm::vec3> neighborOffsets;
        std::vector<cell> liveCells;
        preset curPreset;
        int ***neighborGrid;
        int ***grid;

        void createGrids();
        void reviveCell(glm::vec3 pos, int numStates);
        void setStartingCells();
        void setNeighbors(cell cur);
        void updateGrid();
    public:
        // Renderer renderer;

        cellularAutomata();
        ~cellularAutomata();
        void choosePreset();
        void run();
};

#endif