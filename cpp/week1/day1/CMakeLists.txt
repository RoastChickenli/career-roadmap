cmake_minimum_required(VERSION 3.10)
project(VectorDemo LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# add_executable(vector_demo main.cpp)---------错误
add_executable(cppvector cppvector.cpp)