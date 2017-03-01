#include <RBELib/RBELib.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <RBELib/vals.h>

#define TIMER_CLK		 	18432000. / 8.				// timer uses clk frequency 18.432 MHz / 8 = 2.304 MHz = count frequency
#define COUNT_FREQUENCY		TIMER_CLK / 256.			// count overflow interrupt fires 2.304 MHz kHz / 256 = 9000 times a second
#define TICK				COUNT_FREQUENCY / 8192.		// 4096 ticks in 1 second = 4096 in 9000 counts = 2.197265 counts / tick
#define IR_ONE 4
#define IR_TWO 5

#define HIGH_POT 3
#define LOW_POT 2
#define HIGH_CUR 1
#define LOW_CUR 0
#define KC 750
#define DEBUG_EN 1
#define TOL 3

#define SIGN_OFF 1

unsigned long FREQ1 = 9024;

unsigned char mode, tog;
volatile int count0 = 0;

int pointArrayH[] = { -9, 17, -33 };
int pointArrayL[] = { 69, 42, 55 };

volatile BYTE timerFlag = 0;
volatile unsigned long timerCount = 0;

void readit(int* theta) {
	unsigned char inchar;
	int stage = 0;
	int place = 0;
	unsigned char neg = 0;
	theta[0] = 0;
	theta[1] = 0;

	while (stage != 2) {
		inchar = getCharDebug();
		if (inchar == ',' || inchar == '\n') {
			if (neg){
				theta[stage] = theta[stage] * -1;
				neg = 0;
			}
			stage++;
			place=0;
		} else if (inchar == '-') {
			neg = 1;
			place++;
		}else {
			if (place == 0){
				theta[stage] += (inchar-48)*1000;
			} else if (place == 1) {
				theta[stage] += (inchar-48)*100;
			} else if (place == 2){
				theta[stage] += (inchar-48)*10;
			} else if (place == 3) {
				theta[stage] += (inchar-48)*1;
			}
			place++;
		}
	}

	printf("theta1: %d theta2: %d\n", theta[0], theta[1]);
}

