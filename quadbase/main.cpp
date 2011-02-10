/*
 SENG466 Project 1
 */
#include "WProgram.h"
#include "wiring.h"
#include "common.h"
#include "radioclient.h"
#include "LiquidCrystal.h"
#include "vrbot_protocol.h"
#include "string.h"
#include <ctype.h>

LiquidCrystal lcd(10, 9, 8, 7, 6, 5);

void setup() {

    // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");

	//set up vrbot for speech reco
	vrbot_setup();
	  lcd.print("hello, world!2");
	// Enable global interrupt
	Enable_Interrupt();

	// Initialize serial port
	Serial.begin(9600);

	  lcd.print("hello, world!3");
	radioInitSetup();
}

void loop() {

	//Serial.print("test");

	SD_Recognition();

	delay(200);
	return;
}


int main() {
	init();
	setup();

	for (;;) {
		loop();
	}
}
