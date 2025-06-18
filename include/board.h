#ifndef BOARD_H
#define BOARD_H
#include <utility>
#include <vector>
#include <string>
#include "column.h"
#include "id_generator.h"

class Board {
    int id;
    std::vector<Column> columns;
    std::string name;

public:
    Board(std::string name, std::vector<Column> columns = {
              Column(1, "TODO"),
              Column(2, "DOING"),
              Column(3, "DONE")
          }) : id(IDGenerator::generateBoardID()),
               columns(std::move(columns)),
               name(std::move(name)) {
    }

    Board(int id, std::string name, std::vector<Column> columns);

    void add_task_to_column(const Task &task, const std::string &column_name);

    void add_task_to_column(const Task &task, int id);

    [[nodiscard]] std::vector<Column> get_columns() const {
        return columns;
    }

    std::vector<Column> &get_columns_ref() {
        return columns;
    }

    [[nodiscard]] std::string get_name() const {
        return name;
    }

    [[nodiscard]] int get_id() const {
        return id;
    }

    void set_name(std::string name) {
        this->name = std::move(name);
    }

    bool move_task(int task_id, int destination_column_id);

    bool delete_task(int task_id);

    std::string serialize() const;

    static Board deserialize(const std::string &data);
};

#endif //BOARD_H
