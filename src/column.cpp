#include "column.h"
#include <sstream>
#include <algorithm>

bool Column::delete_task_by_id(int task_id) {
    auto it = std::find_if(tasks.begin(), tasks.end(),
                           [task_id](const Task &t) { return t.get_id() == task_id; });
    if (it != tasks.end()) {
        tasks.erase(it);
        return true;
    }
    return false;
}

std::string Column::serialize() const {
    std::ostringstream oss;
    oss << "ColumnID:" << id << "\n";
    oss << "ColumnName:" << name << "\n";
    oss << "Tasks:" << tasks.size() << "\n";

    for (const auto &task: tasks) {
        oss << task.serialize();
    }
    return oss.str();
}

Column Column::deserialize(const std::string &data) {
    std::istringstream iss(data);
    std::string line;
    int id = 0;
    std::string name;
    int num_tasks = 0;
    std::vector<Task> tasks;

    while (std::getline(iss, line)) {
        if (line.find("ColumnID:") == 0) {
            id = std::stoi(line.substr(9));
        } else if (line.find("ColumnName:") == 0) {
            name = line.substr(11);
        } else if (line.find("Tasks:") == 0) {
            num_tasks = std::stoi(line.substr(6));
        } else if (line.find("TaskID:") == 0) {
            std::string task_data = line + "\n";
            for (int i = 0; i < 2; i++) {
                std::getline(iss, line);
                task_data += line + "\n";
            }
            tasks.push_back(Task::deserialize(task_data));
        }
    }

    return Column(id, name, tasks);
}
