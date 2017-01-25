#include <RBELib/RBELib.h>
//For use of abs()
#include <stdlib.h>
#include <avr/interrupt.h>


#define POT_CHANNEL 4
#define FREQ1	7813
#define FREQ2	391
#define FREQ3	78

unsigned char mode, tog;
volatile int count0 = 0;

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
	int state = 1;
	int tHigh = 0;
	int tLow = 0;
	unsigned char activ;
	int adcVal;


	initADC(6);

	setPinsDir('C', OUTPUT, 4, 0, 1, 2, 3);
	setPinsDir('D', INPUT, 3, 0, 1, 2);

	setPinsVal('C', LOW, 4, 0, 1, 2, 3);


	while(1){
		activ = getPinsVal('D', 3, 0, 1, 2);
		adcVal = (int) getADC(6);

		if ((activ & 1)){
			tHigh = adcVal;
			tLow = 1023-tHigh;
		} else if ((activ & 2) >> 1) {
			tHigh = (FREQ2*adcVal)/1023;
			tLow = FREQ2-tHigh;
		} else {
			tHigh = (FREQ3*adcVal)/1023;
			tLow = FREQ3-tHigh;
		}

		if(state){
			setPinsVal('C', LOW, 4, 0, 1, 2, 3);
			if (count0 >= tHigh){
				state = 0;
				count0 = 0;
			}
		} else {
			setPinsVal('C', HIGH, 4, 0, 1, 2, 3);
			if (count0 >= tLow) {
				state = 1;
				count0 = 0;
			}
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
