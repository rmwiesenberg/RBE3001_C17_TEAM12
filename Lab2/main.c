#include <RBELib/RBELib.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "RBELib/vals.h"
#include <Math.h>


#define TIMER_CLK		 	18432000. / 8.				// timer uses clk frequency 18.432 MHz / 8 = 2.304 MHz = count frequency
#define COUNT_FREQUENCY		TIMER_CLK / 256.			// count overflow interrupt fires 2.304 MHz kHz / 256 = 9000 times a second
#define TICK				COUNT_FREQUENCY / 8192.		// 4096 ticks in 1 second = 4096 in 9000 counts = 2.197265 counts / tick

#define HIGH_POT 3
#define LOW_POT 2
#define HIGH_CUR 1
#define LOW_CUR 0
#define KC 750
#define DEBUG_EN 1

unsigned long FREQ1 = 9024;

unsigned char mode, tog;
volatile int count0 = 0;


void potRead() {

	DDRB = 0xFF; //Set Port as output
	initADC(LOW_POT);		// init ADC on port 4
	while (1) {
		printf("Input a command character: \n\r");
		char cmd = getCharDebug();			// polls for input, locks up program
		if (cmd == 'S') {

			for (int i = 0; i < 250; i++) {
				unsigned int adcVal = getADC(LOW_POT);// implemented in adc.c

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

void calcTipPos(int ang1, int ang2){
	int xTip = 0;
		xTip = 250 * cos(ang1) - 220 * cos(ang1 + ang2);
	int yTip = 0;
		yTip = 250 * sin(ang1) + 220 * sin(ang1 + ang2) + 190;

	printf("x: %d y: %d \n\r", xTip, yTip);
}

void movePID(int ang1, int ang2) {
	initTimer(1,CTC,64);  //start 100.1603Hz timer

	initADC(LOW_POT);
	initADC(HIGH_POT);		// init ADC on port 3
	initADC(0);		// init ADC on port 0 - currSense



	setConst('H', 45, 0, 2); //set the PID constants for high link
	setConst('L', 45, 0, 2); //set the PID constants for low link

	int pid_h = 0; //PID output to motor
	int pid_l = 0;
	int i = 0;
	int aValH = getADC(HIGH_POT);
	int aValL = getADC(LOW_POT);
	int adcCH = getADC(HIGH_CUR);
	int adcCL = getADC(LOW_CUR);

	PotVal potValH, potValL;
	CurVal curValH, curValL;
	setPotVal(&potValH, 'H', aValH);
	setPotVal(&potValL, 'L', aValL);

	DDRB = 0xFF; //Set Port as output


	initSPI();



	while(1) {

		if (timerFlag) {
			timerFlag = 0;
//			float currSense = ((getADC(0) * (5000/1023))-2.5);
			adcCH = getADC(1);
			adcCL = getADC(1);
			setCurVal(curValH, adcCH);
			setCurVal(curValL, adcCH);
			//printf("currSense: %f \n\r", currSense);
			aValH = getADC(HIGH_POT);
			aValL = getADC(LOW_POT);
			setPotVal(&potValH, 'H', aValH);
			setPotVal(&potValL, 'L', aValL);

			unsigned char activ = getPinsVal('D', 3, 0, 1, 2);
			if ((activ & 1)) {
				pid_h = calcPID('H', 0, potValH.angle);
				driveLink(1, pid_h);
				pid_l = calcPID('L', 0, potValL.angle);
				driveLink(0, pid_l);
			}
			else if ((activ & 2) >> 1) {
				pid_h = calcPID('H', 45, potValH.angle);
				driveLink(1, pid_h);
				pid_l = calcPID('L', 45, potValL.angle);
				driveLink(0, pid_l);
			}
			else{
				pid_h = calcPID('H', 90, potValH.angle);
				driveLink(1, pid_h);
				pid_l = calcPID('L', 90, potValL.angle);
				driveLink(0, pid_l);
			}

			//calcTipPos(getLinkAngle('L'), getLinkAngle('H'));

			if (DEBUG_EN) {
//				printPotVal(potVal);
//				printPotVal(potValL);
				printf("count: %d pid_h: %d pid_l: %d currentH: %d currentL: %d \n\r", i, pid_h, pid_l, curValH.mAmp, curValL.mAmp);
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
	initADC(0);
	driveLink(0, 0);
	while(1){
		printf("%f \n\r", (((getADC(0) * 7.2)/1023)-1.6)/(20 * 0.05));
	}

	//potRead();

	//triangleDAC();

	initSPI();
//	while(1){
//		unsigned char activ = getPinsVal('D', 3, 0, 1, 2);
//		if ((activ & 1)) {
//			driveLink(1, 0);
//		}
//		else if ((activ & 2) >> 1) {
//			driveLink(1, -2000);
//		}
//		else{
//			driveLink(1, 2000);
//
//		}
//	}

//	DDRB |= (1<<PB2);
//	while(1) {
//		PORTB &= ~(1<<PB2);
//	}
	return 0;
}
