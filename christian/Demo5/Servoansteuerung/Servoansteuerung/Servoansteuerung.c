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
	char x, y, cmd, wert;
	// Initialisieren der Servos
	servos_init();
	usart_init();
	int motor[16] = {0, 30, 50, 70, 80, 90, 100, 127, 140, 150, 160, 170, 180, 190, 200, 210};
	
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
			cmd = x >> 4;
			wert = x%16;
			
			switch(cmd) {
				case 1: // Servo
					servo_setpos(Servo1, wert * 16);
					break;
				case 2: // Motor 1
					servo_setpos(Servo2, motor[wert]);
					break;
				case 3: // Motor 2
					servo_setpos(Servo3, motor[wert]);
					break;
				case 4: // Motor 3
					servo_setpos(Servo4, motor[wert]);
					break;
			}
			
			//send_data(cmd);
		}
    }
}