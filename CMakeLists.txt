cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(energyloss)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../system)

find_package(Geant4 REQUIRED)
find_package(ROOT REQUIRED)

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

include(${ROOT_USE_FILE})
link_libraries(${ROOT_LIBRARIES})

include_directories(/home/lane/software/marley/include)
add_library(marley SHARED IMPORTED)
set_target_properties(marley PROPERTIES IMPORTED_LOCATION  /home/lane/software/marley/build/libMARLEY.so)

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/*.cc)
file(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}/*.hh)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} ${CLHEP_LIBRARIES} marley)