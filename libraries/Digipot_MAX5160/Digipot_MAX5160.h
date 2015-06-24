// Digipot.h
//
// Encapsulates a Maxim MAX5160 digital potentiometer.
// These digipots are controlled by telling the wiper to move up or down.
// "go to position X" must therefore be done by moving one step at a time
// from where you are currently to where you want to be.
//
// The MAX5160 uses a 3-wire serial interface:
// _cs', _inc', and U/D' pins control the wiper setting.
//
// Truth Table:
// _cs'  U/D'  _inc'  Rw
// H    X     X     O
// L    L     R     O
// L    H     R     O
// L    L     F     -
// L    H     F     +
//
// key:  H: high, L: low, X: don't care, O: previous state,
//      R: rising edge, F: falling edge, -: decrement, +: increment
//
// multiple digipots may use the same clock pin
// a pair of digipots may share one U/D' pin, but be careful
//
// see datasheet for timing requirements

//
// IMPORTANT: this particular digipot forgets its state when it loses power
// (volatile memory). Keep this in mind when using this library.
//
// A subtle problem to be wary of is if your microcontroller is reset but
// the digipot is not. You have two options here:
//
// 1) power the digipot from a microcontroller pin. This gives you a "hardware
//    reset" but costs one pin
// 2) "initialize" the digipot by cycling it all the way to one of its extremes,
//    then return to a particular location.
//

#ifndef __DIGIPOT_MAX5160_H__
#define __DIGIPOT_MAX5160_H__
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class Digipot {
    public:

        //
        // @example use:
        //   create an instance of Digipot called p, using pins 3, 5, and 18
        // Digipot p(3, 5, 18);
        //
        // IMPORTANT NOTE: the value INITPOS(16) is valid when power is first applied.
        // But NOT NECESSARILY when constructing/destructing this object.
        //
        Digipot (uint8_t cs, uint8_t ud, uint8_t inc)
            : MINPOS(0), INITPOS(16), MAXPOS(32)
        {
            _cs     = cs;      // cs: chip select, active low
            _ud     = ud;      // ud: up(high) or down(low)
            _inc    = inc;     // inc: clock signal (triggers on high-to-low transition)
            _curpos = INITPOS; // MAX5160 initial value after power-on

            // NOTE: this could be in its own method call for extra safety.
            // It's in the constructor right now for convenience.
            initPins();        // set pinMode() for the given pins
        }

        //
        // @example use:
        //   p.wiperMove(12); // wiper moves +12 from current position
        //   p.wiperMove(-2); // wiper moves -2 from current position
        //
        uint8_t wiperMove(int8_t steps)
        {
            if (steps == 0) {
                return _curpos;
            }

            // enable the chip
            digitalWrite(_cs, LOW);
            bool positive = steps > 0 ? true : false;
            int8_t magnitude = positive ? steps : (steps * -1);
            if (positive) {
                setUdHigh();
                for (int8_t i = 0; i < magnitude; i++) {
                    tick();

                    // adjust our idea of the wiper position
                    _curpos++;
                    // the physical digipot will stop at the extremes
                    if (_curpos > MAXPOS) {
                        _curpos = MAXPOS;
                    }
                }
            } else {
                setUdLow();
                for (int8_t i = 0; i < magnitude; i++) {
                    tick();

                    // adjust our idea of the wiper position
                    _curpos--;
                    // the physical digipot will stop at the extremes
                    if (_curpos < MINPOS) {
                        _curpos = MINPOS;
                    }
                }
            }
            // de-select the chip and return the current wiper position
            digitalWrite(_cs, HIGH);
            return _curpos;
        }

        //
        // Convenience function for incrementing wiper position
        // @example use:
        // p.wiperUp();  // increase wiper position by 1 step
        //
        uint8_t wiperUp(void)
        {
            return wiperMove(1);
        }

        //
        // Convenience function for decrementing wiper position
        // @example use:
        // p.wiperDown();  // decrease wiper position by 1 step
        //
        uint8_t wiperDown(void)
        {
            return wiperMove(-1);
        }

        //
        // @example use:
        // currentPosition = p.getPosition();  // hopefully this is self-explanatory
        //
        uint8_t getPosition(void)
        {
            return _curpos;
        }

    private:
        int8_t  _curpos; // remember wiper position
        uint8_t _cs;     // chip select pin
        uint8_t _ud;     // up/down select pin
        uint8_t _inc;    // increment pin
        int8_t  MINPOS;  // digipot wiper min value
        int8_t  INITPOS; // digipot wiper starting value
        int8_t  MAXPOS;  // digipot wiper min value


        ////// HELPER FUNCTIONS /////////////////////////////////////////

        // subsequent toggling of INC pin cause wiper to go down
        void setUdLow(void)
        {
            digitalWrite(_ud, LOW);
        }

        // subsequent toggling of INC pin cause wiper to go up
        void setUdHigh(void)
        {
            digitalWrite(_ud, HIGH);
        }

        // toggle the increment pin to advance the position
        void tick(void)
        {
            digitalWrite(_inc, HIGH);
            digitalWrite(_inc, LOW);
        }

        // do this or nothing happens
        void initPins(void)
        {
            pinMode(_cs,  OUTPUT);
            pinMode(_ud,  OUTPUT);
            pinMode(_inc, OUTPUT);
        }
}; // class Digipot

#endif // __DIGIPOT_MAX5160_H__

