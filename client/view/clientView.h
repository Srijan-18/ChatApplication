#pragma once
#include <iostream>
#include <string>
using namespace std;
class ClientView
{
private:
    int input;
    std::string username;
    std::string message;

public:
    int selectOption();
    void viewMessage(std::string);
    std::string getMessage();
    void setMessage();
    void setUsername();
    std::string getusername();
    void displayMessage(string);
};
