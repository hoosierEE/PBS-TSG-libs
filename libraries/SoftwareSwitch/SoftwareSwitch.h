// SoftwareSwitch.h
// Debounces a polled switch using the "immediate" debounce method.

// Authors: Tony Walker, Alex Shroyer
// Copyright (c) 2012-2015 Trustees of Indiana University
//
// Notes:
//  1) Does NOT use interrupts, so there should be no issues with other
//   libraries, etc.
//  2) Will eventually overflow depending on the CPU frequency.

#ifndef __SOFTWARE_SWITCH_H__
#define __SOFTWARE_SWITCH_H__
#include "Arduino.h"

class SoftwareSwitch {

    public:
        SoftwareSwitch(int pin, bool pullup = true){
            SwitchPin = pin;
            Timeout = 0;

            if (pullup) // assumes you want to use internal pullup resistors
                pinMode(SwitchPin, INPUT_PULLUP);
            else
                pinMode(SwitchPin, INPUT);
        }

        // initialize the switch
        void begin(unsigned long interval = 100000) {
            Interval = interval;
            IsDirty  = false;
            State = digitalRead(SwitchPin);
        }


        // update the switch state
        //
        // When a state transition (e.g. from open to closed) occurs, ignore
        // spurious state transitions due to switch bounce for Interval microseconds.
        // If Timeout == 0, then no transition has been seen yet.
        // If Timeout > 0, then either
        //   micros() < Timeout; ignore transitions
        //   micros() > Timeout; stop ignoring transitions
        //
        // This function should be called near the top of loop() once per loop.
        void update(unsigned long rightnow = micros())
        {
            // reset the dirty flag
            IsDirty = false;

            // ignore spurious transitions
            if ((Timeout != 0) && (rightnow < Timeout)) {
                return;
            }

            // check to see if we have waited long enough
            if ((Timeout != 0) && (rightnow > Timeout)) {
                Timeout = 0;
                return;
            }

            // finally, update the switch state
            bool s = digitalRead(SwitchPin);
            if (s != State) {
                State = s;
                Timeout = rightnow + Interval;
                IsDirty = true;
            }
        }

        // return the current state of the switch (LOW or HIGH)
        bool getState(void) { return State; }

        // test for a state transition (e.g., switch open -> closed)
        // note: the IsDirty flag is reset on each update()
        bool hasStateChanged(void) { return IsDirty; }

        // returns true if the swtich was pressed, ignores a switch release;
        // will only return true once (reset by update())
        bool isPressed(void) { return (IsDirty) && (State == LOW); }

    private:
        unsigned SwitchPin;
        unsigned long Interval;
        unsigned long Timeout;
        bool State;
        bool IsDirty;
};

#endif // __SOFTWARE_SWITCH_H__

