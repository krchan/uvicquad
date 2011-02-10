/*
 * servo.cpp
 *
 *  Created on: Jan 22, 2010
 *      Author: derekja
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "servo.h"
#include "common.h"
#include "WProgram.h"

static unsigned int servoValue = 2397;
static unsigned char sreg;

void servoInit() {

	// set non-inverted PCM mode (COM4B1 set, COM4B0 unset), mode 14 (fast PWM, TOP in ICR4)
	// set prescale to 64 - CS 40, 41 set, 42 unset
	TCCR4A = 0b100010;    // COM4B1, COM4B0, COM4C1, COM4C0, WGM41, and WGM40
    TCCR4B = 0b11010;     // WGM43, WGM42, CS42, CS41, and CS40

	// initialize the servo pin as an output:
	//pinMode(ServoPin, OUTPUT);
	DDRH |= _BV(DDH4);

    //Set top to 40000 (which will be 20ms with the prescaler)
    sreg = SREG;
    cli();
    ICR4 = 40000;
    SREG = sreg;


    //the time it should be up for no movement is 3054, this is just an initial value
    //it gets set to servoValue the first time the interrupt is hit

    sreg = SREG;
    cli();
    OCR4B = (unsigned int)1500;
    SREG = sreg;


	return;
}

void servoSet(unsigned int value) {

	servoValue = value;
	sreg = SREG;
	cli();
	OCR4B = servoValue;
	SREG = sreg;
	Serial.print(servoValue);
	Serial.println();

	return;
}
