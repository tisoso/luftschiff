#include <stdio.h>



unsigned int two8BitTo16( char obere8bit, char untere8bit){
	// diese Funktion wurde zum empfangen fon den sensorwerten gedacht
	unsigned int gesamtZahl =0;
	
	gesamtZahl = obere8bit; 
	gesamtZahl = gesamtZahl<<8; // verschiebt die obereren 8 bit nach oben 
	gesamtzahl |= untere8bit; // fügt die unteren acht bit hinzu 
	
	return gesamtZahl;
}

