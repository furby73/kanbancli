#ifndef COLUMN_H
#define COLUMN_H
#include <string>
#include <utility>
#include <vector>
#include "task.h"

class Column {
    int id;
    std::string name;
    std::vector<Task> tasks;

public:
    Column(int id, std::string name, std::vector<Task> tasks = {}) : id(id), name(std::move(name)),
                                                                     tasks(std::move(tasks)) {
    };

    [[nodiscard]] int get_id() const { return id; }
    [[nodiscard]] std::string get_name() const { return name; }
    [[nodiscard]] std::vector<Task> get_tasks() const { return tasks; }
    std::vector<Task> &get_tasks_ref() { return tasks; }

    void set_tasks(const std::vector<Task> &tasks) { this->tasks = tasks; }
    void add_task(const Task &task) { this->tasks.push_back(task); }

    bool delete_task_by_id(int task_id);

    void set_name(const std::string &name) { this->name = name; }

    [[nodiscard]] std::string get_header() const {
        return "[" + std::to_string(id) + "] " + name;
    }

    std::string serialize() const;

    static Column deserialize(const std::string &data);
};

#endif //COLUMN_H
