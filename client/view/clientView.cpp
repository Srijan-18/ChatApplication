#include "clientView.h"

int ClientView::selectOption()
{
    return 0;
}

void ClientView::viewMessage(std::string received_message)
{
    std::cout << received_message << std::endl;
}

std::string ClientView::getMessage()
{
    return message;
}

void ClientView::setMessage()
{
    std::cin >> message;
}

void ClientView::setUsername()
{
    std::cout << "Enter username : ";
    std::cin >> username;
}

std::string ClientView::getusername()
{
    setUsername();
    return username;
}