cmake_minimum_required(VERSION 3.10)
project(SimuladorSO)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "main.cpp"
)

file(GLOB_RECURSE HEADERS
    "include/*.h"
)

add_executable(SimuladorSO ${SOURCES} ${HEADERS})

target_link_libraries(SimuladorSO ${wxWidgets_LIBRARIES})