void lab4() {
	initRBELib();
	debugUSARTInit(115200);
	initSPI();
//	initTimer(1, CTC, 80);

	setPinsDir('B', OUTPUT, 4, 0, 1, 2, 3);
	setPinsVal('B', HIGH, 4, 0, 1, 2, 3);

	initADC(LOW_POT);
	initADC(HIGH_POT);

	initADC(IR_ONE);
	initADC(IR_TWO);

	setConst('H', 45, 8, 2); //set the PID constants for high link
	setConst('L', 45, 8, 2); //set the PID constants for low link

	setServo(0, 0);
	_delay_ms(10);
	setServo(2, 0);

	stopMotors();

	struct Motor motorL, motorH;

	int potValH = getADC(HIGH_POT);
	int potValL = getADC(LOW_POT);
	int curValH = getADC(HIGH_CUR);
	int curValL = getADC(LOW_CUR);
	int curCount = 0;

	setPotVal(&motorH, 'H', potValH);
	setPotVal(&motorL, 'L', potValL);
	setCurVal(&motorH, curValH);
	setCurVal(&motorL, curValL);

	int state = 0;
	int potval[2] = { 90, 0 };

	while (1) {
		gotoAngles(potval[0], potval[1]);
		switch (state) {
		case 0: // find position and velocity of block
			potval[0] = 90;
			potval[1] = 0;
			setPinsVal('B', LOW, 1, 0);
			if ((abs(calcPotAngle('H',getADC(3)) - (potval[1]+90)) < TOL)
					&& (abs(calcPotAngle('L',getADC(2)) - potval[0]) < TOL)) {
				setPinsVal('B', HIGH, 1, 0);
				state = 1;
			}
			break;
		case 1: // Get
			setPinsVal('B', LOW, 1, 1);
			stopMotors();
			readit(potval);
			setPinsVal('B', HIGH, 1, 1);
			state = 2;
			break;
		case 2:
			setPinsVal('B', LOW, 1, 2);
			if ((abs(calcPotAngle('H',getADC(3)) - (potval[1]+90)) < TOL)
					&& (abs(calcPotAngle('L',getADC(2)) - potval[0]) < TOL)) {
				setPinsVal('B', HIGH, 1, 2);
				_delay_ms(25);
				setServo(0,255);
				_delay_ms(350);
				printf("start\n");
				state = 3;
			}
			break;
		case 3:
			potval[0] = 45;
			potval[1] = -45;
			setPinsVal('B', LOW, 2, 0, 2);

			if ((abs(calcPotAngle('H',getADC(3)) - (potval[1]+90)) < TOL)
					&& (abs(calcPotAngle('L',getADC(2)) - potval[0]) < TOL)) {
				setPinsVal('B', HIGH, 2, 0, 2);
				printf("stop\n");
				setServo(0, 255);
				curValH = 0;
				curValL = 0;
				state = 4;
			}
			break;

		case 4:
			if (curCount >= 8) {
				curValH /= 8;
				curValL /= 8;
				setCurVal(&motorH, curValH);
				setCurVal(&motorL, curValL);
				curCount = 0;
				state = 5;
			} else {
				curCount++;
				curValH += getADC(HIGH_CUR);
				curValL += getADC(LOW_CUR);
			}
			break;

		case 5:
			setPinsVal('B', LOW, 2, 1, 2);

			if ((abs(calcPotAngle('H',getADC(3)) - (potval[1]+90)) < TOL)
					&& (abs(calcPotAngle('L',getADC(2)) - potval[0]) < TOL)) {
				setPinsVal('B', HIGH, 2, 1, 2);
				printf("mAmp: %d\n", (motorH.mAmp+motorL.mAmp));
				state = 6;
			}
			break;

		case 6:
			setPinsVal('B', LOW, 3, 0, 1, 2);
			if ((motorH.mAmp + motorL.mAmp) < 1060) {
				potval[0] = 30;
				potval[1] = 0;
			} else {
				potval[0] = 100;
				potval[1] = 90;
			}

			if ((abs(calcPotAngle('H',getADC(3)) - (potval[1]+90)) < TOL)
					&& (abs(calcPotAngle('L',getADC(2)) - potval[0]) < TOL)) {
				setPinsVal('B', HIGH, 3, 0, 1, 2);
				setServo(0, 0);
				state = 0;
				setCurVal(&motorH, 0);
			}
		}
	}
}

void test() {
	initRBELib();
	debugUSARTInit(115200);
	initSPI();
	//	initTimer(1, CTC, 80);

	setPinsDir('B', OUTPUT, 4, 0, 1, 2, 3);
	setPinsVal('B', HIGH, 4, 0, 1, 2, 3);

	initADC(LOW_POT);
	initADC(HIGH_POT);

	initADC(IR_ONE);
	initADC(IR_TWO);

	setConst('H', 45, 8, 2); //set the PID constants for high link
	setConst('L', 45, 8, 2); //set the PID constants for low link

	setServo(0, 0);
	_delay_ms(10);
	setServo(2, 0);

	stopMotors();

	struct Motor motorL, motorH;

	int potValH = getADC(HIGH_POT);
	int potValL = getADC(LOW_POT);
	int curValH = getADC(HIGH_CUR);
	int curValL = getADC(LOW_CUR);

	setPotVal(&motorH, 'H', potValH);
	setPotVal(&motorL, 'L', potValL);
	setCurVal(&motorH, curValH);
	setCurVal(&motorL, curValL);

	int state = 0;
	int potval[2] = { 90, 0 };

	while (1) {
		setPinsVal('B', LOW, 1, 0);
		int ir1val = IRDist(IR_ONE);

		printf("%d\n", ir1val);
	}
}

ISR(TIMER1_COMPA_vect) {		// timer ISR, usable in all file functions
	timerFlag = 1;
	timerCount++;
}

int main(void) {
	lab4();
//	test();
}
