cmake_minimum_required(VERSION 3.12) 

set(PROJECT_NAME clauncher)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

project(${PROJECT_NAME})

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "-g")

file(GLOB SRC_FILES "src/*.c" "lib/Cjp/src/jp.c")
include_directories("include/")
include_directories("lib/Cjp/include/")
include_directories("lib/darray/")
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} ${GTK3_LIBRARIES})
