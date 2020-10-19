#include "clientView.h"

int ClientView::selectOption()
{
    int choice;
    cout << " 1: To enter chat room\n"
         << " 3: Exit "
         << endl;
    cin >> choice;
    return choice;
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

void ClientView::displayMessage(std::string message)
{
    cout << message << endl;
}