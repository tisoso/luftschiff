/*
 * Servoansteuerung.c
 *
 * Beispielprogramm zur Servoansteuerung
 *
 * Created: 10.10.2014 17:02:45
 *  Author: Hendrik Stockhoff
 */ 

#include "Servoansteuerung.h"
#include "Servo_Control.h"

#include "Serielle_Schnittstelle.h"
#include "Interface.h"


// Hauptprogramm
int main(void)
{
	char x, y;
	// Initialisieren der Servos
	servos_init();
	usart_init();
	int motor[16] = {0, 75, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 190, 210, 230, 255};
	
    while(1)
    {
		// Warte 10 mal 100ms
		for(unsigned char i = 0; i <= 1; i++) {
			_delay_ms(100);
		}
		
		UCSR0A |= (1<<RXC0);
		
		//servo_setpos(Servo2, 0);
		
		if(UCSR0A & (1<<RXC0)) {
			x = receive_data();
			y = receive_data();
			
			servo_setpos(Servo1, (x>>4) * 16);
			servo_setpos(Servo2, motor[x%16]);
			servo_setpos(Servo3, motor[y>>4]);
			servo_setpos(Servo4, motor[y%16]);
			send_data((x>>4) * 16);
			send_data(motor[x%16]);
			send_data('#');
		}
		/*
		// Position von 63 bis 188 einstellbar
		// Servomittelstellung ist 125
		servo_setpos(Servo2, 0);		
        
		// Warte 1000ms = 1s
		_delay_ms(3000);
		
		// Servo auf Position 63 setzen
		servo_setpos(Servo2, 50);
		
		// Warte 1000ms = 1s
		_delay_ms(1000);
		
		// Servo auf Position 188 setzen
		servo_setpos(Servo2, 10);
		
		// Warte 1000ms = 1s
		_delay_ms(1000);
		*/
    }
}