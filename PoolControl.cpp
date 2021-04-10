#include "Timer.cpp"
#include "Queue.cpp"
#include "ClockCalendar.cpp"
#include <sstream>
#include <iostream>
#include <string>

class PoolControl {
public:
    void FSM();

    enum states {
        INPUT_WAIT,
        SETUP,
        RESET,
        INIT_PUMP,
        INIT_HEATER,
        TIMEOUT_WAIT,
        SHUTDOWN_PUMP,
        SHUTDOWN_HEATER,
        SERIAL_OUT
    };
    const int min = 60;
    const char* id = "93457826";
    Timer timer;
    Queue log;
    states last_state;
    ClockCalendar cc;
    ostringstream buff;
    int state, pumpToHeater_delay, timeout_delay;
    bool timeout, pump, heater, timer_reset, setup, timer_ending;
    bool already_registered, serial_request;

    void virtual inputs() = 0;
    void virtual outputs() = 0;
    void virtual serialOut() = 0;
};

void PoolControl::FSM() {
    if (timer.getTime() < -5) timer.setTimer(0);
    switch (state) {

    case INPUT_WAIT:
        if (serial_request) {
            state = SERIAL_OUT;
            last_state = INPUT_WAIT;
            break;
        }
        else {
            state = INPUT_WAIT;
        }

        if (timer_reset) {
            state = RESET;
            break;
        }
        else {
            state = INPUT_WAIT;
        }

        if (setup) {
            state = SETUP;
            break;
        }
        else {
            state = INPUT_WAIT;
        }
        break;

    case SETUP:
        if (serial_request) {
            state = SERIAL_OUT;
            last_state = SETUP;
            break;
        }
        else {
            state = SETUP;
        }
        if (setup) {
            state = SETUP;
        }
        else {
            state = INPUT_WAIT;
            setup = false;
        }
        break;

    case RESET:
        if (serial_request) {
            state = SERIAL_OUT;
            last_state = RESET;
            break;
        }
        else {
            state = RESET;
        }
        if (pump) {
            buff.clear();
            buff.str("");
            buff << ' ';
            buff << id;
            buff << " - ReInit - ";
            buff << cc.getDateTime();
            log.push(buff.str());
        }
        timer_reset = false;
        timer_ending = false;
        timer.setTimer(timeout_delay);
        timeout = false;
        state = INIT_PUMP;
        break;

    case INIT_PUMP:
        if (serial_request) {
            state = SERIAL_OUT;
            last_state = INIT_PUMP;
            break;
        }
        else {
            state = INIT_PUMP;
        }
        if (!already_registered) {
            buff.clear();
            buff.str("");
            already_registered = true;
            buff << ' ';
            buff << id;
            buff << " - Init - ";
            buff << cc.getDateTime();
            log.push(buff.str());
        }

        if (timer_reset) {
            state = RESET;
            break;
        }
        timeout = false;
        if (!pump) {
            pump = true;
        }
        if ((timeout_delay - timer.getTime()) == pumpToHeater_delay) {
            state = INIT_HEATER;
        }
        else {
            state = INIT_PUMP;
        }
        break;

    case INIT_HEATER:
        if (serial_request) {
            state = SERIAL_OUT;
            last_state = INIT_HEATER;
            break;
        }
        else {
            state = INIT_HEATER;
        }
        if (timer_reset) {
            state = RESET;
        }
        timeout = false;
        if (!heater) {
            heater = true;
        }
        state = TIMEOUT_WAIT;
        break;

    case TIMEOUT_WAIT:
        if (serial_request) {
            state = SERIAL_OUT;
            last_state = TIMEOUT_WAIT;
            break;
        }
        else {
            state = TIMEOUT_WAIT;
        }

        if (timer_reset) {
            timer_reset = false;
            state = RESET;
            break;
        }

        if (timer.getTime() <= 5) {
            if (!timer_ending) {
                timer_ending = true;
            }
            if (timer.getTime() == 0) {
                timer_ending = false;
                state = SHUTDOWN_HEATER;
                break;
            }
        }
        else {
            timeout = false;
            state = TIMEOUT_WAIT;
        }
        break;
    case SHUTDOWN_HEATER:
        if (timer_reset) {
            state = RESET;
            break;
        }
        else {
            state = SHUTDOWN_HEATER;
        }
        if (serial_request) {
            state = SERIAL_OUT;
            last_state = SHUTDOWN_HEATER;
            break;
        }
        else {
            state = SHUTDOWN_HEATER;
        }
        if (heater) heater = false;
        if (timer.getTime() == -5) {
            state = SHUTDOWN_PUMP;
            break;
        }
        else {
            state = SHUTDOWN_HEATER;
        }
        break;
    case SHUTDOWN_PUMP:
        if (timer_reset) {
            state = RESET;
            break;
        }
        else {
            state = SHUTDOWN_PUMP;
        }
        if (serial_request) {
            state = SERIAL_OUT;
            last_state = SHUTDOWN_PUMP;
            break;
        }
        else {
            state = SHUTDOWN_PUMP;
        }
        buff.clear();
        buff.str("");
        buff << ' ';
        buff << id;
        buff << " - Shutdown - ";
        buff << cc.getDateTime();
        log.push(buff.str());
        already_registered = false;
        if (pump) pump = false;
        state = INPUT_WAIT;
        break;
    case SERIAL_OUT:
        if (log.head != 0) {
            state = SERIAL_OUT;
        }
        else {
            serial_request = false;
            state = last_state;
        }
    default:
        break;
    }
}
