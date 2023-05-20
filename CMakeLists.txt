#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
project(energyloss)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/system)

find_package(Geant4 REQUIRED)
find_package(ROOT REQUIRED COMPONENTS RIO Core)
find_package(CLHEP REQUIRED)

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
include(${Geant4_USE_FILE})
include(${ROOT_USE_FILE})
#include(${CLHEP_USE_FILE})

message(STATUS "Geant4 GDML SUPPORT: ${Geant4_gdml_FOUND}")

#----------------------------------------------------------------------------
# Setup Projects PATHS
set(MAIN_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(MAIN_SOURCE_DIR "${PROJECT_SOURCE_DIR}/source")
set(MAIN_BUILD_DIR "${PROJECT_SOURCE_DIR}/build")

if (NOT MAIN_EXTERNAL_DIR)
  set(MAIN_EXTERNAL_DIR "${PROJECT_SOURCE_DIR}/extern" )
endif()

#----------------------------------------------------------------------------
# Check if BxDecay0 is already installed

set(BXDECAY0_DIR "${MAIN_EXTERNAL_DIR}/bxdecay0")
set(BXDECAY0_INSTALLED FALSE)

file(GLOB BXDECAY0_FILES "${BXDECAY0_DIR}/*")

if (BXDECAY0_FILES)
  message(STATUS "BxDecay0 is already installed at ${BXDECAY0_DIR}")
  set(BXDECAY0_INSTALLED TRUE)
endif()

#----------------------------------------------------------------------------
# Clone and build BxDecay0 if not already installed

if (NOT BXDECAY0_INSTALLED)
  message(STATUS "Cloning and building BxDecay0...")

  execute_process(
    COMMAND ${CMAKE_COMMAND} -E env git clone https://github.com/BxCppDev/bxdecay0.git ${BXDECAY0_DIR}
    WORKING_DIRECTORY ${MAIN_EXTERNAL_DIR}
  )

  execute_process(
    COMMAND ./tools/build.sh --without-dbd-gA-data --with-geant4-extension --geant4-prefix "${Geant4_DIR}"
    WORKING_DIRECTORY ${BXDECAY0_DIR}
  )
endif()

if(NOT BXDECAY0_DIR)
  set(BXDECAY0_DIR "${MAIN_EXTERNAL_DIR}/bxdecay0")
endif()
message(${BXDECAY0_DIR})
find_package(BxDecay0 REQUIRED COMPONENTS Geant4 CONFIG PATHS "${BXDECAY0_DIR}/_build.d" NO_DEFAULT_PATH)
include_directories(${BXDECAY0_DIR}/_install.d/include)

#----------------------------------------------------------------------------
# Check if Marley is already installed

set(MARLEY_DIR "${MAIN_EXTERNAL_DIR}/marley")
set(MARLEY_INSTALLED FALSE)

file(GLOB MARLEY_FILES "${MARLEY_DIR}/*")

if (MARLEY_FILES)
  message(STATUS "Marley is already installed at ${MARLEY_DIR}")
  set(MARLEY_INSTALLED TRUE)
endif()

#----------------------------------------------------------------------------
# Clone and build Marley if not already installed

if (NOT MARLEY_INSTALLED)
  message(STATUS "Cloning and building Marley...")

  execute_process(
    COMMAND ${CMAKE_COMMAND} -E env git clone https://github.com/njlane314/marley.git ${MARLEY_DIR}
    WORKING_DIRECTORY ${MAIN_EXTERNAL_DIR}
  )

  execute_process(
    COMMAND make
    WORKING_DIRECTORY ${MAIN_EXTERNAL_DIR}/marley/build
  )
endif()

#----------------------------------------------------------------------------
# Check if Eigen is already installed

set(EIGEN_DIR "${MAIN_EXTERNAL_DIR}/eigen/Eigen/")
set(EIGEN_INSTALLED FALSE)

file(GLOB EIGEN_FILES "${EIGEN_DIR}/*")

if (EIGEN_FILES)
  message(STATUS "Eigen is already installed at ${EIGEN_DIR}")
  set(EIGEN_INSTALLED TRUE)
endif()

#----------------------------------------------------------------------------
# Clone and build Eigen if not already installed

if (NOT EIGEN_INSTALLED)
  message(STATUS "Cloning and building Eigen...")

  execute_process(
    COMMAND ${CMAKE_COMMAND} -E env git clone https://github.com/libigl/eigen.git ${EIGEN_DIR}
    WORKING_DIRECTORY ${MAIN_EXTERNAL_DIR}
  )
endif()

message(${CMAKE_MODULE_PATH})
message(${EIGEN_DIR})
include_directories(${EIGEN_DIR})

#----------------------------------------------------------------------------
#----------------------------------------------------------------------------
#----------------------------------------------------------------------------
#----------------------------------------------------------------------------
#----------------------------------------------------------------------------
# Find libraries and include directories

find_library(MARLEY
  NAMES MARLEY
  PATHS "${MARLEY_DIR}/build"
  REQUIRED
  NO_DEFAULT_PATH
)

find_library(MARLEY_ROOT
  NAMES MARLEY_ROOT
  PATHS "${MARLEY_DIR}/build"
  REQUIRED
  NO_DEFAULT_PATH
)

file(GLOB SOURCES   ${MAIN_SOURCE_DIR}/*.cc
                    ${MAIN_SOURCE_DIR}/physics/*.cc
                    ${MAIN_SOURCE_DIR}/analysis/*.cc
                    ${MAIN_SOURCE_DIR}/generators/*.cc
                    ${MAIN_SOURCE_DIR}/geometry/*.cc
)

file(GLOB HEADERS   ${MAIN_INCLUDE_DIR}/*.hh
                    ${MAIN_INCLUDE_DIR}/physics/*.hh
                    ${MAIN_INCLUDE_DIR}/analysis/*.hh
                    ${MAIN_INCLUDE_DIR}/generators/*.hh
                    ${MAIN_INCLUDE_DIR}/geometry/*.hh
                    ${MAIN_INCLUDE_DIR}/utilities/*.hh
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE
                            ${MAIN_INCLUDE_DIR}
                            ${MARLEY_DIR}/include
                            ${MAIN_INCLUDE_DIR}/physics
                            ${MAIN_INCLUDE_DIR}/analysis
                            ${MAIN_INCLUDE_DIR}/generators
                            ${MAIN_INCLUDE_DIR}/geometry
                            ${MAIN_INCLUDE_DIR}/utilities
		            ${BXDECAY0_DIR}/_install.d/include
)

target_link_libraries(${PROJECT_NAME} ${Geant4_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${ROOT_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${CLHEP_LIBRARIES})

target_link_libraries(${PROJECT_NAME} ${MARLEY} ${MARLEY_ROOT})
target_link_libraries(${PROJECT_NAME} BxDecay0::BxDecay0 BxDecay0::BxDecay0_Geant4)
