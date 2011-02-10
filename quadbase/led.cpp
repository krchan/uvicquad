#include "led.h"
#include "Wire.h"
#include "WProgram.h"

/*
 * First transmit to the broadcast address, then
 * set the device with the given address.
 */
void LEDSetAddress(uint8_t address) {
	Wire.beginTransmission(0x00);
	Wire.send('A');
	Wire.send(address);
	Wire.send(0xD0);
	Wire.send(0x0D);
	Wire.send(address);
	Wire.endTransmission();
	delay(50);

	return;
}

/*
 * Set the RGB color of the LED at the given address. It also sets
 * the color transition type and whether or not to hold the color
 * since if that bit is not set, the LED will reset to default after
 * a short period of time.
 */
void LEDSetColor(uint8_t address, uint8_t red, uint8_t green, uint8_t blue,
		bool fade, bool holdColor) {
	Wire.beginTransmission(address);
	if (fade) {
		Wire.send('c');
	} else {
		Wire.send('n');
	}
	Wire.send(red);
	Wire.send(green);
	Wire.send(blue);
	if (holdColor) {
		Wire.send('o');
	}
	Wire.endTransmission();
	delay(50);

	return;
}

/*
 * Set the speed of fading between colors.
 * Higher numbers mean fading faster.
 * 255 = instant fading
 */
void LEDSetFadeSpeed(uint8_t address, uint8_t fadeSpeed) {
	Wire.beginTransmission(address);
	Wire.send('f');
	Wire.send(fadeSpeed);
	Wire.endTransmission();
}
