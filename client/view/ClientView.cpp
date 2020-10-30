#include "clientView.h"

int ClientView::selectOption()
{
    int choice;
    cout << "\n\t  \033[36;1m\033[4mUSER OPTIONS\033[0m\n"
         << "\n1 : To enter chat room\n"
         << "2 : Chat with one client\n"
         << "3 : Show online clients\n"
         << "\033[31;1m8 : LOGOUT\n"
         << "9 : EXIT\033[0m\n"
         << "\nYOUR CHOICE : ";
    cin >> choice;
    if (choice < 3)
        cout << "\n\033[33mUse\033[0m \033[33;1m##BACK\033[0m \033[33mTo go to USER MENU.\033[0m\n\n";
    return choice;
}

int ClientView::selectAuthOption()
{
    int choice;
    cout << "\n\t  \033[36;1m\033[4mMAIN MENU\033[0m\n";
    cout << "\n1 : Login\n"
         << "2 : Register\n"
         << "\033[31;1m9 : EXIT\033[0m\n"
         << "\n\033[34;1mYOUR CHOICE\033[0m : ";
    cin >> choice;
    cout << "\n\n";
    return choice;
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
    cout << "\033[1;32m******************************\n"
         << "                              \n"
         << "      Welcome to Chat App     \n"
         << "                              \n"
         << "******************************\033[0m\n"
         << endl;
}

string ClientView::getInputFor(string requiredInput)
{
    string input;
    std::cout << "\033[36;1mEnter " << requiredInput << " : \033[0m";
    cin >> input;
    return input;
}

void ClientView::printOnlineClients(vector<string> online_clients)
{
    for (auto online_client : online_clients)
    {
        if (online_client == "No clients online")
            cout << "\n\033[31;1m" << online_client << "\033[0m" << endl;
        else
            cout << "\n\033[32;1m" << online_client << "\033[0m" << endl;
    }
}

string ClientView::inputPassword()
{
    while (true)
    {
        string password_input[2];
        password_input[0] = getpass("\033[36;1mEnter Password : \033[0m");
        password_input[1] = getpass("\033[36;1mRe-Enter Password : \033[0m");
        
        if(password_input[0].compare(password_input[1]) == 0)
        return password_input[0];

        cout << "\n\033[31;1mPASSWORDS MISMATCH\033[0m\n\n"; 
    }
}