/* EasyAndee.h - Annikken Andee Library for Arduino
Annikken Pte Ltd
Author: Muhammad Hasif

The Easy Andee Library is to allow users to use a feature similar to Arduino's Firmata to control their Arduino using their Bluetooth enabled phone. Users no longer need to code the UI and logic. With the Easy Andee feature on the Andee App, users can create their UI and immediately control their electronics.

This library is free software. This library also uses parts of the Arduino Firmata Library */

#if defined (__arm__)
#include "itoa.h"
#endif

#define EASYANDEE_D_IN 'E'
#define EASYANDEE_D_OUT 'D'
#define EASYANDEE_A_IN 'B'
#define EASYANDEE_A_OUT 'A'
#define SET_ANDEE_NAME 2

#define REPLY_A_IN 'C'
#define REPLY_D_IN 'F'



/* #define PIN_0 'a';
#define PIN_1 'b';
#define PIN_2 'c';
#define PIN_3 'd';
#define PIN_4 'e';
#define PIN_5 'f';
#define PIN_6 'g';
#define PIN_7 'h';
#define PIN_8 'i';
#define PIN_9 'j';
#define PIN_10 'k';
#define PIN_11 'l';
#define PIN_12 'm';
#define PIN_13 'n';
#define PIN_A0 'o';
#define PIN_A1 'p';
#define PIN_A2 'q';
#define PIN_A3 'r';
#define PIN_A4 's';
#define PIN_A5 't'; */

void EasyAndeeBegin();
void EasyAndeeBegin(int);
void setName(const char*);
void EasyAndeePoll();
void processDIn();
void processAIn();
void processAOut();


void resetRX();
void spiSendData(char*, size_t);
bool pollRx(char*);
void sendAndee(unsigned int, unsigned char,char*);