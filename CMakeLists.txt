cmake_minimum_required(VERSION 3.16)

set(projectName "Gossip_Model")

project(${projectName} LANGUAGES CXX)

# Final executable goes under bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Pull in the main/ folder
add_subdirectory(main)