#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <cstring>
#include <iostream>
#include "PoolControl.cpp"
#include "lcdDisplay.cpp"
using namespace std;

class RaspInterface : public PoolControl {
private:
    double time_last_millis;
    LCD display;
    const char* initmsg = "Starting Serial Communication with 93457826\r\n";
    const char* serialPort = "/dev/ttyGS0";
    termios serial;
    void timeHandler();
    unsigned int pSetupKey, pSetupPlus, pSetupMin, pReset, pBuzzer, pPump, pHeater, pSerial;
    bool SetupPlus, SetupMin, setupWritten, defaultWritten, init;

public:
    RaspInterface();
    ~RaspInterface();
    void inputs();
    void outputs();
    void serialOut();

};

RaspInterface::RaspInterface() {
    time_last_millis = millis();
    display.ClrLcd();
    display.lcdLoc(LINE1);
    timer.setTimer(0);
    state = INPUT_WAIT;
    pumpToHeater_delay = 5;
    timeout_delay = 30;
    timeout = true;
    pump = heater = false;
    timer_reset = setup = false;
    timer_ending = false;
    serial_request = false;
    already_registered = false;
    init = false;
    pSetupKey = 0;
    pSetupPlus = 1;
    pSetupMin = 2;
    pReset = 3;
    pBuzzer = 4;
    pPump = 5;
    pHeater = 6;
    pSerial = 7;
    pinMode(pSetupKey, INPUT);
    pinMode(pSetupPlus, INPUT);
    pinMode(pSetupMin, INPUT);
    pinMode(pReset, INPUT);
    pinMode(pBuzzer, OUTPUT);
    pinMode(pPump, OUTPUT);
    pinMode(pHeater, OUTPUT);
    pinMode(pSerial, INPUT);
}


RaspInterface::~RaspInterface()
{
}

void RaspInterface::outputs() {
    timeHandler();

    if (setup) {
        if (!setupWritten) {
            defaultWritten = false;
            display.ClrLcd();
            display.lcdLoc(LINE1);
            display.typeln("Timeout Seconds");
            setupWritten = true;
        }
        display.lcdLoc(LINE2);
        display.typeInt(timeout_delay);
        display.typeln("          ");
    }
    else {
        if (!defaultWritten) {
            setupWritten = false;
            display.ClrLcd();
            display.lcdLoc(LINE1);
            display.typeln("Set ");
            display.typeInt(timeout_delay);
            display.lcdLoc(LINE2);
            display.typeln("Rem ");
            defaultWritten = true;
        }
        display.lcdLoc(0xC4);
        if (timer.getTime() <= 0) {
            display.typeInt(0);
        }
        else {
            display.typeInt(timer.getTime());
        }
        display.typeln("          ");
    }
    digitalWrite(pBuzzer, timer_ending);
    digitalWrite(pPump, pump);
    digitalWrite(pHeater, heater);
}

void RaspInterface::inputs() {
    timeHandler();
    serial_request = digitalRead(pSerial);
    setup = digitalRead(pSetupKey);
    timer_reset = digitalRead(pReset);
    SetupPlus = digitalRead(pSetupPlus);
    SetupMin = digitalRead(pSetupMin);


    if (setup) {
        if (SetupPlus) {
            timeout_delay++;
            SetupPlus = false;
        }
        if (SetupMin) {
            timeout_delay--;
            SetupMin = false;
        }
    }
}

void RaspInterface::timeHandler() {
    if (millis() - time_last_millis > 1000) {
        timer.setTimer(timer.getTime() - 1);
        time_last_millis = millis();
    }
}

void RaspInterface::serialOut() {
    if (serial_request && log.head != 0) {
        int fd = open(serialPort, O_RDWR | O_NOCTTY | O_NDELAY);
        tcgetattr(fd, &serial);
        serial.c_iflag = 0;
        serial.c_oflag = 0;
        serial.c_lflag = 0;
        serial.c_cflag = 0;
        serial.c_cc[VMIN] = 0;
        serial.c_cc[VTIME] = 0;
        serial.c_cflag = B115200 | CS8 | CREAD;
        tcsetattr(fd, TCSANOW, &serial);
        char* tmp = &log.pop()[0];
        if (!init) {
            write(fd, initmsg, strlen(initmsg));
            init = true;
        }
        write(fd, tmp, strlen(tmp));
        delay(100);
        close(fd);
    }
}
