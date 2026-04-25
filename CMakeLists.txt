cmake_minimum_required(VERSION 3.10.0)
project(WaveformAnalyzer VERSION 0.1.0 LANGUAGES C)

add_executable(WaveformAnalyzer io.c main.c waveform.c)

include(CTest)
enable_testing()

