#include <RBELib/RBELib.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <RBELib/vals.h>

#define TIMER_CLK		 	18432000. / 8.				// timer uses clk frequency 18.432 MHz / 8 = 2.304 MHz = count frequency
#define COUNT_FREQUENCY		TIMER_CLK / 256.			// count overflow interrupt fires 2.304 MHz kHz / 256 = 9000 times a second
#define TICK				COUNT_FREQUENCY / 8192.		// 4096 ticks in 1 second = 4096 in 9000 counts = 2.197265 counts / tick

#define HIGH_POT 3
#define LOW_POT 2
#define HIGH_CUR 1
#define LOW_CUR 0
#define KC 750
#define DEBUG_EN 1
#define TOL 3

unsigned long FREQ1 = 9024;

unsigned char mode, tog;
volatile int count0 = 0;

int pointArrayH[] = {-9,17,-33};
int pointArrayL[] = {69,42,55};



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
				_delay_ms(100);
			}
			printf("DONE \n\r");

		} else {
		}
	}
}

volatile BYTE timerFlag = 0;


void movePID() {
	initTimer(1,CTC,64);  //start 100.1603Hz timer

	initADC(LOW_POT);
	initADC(HIGH_POT);		// init ADC on port 3
	initADC(0);		// init ADC on port 0 - currSense

	setConst('H', 45, 8, 2); //set the PID constants for high link
	setConst('L', 45, 8, 2); //set the PID constants for low link

	int pid_h = 0; //PID output to motor
	int pid_l = 0;
	unsigned int i = 0;
	int aValH = getADC(HIGH_POT);
	int aValL = getADC(LOW_POT);
	int adcCH = getADC(HIGH_CUR);
	int adcCL = getADC(LOW_CUR);

	struct Motor motorH, motorL;
	setPotVal(&motorH, 'H', aValH);
	setPotVal(&motorL, 'L', aValL);

	DDRB = 0xFF; //Set Port as output


	initSPI();



	while(1) {

		if (timerFlag) {
			timerFlag = 0;
//			float currSense = ((getADC(0) * (5000/1023))-2.5);
			adcCH = getADC(HIGH_CUR);
			adcCL = getADC(LOW_CUR);
			setCurVal(&motorH, adcCH);
			setCurVal(&motorL, adcCL);

			//printf("currSense: %f \n\r", currSense);
			aValH = getADC(HIGH_POT);
			aValL = getADC(LOW_POT);
			setPotVal(&motorH, 'H', aValH);
			setPotVal(&motorL, 'L', aValL);

			unsigned char activ = getPinsVal('D', 3, 2, 1, 0);
			if (activ == 0) {
				pid_h = calcPID('H', 90, motorH.angle);
				driveLink(1, pid_h);
				pid_l = calcPID('L', 0, motorL.angle);
				driveLink(0, pid_l);

			} else if (activ == 1) {
				pid_h = calcPID('H', 90, motorH.angle);
				driveLink(1, pid_h);
				pid_l = calcPID('L', 30, motorL.angle);
				driveLink(0, pid_l);

			} else if (activ == 2) {
				pid_h = calcPID('H', 90, motorH.angle);
				driveLink(1, pid_h);
				pid_l = calcPID('L', 60, motorL.angle);
				driveLink(0, pid_l);

			} else {
				pid_h = calcPID('H', 90, motorH.angle);
				driveLink(1, pid_h);
				pid_l = calcPID('L', 90, motorL.angle);
				driveLink(0, pid_l);
			}

			setMotorVolt(&motorL, pid_l);
			setMotorVolt(&motorH, pid_h);

			//calcTipPos(getLinkAngle('L'), getLinkAngle('H'));

			if (DEBUG_EN) {
//				if (i > 19) {
//					i = 0;
	//				printPotVal(potVal);
	//				printPotVal(motorL);
					printf("%d,%d,%d,%d\n\r", i, motorL.angle, motorL.dacVolt, (motorL.mAmp-512)/(.05*20));
//				}
			}
			i++;
		}
	}
}

