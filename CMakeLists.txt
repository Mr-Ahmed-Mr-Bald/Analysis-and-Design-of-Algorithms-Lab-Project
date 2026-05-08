cmake_minimum_required(VERSION 3.16)
project(study_session_scheduler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(
    scheduler_core
    src/csv_utils.cpp
    src/problem.cpp
    src/user.cpp
)

target_include_directories(scheduler_core PUBLIC include)

add_executable(gen_problems generators/gen_problems.cpp)
target_link_libraries(gen_problems PRIVATE scheduler_core)

add_executable(gen_user generators/gen_user.cpp)
target_link_libraries(gen_user PRIVATE scheduler_core)

add_executable(gen_test generators/gen_test.cpp)
target_link_libraries(gen_test PRIVATE scheduler_core)
