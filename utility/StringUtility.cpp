#pragma once
#include <vector>
#include <string>

using namespace std;

class StringUtility
{
public:
    static vector<string> splitter(const string &client_response, string delimiter)
    {
        size_t pos_start = 0, pos_end, delim_len = delimiter.length();
        string token;
        vector<string> res;

        while ((pos_end = client_response.find(delimiter, pos_start)) != string::npos)
        {
            token = client_response.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            res.push_back(token);
        }
        res.push_back(client_response.substr(pos_start));
        return res;
    }

    string getPairName(string sender, string reciever)
    {
        if (sender.compare(reciever) > 0)
            return sender + "--" + reciever;
        return reciever + "--" + sender;
    }
};