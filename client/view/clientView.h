#pragma once
#include <iostream>
#include <string>
using namespace std;
class ClientView
{
private:
    int input;
    std::string message;

public:
    void printWelcomeMessage();
    int selectOption();
    int selectAuthOption();
    void viewMessage(std::string);
    std::string getMessage();
    void setMessage();
    void displayMessage(string);
    string getInputFor(string);
};
