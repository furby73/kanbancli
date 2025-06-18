#include "task.h"
#include <sstream>
#include "id_generator.h"

Task::Task(int id, std::string title, std::string description) : id(id), title(std::move(title)),
                                                                 description(std::move(description)) {
    IDGenerator::setLastTaskID(id);
}

std::string Task::serialize() const {
    std::ostringstream oss;
    oss << "TaskID:" << id << "\n";
    oss << "Title:" << title << "\n";
    oss << "Description:" << description << "\n";
    return oss.str();
}

Task Task::deserialize(const std::string &data) {
    std::istringstream iss(data);
    std::string line;
    int id = 0;
    std::string title, description;

    while (std::getline(iss, line)) {
        if (line.find("TaskID:") == 0) {
            id = std::stoi(line.substr(7));
        } else if (line.find("Title:") == 0) {
            title = line.substr(6);
        } else if (line.find("Description:") == 0) {
            description = line.substr(12);
        }
    }

    return Task(id, title, description);
}
