#ifndef RADIOCLIENT_H_
#define RADIOCLIENT_H_

void radioInitSetup();
void radioSend(uint8_t command);
void radio_rxhandler(uint8_t pipenumber);

#endif /* RADIOCLIENT_H_ */
