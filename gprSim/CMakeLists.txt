cmake_minimum_required(VERSION 3.5)
project(accumulator)

get_filename_component(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}" DIRECTORY)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
if (NOT TARGET helpers)
    add_subdirectory(${SRC_DIR}/helpers ${CMAKE_BINARY_DIR}/helpers-build)
endif()

# Add package's source files
add_executable(
    accumulator 
    ${SRC_DIR}/accumulator/src/cpu.cpp
    ${SRC_DIR}/accumulator/src/loader.cpp
    ${SRC_DIR}/accumulator/src/memory.cpp
    ${SRC_DIR}/accumulator/src/main.cpp
)

target_include_directories(
    accumulator PUBLIC 
    ${SRC_DIR}/accumulator/include
)

add_definitions(-DASSEMBLY_PATH="${SRC_DIR}/quadratic_accumulator.s")

# Link to helpers code
target_link_libraries(accumulator
  helpers
)

# Install the executable
install(TARGETS accumulator
  DESTINATION lib/${PROJECT_NAME}
)

