cmake_minimum_required(VERSION 3.27)
project(lab3_sem3)

set(CMAKE_CXX_STANDARD 17)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(lab3_sem3 main.cpp
        )

target_link_libraries(lab3_sem3
        PRIVATE
        Qt6::Core
        Qt6::Widgets
)
add_subdirectory(Google_tests)
include_directories(
        include
)
