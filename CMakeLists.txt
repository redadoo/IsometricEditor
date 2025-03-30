cmake_minimum_required(VERSION 3.14)
project(IsometricGame VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/Game
    ${CMAKE_SOURCE_DIR}/src/MapManager
    ${CMAKE_SOURCE_DIR}/src/Gui
    ${CMAKE_SOURCE_DIR}/src/raygui
)

file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

if(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE m pthread dl rt X11)
endif()

file(COPY ${CMAKE_SOURCE_DIR}/assets/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets)

add_custom_target(play
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/IsometricGame
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMENT "Running the IsometricGame..."
)
