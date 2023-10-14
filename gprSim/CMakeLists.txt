cmake_minimum_required(VERSION 3.5)
project(gpr)

get_filename_component(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}" DIRECTORY)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
if (NOT TARGET helpers)
    add_subdirectory(${SRC_DIR}/helpers ${CMAKE_BINARY_DIR}/helpers-build)
endif()

add_library(
  gpr
  STATIC
  src/loader/source_parser.cpp
  src/loader/encoder.cpp
  src/loader/loader.cpp
  src/loader/symbol_table.cpp
  src/processor/binary_parser.cpp
  src/processor/instructions.cpp
  src/processor/cpu.cpp
  src/processor/memory.cpp
)

# Add package's source files
add_executable(
    gpr_exec
    src/main.cpp
)

target_include_directories(
    gpr PUBLIC 
    include/loader
    include/processor
)

add_definitions(-DASSEMBLY_PATH="${SRC_DIR}/assembly.s")
# add_definitions(-DDEBUG)

# Link to helpers code
target_link_libraries(gpr
  helpers
)

target_link_libraries(gpr_exec
  gpr
)

file(GLOB HEADERS include/*.hpp)
install(FILES ${HEADERS} DESTINATION include/gpr)

# Install the executable
install(TARGETS gpr
  DESTINATION lib
)

install(TARGETS gpr_exec
  DESTINATION bin/${PROJECT_NAME}
)