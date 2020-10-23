#pragma once
#include "server_controller.h"
using namespace std;

void welcomeGreeting(ServerController);

int main()
{
    mongocxx::instance instance{};
    ServerController server_controller;
    server_controller.startServer();
   }

