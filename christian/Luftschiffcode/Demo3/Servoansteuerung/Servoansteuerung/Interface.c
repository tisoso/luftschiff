#include "Interface.h"
// Initialisieren der seriellen Schnittstelle
void usart_init()
{
	UBRR0H = R_UBRR >> 8;
	UBRR0L = R_UBRR;
	
	UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0);	
}

// Senden eines Characters 
void send_data(unsigned char data) 
{
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;	
}

// Empfangen eines Characters
unsigned char receive_data()
{
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}
