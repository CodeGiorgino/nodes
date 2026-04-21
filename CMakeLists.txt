cmake_minimum_required(VERSION 3.11)

project(nodes
    VERSION 1.0
    LANGUAGES CXX C)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies

include(FetchContent)

# https://github.com/raysan5/raylib/blob/master/projects/CMake/CMakeLists.txt

set(PLATFORM Desktop CACHE STRING "raylib platform")
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5)

FetchContent_MakeAvailable(raylib)

# Target executable

file(GLOB_RECURSE SOURCE_FILES
    CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx)

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF)

target_compile_options(${PROJECT_NAME}
    PRIVATE -Wextra -Wall -pedantic)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}
    PRIVATE raylib)
target_include_directories(${PROJECT_NAME}
    SYSTEM PRIVATE $<TARGET_PROPERTY:raylib,INTERFACE_INCLUDE_DIRECTORIES>)

# Assets

file(GLOB ASSETS
    CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/assets)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    COMMENT "Copying assets to build directory..."
)
