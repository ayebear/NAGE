cmake_minimum_required(VERSION 2.8.11)
project(NAGE)

#set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
#set(SFML_STATIC_LIBRARIES TRUE)
#find_package(SFML 2.3 REQUIRED graphics window audio system)

file(GLOB NAGE_SOURCE src/nage/*/*.cpp)

include_directories(/usr/local/include include ../es/lib/ConfigFile)

add_definitions("-Wall -std=c++14 -O3")
add_library(nage SHARED ${NAGE_SOURCE})
add_library(nage_s STATIC ${NAGE_SOURCE})

# Will add this back when there are unit tests
#add_executable(nage_tests ${NAGE_TESTS})
#target_link_libraries(nage_tests LINK_PUBLIC nage_s cfgfile_s)
