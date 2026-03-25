cmake_minimum_required(VERSION 3.16)

set(projectName "Gossip_Model")

project(${projectName} LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(main)