cmake_minimum_required(VERSION 3.14)
project(Osh)

set(CMAKE_CXX_STANDARD 14)

add_executable(osh main.c exec.c history.c parse.c parse2.c built-in.c _includes_.h)