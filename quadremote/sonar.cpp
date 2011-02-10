#include "sonar.h"
#include "common.h"
#include "WProgram.h"
#include <avr/delay.h>
#include <math.h>

static void sonarEcho1();
static void sonarEcho2();
//static void sonarEcho3();

static uint16_t averageValue(uint8_t offset, uint8_t range, uint16_t* sonarBuffer);
//static uint16_t minValue(uint8_t offset, uint8_t range, uint16_t* sonarBuffer);
//static uint16_t maxValue(uint8_t offset, uint8_t range, uint16_t* sonarBuffer);

static uint16_t leftSonarTickCount;
static uint16_t rightSonarTickCount;
//static uint16_t frontSonarTickCount;

/*
 * Index to the sonar buffer array that is used to store the data
 */
static volatile uint8_t sonarBufferIndex = 0;

/*
 * xxxSonarBufer[20] holds 20 sonar readings for the
 * control module to use. sonarBufferPointer points
 * to the buffer location that should be used to store
 * the next sonar reading data
 */
static uint16_t frontSonarBuffer[SONAR_BUFFER_SIZE];
static uint16_t leftFrontSonarBuffer[SONAR_BUFFER_SIZE];
static uint16_t leftBackSonarBuffer[SONAR_BUFFER_SIZE];

void sonarInit() {
	/*
	 * CS32 CS31 CS30 = 011
	 * Set clock prescaler factor to 64
	 * ICNC3 = noise canceler => enabled
	 */
	TCCR3B &= ~_BV(CS32);
	TCCR3B |= (_BV(CS30) | _BV(CS31));
	TCCR3B |= _BV(ICNC3);

	//Set timer 3 to Normal Mode
	TCCR3A &= ~(_BV(WGM30) | _BV(WGM31));
	TCCR3B &= ~(_BV(WGM32) | _BV(WGM33));

	/*
	 * CS42 CS41 CS40 = 011
	 * Set clock prescaler factor to 64
	 * ICNC4 = noise canceler => enabled
	 */
	TCCR4B &= ~_BV(CS42);
	TCCR4B |= (_BV(CS40) | _BV(CS41));
	TCCR4B |= _BV(ICNC4);

	//Set timer 4 to Normal Mode
	TCCR4A &= ~(_BV(WGM40) | _BV(WGM41));
	TCCR4B &= ~(_BV(WGM42) | _BV(WGM43));

//	/*
//	 * CS52 CS51 CS50 = 011
//	 * Set clock prescaler factor to 64
//	 * ICNC5 = noise canceler => enabled
//	 */
//	TCCR5B &= ~_BV(CS52);
//	TCCR5B |= (_BV(CS50) | _BV(CS51));
//	TCCR5B |= _BV(ICNC5);
//
//	//Set timer 5 to Normal Mode
//	TCCR5A &= ~(_BV(WGM50) | _BV(WGM51));
//	TCCR5B &= ~(_BV(WGM52) | _BV(WGM53));

	sonarBufferIndex = 0;

	/*
	 * Initialize the PW pin as input and all the
	 * RX pins of the sonar sensors as output
	 */
	pinMode(LEFTFRONT_SONAR_RX, OUTPUT);
	pinMode(LEFTBACK_SONAR_RX, OUTPUT);
	pinMode(FRONT_SONAR_RX, OUTPUT);

	/*
	 * Assuming sonarInit() gets called immediately after
	 * power up, then a period of 250 ms must be passed
	 * before the RX pin is ready to receive command.
	 */
	_delay_ms(250);

	//Enable each sonar when they are first initialized
	digitalWrite(LEFTFRONT_SONAR_RX, LOW);
	digitalWrite(LEFTBACK_SONAR_RX, LOW);
	digitalWrite(FRONT_SONAR_RX, LOW);

	return;
}

