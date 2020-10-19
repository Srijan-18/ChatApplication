#include "../view/server_view.h"
#include "../model/ServerService.h"

class ServerController
{
    ServerView server_view;
    ServerService server_service;

public:
    void printWelcomeMessage();
    void startServer();
};
