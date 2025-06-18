#include "input.h"
#include <iostream>
#include <queue>
#include <limits>
#include <algorithm>

std::string get_with_padding(const std::string &text, const int width, const char padding) {
    if (width <= 0) return "";
    std::string res = std::string(width, padding);
    if (text.empty()) return res;

    int start = (width - static_cast<int>(text.size())) / 2;
    if (start < 0) start = 0;
    int copy_len = std::min(static_cast<int>(text.size()), width - start);

    for (int i = 0; i < copy_len; i++) {
        res[start + i] = text[i];
    }
    return res;
}

std::queue<std::string> split(const std::string &s, const std::string &delimiter) {
    std::queue<std::string> tokens;
    if (s.empty())
        return tokens;

    size_t start = 0;
    size_t end = s.find(delimiter);

    while (end != std::string::npos) {
        tokens.push(s.substr(start, end - start));
        start = end + delimiter.length();
        end = s.find(delimiter, start);
    }
    tokens.push(s.substr(start));

    return tokens;
}

std::vector<std::vector<std::vector<std::string> > > InputHandler::get_tasks_matrix(const Board &board) const {
    int max_length = 0;
    for (const auto &column: board.get_columns()) {
        max_length = std::max(max_length, static_cast<int>(column.get_tasks().size()));
    }

    if (max_length == 0) {
        return {};
    }

    std::vector<std::vector<std::vector<std::string> > > res(
        board.get_columns().size(),
        std::vector<std::vector<std::string> >(
            max_length,
            std::vector<std::string>(TASK_HEIGHT, std::string(COLUMN_WIDTH, ' '))
        )
    );

    for (int i = 0; i < board.get_columns().size(); i++) {
        for (int j = 0; j < board.get_columns()[i].get_tasks().size(); j++) {
            std::string task_title = board.get_columns()[i].get_tasks()[j].get_title();
            int task_id = board.get_columns()[i].get_tasks()[j].get_id();
            std::string id_title = "[" + std::to_string(task_id) + "] " + task_title;
            std::queue<std::string> word_queue = split(id_title, " ");

            for (int line = 0; !word_queue.empty() && line < TASK_HEIGHT; line++) {
                std::string block_line;
                while (!word_queue.empty() &&
                       block_line.size() + word_queue.front().size() < COLUMN_WIDTH) {
                    block_line += word_queue.front() + " ";
                    word_queue.pop();
                }

                if (!block_line.empty() && block_line.back() == ' ') {
                    block_line.pop_back();
                }

                block_line = get_with_padding(block_line, COLUMN_WIDTH, ' ');
                res[i][j][line] = block_line;
            }
        }
    }
    return res;
}

void InputHandler::display(const Board &board) const {
    const int total_width = static_cast<int>(board.get_columns().size()) * COLUMN_WIDTH;
    std::string header = "[" + std::to_string(board.get_id()) + "] " + board.get_name();
    std::cout << get_with_padding(header, total_width, '=') << "\n";

    for (auto &column: board.get_columns()) {
        std::cout << get_with_padding(column.get_header(), COLUMN_WIDTH, '=');
    }
    std::cout << "\n";

    std::vector<std::vector<std::vector<std::string> > > blocks = get_tasks_matrix(board);
    if (blocks.empty()) {
        std::cout << get_with_padding("No tasks", total_width, ' ') << "\n";
        std::cout << "=" << std::string(total_width - 1, '=') << "\n\n";
        return;
    }

    int max_tasks = blocks[0].size();
    for (int task_idx = 0; task_idx < max_tasks; task_idx++) {
        for (int line = 0; line < TASK_HEIGHT; line++) {
            for (auto &block: blocks) {
                if (task_idx < block.size()) {
                    std::cout << block[task_idx][line];
                } else {
                    std::cout << std::string(COLUMN_WIDTH, ' ');
                }
            }
            std::cout << "\n";
        }
    }
    std::cout << "=" << std::string(total_width - 1, '=') << "\n\n";
}

