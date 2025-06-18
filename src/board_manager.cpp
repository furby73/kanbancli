#include "board_manager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "id_generator.h"

BoardManager::BoardManager() {
    load_boards();

    if (boards.empty()) {
        boards.emplace_back("Default Board");
        current_board_index = 0;
    }
}

void BoardManager::add_board(const Board &board) {
    boards.push_back(board);
    current_board_index = boards.size() - 1;
}

bool BoardManager::switch_board(int board_id) {
    for (int i = 0; i < boards.size(); i++) {
        if (boards[i].get_id() == board_id) {
            current_board_index = i;
            return true;
        }
    }
    return false;
}

Board *BoardManager::get_current_board() {
    if (boards.empty()) return nullptr;
    return &boards[current_board_index];
}

void BoardManager::list_boards() const {
    if (boards.empty()) {
        std::cout << "No boards available.\n";
        return;
    }

    std::cout << "\nAvailable Boards:\n";
    for (const auto &board: boards) {
        std::cout << "[" << board.get_id() << "] " << board.get_name() << "\n";
    }
    std::cout << std::endl;
}

void BoardManager::save_boards() {
    std::ofstream file(SAVE_FILE);
    if (!file) {
        std::cerr << "Error saving boards to file.\n";
        return;
    }

    file << "BoardCount:" << boards.size() << "\n";
    for (const auto &board: boards) {
        file << board.serialize();
    }
    file.close();
}

void BoardManager::load_boards() {
    std::ifstream file(SAVE_FILE);
    if (!file) return;

    std::string line;
    int board_count = 0;

    while (std::getline(file, line)) {
        if (line.find("BoardCount:") == 0) {
            board_count = std::stoi(line.substr(11));
        } else if (line.find("BoardID:") == 0) {
            std::string board_data = line + "\n";
            while (std::getline(file, line) && !line.empty()) {
                board_data += line + "\n";
            }
            boards.push_back(Board::deserialize(board_data));
        }
    }

    if (!boards.empty()) {
        current_board_index = 0;
    }
}

int BoardManager::create_new_board(const std::string &name) {
    boards.emplace_back(name);
    current_board_index = boards.size() - 1;
    return boards.back().get_id();
}

Board *BoardManager::find_board(int board_id) {
    auto it = std::find_if(boards.begin(), boards.end(),
                           [board_id](const Board &b) { return b.get_id() == board_id; });

    if (it != boards.end()) {
        return &(*it);
    }
    return nullptr;
}
