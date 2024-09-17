cmake_minimum_required(VERSION 3.0)

project(SudokuSolver)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set project root path
set(PROJECT_ROOT "${CMAKE_SOURCE_DIR}")
add_definitions(-DPROJECT_ROOT="${PROJECT_ROOT}")

# Find SFML library (static version)
set(SFML_STATIC_LIBRARIES True)
add_definitions(-DSFML_STATIC)
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

set(INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/src/logic
    ${CMAKE_SOURCE_DIR}/src/gui
)
include_directories(${SFML_INCLUDE_DIR} ${INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES 
    "${CMAKE_SOURCE_DIR}/src/logic/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/gui/*.cpp"
)

# Create exec and link with SFML library
add_executable(SudokuSolver ${SOURCES} main.cpp)
target_link_libraries(SudokuSolver sfml-graphics sfml-window sfml-system)