void sonarMeasureDistance() {
	/*
	 * sonarBufferIndex should always be between
	 * 0 and 19 since only 20 data readings should
	 * be kept.
	 */
	++sonarBufferIndex;

	if (sonarBufferIndex >= 20) {
		sonarBufferIndex = 0;
	}

	/*
	 * Clock is 16MHz, with a prescaler of 64, that means
	 * each timer tick is 4us. For the sonar, 147 us = 1 inch
	 * so we need to divide by 147 / 4 = 36.75.
	 */

	/*
	 * Left Sonar Reading
	 */
	sonarEcho1();
	_delay_ms(40);
	leftFrontSonarBuffer[sonarBufferIndex] = leftSonarTickCount / 36.75;
//	leftFrontSonarBuffer[sonarBufferIndex] = analogRead(LEFTFRONT_SONAR_AN) / 2;
//	Serial.print("Left Front: ");
//	Serial.println(leftFrontSonarBuffer[sonarBufferIndex]);
	/*
	 * Right Sonar Reading
	 */
	sonarEcho2();
	_delay_ms(40);
	leftBackSonarBuffer[sonarBufferIndex] = rightSonarTickCount / 36.75;
//	leftBackSonarBuffer[sonarBufferIndex] = analogRead(LEFTBACK_SONAR_AN) / 2;
//	Serial.print("Left Back: ");
//	Serial.println(leftBackSonarBuffer[sonarBufferIndex]);

	/*
	 * Front Sonar Reading
	 */
	digitalWrite(FRONT_SONAR_RX, HIGH);
	_delay_ms(50);
	frontSonarBuffer[sonarBufferIndex] = analogRead(FRONT_SONAR_AN) / 2;

//	sonarEcho3();
//	_delay_ms(38);
//	frontSonarBuffer[sonarBufferIndex] = frontSonarTickCount / 36.75;
//	frontSonarBuffer[sonarBufferIndex] = analogRead(FRONT_SONAR_AN) / 2;

//	Serial.print("Left Sonar: ");
//	Serial.print((int) leftSonarBuffer[sonarBufferIndex]);
//	Serial.println();
//
//	Serial.print("Right Sonar: ");
//	Serial.print((int) rightSonarBuffer[sonarBufferIndex]);
//	Serial.println();
//
//	Serial.print("Front Sonar: ");
//	Serial.print((int) frontSonarBuffer[sonarBufferIndex]);
//	Serial.println();

	return;
}

/*
 * This function processes the sonarBuffer and returns
 * the distance. It also rejects incorrect values by
 * analyzing the trend in which the distance is changing.
 */
uint16_t sonarGetDistance(int sonarID) {
	uint8_t currentIndex = sonarBufferIndex;
	uint16_t result = 0;

	switch (sonarID) {
	case LEFTFRONT_SONAR:
		result = leftFrontSonarBuffer[currentIndex];

		break;

	case LEFTBACK_SONAR:
		result = leftBackSonarBuffer[currentIndex];

		break;

	case FRONT_SONAR:
		result = averageValue(currentIndex, 4, frontSonarBuffer);

		break;

	default:
		break;
	}

	return result;
}

static uint16_t averageValue(uint8_t offset, uint8_t range, uint16_t* sonarBuffer) {
	uint8_t counter = 0;
	uint16_t sum = 0;

	for (counter = 0; counter < range; ++counter) {
		sum += *(sonarBuffer + offset);
		if (offset == 0) {
			offset = SONAR_BUFFER_SIZE - 1;
		}
		else {
			--offset;
		}
	}
	return sum / range;
}

//static uint16_t minValue(uint8_t offset, uint8_t range, uint16_t* sonarBuffer) {
//	uint8_t counter = 0;
//	uint16_t min = *(sonarBuffer + offset);
//
//	for (counter = 0; counter < range; ++counter) {
//		if (*(sonarBuffer + offset) < min) {
//			min = *(sonarBuffer + offset);
//		}
//
//		if (offset == 0) {
//			offset = SONAR_BUFFER_SIZE - 1;
//		}
//		else {
//			--offset;
//		}
//	}
//	return min;
//}
//
//static uint16_t maxValue(uint8_t offset, uint8_t range, uint16_t* sonarBuffer) {
//	uint8_t counter = 0;
//	uint16_t max = *(sonarBuffer + offset);
//
//	for (counter = 0; counter < range; ++counter) {
//		if (*(sonarBuffer + offset) < max) {
//			max = *(sonarBuffer + offset);
//		}
//
//		if (offset == 0) {
//			offset = SONAR_BUFFER_SIZE - 1;
//		}
//		else {
//			--offset;
//		}
//	}
//	return max;
//}

