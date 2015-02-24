#include "Servoansteuerung.h"
#include "Servo_Control.h"
#include "avr/io.h"

// Timer Vorteiler
#define VORTEILER 64

// Wert für 1ms
#define MILLISEC_BASE (F_CPU / VORTEILER / 1000)

// Wert für die Mittelstellung der Servos
#define MITTE (MILLISEC_BASE / 2)

// Wert für maximale Periode => 20ms
#define MAXPERIOD ((MILLISEC_BASE * 20)-2000)

// Anzahl der Servos
#define NR_SERVOS 9	

// Anschlussport der Servos
#define SERVO_PORT PORTB
#define SERVO_PORT2 PORTD

// Richtungsregister für die entsprechenden Ports der Servos
#define SERVO_DDR DDRB
#define SERVO_DDR2 DDRD

// Array für die Servoanschlüsse
unsigned char ServoPuls[NR_SERVOS] = {0,1<<PB3, 1<<PB2, 1<<PB1, 1<<PB0, 1<<PD7, 1<<PD6, 1<<PD5, 1<<PD3};

// Array für die Servopositionen
volatile unsigned char ServoValue[NR_SERVOS];

// ### Interrupt Service Routine, Compare Match
ISR(TIMER1_COMPA_vect)
{
	static unsigned char ServoId = 0;
	static unsigned int pause = MAXPERIOD;
	
	// Abschalten des vorherigen Servoimpulses
	// --> PIN zuruesetzen auf LOW
	if(ServoId >= 5) SERVO_PORT2 &=~ ServoPuls[ServoId];
	else SERVO_PORT &=~ServoPuls[ServoId];

	if(++ServoId >= NR_SERVOS) {
		
		// Setzen der restlichen Pausenzeit
		OCR1A = pause;

		// Zurücksetzen der ServoId
		ServoId = 0;

		// Zurücksetzen der maximalen Periode von 20ms
		pause = MAXPERIOD;
		}

	else {

		// Starten des Servoimpulses
		// --> PIN auf HIGH setzen
		if(ServoId >= 5) SERVO_PORT2 |= ServoPuls[ServoId];
		else SERVO_PORT |= ServoPuls[ServoId];
		
		// Impulslaenge setzen, damit der naechste Timer Compare Match entsprechend sich ereignet
		OCR1A = MILLISEC_BASE + ServoValue[ServoId];
		
		// Abziehen der Bereits verwendeten Zeit von der maximalen Periode
		pause -= ServoValue[ServoId];
		
	}
}
// ### Initialisierung Timer1
void timer1_init(void) 
{
	OCR1A = MILLISEC_BASE + ServoValue[0];
	// Timer1 initialisierung im CTC-Modus
	TCCR1B |= (1<<WGM12);
	
	TCCR1B |= (1<<CS11) | (1<<CS10);
	TIMSK1 |= (1<<OCIE1A);

	sei();
}

// ### Initialisierung Servos
void servos_init(void) {
	
	
	unsigned char i;
	SERVO_DDR |= ServoPuls[1] | ServoPuls[2] | ServoPuls[3] | ServoPuls[4];
	SERVO_DDR2 |= ServoPuls[5] | ServoPuls[6] | ServoPuls[7] | ServoPuls[8];
	// alle Servos auf Mittelstellung initialisieren
	for(i = 0; i < NR_SERVOS; i++) {
		ServoValue[i] = MITTE;
	}
	timer1_init();
	
}


// ### Funktion zum setzen der Servoposition
void servo_setpos(unsigned char servo, unsigned char pos){
	ServoValue[servo] = pos;
}
