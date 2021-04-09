#include <ctime>
#include <sstream>
#include <iostream>
#include <string>

class ClockCalendar {
public:
    ClockCalendar();
    ~ClockCalendar();
    std::string getTime();
    std::string getDate();
    std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);
    int year = now->tm_year + 1900;
    int month = now->tm_mon + 1;
    int day = now->tm_mday;
};

ClockCalendar::ClockCalendar() {

}

ClockCalendar::~ClockCalendar() {

}

std::string ClockCalendar::getTime() {
    std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);
    std::ostringstream buffer1;
    buffer1 << now->tm_hour << ':' << now->tm_min << ':' << now->tm_sec;
    return (buffer1.str());
}

std::string ClockCalendar::getDate() {
    std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);
    std::ostringstream buffer2;
    buffer2 << now->tm_mday << '/' << now->tm_mon + 1 << '/' << now->tm_year + 1900;
    return (buffer2.str());
}

