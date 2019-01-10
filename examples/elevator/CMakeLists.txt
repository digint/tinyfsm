cmake_minimum_required(VERSION 3.1)

project(tinyfsm-example-elevator)

add_executable(${PROJECT_NAME}
    elevator.hpp
    elevator.cpp
    motor.hpp
    motor.cpp
    fsmlist.hpp
    main.cpp
)

target_link_libraries(${PROJECT_NAME} tinyfsm)
