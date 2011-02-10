/*
 * Control.cpp
 *
 *  Created on: Mar 25, 2010
 *      Author: jensweber
 */

#include "Control.h"
#include "Arduino/WProgram.h"
#include "common.h"

static int THRUST_MOTOR_PIN_1 = 13;
static int THRUST_MOTOR_PIN_2 = 12;
static int THRUST_MOTOR_PIN_E = 4;

/*prototypes*/
extern uint16_t sonarGetDistance( int sonar );

int speed_right;
int speed_left;


extern int command;

int state = STOP;

const int FOLLOW_WALL_SET_POINT_BACK = 21;

int follow_wall_prev_error_right;
int follow_wall_error_right;
int follow_wall_p_right;
int FOLLOW_WALL_KP_RIGHT = 2;
int follow_wall_d_right;
int FOLLOW_WALL_KD_RIGHT = 3;

const int FOLLOW_WALL_SET_POINT_FRONT = 20;

int follow_wall_prev_error_left;
int follow_wall_error_left;
int follow_wall_p_left;
int FOLLOW_WALL_KP_LEFT = 2;
int follow_wall_d_left;
int FOLLOW_WALL_KD_LEFT = 3;

void control()
{
	int temp1;
	int temp2;
	int temp3;

	digitalWrite(THRUST_MOTOR_PIN_1, LOW);
	digitalWrite(THRUST_MOTOR_PIN_2, HIGH);
Serial.println(state);
	switch( state )
		{
			case STOP:
				state = command;
				Serial.println("stop");

				speed_left = 0;
				speed_right = 0;
				// Turn off the thrust motor
				analogWrite(THRUST_MOTOR_PIN_E, 0);
				//digitalWrite(44, LOW);
				break;

			case FOLLOW_WALL:
				Serial.println("follow");

				temp1 = sonarGetDistance( LEFTFRONT_SONAR );
				temp2 = sonarGetDistance( LEFTBACK_SONAR );
				temp3 = sonarGetDistance( FRONT_SONAR );
				follow_wall_error_right = temp2 - FOLLOW_WALL_SET_POINT_BACK;



				//				follow_wall_error_right = sonarGetDistance( LEFTBACK_SONAR ) - FOLLOW_WALL_SET_POINT_RIGHT;
				follow_wall_p_right = follow_wall_error_right;
				follow_wall_d_right = follow_wall_error_right - follow_wall_prev_error_right;
				follow_wall_prev_error_right = follow_wall_error_right;


				//this scaling factor of 1/3 should keep the value between 100 and -100;
//				speed_right = ( FOLLOW_WALL_KP_RIGHT*follow_wall_p_right
//						+ FOLLOW_WALL_KD_RIGHT*follow_wall_d_right
//						+ FOLLOW_WALL_KI_RIGHT*follow_wall_i_right )/3;
				speed_right = ( FOLLOW_WALL_KP_RIGHT * follow_wall_p_right + FOLLOW_WALL_KD_RIGHT * follow_wall_d_right );
				if (speed_right>255) speed_right=255;
				if (speed_right<-255) speed_right=-255;
//				Serial.print( "Left Back Sonar" );
//				Serial.println( follow_wall_error_right + FOLLOW_WALL_SET_POINT_RIGHT );
//				Serial.print( "Speed Back: " );
//				Serial.println( speed_right );
//				Serial.print("Left Back Sonar Value: ");
//				Serial.println(temp2);

				follow_wall_error_left = temp1 - FOLLOW_WALL_SET_POINT_FRONT;

//				follow_wall_error_left = sonarGetDistance( LEFTFRONT_SONAR ) - FOLLOW_WALL_SET_POINT_LEFT;
				follow_wall_p_left = follow_wall_error_left;
				follow_wall_d_left = follow_wall_error_left - follow_wall_prev_error_left;
				follow_wall_prev_error_left = follow_wall_error_left;

				//this scaling factor of 1/3 should keep the value between 100 and -100;
//				speed_left = ( FOLLOW_WALL_KP_LEFT*follow_wall_p_left
//						+ FOLLOW_WALL_KD_LEFT*follow_wall_d_left
//						+ FOLLOW_WALL_KI_LEFT*follow_wall_i_left )/3 - 25;
				speed_left = ( FOLLOW_WALL_KP_LEFT * follow_wall_p_left + FOLLOW_WALL_KD_LEFT * follow_wall_d_left );
				if (speed_left>255) speed_left=255;
				if (speed_left<-255) speed_left=-255;
//				Serial.print( "Left Front Sonar" );
//				Serial.println( follow_wall_error_left + FOLLOW_WALL_SET_POINT_LEFT );
//				Serial.print( "Speed Front: " );
//				Serial.println( speed_left );
//				Serial.print("Left Front Sonar Value: ");
//				Serial.println(temp1);

				// Turn on the thrust motor
				analogWrite(THRUST_MOTOR_PIN_E, 55);
				//digitalWrite(44, HIGH);

				Serial.print("Sonar Left Front: ");
				Serial.println(temp1);
				Serial.print("Sonar Left Back: ");
				Serial.println(temp2);
				Serial.print("Speed left: ");
				Serial.println(speed_left);
				Serial.print("Speed right: ");
				Serial.println(speed_right);

				state = command;
				//if (temp3<20) state = command = STOP;
				break;
//			case TURN:
//				speed = 0;
//				if( yaw says we've stopped)
//				{
//					turn = 50;
//					delay( 1000 );//wait for a bit while we reorient
//					while( !( sonar( FRONT ) > OBSTACLE_AVOIDANCE_THREASHOLD );
//					turn = 0;
//					speed = SPEED;
//					while( sonar( RIGHT ) > FOLLOW_WALL_SET_POINT_RIGHT );
//					state = FOLLOW_WALL;
//				}
//				break;
			default:
				break;
		}
  }

