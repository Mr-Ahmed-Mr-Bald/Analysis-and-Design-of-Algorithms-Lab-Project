cmake_minimum_required(VERSION 3.16)
project(scheduler CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Shared library of core modules
add_library(scheduler_core
    src/csv_utils.cpp
    src/problem.cpp
    src/user.cpp
    src/benefit.cpp
    src/greedy.cpp
    src/knapsack.cpp
    src/evaluator.cpp
    src/display.cpp
)
target_include_directories(scheduler_core PUBLIC include)

# Main
add_executable(scheduler src/main.cpp)
target_link_libraries(scheduler PRIVATE scheduler_core)

# Utilities
add_executable(gen_problems generators/gen_problems.cpp)
target_link_libraries(gen_problems PRIVATE scheduler_core)

add_executable(gen_user generators/gen_user.cpp)
target_link_libraries(gen_user PRIVATE scheduler_core)

add_executable(gen_test generators/gen_test.cpp)
target_link_libraries(gen_test PRIVATE scheduler_core)

add_custom_command(TARGET scheduler POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/data"
        "$<TARGET_FILE_DIR:scheduler>/data"
    COMMENT "Copying data directory to executable location"
)