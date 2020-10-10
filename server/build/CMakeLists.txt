cmake_minimum_required(VERSION 3.18.3)

project(ChatApplication)

add_executable(ChatApp 
                ../view/server_view.cpp
                ../controller/server_controller.cpp
                ../controller/main.cpp )
