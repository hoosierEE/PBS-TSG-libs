//
// EventTimer.h
// polled microsecond event timer

// use cases:
//   1) do things at regular intervals
//   2) do different things at different intervals
//   3) pseudo-multitasking on an Arduino (e.g. blink LEDs at different rates)
//   4) avoid blocking delays
//   5) stopwatch { begin(); end(); begin(); ... }

// usage:
//   1) instantiate one or more EventTimer(s)
//   2) begin() each instance with the current time
//   3) update() at the top of loop()
//   4) poll hasExpired() to determine when other actions should happen

// notes:
//   1) microseconds avoids Arduino millis() rounding error
//   2) does NOT use interrupts; must be continuously polled
//   3) timer overflows do not affect update()
//   4) (interval > 0) or it won't work

#ifndef __EVENT_TIMER_H__
#define __EVENT_TIMER_H__
#include "Arduino.h"

class EventTimer {
    public:

        // disabled until begin()
        explicit EventTimer(uint32_t intervalInMicroseconds)
        {
            interval = intervalInMicroseconds;
            expired = false;
            running = false;
        }

        //  to synchronize several timers to each other:
        //      uint32_t currentTime = micros();
        //      myTimer1.begin(currentTime);
        //      myTimer2.begin(currentTime);
        //
        //  otherwise:
        //      myEventTimerInstance.begin();  // implicit `now`
        void begin(uint32_t now = micros())
        {
            currentTime = now;
            if (interval == 0) {
                // zero interval is pointless; refuse to start
                expired = false;
                running = false;
            }
            else {
                last = now;
                next = now + interval;
                expired = false;
                running = true;
            }
        }

        void end(void)
        {
            running = false;
            expired = false;
        }

        // update the timer
        // optionally override the `now` by giving some other number.
        void update(uint32_t now = micros())
        {
            if (!running) return; // no-op, unless timer was started with begin()
            currentTime = now;

            expired = false;
            // determine if the timer has overflowed (next <= now < last) or not (last < next <= now)
            if (((last < next) && (next <= now)) || ((next <= now) && (now < last))) {
                last = next;
                next += interval;
                expired = true; // THE MOST IMPORTANT THING IN THIS CLASS!
            }
        }

        inline bool hasExpired(void) const { return expired; }
        inline bool isRunning(void) const { return running; }
        inline uint32_t getLastTimeout(void) const { return last; }
        inline uint32_t getNextTimeout(void) const { return next; }
        inline uint32_t getInterval(void) const { return interval; }

    private:
        bool expired; // has the timer expired?
        bool running;
        uint32_t last; // when the timer expire last or the timer was started
        uint32_t next; // when the timer will expire next
        uint32_t currentTime;
        uint32_t interval;

}; // class EventTimer

#endif // __EVENT_TIMER_H__

