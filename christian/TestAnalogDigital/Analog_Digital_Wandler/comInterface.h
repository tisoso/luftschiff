/*
 * comInterface.h
 *
 * All comInterface functions are there to ensure a save handling of stream communication.
 * Further information can be read at the functions themselves.
 *
 * Author: Julian Engelskirchen
 */ 


#ifndef COMINTERFACE_H_
#define COMINTERFACE_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "Analog_Digital_Wandler.h"

#define BAUDRATE 9600UL
#define MYUBRR (F_CPU / 16 / BAUDRATE - 1)
//maximum of bytes in stream
#define MAX_COMMANDS 50
//maximum time out to wait before triggering emergency handling
#define TIME_TIMEOUT 100

// Zeichen für empfangen Stream "Ende" --> 0x00
#define CMD_END 0x00

void initUSART(void);

void sendStream(unsigned char* stream, uint8_t size);
void sendByte(unsigned char byte);

bool isTimeout(void);
uint8_t getReceivedStream(unsigned char* stream);

#endif /* COMINTERFACE_H_ */
