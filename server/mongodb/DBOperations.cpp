#pragma once
#include "DBOperations.h"

bool MongoDB::checkUserPresence(string username)
{
    mongocxx::database db = client["ChatAppDB"];

    mongocxx::collection user_credentials_collection = db["user_credentials"];

    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result =
        user_credentials_collection.find_one(document{} << "USERNAME" << username << finalize);
    if (maybe_result)
        return true;

    return false;
}

bool MongoDB::authenticateUser(string username, string password)
{
    mongocxx::database db = client["ChatAppDB"];

    mongocxx::collection user_credentials_collection = db["user_credentials"];

    if (checkUserPresence(username))
    {
        bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result =
            user_credentials_collection.find_one(document{} << "USERNAME" << username << "PASSWORD" << password << finalize);
        if (maybe_result)
            return true;
    }
    return false;
}

void MongoDB::saveGivenUser(string userName, string password)
{
    mongocxx::database db = client["ChatAppDB"];

    mongocxx::collection user_credentials_collection = db["user_credentials"];

    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value doc_value = builder
                                         << "USERNAME"
                                         << userName
                                         << "PASSWORD"
                                         << password
                                         << bsoncxx::builder::stream::finalize;

    bsoncxx::stdx::optional<mongocxx::result::insert_one> result =
        user_credentials_collection.insert_one(doc_value.view());
}

void MongoDB::saveMessage(string sender, string reciever, string message)
{
    mongocxx::database db = client["ChatAppDB"];

    mongocxx::collection collection = db["PairChats"];

    sender.resize(sender.length() - 1);
    message.resize(message.length() - 1);
    
    string pair_key = string_utility.getPairName(sender, reciever);

    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value doc_value = builder
                                         << "Sender"
                                         << sender
                                         << "Reciever"
                                         << reciever
                                         << "Timestamp"
                                         << time_utility.getTimeStamp()
                                         << "Message"
                                         << message
                                         << bsoncxx::builder::stream::finalize;

    if (collection.find_one(document{} << "PAIR" << pair_key << finalize))
    {
        collection.find_one_and_update(document{} << "PAIR" << pair_key << finalize, document{} << "$push" << bsoncxx::builder::stream::open_document << "CHATS" << doc_value << bsoncxx::builder::stream::close_document << finalize);
        return;
    }

    auto builder2 = bsoncxx::builder::stream::document{};
    bsoncxx::document::value doc_value2 = builder2
                                          << "PAIR"
                                          << pair_key
                                          << "CHAT_HISTORY"
                                          << bsoncxx::builder::stream::open_array
                                          << doc_value
                                          << bsoncxx::builder::stream::close_array
                                          << bsoncxx::builder::stream::finalize;
    collection.insert_one(doc_value2.view());
}
