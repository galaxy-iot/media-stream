#ifndef EVENT_CONTEXT_H
#define EVENT_CONTEXT_H

#include "event.h"

// handle event lifeline
// event scheduler
// an interface
class EventContext {
    public:
        virtual int add_raw_event(IO *event) = 0;
        virtual int remove_raw_event(IO *event,int events) = 0;
        virtual int schedule_raw_events() = 0;
        virtual int init() = 0;
};


#endif