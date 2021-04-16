#ifndef IMPTIMER_H_INCLUDED
#define IMPTIMER_H_INCLUDED

#include "CommonFunc.h"

class ImpTimer
{
public:
    ImpTimer();
    ~ImpTimer();

    void start();

    int get_ticks();

private:
    int start_tick_;
    bool is_started_;
};

#endif // IMPTIMER_H_INCLUDED
