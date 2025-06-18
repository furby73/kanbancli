#include "board.h"
#include <utility>
#include <vector>
#include <string>
#include <sstream>
#include "column.h"
#include <algorithm>
#include "id_generator.h"

Board::Board(int id, std::string name, std::vector<Column> columns)
    : id(id), columns(std::move(columns)), name(std::move(name)) {
    IDGenerator::setLastBoardID(id);
}

void Board::add_task_to_column(const Task &task, const std::string &column_name) {
    for (auto &column: columns) {
        if (column.get_name() == column_name) {
            column.add_task(task);
            return;
        }
    }
}

void Board::add_task_to_column(const Task &task, const int id) {
    if (id >= 0 && id < columns.size()) {
        columns[id].add_task(task);
    }
}

bool Board::move_task(int task_id, int destination_column_id) {
    if (destination_column_id < 0 || destination_column_id >= columns.size()) {
        return false;
    }

    for (int i = 0; i < columns.size(); i++) {
        auto &tasks = columns[i].get_tasks_ref();
        auto it = std::find_if(tasks.begin(), tasks.end(),
                               [task_id](const Task &t) { return t.get_id() == task_id; });

        if (it != tasks.end()) {
            Task task_to_move = std::move(*it);
            tasks.erase(it);
            columns[destination_column_id].add_task(std::move(task_to_move));
            return true;
        }
    }
    return false;
}

bool Board::delete_task(int task_id) {
    for (auto &column: columns) {
        if (column.delete_task_by_id(task_id)) {
            return true;
        }
    }
    return false;
}

std::string Board::serialize() const {
    std::ostringstream oss;
    oss << "BoardID:" << id << "\n";
    oss << "Name:" << name << "\n";
    oss << "Columns:" << columns.size() << "\n";

    for (const auto &column: columns) {
        oss << column.serialize();
    }
    return oss.str();
}

Board Board::deserialize(const std::string &data) {
    std::istringstream iss(data);
    std::string line;
    int id = 0;
    std::string name;
    int num_columns = 0;
    std::vector<Column> columns;

    while (std::getline(iss, line)) {
        if (line.find("BoardID:") == 0) {
            id = std::stoi(line.substr(8));
        } else if (line.find("Name:") == 0) {
            name = line.substr(5);
        } else if (line.find("Columns:") == 0) {
            num_columns = std::stoi(line.substr(8));
        } else if (line.find("ColumnID:") == 0) {
            std::string column_data = line + "\n";
            for (int i = 0; i < 2; i++) {
                std::getline(iss, line);
                column_data += line + "\n";
            }
            columns.push_back(Column::deserialize(column_data));
        }
    }

    return Board(id, name, columns);
}
