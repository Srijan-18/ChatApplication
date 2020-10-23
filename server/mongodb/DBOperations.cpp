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