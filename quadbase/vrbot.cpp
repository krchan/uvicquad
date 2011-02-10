//vrbot

// Created on: Mar 25, 2010
//     Author: derekja

#include "WProgram.h"
#include "SoftwareSerial.h"
#include "vrbot_protocol.h"
#include "radioclient.h"
#include "string.h"
#include "LiquidCrystal.h"
#include "radioclient.h"
#include "packet.h"

extern LiquidCrystal lcd;


uint8_t _receivePin;
uint8_t _transmitPin;
long _baudRate;
int _bitPeriod;
char message[20];



void vrbot_setup(){  
  
  // PC UART Init
  pinMode(0, INPUT);         // sets the digital pin as input
  pinMode(1, OUTPUT);        // sets the digital pin as output

  // VRbot UART Init
  pinMode(12, INPUT);        // sets the digital pin as input
  pinMode(13, OUTPUT);       // sets the digital pin as output
  
  // Input Output Init
  pinMode(2,INPUT);          // sets the digital pin as input
  pinMode(3,OUTPUT);         // sets the digital pin as output
  pinMode(4,OUTPUT);         // sets the digital pin as output
  
  digitalWrite(3,HIGH);      // Set pin 3 high
  digitalWrite(4,LOW);       // Set pin 4 low
  
  
 
 
  // connect Di2 to Di3 to enter Normal mode - Di2 is high
  // connect Di2 to Di4 to enter Bridge mode - Di2 is low
  // bridge mode allow direct communication between VRbot module and VRbot GUI software

  if (digitalRead(2)==LOW){   // if Di2 is LOW enter Bridge mode 

		lcd.setCursor(0, 1);      
        lcd.print("bridge mode");
      while(1)
      {
        int pc2vr = digitalRead(0);
        digitalWrite(13, pc2vr);
    
        int vr2pc = digitalRead(12);
        digitalWrite(1, vr2pc);
     }
  } 

  // if Di2 is HIGH enter Normal mode
  Serial.begin(9600);
  delay(200);
  Serial.println("Arduino 2009 VRBot control program");
  Serial.println("Normal Mode");
  lcd.clear();
  lcd.setCursor(0, 0);      
  lcd.print("normal mode");
  
  // Set up and detect VRbot
  
  VRbot_setup_int();  
   lcd.clear();
  lcd.setCursor(0, 0);      
  lcd.print("normal mode2");
  
  if (!VRbot_Detect())                 
     Serial.println("VRbot NA");
  else {
       Serial.println("VRbot detected");
       
       // Set VRbot timeout to 5 seconds
       Serial.println("Setting timeout to: 5 seconds");
       VRbot_SetTimeout(5);             
       
       // Set VRbot language to English       
       Serial.println("Setting Language to: English");
       VRbot_SetLanguage(0);  
  }  
    lcd.setCursor(0, 0);      
  lcd.print("normal mode3");
  
}


// Speaker Dependent recognition function
// Wait for user trigger word set by user with VRBot GUI
// Set wordset Group 1 
// Wait for command 0 or command 1 set by user with VRBot GUI, other commands are ignored

void SD_Recognition()
{
  int cmd;
    
  Serial.println("Say 'Hovercraft'!");
  lcd.clear();
  lcd.setCursor(0, 0);      
  lcd.print("Say 'Hovercraft'");
  
  VRbot_RecognizeSD(0);      // start SD trigger word recognition and wait for trigger
  cmd = VRbot_CheckResult(); // check recognition result

  if( cmd == -1) // timeout 
  {
    Serial.println("Timeout");
    return;
  }
  
  if( cmd == -2) // error 
  {
    lcd.setCursor(0, 1);      
    lcd.print("Error");
  
    Serial.println("Error Trigger");
    return;
  } 
  
  Serial.println("Group 1");
  Serial.println("Say a command!");
  lcd.clear();
  lcd.setCursor(0, 0);      
  lcd.print("Command?");
  radioSend(NONE);
  
  VRbot_RecognizeSD(1);        // start SD recognition group 1 and wait for a command
  cmd = VRbot_CheckResult();   // check recognition result
 
  switch (cmd){
    
    case -2: // Error
      lcd.setCursor(0, 1);      
      lcd.print("Error");
    
      Serial.println("Error");
      break;

    case -1: // Timeout
      lcd.setCursor(0, 1);      
      lcd.print("Timeout");
    
      Serial.println("Timeout");
      break;
  
    case 0: // USER SD WORD 0
      Serial.println("Command: Turn Left");
      lcd.setCursor(0, 1);      
      lcd.print("Turn Left");
      radioSend(TURN_LEFT);
      
      break;

    case 1: // USER SD WORD 1
      Serial.println("Commmand: Turn Right");
      lcd.setCursor(0, 1);      
      lcd.print("Turn Right");
      radioSend(TURN_RIGHT);
      
      break;
  
    case 2: // USER SD WORD 2
      Serial.println("Commmand: Stop");
      lcd.setCursor(0, 1);      
      lcd.print("Stop");
      radioSend(STOP);
      break;
      
    case 3: // USER SD WORD 3
      Serial.println("Commmand: Forward");
      lcd.setCursor(0, 1);      
      lcd.print("Forward");
      radioSend(FORWARD);
      break;  
      
    case 4: // USER SD WORD 4
      Serial.println("Commmand: Reverse");
      lcd.setCursor(0, 1);      
      lcd.print("Reverse");
      radioSend(REVERSE);
      break;
      
    default: // Other command
      lcd.setCursor(0, 1);      
      lcd.print("Cmd Error");
    
      Serial.println("Commmand: error!");
      break;
    
  }
  
}

