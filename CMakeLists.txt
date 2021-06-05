cmake_minimum_required(VERSION 3.15)

project(Client_test)

set(CMAKE_CXX_STANDARD 14)



set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREAD ON)
set(Boost_USE_STATIC_RUNTIME OFF)

option(LINK ON)
set(CMAKE_CXX_CLANG_TIDY clang-tidy -header-filter=. -checks=-*,clang-analyzer-*,-clang-analyzer-cplusplus*)


if(LINK)
    add_library(Client STATIC client.hpp client.cpp)
    add_library(Server STATIC server.hpp server.cpp)
else()
    add_library(Client SHARED client.hpp client.cpp)
    add_library(Server SHARED server.hpp server.cpp)
endif()


FIND_PACKAGE(Boost 1.45.0 COMPONENTS system thread) 



if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(server main_sr.cpp)
    add_executable(client main_cl.cpp)
    target_link_libraries(server Server ${Boost_LIBRARIES})
    target_link_libraries(client Client ${Boost_LIBRARIES})  
else()
    message("Boost not found")
endif()