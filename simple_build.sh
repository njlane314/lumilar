#chmod +x run_build.sh
#!/bin/bash

BUILD_DIR="$PWD/build"
SOURCE_DIR="$PWD"

BUILD=0
RUN_TEST=0
RUN_GEN=0
RUN_RAD=0

while getopts ":btgr" opt; do
  case ${opt} in
    b )
      BUILD=1
      ;;
    t )
      RUN_TEST=1
      ;;
    g )
      RUN_GEN=1
      ;;
    r )
      RUN_RAD=1
      ;;
    \? )
      echo "-- Failed to parse arguments" 1>&2
      ;;
  esac
done

if [ $BUILD -eq 1 ]; then
    echo "-- Building..."
    cmake -S "$SOURCE_DIR" -B "$BUILD_DIR"
    cmake --build "$BUILD_DIR"
    echo "-- Build complete"
fi

cd "$SOURCE_DIR/extern/marley"
source ./setup_marley.sh
cd "$SOURCE_DIR"

if [ $RUN_TEST -eq 1 ]; then
    cd "$SOURCE_DIR/system"
    ./lumilar -g generator/gun_electron_mono.mac -d detector/debug_tpc.mac  
fi

if [ $RUN_GEN -eq 1 ]; then 
    cd "$SOURCE_DIR/system"
    ./lumilar -g generator/marley_solar.mac -d detector/debug_tpc.mac  
fi

if [ $RUN_RAD -eq 1 ]; then
    cd "$SOURCE_DIR/system"
    ./lumilar -g generator/bxdecay0_debug.mac -d detector/debug_tpc.mac  
fi

cd "$SOURCE_DIR"

# ./run_build.sh -b -t -g -r