/*******************************************************************************/

void VRbot_setup_int()
{
  _baudRate = 9600;
  _bitPeriod = 1000000 / _baudRate;
  _receivePin = 12;
  _transmitPin = 13;
  digitalWrite(_transmitPin, HIGH);
  delayMicroseconds( _bitPeriod); 
}

unsigned char VRbot_read(void)
{
  uint8_t val = 0;
  int bitDelay = _bitPeriod - clockCyclesToMicroseconds(100);
  
  // one byte of serial data (LSB first)
  // ...--\    /--\/--\/--\/--\/--\/--\/--\/--\/--...
  //	 \--/\--/\--/\--/\--/\--/\--/\--/\--/
  //	start  0   1   2   3   4   5   6   7 stop

  while (digitalRead(_receivePin));

  // confirm that this is a real start bit, not line noise
  if (digitalRead(_receivePin) == LOW) {
    // frame start indicated by a falling edge and low start bit
    // jump to the middle of the low start bit
    delayMicroseconds(bitDelay / 2 - clockCyclesToMicroseconds(50));
	
    // offset of the bit in the byte: from 0 (LSB) to 7 (MSB)
    for (int offset = 0; offset < 8; offset++) {
	// jump to middle of next bit
	delayMicroseconds(bitDelay);
	
	// read bit
	val |= digitalRead(_receivePin) << offset;
    }
	
    delayMicroseconds(_bitPeriod);
    
    return val;
  }
  
  return -1;
}

void VRbot_write(uint8_t b)
{
  if (_baudRate == 0)
    return;
    
  int bitDelay = _bitPeriod - clockCyclesToMicroseconds(50); // a digitalWrite is about 50 cycles
  byte mask;

  digitalWrite(_transmitPin, LOW);
  delayMicroseconds(bitDelay);

  for (mask = 0x01; mask; mask <<= 1) {
    if (b & mask){ // choose bit
      digitalWrite(_transmitPin,HIGH); // send 1
    }
    else{
      digitalWrite(_transmitPin,LOW); // send 1
    }
    delayMicroseconds(bitDelay);
  }

  digitalWrite(_transmitPin, HIGH);
  delayMicroseconds(bitDelay);
}

/*******************************************************************************/

unsigned char VRbot_Detect(void) {
  unsigned char i;
  for (i = 0; i < 5; ++i) {
    delay(100);    
    VRbot_write(CMD_BREAK);        
    if ( VRbot_read() == STS_SUCCESS)
      return 255;
  }
  return 0;
}

unsigned char VRbot_SetLanguage(unsigned char lang) {
        
  VRbot_write(CMD_LANGUAGE);
  delay(5);
  VRbot_write(ARG_ZERO + lang);

  if (VRbot_read() == STS_SUCCESS)
    return 255;
  return 0;
}

void VRbot_RecognizeSD(unsigned char group) {
  VRbot_write(CMD_RECOG_SD);
  delay(5);
  VRbot_write(ARG_ZERO + group);
}



void VRbot_SetTimeout(unsigned char s) {
  VRbot_write(CMD_TIMEOUT);
  delay(5);
  VRbot_write(ARG_ZERO + s);
  delay(5);
}

signed char VRbot_CheckResult(void) {
  
  unsigned char rx;
  rx = VRbot_read();
  if (rx == STS_SIMILAR || rx == STS_RESULT) {
    delay(5);
    VRbot_write(ARG_ACK);
    return (VRbot_read() - ARG_ZERO);
  }
  if (rx == STS_TIMEOUT)
    return -1;
    
  return -2;
}
