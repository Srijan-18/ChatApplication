#include <string>
using namespace std;

struct Client
{
    int socket = 0;
    string name;
    string password;
    bool online_status = false;
    bool chatroom_status = false;
    bool individual_chat = false;
};
