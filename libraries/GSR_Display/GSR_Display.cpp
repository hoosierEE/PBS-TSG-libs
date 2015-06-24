//
// GSR_Display.cpp
//
// Encapsulates the TFT and provides drawing primities for this app
//
// Authors: Tony Walker, Alex Shroyer
// Copyright (c) 2013, 2014 Trustees of Indiana University
//

#include "GSR_Display.h"

GSR_Display::GSR_Display(unsigned CS_PIN, unsigned DC_PIN, unsigned RESET_PIN)
{
    pTFT = new Adafruit_ST7735(CS_PIN, DC_PIN, RESET_PIN);
}

GSR_Display::~GSR_Display()
{
    delete pTFT;
}

void GSR_Display::begin(unsigned bgcolor)
{
    BgColor = bgcolor;
    pTFT->initR(INITR_BLACKTAB); // initialize a ST7735R chip (black tab version of chip)
    pTFT->fillScreen(BgColor);   // clear the screen and set the background
    pTFT->setRotation(0);
}

void GSR_Display::clear(void)
{
    pTFT->fillScreen(BgColor); // clear the screen and set the background
}

void GSR_Display::setupChart(void)
{
    clear();

    // draw the chart axis
    pTFT->drawFastHLine(YMin - 1, XMin - 1, YMax + 1, ST7735_WHITE);
    pTFT->drawFastVLine(YMin - 1, XMin - 1, XMax + 1, ST7735_WHITE);

    // draw center line
    pTFT->drawFastVLine((YMax - YMin) / 2 + YMid, 0, XMin - 1, ST7735_WHITE);
}

void GSR_Display::say(String message)
{
  for (int i = BarMin - 10; i < BarMin; i++)
    pTFT->drawFastVLine(i, BarLeft, BarRight, ST7735_BLACK);
  pTFT->setRotation(1);
  pTFT->setCursor(10, 13);
  pTFT->print(message);
  pTFT->setRotation(0);
}

void GSR_Display::statusBar(String message)
{
  for (int i = BarMin; i < BarMax; i++)
    pTFT->drawFastVLine(i, BarLeft, BarRight, ST7735_BLACK);
  pTFT->setRotation(1);
  pTFT->setCursor(10, 3);
  pTFT->print(message);
  pTFT->setRotation(0);
}

void GSR_Display::drawPixel(unsigned x, unsigned y, unsigned color)
{
    pTFT->drawPixel(y + YMin, x + XMin, color);
}

void GSR_Display::clearColumn(unsigned x)
{
    pTFT->drawFastHLine(YMin, x + XMin, YMax, BgColor);
}

void GSR_Display::updatePosBar(unsigned x)
{
    pTFT->drawFastHLine(YMin, x + XMin, YMax, BgColor);
}
