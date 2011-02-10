#ifndef SONAR_H_
#define SONAR_H_

#include <stdint.h>

void sonarInit();
void sonarMeasureDistance();
uint16_t sonarGetDistance(int sonarID);


/*
 * Macros to change Timer 3 settings when used for
 * input capture and measure the pulse width of
 * the PW pin from the sonar
 */
#define SET_RISING_EDGE3()       (TCCR3B |= _BV(ICES3))
#define SET_FALLING_EDGE3()      (TCCR3B &= ~(_BV(ICES3)))
#define IS_RISING_EDGE3()        (TCCR3B & _BV(ICES3))
#define IS_FALLING_EDGE3()       ~(TCCR3B & _BV(ICES3))
#define SET_IC_ENABLE3()         (TIMSK3 |= _BV(ICIE3))
#define SET_IC_DISABLE3()        (TIMSK3 &= ~_BV(ICIE3))
#define CLEAR_IC_FLAG3()         (TIFR3 |= _BV(ICF3))

/*
 * Macros to change Timer 4 settings when used for
 * input capture and measure the pulse width of
 * the PW pin from the sonar
 */
#define SET_RISING_EDGE4()       (TCCR4B |= _BV(ICES4))
#define SET_FALLING_EDGE4()      (TCCR4B &= ~(_BV(ICES4)))
#define IS_RISING_EDGE4()        (TCCR4B & _BV(ICES4))
#define IS_FALLING_EDGE4()       ~(TCCR4B & _BV(ICES4))
#define SET_IC_ENABLE4()         (TIMSK4 |= _BV(ICIE4))
#define SET_IC_DISABLE4()        (TIMSK4 &= ~_BV(ICIE4))
#define CLEAR_IC_FLAG4()         (TIFR4 |= _BV(ICF4))

/*
 * Macros to change Timer 5 settings when used for
 * input capture and measure the pulse width of
 * the PW pin from the sonar
 */
//#define SET_RISING_EDGE5()       (TCCR5B |= _BV(ICES5))
//#define SET_FALLING_EDGE5()      (TCCR5B &= ~(_BV(ICES5)))
//#define IS_RISING_EDGE5()        (TCCR5B & _BV(ICES5))
//#define IS_FALLING_EDGE5()       ~(TCCR5B & _BV(ICES5))
//#define SET_IC_ENABLE5()         (TIMSK5 |= _BV(ICIE5))
//#define SET_IC_DISABLE5()        (TIMSK5 &= ~_BV(ICIE5))
//#define CLEAR_IC_FLAG5()         (TIFR5 |= _BV(ICF5))

#define FRONT_SONAR_RX 37
#define LEFTFRONT_SONAR_RX 36
#define LEFTBACK_SONAR_RX 35

#define FRONT_SONAR_AN 0

#define SONAR_BUFFER_SIZE 20

#endif /* SONAR_H_ */
