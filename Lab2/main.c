#include <RBELib/RBELib.h>
//For use of abs()
#include <stdlib.h>
#include <avr/interrupt.h>
#include <include/Lib.h>

#define TIMER_CLK		 	18432000. / 8.				// timer uses clk frequency 18.432 MHz / 8 = 2.304 MHz = count frequency
#define COUNT_FREQUENCY		TIMER_CLK / 256.			// count overflow interrupt fires 2.304 MHz kHz / 256 = 9000 times a second
#define TICK				COUNT_FREQUENCY / 4096.		// 4096 ticks in 1 second = 4096 in 9000 counts = 2.197265 counts / tick

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

volatile int timer = 0;				// timer counter

void testDAC() {
	initRBELib();
	debugUSARTInit(115200);
	initSPI();

	initTimer(0, CTC, 0);


	int dacWrite = 0;

	while(1) {
		if(dacWrite > 4095) {
			dacWrite = 0;
		}
		if(timer > TICK) {
			timer = 0;
			dacWrite++;
		}

		setDAC(0, 4095 - dacWrite);
		setDAC(1, dacWrite);
	}
}

ISR(TIMER0_OVF_vect) {		// timer ISR, usable in all file functions
	timer++;
}

int main(void) {
	initRBELib();
	debugUSARTInit(115200);
	//potRead();
	testDAC();

	return 0;
}
