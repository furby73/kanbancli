#include "id_generator.h"

int IDGenerator::nextBoardID = 1;
int IDGenerator::nextTaskID = 1;

int IDGenerator::generateBoardID() {
    return nextBoardID++;
}

int IDGenerator::generateTaskID() {
    return nextTaskID++;
}

void IDGenerator::setLastBoardID(int id) {
    if (id >= nextBoardID) {
        nextBoardID = id + 1;
    }
}

void IDGenerator::setLastTaskID(int id) {
    if (id >= nextTaskID) {
        nextTaskID = id + 1;
    }
}
