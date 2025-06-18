#ifndef ID_GENERATOR_H
#define ID_GENERATOR_H

class IDGenerator {
    static int nextBoardID;
    static int nextTaskID;

    IDGenerator() = default;

public:
    static int generateBoardID();

    static int generateTaskID();

    static void setLastBoardID(int id);

    static void setLastTaskID(int id);
};

#endif // ID_GENERATOR_H
