#ifndef INPUT_H
#define INPUT_H
#include <utility>
#include "board_manager.h"

class InputHandler {
    const int COLUMN_WIDTH = 30;
    const int TASK_HEIGHT = 3;
    BoardManager &board_manager;

public:
    explicit InputHandler(BoardManager &manager) : board_manager(manager) {
    };

    [[nodiscard]] std::vector<std::vector<std::vector<std::string> > > get_tasks_matrix(const Board &board) const;

    void display(const Board &board) const;

    void loop();
};

#endif //INPUT_H
