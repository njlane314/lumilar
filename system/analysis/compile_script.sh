#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "-- Usage: ./compile_script.sh <input_script.c>"
    exit 1
fi

input_script="$1"
output_executable="${input_script%.*}"

if [ ! -f "$input_script" ]; then
    echo "-- Input script file not found: $input_script"
    exit 1
fi

g++ -o "$output_executable" "$input_script" Dict.cxx $(root-config --cflags --libs)

if [ $? -eq 0 ]; then
    echo "-- Compilation successful"
    echo "-- Executable: $output_executable"
else
    echo "-- Compilation failed"
fi