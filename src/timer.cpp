#include "other.hpp"
#include <iostream>

using namespace std;

void Timer::timeTick()
{
    timeCounter++;
    if (timeCounter >= (TARGET_FPS / FRAME_SPEED))
    {
        timeCounter = 0;
        if (timeSeconds == 59)
        {
            timeSeconds = 0;
            this->onTimeTick();
            return;
        }
        timeSeconds += 1;
        this->onTimeTick();
    }
}