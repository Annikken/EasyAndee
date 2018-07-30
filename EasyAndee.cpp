/* EasyAndee.h - Annikken Andee Library for Arduino
Annikken Pte Ltd
Author: Muhammad Hasif

The Easy Andee Library is to allow users to use a feature similar to Arduino's Firmata to control their Arduino using their Bluetooth enabled phone. Users no longer need to code the UI and logic. With the Easy Andee feature on the Andee App, users can create their UI and immediately control their electronics.

This library is free software */

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <SPI.h>
#include <EasyAndee.h>

#define RX_DELAY 50
#define RX_MAX 20
#define TX_MAX 20
 
unsigned int SS_PIN = 8;

char txBuffer[TX_MAX];
char rxBuffer[RX_MAX];

/****************************************************************************
 * EasyAndee Functions
 *****************************************************************************/

void EasyAndeeBegin()
{
	pinMode(SS_PIN,OUTPUT);
	digitalWrite(SS_PIN,HIGH);
	#if defined(__SAM3X8E__)
		SPI.begin(SS_PIN);
		SPI.setClockDivider(SS_PIN, 21);
	#else		
		SPI.begin();	
	#endif 	
	
	delay(800);
}
void EasyAndeeBegin(int pin)
{
	SS_PIN = pin;
	EasyAndeeBegin();
}
void setName(const char* name)
{
	char limit[33];
	if(strlen(name) > 32)
	{
		memcpy(limit,name,32);
		limit[32] = '\0';
		sendAndee(99,2,(char*)limit);//SET_ANDEE_NAME = 2
	}
	else
	{
		sendAndee(99,2,(char*)name);//SET_ANDEE_NAME = 2
	}
}


void EasyAndeePoll()
{
	pollRx(rxBuffer);
	switch(rxBuffer[1])
	{
		case EASYANDEE_D_OUT: //done, TODO test
			//processDOut();
			pinMode(rxBuffer[2] - 97,OUTPUT);
			digitalWrite(rxBuffer[2] - 97,rxBuffer[3]-48);
			break;
		
		case EASYANDEE_D_IN:
			processDIn();
			break;
		
		case EASYANDEE_A_OUT: //done, TODO test
			processAOut();
			break;
		
		case EASYANDEE_A_IN:
			processAIn();
			break;		
		default:
			Serial.println("Unknown Command");
			break;
	}
}

void processDIn()
{
	
}

void processAOut()
{
	char analogBuffer[4];
	analogBuffer[0] = rxBuffer[3];
	analogBuffer[1] = rxBuffer[4];
	analogBuffer[2] = rxBuffer[5];
	analogBuffer[3] = '\0';

	int analogValue = strtol(analogBuffer,NULL,10);	  

	if(rxBuffer[2] == 'd')//pin 3
	{
		pinMode(3,OUTPUT);
		analogWrite(3,analogValue);
	}
	else if(rxBuffer[2] == 'f')//pin 5
	{
		pinMode(5,OUTPUT);
		analogWrite(5,analogValue);
	}
	else if(rxBuffer[2] == 'g')//pin 6
	{
		pinMode(6,OUTPUT);
		analogWrite(6,analogValue);
	}
	else if(rxBuffer[2] == 'j')//pin 9
	{
		pinMode(9,OUTPUT);
		analogWrite(9,analogValue);
	}  
}

void processAIn()
{
	
}

/****************************************************************************
 * Function to send and receive data to and from PIC32 using SPI
 *****************************************************************************/
void resetRX()
{
	memset(rxBuffer,0x00,RX_MAX);
}

void spiSendData(char* txBuffer, size_t bufferLength){
	unsigned int txCount = 0;
	
	SPI.beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE0));
	
	digitalWrite(SS_PIN,LOW);
	for(txCount = 0;txCount < bufferLength;txCount++)//send whole buffer
	{		
		SPI.transfer(txBuffer[txCount]);//transfer and receive 1 char in SPI
		delayMicroseconds(40);
	}
	digitalWrite(SS_PIN,HIGH);
	
	SPI.endTransaction();
	delay(10);
}

bool pollRx(char* buffer)
{
	unsigned int rxCount = 0;
	unsigned char tempChar;	
	resetRX();
	delay(3);	
	SPI.beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE0));
	
	digitalWrite(SS_PIN,LOW);	
	for(int i = 0;i<RX_DELAY;)
	{
		tempChar = SPI.transfer(0x00);
		if(tempChar > 32)
		{				
			if(tempChar == ']')
			{
				Serial.print("pollRx:");Serial.println(buffer);
				buffer[rxCount] = '\0';
				
				digitalWrite(SS_PIN,HIGH);
				SPI.endTransaction();
				
				delay(5);
				return true;			
				break;
			}
			else if(tempChar == 173)
			{
				Serial.println("pollRx: No Reply");
				digitalWrite(SS_PIN,HIGH);
				SPI.endTransaction();
				
				delay(5);
				return false;
				break;
			}
			else
			{
				buffer[rxCount++] = tempChar;	
			}
		}
		else
		{
			i++;
		}
	}	
	digitalWrite(SS_PIN,HIGH);
	
	SPI.endTransaction();
	delay(5);	
	return false;
} 

void sendAndee(unsigned int id,unsigned char andeeCommand,char* message){
	memset(txBuffer,0x00,TX_MAX);
	sprintf(txBuffer,"#%d#%d#%s;",id, andeeCommand, message);	
	
	spiSendData( txBuffer,strlen(txBuffer) );	
}
