/*
 * Analog_Digital_Wandler.c
 *
 * Created: 10.10.2014 17:49:57
 *  Author: Hendrik Stockhoff
 *	
 * Es stehen für den Analog-Digital-Wandler(AD-Wandler) folgende Anschlüsse zu Verfügung:
 *
 *		ADC0
 *		ADC1
 *		ADC2
 *		ADC3
 *		ADC4
 *		ADC5
 *		AKKU (Zur Messung der Akkuspannung vorgesehen)
 *
 * Die AD-Wandler Referenzspannung ist auf 5V eingestellt. In der Datei "ADC_Control.c" kann sie auf andere Werte umgestellt werden.
 * (REFS1 = 1 und REFS0 = 1 => 1,1V interne Referenzspannung)
 */ 


#include "Analog_Digital_Wandler.h"
#include "ADC_Control.h"
#include "comInterface.h"

int main(void)
{
	
	// Initialisieren des Analog-Digital-Wandlers
	initADC();
	
	// Initialisieren der seriellen Schnittstelle
	initUSART();
	
	// Variable vereinbaren und initialisieren
	unsigned int ADC_value = 0;
	
    while(1)
    {
		// Analog-to-Digital Wandlung an AKKU ausführen und den Wert in ADC_value laden
		ADC_value = ADC_convert(ADC1);
		
		// Senden des AD-Wertes über die serielle schnittstelle
		// Da der AD-Wert die größe von 10 Bit hat, muss er aufgespalten werden und zuerst die oberen 
		// und anschließend die unteren 8 Bit gesendet werden
		//ADC_value *= 5.0 / 512 * 2.54;
		//sendByte(ADC_value>>8); // Senden des HIGH-Byte (obere 8 Bit)
		sendByte(ADC_value>>2);	// Senden des LOW-Byte	(untere 8 Bit)
		
		// LF, Zeilenvorschub (line feed)
		//sendByte('\n');
		
		// Warten, 1000ms = 1s
		_delay_ms(1000);
		
		
    }
}