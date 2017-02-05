#include <RBELib/RBELib.h>
//For use of abs()
#include <stdlib.h>
#include <avr/interrupt.h>
#include <RBELib/vals.h>


#define TIMER_CLK		 	18432000. / 8.				// timer uses clk frequency 18.432 MHz / 8 = 2.304 MHz = count frequency
#define COUNT_FREQUENCY		TIMER_CLK / 256.			// count overflow interrupt fires 2.304 MHz kHz / 256 = 9000 times a second
#define TICK				COUNT_FREQUENCY / 8192.		// 4096 ticks in 1 second = 4096 in 9000 counts = 2.197265 counts / tick

#define POT_CHANNEL 2
#define KC 750
#define DEBUG_EN 1

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
				int potAngle = (int) ((adcVal * (265. / 1023.))-83);

				printf("%d,%d,%d,%d\n\r", i, adcVal, potOut_mVolts, potAngle);
				_delay_ms(1000);
			}
			printf("DONE \n\r");

		} else {
		}
	}
}

volatile BYTE timerFlag = 0;

void movePID(int ang1, int ang2) {
	initTimer(1,CTC,64);  //start 100.1603Hz timer
	setConst('H', 45, 0, 5); //set the PID constants for high link
	int pid_h = 0; //PID output to motor
	int i = 0;
	int aVal = getADC(2);
	PotVal potVal;
	setPotVal(&potVal, 'H', aVal);

	DDRB = 0xFF; //Set Port as output
	initADC(POT_CHANNEL);		// init ADC on port 4
	initSPI();
	driveLink(1, 0);

	while(1) {
		if (timerFlag) {
			timerFlag = 0;

			aVal = getADC(POT_CHANNEL);
			setPotVal(&potVal, 'H', aVal);

			pid_h = calcPID('H', 90, potVal.angle);
			driveLink(1, pid_h);

			if (DEBUG_EN) {
				printf("count: %d pid_h: %d ", i, pid_h);
				printPotVal(potVal);
			}
			i++;
		}
	}
}

//volatile int timer = 0; // timer counter

void triangleDAC() {
	initSPI();


	//initTimer(0,NORMAL,0);


	int dacWrite = 0;
	int pastDAC = 0;
	int mode = 1;
	setDAC(0, 4095);
	while(1) {
		_delay_ms(100);

		//if reach 0, start counting up
		if (dacWrite <= 0) {
			mode = 100;
		}

		//if reach max, start counting down
		if(dacWrite >= 4095) {
			mode = -100;
			dacWrite = 4095;
		}

		//
		dacWrite += mode;

		if (dacWrite != pastDAC) {
			pastDAC = dacWrite;
			setDAC(0, (4095 - dacWrite));
			setDAC(1, dacWrite);
			printf("%i %i\n\r", dacWrite, (4095-dacWrite));
		}
	}
}

ISR(TIMER1_COMPA_vect) {		// timer ISR, usable in all file functions
	timerFlag = 1;
}

int main(void) {

	initRBELib();
	debugUSARTInit(115200);
	movePID(0,0); //moves the arm to two angles
	potRead();

	//potRead();

	//triangleDAC();

	initSPI();
	while(1){
		unsigned char activ = getPinsVal('D', 3, 0, 1, 2);
		if ((activ & 1)) {
			driveLink(1, 0);
		}
		else if ((activ & 2) >> 1) {
			driveLink(1, -2000);
		}
		else{
			driveLink(1, 2000);

		}
	}

//	DDRB |= (1<<PB2);
//	while(1) {
//		PORTB &= ~(1<<PB2);
//	}
	return 0;
}
