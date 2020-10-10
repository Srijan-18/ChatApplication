cmake_minimum_required(VERSION 3.18.3)

project(ChatApplication)

add_executable(ChatApp 
                ./server/view/server_view.cpp
                ./server/controller/server_controller.cpp
                ./server/controller/main.cpp )
