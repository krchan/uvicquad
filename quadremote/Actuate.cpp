/*
 * Actuate.cpp
 *
 *  Created on: Mar 25, 2010
 */

#include "Actuate.h"
#include "WProgram.h"

int old_left_propulsion;
int old_right_propulsion;

int left_propulsion;
int right_propulsion;

int left_propulsion_scaled;
int right_propulsion_scaled;

extern int speed_right;
extern int speed_left;

void actuate()
{

	right_propulsion = speed_right;
	right_propulsion_scaled = speed_right;
	//right_propulsion_scaled = ( right_propulsion * 255 ) / 200;//scale value

	left_propulsion = speed_left;
	left_propulsion_scaled = speed_left;
	//left_propulsion_scaled = ( left_propulsion * 255 ) / 200;//scale value

	if( left_propulsion_scaled == 0 && right_propulsion_scaled == 0)
	{
		digitalWrite( LEFT_MOTOR_PIN_E, 0 );
		digitalWrite( RIGHT_MOTOR_PIN_E, 0 );
		return;
	}
	else
	{
		if( left_propulsion_scaled > 0 )
		{
//			Serial.println("Forwards");
			digitalWrite( LEFT_MOTOR_PIN_1, 0 );
			digitalWrite( LEFT_MOTOR_PIN_2, 1 );
		}
		else
		{
//			Serial.println("Backwards");
			left_propulsion_scaled = -left_propulsion_scaled;
			digitalWrite( LEFT_MOTOR_PIN_1, 1 );
			digitalWrite( LEFT_MOTOR_PIN_2, 0 );
		}

		if( left_propulsion_scaled > 0 )
		{
			analogWrite( LEFT_MOTOR_PIN_E, left_propulsion_scaled + 45 );
		}
		else
		{
			digitalWrite( LEFT_MOTOR_PIN_E, 0 );
		}

		Serial.print("Left Scaled: ");
		Serial.println(left_propulsion_scaled);

		if( right_propulsion_scaled > 0 )
		{
			//Serial.println("Backwards");
			digitalWrite( RIGHT_MOTOR_PIN_1, 0 );
			digitalWrite( RIGHT_MOTOR_PIN_2, 1 );
		}
		else
		{
			//Serial.println("Forwards");
			right_propulsion_scaled = -right_propulsion_scaled;
			digitalWrite( RIGHT_MOTOR_PIN_1, 1 );
			digitalWrite( RIGHT_MOTOR_PIN_2, 0 );
		}
		if( right_propulsion_scaled > 0 )
		{
			if( right_propulsion_scaled > 127 && right_propulsion_scaled < 200){
				analogWrite( RIGHT_MOTOR_PIN_E, (int) ((right_propulsion_scaled + 40) * 0.71) );
			}
			else if (right_propulsion_scaled >= 200) {
				analogWrite( RIGHT_MOTOR_PIN_E, (int) ((right_propulsion_scaled + 40) * 0.64) );
			}
			else {
				analogWrite( RIGHT_MOTOR_PIN_E, right_propulsion_scaled + 40);
			}
		}
		else
		{
			digitalWrite( RIGHT_MOTOR_PIN_E, 0 );
		}

Serial.print("Right Scaled: ");
Serial.println(right_propulsion_scaled);
	}
}
