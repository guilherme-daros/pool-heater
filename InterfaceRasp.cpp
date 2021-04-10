#include <iostream>
#include "PoolControl.cpp"
#include "lcdDisplay.cpp"
using namespace std;

class RaspInterface : public PoolControl {
private:
    double time_last_millis;
    LCD display;
    void timeHandler();
    int pSetupKey, pSetupPlus, pSetupMin, pReset, pBuzzer, pPump, pHeater;
    bool SetupPlus, SetupMin, setupWritten, defaultWritten;

public:
    RaspInterface();
    ~RaspInterface();

    void inputs();
    void outputs();
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
    pSetupKey = 0;
    pSetupPlus = 1;
    pSetupMin = 2;
    pReset = 3;
    pBuzzer = 4;
    pPump = 5;
    pHeater = 6;
    pinMode(pSetupKey, INPUT);
    pinMode(pSetupPlus, INPUT);
    pinMode(pSetupMin, INPUT);
    pinMode(pReset, INPUT);
    pinMode(pBuzzer, OUTPUT);
    pinMode(pPump, OUTPUT);
    pinMode(pHeater, OUTPUT);
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
