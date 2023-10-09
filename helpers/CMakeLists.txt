cmake_minimum_required(VERSION 3.5)
project(helpers)

get_filename_component(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}" DIRECTORY)

set(CMAKE_CXX_STANDARD 20)

# Add your package's source files
add_library(
    helpers 
    ${SRC_DIR}/helpers/src/debug_helpers.cpp
    ${SRC_DIR}/helpers/src/file_handling_helpers.cpp
    ${SRC_DIR}/helpers/src/bit_operations.cpp
)

target_include_directories(
    helpers PUBLIC 
    ${SRC_DIR}/helpers/include
    ${SRC_DIR}/accumulator/include
)

# Install the executable
install(TARGETS helpers
  DESTINATION lib/${PROJECT_NAME}
)
