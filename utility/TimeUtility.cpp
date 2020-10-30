
#include <iostream>
#include <cstdio>
#include <ctime>

using namespace std;

class TimeUtility
{
public:
    std::time_t getTimeStamp()
    {
        return std::time(nullptr);
    }

    string getStandardTimeFormat(string timeInTimeStampFormat)
    {
        std::time_t rawtime = std::stol(timeInTimeStampFormat);
        std::tm *timeinfo;
        char buffer[19];
        timeinfo = std::localtime(&rawtime);
        std::strftime(buffer, 19, "%d/%m/%Y %H:%M", timeinfo);

        return buffer;
    }
};