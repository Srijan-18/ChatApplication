#include "clientView.h"

int ClientView::selectOption()
{
    int choice;
    cout << "\n***** USER OPTIONS *****\n"
         <<"\n1 : To enter chat room\n"
         << "9 : Exit\n"
         << "\nYOUR CHOICE : ";
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

void ClientView::displayMessage(std::string message)
{
    cout << message << endl;
}

void ClientView::printWelcomeMessage()
{
    cout << "******************************\n"
         << "                              \n"
         << "      Welcome to Chat App     \n"
         << "                              \n"
         << "******************************\n"
         << endl;
}

string ClientView::getInputFor(string requiredInput)
{
    string input;
    std::cout << "Enter " << requiredInput << " : ";
    cin >> input;
    return input;
}