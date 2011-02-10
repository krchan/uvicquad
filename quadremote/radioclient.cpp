/*
 * Radio client API
 */

#include "radio.h"
#include "packet.h"
#include "Wprogram.h"
#include "common.h"

static uint8_t RemoteStationAddr[RADIO_ADDRESS_LENGTH] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x88 };
static uint8_t BaseStationAddr[RADIO_ADDRESS_LENGTH] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x77 };

static radiopacket_t packet;

extern int command;

void radioInitSetup() {
	/*
	 * Initialize the SPI connection, configure the I/O pins,
	 * and set the register defaults
	 */
	Radio_Init();

	/*
	 * Configure pipe 0 as a receiver.  Pipe 0 has to be enabled
	 * for the radio's link layer protocol to work.  This line
	 * shouldn't be necessary since pipe 0 is enabled by default,
	 * but it's nice to be explicit.
	 */
	Radio_Configure_Rx(RADIO_PIPE_0, RemoteStationAddr, ENABLE);

	/*
	 * Configure the radio's data rate (must match the other radio)
	 * and the broadcast power
	 */
	Radio_Configure(RADIO_2MBPS, RADIO_HIGHEST_POWER);

	/*
	 * set the address to send to, dangling prepositions be damned.
	 */
	Radio_Set_Tx_Addr(BaseStationAddr);

}

void radioSend() {
	/*
	 * Remote Station should only send message of type SENSORDATA to the
	 * Base Station.
	 */
	packet.type = SENSORDATA;

	packet.payload.hovercraftData.command = NONE;

//	memcpy(packet.payload.message.address, RemoteStationAddr, RADIO_ADDRESS_LENGTH);
//	snprintf((char*)packet.payload.message.messagecontent, 20, msg);

	// Send the packet to the address specified with Radio_Set_Tx_Addr above.
	Radio_Transmit(&packet, RADIO_WAIT_FOR_TX);
}

/*
 * To be completed.
 * This is the side where we decompose the content of the packet
 * and find out what command was sent from the Base Station
 */
void radio_rxhandler(uint8_t pipenumber)
{
//	Serial.println("radio_rxhandler");

	Radio_Receive(&packet);

	if (packet.type == COMMAND){
		Serial.println("Received Command");

		switch (packet.payload.hovercraftData.command){

		case NONE:
			Serial.println("Command is NONE");
			break;

		case TURN_LEFT:
			Serial.println("Command is TURN_LEFT");
			break;

		case TURN_RIGHT:
			Serial.println("Command is TURN_RIGHT");
			break;

		case NEXT_LEFT:
			Serial.println("Command is NEXT_LEFT");
			break;

		case NEXT_RIGHT:
			Serial.println("Command is NEXT_RIGHT");
			break;

		case STOP:
			Serial.println("Command is STOP");
			command = STOP;
			break;

		case FORWARD:
			Serial.println("Command is FORWARD");
			command = FOLLOW_WALL;
			break;

		case REVERSE:
			Serial.println("Command is REVERSE");
			break;

		case FOLLOW_WALL:
			Serial.println("Command is FOLLOW_WALL");
			break;

		default:
			break;
		}
	}
}


