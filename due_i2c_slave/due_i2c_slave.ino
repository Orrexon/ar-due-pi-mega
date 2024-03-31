#include <Arduino.h>
#include <Wire.h>


#define STATE_BLINK 1
#define STATE_ATMEGA328P 2

#define atmega328p 53

volatile int state = 0;
volatile byte RxByte;
volatile byte TxByte;
volatile byte new_order = 0;
 
void I2C_RxHandler(int numBytes)
{
	while(Wire.available()) {  // Read Any Received Data
		RxByte = Wire.read();
  	}
	new_order = 1;
}

void I2C_TxHandler(void)
{
	Wire.write(TxByte);
}

void transition_state(int old_state){
	switch(old_state){
		case STATE_BLINK:
		{
			digitalWrite(13, LOW);
		}
		case STATE_ATMEGA328P:
		{
			digitalWrite(atmega328p, LOW);
		}
	}
	TxByte = 0xFF;
}

void start_builtin_blink(){
	pinMode(13, OUTPUT);
	state = STATE_BLINK;
	TxByte = 0x01;
}

void start_atmega328p(){
	pinMode(atmega328p, OUTPUT);
	state = STATE_ATMEGA328P;
	TxByte = 0x02;
}

void setup() {

	Wire.begin(0x55); // Initialize I2C (Slave Mode: address=0x55 )
	Wire.onReceive(I2C_RxHandler);
	Wire.onRequest(I2C_TxHandler);
}


void loop() {
	if(new_order){
		switch(RxByte){
			case 0b00000001:
				{
					transition_state(state);
					start_builtin_blink();
				}break;
			case 0x02:
				{
					transition_state(state);
					start_atmega328p();
				}break;
			case 'q':
				{
					transition_state(state);
					state = 0;
				}
			default:break;
		}
		new_order = 0;
	}
	switch(state){
		case STATE_BLINK:
			{
				delay(1000);
				digitalWrite(13, HIGH);
				delay(1000);
				digitalWrite(13, LOW);
			}break;
		case STATE_ATMEGA328P:
			{
				if(digitalRead(atmega328p) == LOW){
					digitalWrite(atmega328p, HIGH);
				}
			}break;
	}
}
