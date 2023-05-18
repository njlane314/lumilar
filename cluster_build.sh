#!/bin/bash

BUILD_DIR="$PWD/build"
SOURCE_DIR="$PWD"

echo "-- Building..."
cmake -S "$SOURCE_DIR" -B "$BUILD_DIR"
cmake --build "$BUILD_DIR"
echo "-- Build complete"
