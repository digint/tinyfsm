cmake_minimum_required(VERSION 3.1)

project(tinyfsm-example-api)

macro(api_exe target)
    add_executable(${PROJECT_NAME}-${target}
        ${target}.cpp
    )
    target_link_libraries(${PROJECT_NAME}-${target} tinyfsm)
endmacro()

api_exe(debugging_switch)
api_exe(mealy_machine)
api_exe(moore_machine)
api_exe(multiple_switch)
api_exe(resetting_switch)
api_exe(simple_switch)
