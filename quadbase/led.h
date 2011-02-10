#ifndef __LED_H__
#define __LED_H__

#include <stdint.h>

void LEDSetAddress(uint8_t address);
void LEDSetColor(uint8_t address, uint8_t red, uint8_t green, uint8_t blue, bool fade, bool holdColor);
void LEDSetFadeSpeed(uint8_t address, uint8_t fadeSpeed);

#endif
