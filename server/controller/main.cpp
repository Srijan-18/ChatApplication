#include "server_controller.h"

using namespace std;

void welcomeGreeting(ServerController);

int main()
{
    ServerController server_controller;
    server_controller.startServer();
    welcomeGreeting(server_controller);
}

void welcomeGreeting(ServerController server_controller)
{
    server_controller.printWelcomeMessage();
}