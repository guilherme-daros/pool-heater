
class Timer {
    int timer;

public:
    Timer();
    ~Timer();
    void setTimer(int);
    int getTime();
};

Timer::Timer() {
    timer = 0;
}

Timer::~Timer() {}

void Timer::setTimer(int newTimer) {
    timer = newTimer;
}
int Timer::getTime() {
    return timer;
}