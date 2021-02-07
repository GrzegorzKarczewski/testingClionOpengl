cmake_minimum_required(VERSION 3.17)
project(testingClionOpengl)

set(CMAKE_CXX_STANDARD 11)

add_executable(testingClionOpengl main.cpp)
target_link_libraries(testingClionOpengl GL GLU GLEW glfw X11 Xxf86vm Xrandr pthread Xi dl)
