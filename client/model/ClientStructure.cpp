#include <string>
using namespace std;

struct Client
{
    int socket;
    string name;
    string password;
    bool online_status = false;
};
