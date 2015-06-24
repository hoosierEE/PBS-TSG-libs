// RotaryEncoder.h
// polling-based rotary encoder reader
//
// Author:
//  Alex Shroyer
// Copyright (c) 2014 Trustees of Indiana University
//

#ifndef __PANEL_ENCODER_H__
#define __PANEL_ENCODER_H__

#include "Arduino.h"

class RotaryEncoder
{

  public:
    RotaryEncoder(uint8_t pinA, uint8_t pinB);
    ~RotaryEncoder();

    void update(void);      // read inputs

    int8_t position(void);  // returns current (relative) position
    int8_t direction(void); // returns (-1, 0, 1) for (left, unchanged, right)
    int8_t getKnobPosition(void);
    void   setKnobPosition(int8_t knobPos);

  private:
    uint8_t _pinA;
    uint8_t _pinB;
    int8_t read(void);
    static const int8_t stateTransitionTable[16]; // initialized in Encoder.cpp
    static int8_t _position;
    uint8_t oldState = 0;
    uint8_t currentState = 0;
    int8_t _knobPos;
    int8_t _oldKnobPos;

};

#endif // __PANEL_ENCODER_H__

