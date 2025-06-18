#ifndef TASK_H
#define TASK_H
#include <string>
#include <utility>
#include <sstream>
#include "id_generator.h"

class Task {
    int id;
    std::string title;
    std::string description;

public:
    Task(std::string title, std::string description = "") : id(IDGenerator::generateTaskID()),
                                                            title(std::move(title)),
                                                            description(std::move(description)) {
    }

    Task(int id, std::string title, std::string description);

    [[nodiscard]] std::string get_title() const { return title; }
    [[nodiscard]] std::string get_description() const { return description; }
    [[nodiscard]] int get_id() const { return id; }

    std::string serialize() const;

    static Task deserialize(const std::string &data);
};

#endif //TASK_H
