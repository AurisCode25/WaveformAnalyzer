cmake_minimum_required(VERSION 3.10)

project(Code C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra)

add_executable(waveform_analyser main.c waveform.c io.c
)

set_target_properties(waveform_analyser PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

target_link_libraries(waveform_analyser m)