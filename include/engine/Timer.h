#ifndef TIMER_H
#define TIMER_H

#include <SDL.h>

class Timer {
private:
    Uint64 mLastUpdate = 0;
    Uint64 mInterval;
    
public:
    Timer(Uint64 intervalMs) : mInterval(intervalMs) {}
    
    bool shouldUpdate() {
        Uint64 currentTime = SDL_GetTicks64();
        if (currentTime - mLastUpdate >= mInterval) {
            mLastUpdate = currentTime;
            return true;
        }
        return false;
    }
    
    void reset() {
        mLastUpdate = 0;
    }
};

#endif // TIMER_H