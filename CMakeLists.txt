cmake_minimum_required(VERSION 3.0)
project(buses_1)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

set(SOURCES
        src/main.cpp
        src/Crossroad.cpp
        src/Street.cpp
        src/Bus.cpp
        src/City.cpp
)

add_executable(buses_1 ${SOURCES})
