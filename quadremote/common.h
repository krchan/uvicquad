/*
 * This file defines the common variables needed by the project.
 */
#ifndef COMMON_H_
#define COMMON_H_

#define TRUE 1
#define FALSE 0
#define ONBOARD_LED 13

#define Disable_Interrupt()  asm volatile ("cli"::)
#define Enable_Interrupt()   asm volatile ("sei"::)

typedef enum {
	LEFTFRONT_SONAR,
	LEFTBACK_SONAR,
	FRONT_SONAR
} sonarID;

typedef enum
{
	NONE,
	TURN_LEFT,
	TURN_RIGHT,
	NEXT_LEFT,
	NEXT_RIGHT,
	STOP,
	FORWARD,
	REVERSE,
	FOLLOW_WALL,
} COMMAND_TYPE;

//typedef enum {
//	STOP,
//	FOLLOW_WALL,
//	TURN,
//	AVOID_OBSTACLE,
//} controlState;

#endif /* COMMON_H_ */
