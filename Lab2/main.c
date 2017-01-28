#include <RBELib/RBELib.h>
//For use of abs()
#include <stdlib.h>
#include <avr/interrupt.h>

#define POT_CHANNEL 2

unsigned long FREQ1 = 9024;

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
				int potAngle = (int) ((adcVal * (250. / 1023.))-83);

				printf("%d,%d,%d,%d\n\r", i, adcVal, potOut_mVolts, potAngle);
				_delay_ms(1000);
			}
			printf("DONE \n\r");

		} else {
		}
	}
}

void part2() {
	initTimer(0, CTC, 1);
	int state = 1;
	unsigned long tHigh = 0;
	unsigned long tLow = 0;
	unsigned char activ;
	unsigned long adcVal;

	unsigned long duty;
	unsigned long freq;

	unsigned long FREQ2 = FREQ1 / 20;
	unsigned long FREQ3 = FREQ1 / 100;

	initADC(4);

	setPinsDir('C', OUTPUT, 4, 0, 1, 2, 3);
	setPinsDir('D', INPUT, 3, 0, 1, 2);

	setPinsVal('C', LOW, 4, 0, 1, 2, 3);

	while (1) {
		activ = getPinsVal('D', 3, 0, 1, 2);
		adcVal = getADC(4);
		duty = 100 * adcVal / 1023;

		if ((activ & 1)) {
			freq = 1;
			tHigh = (FREQ1 * adcVal) / 1023;
			tLow = FREQ1 - tHigh;
			printf("%lu,%lu,%d,%lu\n\r", duty, freq, state, adcVal);

		} else if ((activ & 2) >> 1) {
			freq = 20;
			tHigh = (FREQ2 * adcVal) / 1023;
			tLow = FREQ2 - tHigh;
			printf("%lu,%lu,%d,%lu\n\r", duty, freq, state, adcVal);

		} else {
			freq = 100;
			tHigh = (FREQ3 * adcVal) / 1023;
			tLow = FREQ3 - tHigh;
			printf("%lu,%lu,%d,%lu\n\r", duty, freq, state, adcVal);

		}

		if (state) {
			setPinsVal('C', LOW, 4, 0, 1, 2, 3);
			if (count0 >= tHigh) {
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

void part3() {
	initTimer(0, CTC, 1);
	initADC(4);
	setPinsDir('C', OUTPUT, 4, 0, 1, 2, 3);

	unsigned long adcVal;
	unsigned long FREQ2 = FREQ1 / 225;
	int i = 0;

	printf("Input a command character: \n\r");
	char cmd = getCharDebug();			// polls for input, locks up program

	while (i <= 225) {
		if (count0 >= FREQ2) {
			setPinsVal('C', LOW, 4, 0, 1, 2, 3);
			adcVal = getADC(4);
			printf("%d,%lu\n\r", i, adcVal);
			i++;
			count0 = 0;
		}
		setPinsVal('C', HIGH, 4, 0, 1, 2, 3);
	}
}

ISR(TIMER0_COMPA_vect) {
	count0++;
}

int main(void) {
	initRBELib();
	debugUSARTInit(115200);
	potRead();
//	part2();
//	part3();

	return 0;
}
