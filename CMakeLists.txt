cmake_minimum_required(VERSION 3.11)

project(nodes
    VERSION 1.0
    LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies

include(FetchContent)

# https://github.com/raysan5/raylib/blob/master/projects/CMake/CMakeLists.txt

set(PLATFORM Desktop)

FetchContent_Declare(raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5)

FetchContent_MakeAvailable(raylib)

# Target executable

file(GLOB_RECURSE SOURCE_FILES
    CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx)

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES})

target_compile_options(${PROJECT_NAME}
    PRIVATE -Wextra -Wall -pedantic)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}
    PRIVATE raylib)
