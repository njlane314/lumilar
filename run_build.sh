#chmod +x run_build.sh
#!/bin/bash

CACHE_FILE="$PWD/build/.directory_cache.txt"
BUILD_DIR="$PWD/build"
SOURCE_DIR="$PWD"

BUILD=0
RUN_TEST=0

while getopts ":bt" opt; do
  case ${opt} in
    b )
      BUILD=1
      ;;
    t )
      RUN_TEST=1
      ;;
    \? )
      echo "-- Failed to parse arguments" 1>&2
      ;;
  esac
done

if [ -z "$GEANT4_DIR" ] || [ -z "$ROOT_DIR" ] || [ -z "$MARLEY_DIR" ]; then
    
    if [ -f "$CACHE_FILE" ]; then
        source "$CACHE_FILE"

        if [ $BUILD -eq 1 ]; then
            echo "-- Building..."
            cmake -S "$SOURCE_DIR" -B "$BUILD_DIR"
            cmake --build "$BUILD_DIR"
            echo "-- Build complete"
        fi
    else
        read -p "*** Enter path to your GEANT4 directory: " GEANT4_DIR
        read -p "*** Enter path to your ROOT directory: " ROOT_DIR
        read -p "*** Enter path to your Marley directory: " MARLEY_DIR

        if [ ! -d "$GEANT4_DIR" ]; then
            echo "-- Failed to find GEANT4 directory"
        fi

        if [ ! -d "$ROOT_DIR" ]; then
            echo "-- Failed to find ROOT directory"
        fi

        if [ ! -d "$MARLEY_DIR" ]; then
            echo "-- Failed to find Marley directory"
        fi

        if [ -d "$GEANT4_DIR" ] && [ -d "$ROOT_DIR" ] && [ -d "$MARLEY_DIR" ]; then
            echo "export GEANT4_DIR=$GEANT4_DIR" > "$CACHE_FILE"
            echo "export ROOT_DIR=$ROOT_DIR" >> "$CACHE_FILE"
            echo "export MARLEY_DIR=$MARLEY_DIR" >> "$CACHE_FILE"

            source "$CACHE_FILE"
            echo "-- Set environment variables"

            if [ $BUILD -eq 1 ]; then
                echo "-- Building..."
                cmake -S "$SOURCE_DIR" -B "$BUILD_DIR"
                cmake --build "$BUILD_DIR"
                echo "-- Build complete"
            fi
        fi
    fi
else
    if [ $BUILD -eq 1 ]; then
        echo "-- Building..."
        cmake -S "$SOURCE_DIR" -B "$BUILD_DIR"
        cmake --build "$BUILD_DIR"
        echo "-- Build complete"
    fi
fi

cd "$MARLEY_DIR"
source ./setup_marley.sh

if [ $RUN_TEST -eq 1 ]; then
    cd "$SOURCE_DIR/system"
    ./energyloss -g generator/gun_electron_lin.mac -d detector/simpleLArTPC.txt  
fi

cd "$SOURCE_DIR"

# ./run_build.sh -b -t