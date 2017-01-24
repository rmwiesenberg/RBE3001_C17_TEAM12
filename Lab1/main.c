#include "RBELib/RBELib.h"
//For use of abs()
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define POT_CHANNEL 4
#define FREQ1	1
#define FREQ2	20
#define FREQ3	100

unsigned char mode, tog;
int count0;

void potRead() {

	DDRB = 0xFF; //Set Port as output
	initADC(POT_CHANNEL);		// init ADC on port 4
	while (1) {
		printf("Input a command character: \n\r");
		char cmd = getCharDebug();			// polls for input, locks up program
		if (cmd == 'S') {

			for (int i = 0; i < 250; i++) {
				unsigned int adcVal = getADC(POT_CHANNEL);// implemented in adc.c

				int potOut_mVolts = (int) (adcVal * (5000. / 1023.));
				int potAngle = (int) (adcVal * (300. / 1023.));

				printf("%d,%d,%d,%d\n\r", i, adcVal, potOut_mVolts, potAngle);

			}
			printf("DONE \n\r");

		} else {
		}
	}
}

void part2(){
	initTimer(0, CTC, 1);
	int freq = FREQ1;
	int curclk = fclk_IO/prescale;
	int state = 1;

	setPinsDir('A',1,0,1,2,3,4,5,6,7);
	setPinsDir('B',0,0,1,2,3,4,5,6,7);

	while(1){
		int countTo = curclk/freq;
		if (count0 >= countTo){
			state =! state;
			setPinsVal('A',state,0,1,2,3,4,5,6,7);
			count0 = 0;
		}
	}
}

ISR(TIMER0_COMPA_vect) {
	count0++;
}


int main(void) {
	initRBELib();
	debugUSARTInit(115200);
//	potRead();
	part2();


	return 0;
}
