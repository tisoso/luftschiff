/*
 * Serielle_Schnittstelle.c
 *
 * Created: 11.10.2014 13:11:21
 *  Author: Hendrik Stockhoff
 */ 


#include "Serielle_Schnittstelle.h"
#include "Interface.h"
/*
int main(void)
{
	usart_init();
	
	//unsigned char x = 0;
	
    while(1)
    {
		
		// Warte 10 mal 100ms
		for(unsigned char i = 0; i <= 10; i++) {
			_delay_ms(100);
		}
		
		UCSR0A |= (1<<RXC0);
		
		// Wenn ein Zeichen angekommen ist, abholen und im Echo zurücksenden
		if(UCSR0A & (1<<RXC0)) {
			x = receive_data();
			send_data(x);
		}
    }
}
*/