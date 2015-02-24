#ifndef SERVO_CONTROL_H_
#define SERVO_CONTROL_H_

#include "Servoansteuerung.h"

// definieren der Servos
#define Servo1 1
#define Servo2 2
#define Servo3 3
#define Servo4 4
#define Servo5 5
#define Servo6 6
#define Servo7 7
#define Servo8 8

// ### Initialisieren der Servos auf Mittelstellung und des Timers
void servos_init();

// ### Funktion zum setzen der Servopositionen
void servo_setpos(unsigned char servo, unsigned char pos);

#endif
