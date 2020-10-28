#pragma once
#include <iostream>
#include <string>
#include <vector>
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
    std::string getMessage();
    void setMessage();
    void displayMessage(string);
    string getInputFor(string);
    void printOnlineClients(vector<string>);
};
