#include "ADC_Control.h"

void initADC(void)
{
	unsigned int result;
	
	// Einstellen der AD-Wandler Referenzspannung auf 5V mit externem Kondensator an AREF
	ADMUX |= (1<<REFS0);
	
	// Wählen des Vorteilers
	ADCSRA |= (1<<ADPS0) | (1<<ADPS2);
	
	// Freigeben des AD-Wandlers
	ADCSRA |= (1<<ADEN);

	// Dummy-Wandlungen ausführen und verwerfen
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC));
	result = ADC;
}

unsigned int ADC_convert(unsigned char port) {
	unsigned int conversion;
	
	// zurücksetzen des Multiplexers
	ADMUX = 0x00;
	
	// Auswahl des jeweiligen AD-Wandler Anschlusses
	switch (port)
	{
		// Zur besseren Übersicht können die Hexadezimalzahlen auch in binäre Werte umgerechnet werden
		
		case 0x01: {
			
			// 0x01 = 0b00000001
			ADMUX = 0x01;	
			}
		break;

		case 0x02: {
			ADMUX = 0x02;
			}
		break;

		case 0x03: {
			ADMUX = 0x03;
			}
		break;

		case 0x04: {
			ADMUX = 0x04;
			}
		break;

		case 0x05: {
			ADMUX = 0x05;
			}
		break;

		case 0x07: {
			ADMUX = 0x07;
			}
		break;

	}
	
	// Setzen der Referenzspannung
	// Bei einer Referenzspannungsänderung muss auch hier der Wert angepasst werden
	ADMUX |= (1<<REFS0);
	
	// Wandlung ausführen
	ADCSRA |= (1<<ADSC);
	// warten, bis die Wandlung beendet ist
	while (ADCSRA & (1<<ADSC));
	
	// Auslesen des gewandelten Wertes und speichern in "conversation"
	conversion = ADC;
	return conversion;
}
