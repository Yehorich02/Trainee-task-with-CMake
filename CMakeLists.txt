cmake_minimum_required(VERSION 3.15)

project(Client_test)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-g -pthread -Wall -Wextra -pedantic -Werror -fsanitize=address,undefined")

set(CMAKE_CXX_CLANG_TIDY clang-tidy -header-filter=. -checks=-*,clang-analyzer-*,-clang-analyzer-cplusplus*)

option(BUILD_SHARED_LIBS "Build shared instead of static library" OFF)

if (BUILD_SHARED_LIBS)
    add_library(Client SHARED client.hpp client.cpp)
    add_library(Server SHARED server.hpp server.cpp)
else ()
    add_library(Client STATIC client.hpp client.cpp)
    add_library(Server STATIC server.hpp server.cpp)
endif ()

add_library(boost_asio INTERFACE)
add_library(Boost::asio ALIAS boost_asio)

add_executable(server main_sr.cpp)
add_executable(client main_cl.cpp)
target_include_directories(server INTERFACE include)
target_include_directories(client INTERFACE include)
target_link_libraries(server PUBLIC Server INTERFACE Boost::system)
target_link_libraries(client PUBLIC Client INTERFACE Boost::system)