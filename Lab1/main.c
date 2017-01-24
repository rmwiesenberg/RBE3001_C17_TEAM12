#include <RBELib/RBELib.h>
//For use of abs()
#include <stdlib.h>
#include <avr/io.h>

#define POT_CHANNEL 4

void blinkDebug() {
	DDRB = 0xFF;
	PORTB = 0xFF;
	_delay_ms(1000);
	PORTB = 0x00;
	_delay_ms(1000);
	DDRB = 0x00;
}


void printData(int counter, unsigned int adcVal) {
	// data format: "[counter],[adcVal],[potOut_mVolts],[potAngle]\n"
	int potOut_mVolts = (int) (adcVal * (5000. / 1023.));
	int potAngle = (int) (adcVal * (300. / 1023.));

	printf("%d,%d,%d,%d\n\r", counter, adcVal, potOut_mVolts, potAngle);
}

void setupPotRead() {
	initRBELib(); 				// rebind stdout to USART
	debugUSARTInit(115200); 	// enable serial communication at 115200 bps

	initADC(POT_CHANNEL);		// init ADC on port 4

}

void loopPotRead() {
	printf("Input a command character: \n\r");
	char cmd = getCharDebug();				// polls for input, locks up program
	if (cmd == 'S') {
		blinkDebug();		// blink to indicate action
		for (int i = 0; i < 250; i++) {
			unsigned int adcVal = getADC(POT_CHANNEL);	// implemented in adc.c
			printData(i, adcVal);
			_delay_ms(1000);		// for human readability, remove for burst mode
		}
		printf("DONE \n\r");
		blinkDebug();
	} else {}
}

int main(void) {
	initRBELib();

	//Set the baud rate of the UART
	debugUSARTInit(115200);

	DDRB = 0xFF; //Set Port as output

	setupPotRead();
	while(1) loopPotRead();

	return 0;
}
