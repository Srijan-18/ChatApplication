#include "../model/ClientService.h"
#include "../view/clientView.h"

class ClientController
{
    ClientService client_service;
    ClientView client_view;
    std::string client_message_view;

public:
    void startChat()
    {
        bool end_key = true;
        client_service.setClientName(client_view.getusername());
        while (end_key)
        {
            if (client_service.getConnectionStatus() != true)
            {
                break;
            }
            int user_choice = client_view.selectOption();
            switch (user_choice)
            {
                case 1:
                    {
                    client_service.chatroomMessage();
                    client_service.closeConnection();
                    }
                break;
                case 3:
                    end_key = false;
                    break;
                default:
                    std::cout << "Invalid Input " << std::endl;
                    break;
            }
        }
    }
};

int main()
{
    system("clear");
    ClientController client_contoller;
    client_contoller.startChat();

    return 0;
}