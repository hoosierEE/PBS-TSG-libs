//
// QuadPressurePad.h
// Interface to a group of 4 resistive pressures-sensitive pads
//
// Author: Alex Shroyer
// Copyright (c) 2014 Trustees of Indiana University
//

// In the WheelBaby experiment, the 4 pads are arranged in a square.
// Since each pad is a variable resistive element, it is connected to:
// 1. a voltage reference (Vcc. For the Due this is 3.3VDC, for other Arduinos it might be 5VDC)
// 2. a pulldown resistor (currently 10k)
// 3. an analog input pin
//
// This API abstracts lots of details about the pressure pads, and allows the calling program to
// take a very high-level view of what the pressure pads are, and what they do.

// Things this interface cares about:
//      1. steady-state pad values
//      2. changes from steady state either above or below a given threshold
//      3. displaying values as a char array

// Things it does NOT care about:
//      1. roll/pitch/yaw or otherwise inferring orientation
//      2. accurate weights
//      3. gestures or patterns of any kind

// Reasons to use this interface:
//      1. hides the busy-work of reading 4 pads "simultaneously" (actually 1-at-a-time on most Arduinos)
//      2. makes it really easy to tell when a change occurs (touch happened!)

#ifndef __QUAD_PRESSURE_PAD_H__
#define __QUAD_PRESSURE_PAD_H__

#include "Arduino.h"

class QuadPressurePad {

    public:

        // 4 analogRead()-able pins
        //
        // Currently this does not care about orientation,
        // but the prototype has these arranged in a clockwise pattern.
        QuadPressurePad(uint8_t PAD_0, uint8_t PAD_1, uint8_t PAD_2, uint8_t PAD_3)
        {
            // Put pins in an array for convenience.
            padPin[0] = PAD_0;
            padPin[1] = PAD_1;
            padPin[2] = PAD_2;
            padPin[3] = PAD_3;
            touched = false;
        }

        // Update the values in pads[]
        //
        // Call this function once per loop().
        void update()
        {
            for (uint8_t i = 0; i < NUM_PADS; i++)
                pads[i] = analogRead(padPin[i]);
            updateAllPads();
            updateMotion();
        }

        // Return (a pointer to) the array of readings of the pads.
        int16_t* rawValues()
        {
            return pads;
        }

        // Was the pad touched?
        // Note: this will return true at most once per update() cycle.
        //
        // Return true (only once!) since the last touch was registered.
        bool isTouched()
        {
            bool result{touched};
            // reset the flag for next time
            touched = false;
            return result;
        }

    private:

        // Update the history of all 4 pads, column-wise, in one go.
        //
        // NOTE: contains static variable for the sample index
        // Do once per update()
        void updateAllPads()
        {
            static uint8_t index{0};
            for (uint8_t i = 0; i < NUM_PADS; i++)
                padHistory[i][index] = pads[i];
            index = (index + 1) % NUM_SAMPLES;
        }

        // Calculate the average value of a single pad over the most recent N samples.
        // Do once per update()
        int16_t getSinglePadAverage(uint8_t padIndex)
        {
            int16_t sum{0};
            for (uint8_t i = 0; i < NUM_SAMPLES; i++)
                sum += double(padHistory[padIndex][i]) / NUM_SAMPLES;
            return sum;
        }

        // Update params for average value of the pad and whether it's been touched.
        // Do once per update()
        void updateMotion()
        {
            // update the moving average for the pad at padIndex
            for (uint8_t i = 0; i < NUM_PADS; i++) {
                movingPadAvg[i] = getSinglePadAverage(i);
                int16_t deviation{pads[i] - movingPadAvg[i]};
                if (deviation < 0)
                    deviation *= -1;
                if (deviation > 100) // hard-coded sensitivity value
                    touched = true;
            }
        }

        static const uint8_t NUM_PADS{4};
        static const uint8_t NUM_SAMPLES{16};
        uint8_t padPin[NUM_PADS]; // index to the physical pin
        int16_t pads[NUM_PADS]; // current value
        int16_t padHistory[NUM_PADS][NUM_SAMPLES]; // history of pad values
        int16_t movingPadAvg[NUM_PADS]; // average of historical values
        bool touched;

}; // class QuadPressurePad
#endif // __QUAD_PRESSURE_PAD_H__

