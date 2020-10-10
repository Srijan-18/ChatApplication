#include <iostream>

class ServerView
{
    void printWelcomeMessage();

    friend class ServerController;
};