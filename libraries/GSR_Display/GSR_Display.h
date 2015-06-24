//
// GSR_Display.h
//
// Encapsulates the TFT and provides drawing primities for this app
//
// Authors: Tony Walker, Alex Shroyer
// Copyright (c) 2013, 2014 Trustees of Indiana University
//

#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

class GSR_Display {
  public:
    GSR_Display(unsigned CS_PIN, unsigned DC_PIN, unsigned RESET_PIN);
    ~GSR_Display();

    // text displayed up top
    void say(String message);
    void statusBar(String message);

    void begin(unsigned bgcolor);
    void clear(void);
    void setupChart(void);

    // chart displayed below
    const static uint8_t XMax{150};
    const static uint8_t YMax{100};
    // for the status line:
    const static uint8_t BarMax  {127};
    const static uint8_t BarMin  {117};
    const static uint8_t BarLeft {  5};
    const static uint8_t BarRight{141};

    void drawPixel(unsigned x, unsigned y, unsigned color = ST7735_WHITE);
    void clearColumn(unsigned x);
    void updatePosBar(unsigned x);

  protected:
    Adafruit_ST7735  *pTFT;
    unsigned         BgColor;

  private:
    // for the chart:
    const static uint8_t XMin  {  5};
    const static uint8_t YMin  {  5};
    const static uint8_t YMid  {  7};
    const static uint8_t Width {140};
    const static uint8_t Height{ 90};
};
#endif // __DISPLAY_H__

