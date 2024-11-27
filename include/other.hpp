#ifndef _OTHER_H_
#define _OTHER_H_

#include "settings.hpp"

struct CollisionInfo
{
    int x1;
    int x2;
    int y;
    int width;
    int height;
    int minusXKick;
};

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