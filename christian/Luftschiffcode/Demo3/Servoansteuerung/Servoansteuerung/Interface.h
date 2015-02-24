/*
 * Serielle_Schnittstelle.c
 *
 * Created: 11.10.2014 13:11:21
 *  Author: Hendrik Stockhoff
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#ifndef INTERFACE_H_
#define INTERFACE_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#define Baud 9600UL						// Baudrate
#define R_UBRR ((F_CPU/(16L*Baud))-1)	// Registerspezifische Deklarationen


// Initialisieren der seriellen Schnittstelle
void usart_init();

// Senden eines Characters
void send_data(unsigned char data);

// Empfangen eines Characters
unsigned char receive_data();


#endif /* INTERFACE_H_ */