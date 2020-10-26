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
                                          << "CHATS"
                                          << bsoncxx::builder::stream::open_array
                                          << doc_value
                                          << bsoncxx::builder::stream::close_array
                                          << bsoncxx::builder::stream::finalize;
    collection.insert_one(doc_value2.view());
}

string MongoDB::getChats(string sender, string reciever)
{
    string chat_history;
    mongocxx::database db = client["ChatAppDB"];

    mongocxx::collection collection = db["PairChats"];

    reciever.resize(reciever.length() - 1);
    string pair_key = string_utility.getPairName(sender, reciever);

    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result = collection.find_one(document{} << "PAIR" << pair_key << finalize);

    if (maybe_result)
    {
        auto cursor = collection.find(make_document(kvp("PAIR", pair_key)));

        for (bsoncxx::v_noabi::document::view doc : cursor)
        {
            for (bsoncxx::document::element ele : doc)
            {
                if (ele && ele.type() == bsoncxx::type::k_array)
                {
                    bsoncxx::array::view subarray{ele.get_array().value};
                    for (bsoncxx::array::element chat : subarray)
                    {

                        if (chat.type() == bsoncxx::type::k_document)
                        {
                            bsoncxx::document::view subdoc{chat.get_document()};

                            string message = "";
                            if (getElementFromView(subdoc, "Sender") == sender)
                                message += "\033[1;32mYou\033[0m : " + getElementFromView(subdoc, "Message") + " \033[5;36m[" + time_utility.getStandardTimeFormat(getElementFromView(subdoc, "Timestamp")) + "]\033[0m \n";
                            else
                                message += "\033[1;34m" + getElementFromView(subdoc, "Sender") + "\033[0m" + " : " + getElementFromView(subdoc, "Message") + " \033[5;36m[" + time_utility.getStandardTimeFormat(getElementFromView(subdoc, "Timestamp")) + "]\033[0m \n";
                            chat_history =  chat_history.append(message);
                        }
                    }
                }
            }
        }
    }
    return chat_history;
}

string MongoDB::getElementFromView(bsoncxx::v_noabi::document::view view, string fieldName)
{
    bsoncxx::document::element element = view[fieldName];
    if (element.type() == bsoncxx::type::k_int64)
        return to_string(element.get_int64().value);
    return element.get_utf8().value.to_string();
}