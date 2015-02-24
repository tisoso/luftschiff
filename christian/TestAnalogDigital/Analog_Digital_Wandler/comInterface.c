/*
 * comInterface.c
 *
 * All comInterface functions are there to ensure a save handling of stream communication.
 * Further information can be read at the functions themselves.
 *
 * Author: Julian Engelskirchen
 */ 

#include "comInterface.h"

//this array contains every byte of the stream as well as the buffer of the stream
unsigned char stream[MAX_COMMANDS];
//saves the amount of bytes in the stream, ready to be read
uint8_t streamLength = 0;
//saves the amount of bytes in the stream buffer
uint8_t streamBuf = 0;
//counts the time after the last successful communication
uint8_t timeout = 0;

void initUSART(void) {
	//Set baud rate
	UBRR0H = (unsigned char)(MYUBRR >> 8);
	UBRR0L = (unsigned char)MYUBRR;
	
	//Enable receiver and transmitter as well as the receiver interrupt
	UCSR0B = (1 << RXCIE0)|(1 << RXEN0)|(1 << TXEN0);
	
	// Set frame format to 8data and 1stop bit
	UCSR0C = (1 << UCSZ00)|(1 << UCSZ01);
	sei();
}

//"private" function, checks the checksum
void checkStream() {
	//initialized checksum byte
	uint8_t checksum = 0x00;
	
	//copy length of stream buffer
	uint8_t streamBufSize = streamBuf;
	
	//ignore checksum byte for checksum building
	streamBuf--;
	
	//iterate through all new bytes
	for(; streamBuf > 0; streamBuf--) {
		
		//XOR every new byte together
		checksum ^= stream[streamLength + streamBuf - 1];
	}
	
	//check whether calculated checksum equals received checksum
	if(checksum == stream[streamLength + streamBufSize - 1]) {
		
		//add new bytes to rest of stream without checksum byte
		streamLength += streamBufSize - 1;
		
		//reset timeout
		timeout = 0;
	}
}

//This timer interrupt handles each byte received. It is triggered when there is a new byte
//available in the UDR register. The interrupt is enabled by setting the byte RXCIEn in
//register UCSRnB to 1.
ISR(USART_RX_vect) {
	//read the receive register into a buffer
	uint8_t buf = UDR0;
	
	//check whether stream reached it's end
	if(buf == CMD_END) {
		
		//execute a checksum check
		checkStream();
		
	//check for overflow of buffer array
	} else if(streamLength + streamBuf < MAX_COMMANDS) {
		
		//add new byte to stream buffer
		stream[streamLength] = buf;
		
		//increase size index of stream buffer
		streamBuf++;
	}
}

//This function checks for a timeout and must be called in equally spaced time intervals.
//The timeout variable used to check for a timeout is increased at each call of this function
//and is only reseted after a successful receive. It returns true if the timeout defined in
//the header has been reached.
bool isTimeout(void) {
	
	//increase timeout 
	timeout++;
	
	//check whether the timeout variable is greater than the timeout set
	return (timeout > TIME_TIMEOUT);
}

//This function sends a stream of bytes. A pointer to the first array element as well as the
//the amount of bytes to be send must be supplied as parameter.
//THIS IS A BLOCKING FUNCTION and can take up to a few ms to finish!
void sendStream(unsigned char* stream, uint8_t size) {
	
	//iterate through every byte to send
	for(;size > 0; size--) {
		
		//call sending function on each byte
		sendByte(*stream++);
	}
}

//This function sends a single byte. What's there more to explain?
//THIS IS A BLOCKING FUNCTION and can take up to a few ms to finish!
void sendByte(unsigned char byte) {
	//wait until register is ready to accept next byte
	while ((UCSR0A & (1 << UDRE0)) == 0) {};
		
	//write byte into transmit register
	UDR0 = byte;
}

//This function checks whether there is new information available. The function returns true
//if there is at least one byte available.
bool isDataReady(void) {
	
	//data is ready when stream size is greater zero
	return (streamLength > 0);
}

//This function can be used to get a copy of the received stream. The function will flush
//the original stream, so a new call of this function should only be used after work on every
//old byte has successful ended. The function needs a pointer to an array it can copy the
//stream into. Make sure this array is at least MAX_COMMANDS bytes long. The amount of
//bytes copied into the array will be returned by the function.
uint8_t getReceivedStream(unsigned char* streamOut) {
	
	//disable Interrupts to make sure no new data gets written into the stream
	cli();
	
	//save size of stream for later
	uint8_t temp = streamLength;
	
	//iterate through every array field which has information in it
	for(; streamLength > 0; streamLength--) {
		
		//copy received command array into given array
		streamOut[streamLength - 1] = stream[streamLength - 1];
	} 
	
	//iterate through stream buffer
	for(uint8_t i = 0; i < streamBuf; i++) {
		
		//byte from the stream buffer into the front of the array
		stream[i] = stream[temp + i];
	}
	
	//switch interrupts back on
	//reti();
	sei();
	
	//return size of arguments written into the output stream
	return temp;
}
