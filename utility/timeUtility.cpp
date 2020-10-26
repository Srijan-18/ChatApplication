
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
};