void InputHandler::loop() {
    while (true) {
        Board *current_board = board_manager.get_current_board();
        if (!current_board) {
            std::cout << "No boards available. Creating a default board...\n";
            board_manager.create_new_board("Default Board");
            current_board = board_manager.get_current_board();
        }

        display(*current_board);

        std::cout << "Options:\n";
        std::cout << " 1. Add task\n";
        std::cout << " 2. Move task\n";
        std::cout << " 3. Delete task\n";
        std::cout << " 4. View task details\n";
        std::cout << " 5. Create new board\n";
        std::cout << " 6. Switch board\n";
        std::cout << " 7. List boards\n";
        std::cout << " 8. Rename current board\n";
        std::cout << " 9. Save boards\n";
        std::cout << "10. Load boards\n";
        std::cout << "11. Exit\n";
        std::cout << "> ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input! Please enter a number.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                // Add task
                std::cout << "Enter column index (1-" << current_board->get_columns().size() << "): ";
                int column_idx;
                if (!(std::cin >> column_idx)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid input!\n";
                    break;
                }
                std::cin.ignore();

                if (column_idx < 1 || column_idx > current_board->get_columns().size()) {
                    std::cout << "Invalid column index!\n";
                    break;
                }

                std::cout << "Enter task title: ";
                std::string title;
                getline(std::cin, title);

                std::cout << "Enter task description: ";
                std::string description;
                getline(std::cin, description);

                current_board->add_task_to_column(Task(title, description), column_idx - 1);
                std::cout << "Task added with ID: ["
                        << current_board->get_columns()[column_idx - 1].get_tasks().back().get_id() << "]\n";
                break;
            }

            case 2: {
                // Move task
                std::cout << "Enter task ID to move: ";
                int task_id;
                if (!(std::cin >> task_id)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid task ID!\n";
                    break;
                }

                std::cout << "Enter destination column index (1-" << current_board->get_columns().size() << "): ";
                int dest_col_idx;
                if (!(std::cin >> dest_col_idx)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid column index!\n";
                    break;
                }

                if (current_board->move_task(task_id, dest_col_idx - 1)) {
                    std::cout << "Task [" << task_id << "] moved successfully!\n";
                } else {
                    std::cout << "Failed to move task. Check IDs.\n";
                }
                break;
            }

            case 3: {
                // Delete task
                std::cout << "Enter task ID to delete: ";
                int task_id;
                if (!(std::cin >> task_id)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid task ID!\n";
                    break;
                }

                if (current_board->delete_task(task_id)) {
                    std::cout << "Task [" << task_id << "] deleted successfully!\n";
                } else {
                    std::cout << "Task not found!\n";
                }
                break;
            }

            case 4: {
                // View task details
                std::cout << "Enter task ID: ";
                int task_id;
                if (!(std::cin >> task_id)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid task ID!\n";
                    break;
                }

                bool found = false;
                for (const auto &column: current_board->get_columns()) {
                    for (const auto &task: column.get_tasks()) {
                        if (task.get_id() == task_id) {
                            found = true;
                            std::cout << "\nTask ID: [" << task_id << "]\n";
                            std::cout << "Title: " << task.get_title() << "\n";
                            std::cout << "Description: " << task.get_description() << "\n\n";
                            break;
                        }
                    }
                    if (found) break;
                }
                if (!found) {
                    std::cout << "Task [" << task_id << "] not found.\n";
                }
                break;
            }

            case 5: {
                // Create new board
                std::cout << "Enter board name: ";
                std::string name;
                getline(std::cin, name);

                int new_id = board_manager.create_new_board(name);
                std::cout << "Board created: [" << new_id << "] " << name << "\n";
                break;
            }

            case 6: {
                // Switch board
                std::cout << "Enter board ID: ";
                int board_id;
                if (!(std::cin >> board_id)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid board ID!\n";
                    break;
                }

                if (board_manager.switch_board(board_id)) {
                    Board *b = board_manager.get_current_board();
                    std::cout << "Switched to board: [" << b->get_id() << "] " << b->get_name() << "\n";
                } else {
                    std::cout << "Board [" << board_id << "] not found!\n";
                }
                break;
            }

            case 7: {
                // List boards
                board_manager.list_boards();
                break;
            }

            case 8: {
                // Rename board
                std::cout << "Enter new board name: ";
                std::string new_name;
                getline(std::cin, new_name);
                current_board->set_name(new_name);
                std::cout << "Board [" << current_board->get_id() << "] renamed to: " << new_name << "\n";
                break;
            }

            case 9: {
                // Save boards
                board_manager.save_boards();
                std::cout << "Boards saved!\n";
                break;
            }

            case 10: {
                // Load boards
                board_manager.load_boards();
                std::cout << "Boards loaded!\n";
                break;
            }

            case 11: {
                // Exit
                board_manager.save_boards();
                return;
            }

            default:
                std::cout << "Invalid choice! Please try again.\n";
        }
    }
}
