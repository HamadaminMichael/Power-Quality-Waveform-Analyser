cmake_minimum_required(VERSION 3.15)
project(Power-Quality-Waveform-Analyser C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(Power-Quality-Waveform-Analyser
        main.c
        waveform.c
        io.c
)

if(UNIX)
    target_link_libraries(Power-Quality-Waveform-Analyser m)
endif()