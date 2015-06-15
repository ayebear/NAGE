cmake_minimum_required(VERSION 2.8.11)
project(NAGE)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
#set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.3 REQUIRED system window graphics audio)

file(GLOB NAGE_SOURCE src/nage/*/*.cpp)
#aux_source_directory(tests NAGE_TESTS)

add_subdirectory(lib/ConfigFile)
include_directories(${SFML_INCLUDE_DIR} include lib/ConfigFile)

add_definitions("-Wall -std=c++14 -O3")
add_library(nage SHARED ${NAGE_SOURCE})
add_library(nage_s STATIC ${NAGE_SOURCE})
#add_executable(nage_tests ${NAGE_TESTS})
#target_link_libraries(nage_tests LINK_PUBLIC nage_s cfgfile_s)
