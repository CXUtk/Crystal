cmake_minimum_required(VERSION 3.14)

project(glad LANGUAGES CXX)

add_library(glad STATIC "glad.c" "glad.h" "KHR/khrplatform.h")