/*
 * This file defines the common variables needed by the project.
 * Pin assignments should be declared here to avoid collision.
 */

#ifndef __COMMON_H__
#define __COMMON_H__

#define TRUE 1
#define FALSE 0

#define Disable_Interrupt()  asm volatile ("cli"::)
#define Enable_Interrupt()   asm volatile ("sei"::)

/*
 * Pin definitions are here
 */
#define JOYSTICK_BUTTON 7
#define X_AXIS 6
#define Y_AXIS 5
#define ONBOARD_LED 13

void GetSonarDistance(char *SonarDistanceBuffer);
uint8_t DistanceToIntensity(int distance);

#endif