/**
 * Set Input Capture to look for a rising edge, clear
 * the interrupt flag and then enable Input Capture.
 * After that, set RX to HIGH to enable the sonar.
 */
static void sonarEcho1() {
	SET_RISING_EDGE3();
	CLEAR_IC_FLAG3();
	SET_IC_ENABLE3();

	digitalWrite(LEFTFRONT_SONAR_RX, HIGH);

	return;
}

/**
 * Set Input Capture to look for a rising edge, clear
 * the interrupt flag and then enable Input Capture.
 * After that, set RX to HIGH to enable the sonar.
 */
static void sonarEcho2() {
	SET_RISING_EDGE4();
	CLEAR_IC_FLAG4();
	SET_IC_ENABLE4();

	digitalWrite(LEFTBACK_SONAR_RX, HIGH);

	return;
}

///**
// * Set Input Capture to look for a rising edge, clear
// * the interrupt flag and then enable Input Capture.
// * After that, set RX to HIGH to enable the sonar.
// */
//static void sonarEcho3() {
//	SET_RISING_EDGE5();
//	CLEAR_IC_FLAG5();
//	SET_IC_ENABLE5();
//
//	digitalWrite(FRONT_SONAR_RX, HIGH);
//
//	return;
//}

ISR(TIMER3_CAPT_vect)
{
	Disable_Interrupt();

	/*
	 * Once the rising edge of PW is detected, it means
	 * RX has been staying HIGH long enough. Set it to
	 * LOW now to disable sonar.
	 */
	digitalWrite(LEFTFRONT_SONAR_RX, LOW);

	/*
	 * Reset Timer 3 when the rising edge of PW is
	 * detected, then change the Input Capture configuration
	 * to detect the falling edge and clear the interrupt flag.
	 */
	if (IS_RISING_EDGE3()) {
		TCNT3 = 0;
		SET_FALLING_EDGE3();
		CLEAR_IC_FLAG3();
	} else {
		/*
		 * Store the ICR3 value and disable Input Capture
		 * so it does not interfere with other components.
		 */
		leftSonarTickCount = ICR3;
		SET_RISING_EDGE3();
		CLEAR_IC_FLAG3();
		SET_IC_DISABLE3();
	}

	Enable_Interrupt();
}

ISR(TIMER4_CAPT_vect)
{
	Disable_Interrupt();

	/*
	 * Once the rising edge of PW is detected, it means
	 * RX has been staying HIGH long enough. Set it to
	 * LOW now to disable sonar.
	 */
	digitalWrite(LEFTBACK_SONAR_RX, LOW);

	/*
	 * Reset Timer 4 when the rising edge of PW is
	 * detected, then change the Input Capture configuration
	 * to detect the falling edge and clear the interrupt flag.
	 */
	if (IS_RISING_EDGE4()) {
		TCNT4 = 0;
		SET_FALLING_EDGE4();
		CLEAR_IC_FLAG4();
	} else {
		/*
		 * Store the ICR4 value and disable Input Capture
		 * so it does not interfere with other components.
		 */
		rightSonarTickCount = ICR4;
		SET_RISING_EDGE4();
		CLEAR_IC_FLAG4();
		SET_IC_DISABLE4();
	}

	Enable_Interrupt();
}

//ISR(TIMER5_CAPT_vect)
//{
//	Disable_Interrupt();
//
//	/*
//	 * Once the rising edge of PW is detected, it means
//	 * RX has been staying HIGH long enough. Set it to
//	 * LOW now to disable sonar.
//	 */
////	digitalWrite(FRONT_SONAR_RX, LOW);
//
//	/*
//	 * Reset Timer 4 when the rising edge of PW is
//	 * detected, then change the Input Capture configuration
//	 * to detect the falling edge and clear the interrupt flag.
//	 */
//	if (IS_RISING_EDGE5()) {
//		TCNT5 = 0;
//		SET_FALLING_EDGE5();
//		CLEAR_IC_FLAG5();
//	} else {
//		/*
//		 * Store the ICR5 value and disable Input Capture
//		 * so it does not interfere with other components.
//		 */
//		frontSonarTickCount = ICR5;
//		SET_RISING_EDGE5();
//		CLEAR_IC_FLAG5();
//		SET_IC_DISABLE5();
//	}
//
//	Enable_Interrupt();
//}
