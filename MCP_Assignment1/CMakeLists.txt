cmake_minimum_required(VERSION 3.10)
project(MCP_Assignment1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_executable(MCP_Assignment1 main.cpp)
target_link_libraries(MCP_Assignment1 PRIVATE ${OpenCV_LIBS})
