#include "Timer.cpp"
#include "Queue.cpp"
#include "ClockCalendar.cpp"

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
    const int id = 93457826;
    Timer timer;
    Queue log;
    ClockCalendar cc;
    string buff;
    int state, pumpToHeater_delay, timeout_delay;
    bool timeout, pump, heater, timer_reset, setup, timer_ending;
    bool already_registered, serial_request;

    void virtual inputs() = 0;
    void virtual outputs() = 0;
};

void PoolControl::FSM() {
    if (timer.getTime() < -5) timer.setTimer(0);
    switch (state) {

    case INPUT_WAIT:
        if (serial_request) {
            state = SERIAL_OUT;
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
            break;
        }
        else {
            state = RESET;
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
            break;
        }
        else {
            state = INIT_PUMP;
        }
        if (!already_registered)
        {
            already_registered = true;
            buff += "Init - ";
            buff += cc.getDateTime();
            log.push(buff);
            std::cout << buff << std::endl;
            buff = "";
        }

        if (timer_reset) {
            state = RESET;
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
            break;
        }
        else {
            state = SHUTDOWN_PUMP;
        }
        buff += "Shutdown - ";
        buff += cc.getDateTime();
        log.push(buff);
        already_registered = false;
        std::cout << buff << std::endl;
        buff = "";
        if (pump) pump = false;
        state = INPUT_WAIT;
        break;
    case SERIAL_OUT:
        while (log.head != 0) {
        }
    default:
        break;
    }
}
