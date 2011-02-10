/*
 * Plan.cpp
 *
 *  Created on: Mar 25, 2010
 *      Author: jensweber
 */

#include "Plan.h"
#include "radio/radio.h"
#include "radio/packet.h"

uint8_t rx_addr[RADIO_ADDRESS_LENGTH] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x88 };
uint8_t tx_addr[RADIO_ADDRESS_LENGTH] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x77 };

radiopacket_t packet;

extern int command = 0;

void plan()
{
	//process radio
	/* ... */
	command = FOLLOW_WALL;
}
