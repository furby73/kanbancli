#ifndef BOARD_MANAGER_H
#define BOARD_MANAGER_H

#include <vector>
#include <string>
#include "board.h"
#include <fstream>
#include <filesystem>
#include <algorithm>

class BoardManager {
    std::vector<Board> boards;
    int current_board_index = 0;
    const std::string SAVE_FILE = "kanban_boards.txt";

public:
    BoardManager();

    void add_board(const Board &board);

    bool switch_board(int board_id);

    Board *get_current_board();

    void list_boards() const;

    void save_boards();

    void load_boards();

    int create_new_board(const std::string &name);

    Board *find_board(int board_id);
};

#endif //BOARD_MANAGER_H