void lab2BP4() {
	initTimer(1,CTC,64);  //start 100.1603Hz timer

	initADC(LOW_POT);
	initADC(HIGH_POT);		// init ADC on port 3
	initADC(0);		// init ADC on port 0 - currSense

	setConst('H', 45, 8, 2); //set the PID constants for high link
	setConst('L', 45, 8, 2); //set the PID constants for low link

	int pid_h = 0; //PID output to motor
	int pid_l = 0;
	unsigned int i = 0;
	int aValH = getADC(HIGH_POT);
	int aValL = getADC(LOW_POT);
	int adcCH = getADC(HIGH_CUR);
	int adcCL = getADC(LOW_CUR);

	struct Motor motorH, motorL;
	setPotVal(&motorH, 'H', aValH);
	setPotVal(&motorL, 'L', aValL);

	DDRB = 0xFF; //Set Port as output


	initSPI();



	while(1) {

		if (timerFlag) {
			timerFlag = 0;
//			float currSense = ((getADC(0) * (5000/1023))-2.5);
			adcCH = getADC(HIGH_CUR);
			adcCL = getADC(LOW_CUR);
			setCurVal(&motorH, adcCH);
			setCurVal(&motorL, adcCL);

			//printf("currSense: %f \n\r", currSense);
			aValH = getADC(HIGH_POT);
			aValL = getADC(LOW_POT);
			setPotVal(&motorH, 'H', aValH);
			setPotVal(&motorL, 'L', aValL);

			unsigned char activ = getPinsVal('D', 3, 2, 1, 0);
			if (activ == 0) {
				pid_h = calcPID('H', 90, motorH.angle);
				driveLink(1, pid_h);
				pid_l = calcPID('L', 0, motorL.angle);
				driveLink(0, pid_l);

			} else if (activ == 1) {
				pid_h = calcPID('H', 0, motorH.angle);
				driveLink(1, pid_h);
				pid_l = calcPID('L', 45, motorL.angle);
				driveLink(0, pid_l);

			} else if (activ == 2) {
				pid_h = calcPID('H', 45, motorH.angle);
				driveLink(1, pid_h);
				pid_l = calcPID('L', 45, motorL.angle);
				driveLink(0, pid_l);

			} else {
				pid_h = calcPID('H', 90, motorH.angle);
				driveLink(1, pid_h);
				pid_l = calcPID('L', 90, motorL.angle);
				driveLink(0, pid_l);
			}

			setMotorVolt(&motorL, pid_l);
			setMotorVolt(&motorH, pid_h);

			//calcTipPos(getLinkAngle('L'), getLinkAngle('H'));

			if (DEBUG_EN) {
				if (i > 19) {
				calcTipPos(motorL, motorH);
					i = 0;
	//				printPotVal(potVal);
	//				printPotVal(motorL);
//					printf("%d,%d,%d,%d\n\r", i, motorL.angle, motorL.dacVolt, motorL.mAmp);
				}
			}
			i++;
		}
	}
}

