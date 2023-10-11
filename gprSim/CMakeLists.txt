cmake_minimum_required(VERSION 3.5)
project(gpr)

get_filename_component(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}" DIRECTORY)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
if (NOT TARGET helpers)
    add_subdirectory(${SRC_DIR}/helpers ${CMAKE_BINARY_DIR}/helpers-build)
endif()

# Add package's source files
add_executable(
    gpr 
    src/parser.cpp
    src/cpu.cpp
    src/loader.cpp
    src/memory.cpp
    src/main.cpp
)

target_include_directories(
    gpr PUBLIC 
    include
)

add_definitions(-DASSEMBLY_PATH="${SRC_DIR}/quadratic_gpr.s")

# Link to helpers code
target_link_libraries(gpr
  helpers
)

# Install the executable
install(TARGETS gpr
  DESTINATION lib/${PROJECT_NAME}
)

