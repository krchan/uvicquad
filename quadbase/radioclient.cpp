/*
 * Radio client API
 */

#include "radio.h"
#include "packet.h"
#include "Wprogram.h"

static uint8_t RemoteStationAddr[RADIO_ADDRESS_LENGTH] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x88 };
static uint8_t BaseStationAddr[RADIO_ADDRESS_LENGTH] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x77 };

static radiopacket_t packet;

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
	Radio_Configure_Rx(RADIO_PIPE_0,BaseStationAddr , ENABLE);

	Radio_Configure(RADIO_2MBPS, RADIO_HIGHEST_POWER);

	Radio_Set_Tx_Addr(RemoteStationAddr);

}

void radioSend(uint8_t command) {

	packet.type = COMMAND;

	packet.payload.hovercraftData.command = command;

	Radio_Transmit(&packet, RADIO_WAIT_FOR_TX);
}


void radio_rxhandler(uint8_t pipenumber)
{
	Radio_Receive(&packet);
	Serial.print((char *) packet.payload.message.messagecontent);
}


