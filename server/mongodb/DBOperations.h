#include <cstdint>
#include <iostream>
#include <vector>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/types.hpp>

#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>

using namespace std;

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

class MongoDB
{
    mongocxx::client client{mongocxx::uri{}};

public:
    bool checkUserPresence(string username);
    bool authenticateUser(string username, string password);
    void saveGivenUser(string userName, string password);
    friend class ServerService;
};