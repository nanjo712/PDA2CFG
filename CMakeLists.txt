cmake_minimum_required(VERSION 3.19)

project(pda_cfg)

set(CMAKE_CXX_STANDARD 20)

add_library(
    CFG 
    src/cfg.cpp
)
target_include_directories(CFG PUBLIC include)

add_executable(
    simplify_cfg
    src/main.cpp
)
target_link_libraries(simplify_cfg CFG)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/cfg.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_library(
    PDA
    src/pda.cpp
)
target_include_directories(PDA PUBLIC include)

add_executable(
    pda_cfg
    src/pda_cfg.cpp
)
target_include_directories(pda_cfg PUBLIC include)
target_link_libraries(pda_cfg CFG PDA)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/pda.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
