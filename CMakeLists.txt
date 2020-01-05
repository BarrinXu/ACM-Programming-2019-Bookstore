cmake_minimum_required(VERSION 3.15)
project(book_store)

set(CMAKE_CXX_STANDARD 14)

add_executable(code main.cpp User.h User.cpp User.h book.h book.cpp)
