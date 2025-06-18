#include <iostream>
#include "board_manager.h"
#include "input.h"

int main() {
    BoardManager manager;
    InputHandler input(manager);
    input.loop();
    return 0;
}
