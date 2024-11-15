#ifndef _TIMER_H_
#define _TIMER_H_

#include "settings.hpp"

class Timer
{
    protected:
        virtual void onTimeTick()=0;
        int timeCounter;
        int timeSeconds;
    public:
        void timeTick();
};

#endif