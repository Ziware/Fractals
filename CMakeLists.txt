cmake_minimum_required(VERSION 3.20)
project(Fractals)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fopenmp -pthread -fpermissive")

add_executable(Fractals main.cpp FormSFML.h)

find_package (OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(Fractals ${OPENGL_LIBRARIES} m pthread)

set(SFML_STATIC_LIBRARIES TRUE)
include_directories(C:/Programming/AESC/SFML/include)
set(SFML_DIR C:/Programming/AESC/SFML/lib/cmake/SFML)

find_package(SFML COMPONENTS system window graphics network audio REQUIRED)
target_link_libraries(Fractals sfml-system sfml-window sfml-graphics sfml-audio sfml-network)