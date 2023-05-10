cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
project(energyloss)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

# Add OpenMP flags
find_package(OpenMP REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/system)

find_package(Geant4 REQUIRED)
find_package(ROOT REQUIRED)

include(${Geant4_USE_FILE})
include(${ROOT_USE_FILE})

set(MAIN_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(MAIN_SOURCE_DIR "${PROJECT_SOURCE_DIR}/source")

set(MARLEY_DIR "/home/lane/Software/src/marley")
message(STATUS "MARLEY_DIR is set to ${MARLEY_DIR}")

find_library(MARLEY
  NAMES MARLEY
  PATHS "${MARLEY_DIR}/build/"
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
                            )

target_link_libraries(${PROJECT_NAME} ${Geant4_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${ROOT_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${MARLEY} ${MARLEY_ROOT})
target_link_libraries(${PROJECT_NAME} ${CLHEP_LIBRARIES})
target_link_libraries(${PROJECT_NAME} Threads::Threads)
target_link_libraries(${PROJECT_NAME} ${OpenMP_CXX_LIBRARIES}) # Link with OpenMP library