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
    char buffer1[10];
    sprintf(buffer1, "%i:%i:%i", now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
    return (buffer1);
}

std::string ClockCalendar::getDate() {
    std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);
    char buffer2[10];
    sprintf(buffer2, "%i:%i:%i", now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
    return (buffer2);
}

