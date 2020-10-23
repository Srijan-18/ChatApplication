#pragma once
#include <vector>
#include <string>
class StringUtility
{
public:
    static std::vector<std::string> splitter(const std::string &client_response, std::string delimiter)
    {
        size_t pos_start = 0, pos_end, delim_len = delimiter.length();
        std::string token;
        std::vector<std::string> res;

        while ((pos_end = client_response.find(delimiter, pos_start)) != std::string::npos)
        {
            token = client_response.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            res.push_back(token);
        }
        res.push_back(client_response.substr(pos_start));
        return res;
    }
};