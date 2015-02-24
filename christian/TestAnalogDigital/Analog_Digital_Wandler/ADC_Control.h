#ifndef ADC_CONTROL_H_
#define ADC_CONTROL_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#define ADC0 0x00
#define ADC1 0x01
#define ADC2 0x02
#define ADC3 0x03
#define ADC4 0x04
#define ADC5 0x05
#define AKKU 0x07

void initADC();

unsigned int ADC_convert(unsigned char port);

#endif
