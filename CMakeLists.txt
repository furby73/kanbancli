cmake_minimum_required(VERSION 3.30)
project(untitled2)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled2 src/main.cpp
        include/task.h
        include/column.h
        src/board.cpp
        include/board.h
        src/input.cpp
        include/input.h
        src/column.cpp
        src/task.cpp
        src/board_manager.cpp
        include/board_manager.h
        include/id_generator.h
        src/id_generator.cpp)

target_include_directories(untitled2 PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)