void lab2BP5() {
	initTimer(1,CTC,64);  //start 100.1603Hz timer

	initADC(LOW_POT);
	initADC(HIGH_POT);		// init ADC on port 3
	initADC(0);		// init ADC on port 0 - currSense

	setConst('H', 45, 8, 2); //set the PID constants for high link
	setConst('L', 45, 8, 2); //set the PID constants for low link

	int pid_h = 0; //PID output to motor
	int pid_l = 0;
	BYTE i = 0;
	unsigned int timer = 0;

	int aValH = getADC(HIGH_POT);
	int aValL = getADC(LOW_POT);
	int adcCH = getADC(HIGH_CUR);
	int adcCL = getADC(LOW_CUR);


	struct Motor motorH, motorL;
	setPotVal(&motorH, 'H', aValH);
	setPotVal(&motorL, 'L', aValL);

	setPinsDir('B', OUTPUT, 8, 1, 2, 3, 4, 5, 6, 7); //Set Port as output
//	setPinsDir('C', OUTPUT, 1, 1);


	initSPI();

	while(1) {
		for (i = 0; i < (sizeof(pointArrayH)/sizeof(pointArrayH[0])); i++) {
			while( (abs(motorH.angle - pointArrayH[i]) > TOL) ||
					(abs(motorL.angle - pointArrayL[i]) > TOL )) {

				if (timerFlag) {
//					if (timer > 19) {
//						timer = 0;
//						if (getPinsVal('C', 1, 1) & 1) {
//							setPinsVal('C', 1, 1, 1);
//
//						} else {
//							setPinsVal('C', 0, 1, 1);
//						}
//					}

					timerFlag = 0;
		//			float currSense = ((getADC(0) * (5000/1023))-2.5);
					adcCH = getADC(HIGH_CUR);
					adcCL = getADC(LOW_CUR);
					setCurVal(&motorH, adcCH);
					setCurVal(&motorL, adcCL);

					//printf("currSense: %f \n\r", currSense);
					aValH = getADC(HIGH_POT);
					aValL = getADC(LOW_POT);
					setPotVal(&motorH, 'H', aValH);
					setPotVal(&motorL, 'L', aValL);


					pid_h = calcPID('H', pointArrayH[i], motorH.angle);
					driveLink(1, pid_h);
					pid_l = calcPID('L', pointArrayL[i], motorL.angle);
					driveLink(0, pid_l);


					setMotorVolt(&motorL, pid_l);
					setMotorVolt(&motorH, pid_h);

					//calcTipPos(getLinkAngle('L'), getLinkAngle('H'));

					if (DEBUG_EN) {
//						if (timer > 19) {
//							printf("PoinH: %d, PointL: %d, ", pointArrayH[i], pointArrayL[i]);
							calcTipPos(motorL, motorH);
//							timer = 0;
			//				printPotVal(potVal);
			//				printPotVal(motorL);
		//					printf("%d,%d,%d,%d\n\r", i, motorL.angle, motorL.dacVolt, motorL.mAmp);
//						}
					}
					timer++;
				}
			}
			stopMotors();
			_delay_ms(500);
		}
	}
}
void lab2BP6() {
	initTimer(1,CTC,64);  //start 100.1603Hz timer

	initADC(LOW_POT);
	initADC(HIGH_POT);		// init ADC on port 3
	initADC(0);		// init ADC on port 0 - currSense

	setConst('H', 45, 8, 2); //set the PID constants for high link
	setConst('L', 45, 8, 2); //set the PID constants for low link

	int pid_h = 0; //PID output to motor
	int pid_l = 0;
	unsigned int i = 0;
	int aValH = getADC(HIGH_POT);
	int aValL = getADC(LOW_POT);
	int adcCH = getADC(HIGH_CUR);
	int adcCL = getADC(LOW_CUR);

	struct Motor motorH, motorL;
	setPotVal(&motorH, 'H', aValH);
	setPotVal(&motorL, 'L', aValL);

	DDRB = 0xFF; //Set Port as output


	initSPI();



	while(1) {

		if (timerFlag) {
			timerFlag = 0;
//			float currSense = ((getADC(0) * (5000/1023))-2.5);
			adcCH = getADC(HIGH_CUR);
			adcCL = getADC(LOW_CUR);
			setCurVal(&motorH, adcCH);
			setCurVal(&motorL, adcCL);

			//printf("currSense: %f \n\r", currSense);
			aValH = getADC(HIGH_POT);
			aValL = getADC(LOW_POT);
			setPotVal(&motorH, 'H', aValH);
			setPotVal(&motorL, 'L', aValL);

			unsigned char activ = getPinsVal('D', 3, 2, 1, 0);
			if (activ == 0) {
				pid_h = calcPID('H', 90, motorH.angle);
				driveLink(1, pid_h);
				pid_l = calcPID('L', 0, motorL.angle);
				driveLink(0, pid_l);

			} else if (activ == 1) {
				pid_h = calcPID('H', 0, motorH.angle);
				driveLink(1, pid_h);
				pid_l = calcPID('L', 45, motorL.angle);
				driveLink(0, pid_l);

			} else if (activ == 2) {
				pid_h = calcPID('H', 45, motorH.angle);
				driveLink(1, pid_h);
				pid_l = calcPID('L', 45, motorL.angle);
				driveLink(0, pid_l);

			} else {
				pid_h = calcPID('H', 90, motorH.angle);
				driveLink(1, pid_h);
				pid_l = calcPID('L', 90, motorL.angle);
				driveLink(0, pid_l);
			}

			setMotorVolt(&motorL, pid_l);
			setMotorVolt(&motorH, pid_h);

			//calcTipPos(getLinkAngle('L'), getLinkAngle('H'));

			if (DEBUG_EN) {
				if (i > 19) {
				calcTipPos(motorL, motorH);
					i = 0;
	//				printPotVal(potVal);
	//				printPotVal(motorL);
//					printf("%d,%d,%d,%d\n\r", i, motorL.angle, motorL.dacVolt, motorL.mAmp);
				}
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
	initSPI();
	initTimer(1,CTC,20);
	int i;
	int gValue = 0;
	printf("Begin:\n\r");

	while(1){
		driveLink(1, 4095);
		printf("Encoder Counts %d", encoderCounts(0));
		/*
		if (timerFlag) {
			for (i = 0; i < 1; i++) {
				gValue = getAccel(i);
				printf("channel %1d: %5d ", i, gValue);
			}
			printf("\n\r");
		}
		*/
	}
	return 0;
}
