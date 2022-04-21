cmake_minimum_required(VERSION 3.22)
project(lab_6)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE RELEASE)

# mpic++ -o main main.cpp
# mpiexec -n 2 ./main

add_executable(task_1 task_1/main.cpp)
add_executable(task_2 task_2/main.cpp)
add_executable(task_3 task_3/main.cpp)
add_executable(task_4 task_4/main.cpp)
