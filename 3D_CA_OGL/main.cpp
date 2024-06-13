#include "cellularAutomata.hpp"

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

int main() {
    cellularAutomata app;
    app.choosePreset();
    app.startRenderer(SCR_WIDTH, SCR_HEIGHT, "Testing");
    app.run();
    
    return 0;
}