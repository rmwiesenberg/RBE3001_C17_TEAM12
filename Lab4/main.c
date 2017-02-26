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

int pointArrayH[] = {-9,17,-33};
int pointArrayL[] = {69,42,55};

volatile BYTE timerFlag = 0;


void robotHome() {
	int angH = calcPotAngle('H',getADC(3));
	int angL = calcPotAngle('L',getADC(2));

	/* Values to print */
//	int encVal = 0, potVal = 0, xVal = 0, yVal = 0, zVal = 0;
//	encVal = encCount(0);
//	potVal = angL;
//	xVal = getAccel(0);
//	yVal = getAccel(1);
//	zVal = getAccel(2);

	while((angH < 88) | (angH > 92) | (angL < 88) | (angL > 92)) {
		if (timerFlag) {
			timerFlag = 0;
			gotoAngles(90,90);
			angL = calcPotAngle('L',getADC(2));
			angH = calcPotAngle('H',getADC(3));
//			if (DEBUG_EN) {
//				encVal = encCount(0);
//				potVal = angL;
//				xVal = getAccel(0);
//				yVal = getAccel(1);
//				zVal = getAccel(2);
//				printf("%d, %d, %d, %d, %d\n\r", potVal, encVal, xVal, yVal, zVal);
//			}
		}
	}
	stopMotors();

	resetEncCount(0);
	resetEncCount(1);
}


void signOff1() {
	initRBELib();
	debugUSARTInit(115200);
	initSPI();
	encInit(1);
	encInit(0);
	resetEncCount(0);
	resetEncCount(1);
	initTimer(1, CTC, 78);
	int encCountL = 0;
	int encCountH = 0;
	int mode = 0;
	BYTE butts = 0;
	driveLink(0,0);
	setPinsDir('B', 0, 4, 0, 1, 2, 3);
	butts = getPinsVal('B', 4, 0, 1, 2, 3);

	while(1) {
		butts = getPinsVal('B', 4, 0, 1, 2, 3);

		if (timerFlag) {
			timerFlag = 0;
			if ((~butts & 0x01)) {
				mode = 0;
			} else if (((~butts) & 0x02)) {
				mode = 1;
			} else if (((~butts) & 0x04)) {
				mode = 2;
			} else if (((~butts) & 0x08)) {
				mode = 3;
			}

			cli();
			switch(mode) {
			case 0:
				driveLink(1,0);
				break;

			case 1:
				driveLink(1, 768);
				break;

			case 2:
				driveLink(1,-768);
				break;

			case 3:
				driveLink(1,1536);
				break;
			}
			sei();

			encCountL = encCount(0);
			encCountH = encCount(1);

			printf("%d, %d\n", encCountL, encCountH);
		}
	}
}

void signOff2() {
	initRBELib();
	debugUSARTInit(115200);
	initSPI();
	initTimer(1,CTC,40);
	int xVal = 0, yVal = 0, zVal = 0;
	printf("Begin:\n\r");

	_delay_ms(1000);

	while(1){
//		driveLink(1, 4095);
//		printf("Encoder Counts %d", encoderCounts(0));
		if (timerFlag) {
			timerFlag = 0;
			xVal = getAccel(0);
			yVal = getAccel(1);
			zVal = getAccel(2);
			if (DEBUG_EN) printf("x: %5d y: %5d z: %5d\n\r", xVal, yVal, zVal);
//			printf("youfuckngbitch\n\r");
		}

	}
}

void signOff3() {
	initRBELib();
	debugUSARTInit(115200);
	initSPI();
	initADC(LOW_POT);
	initADC(HIGH_POT);		// init ADC on port 3
	encInit(1);
	encInit(0);
	resetEncCount(0);
	resetEncCount(1);
	setConst('H', 45, 8, 2); //set the PID constants for high link
	setConst('L', 45, 8, 2); //set the PID constants for low link
	initTimer(1, CTC, 78);

	printf("\n\rHello World\n\r");

	setPinsDir('B', 0, 1, 0);

	stopMotors();

	int butts = getPinsVal('B', 1, 0);
	while (butts) {
		if (timerFlag) {
			timerFlag = 0;
			printf("Why me\n\r");
		}
		butts = getPinsVal('B', 1, 0);
	}
	robotHome();
	printf("Home sweet home~\n\r");
}


void signOff4() {
	initRBELib();
	debugUSARTInit(115200);
	initSPI();
	initADC(LOW_POT);
	initADC(HIGH_POT);		// init ADC on port 3
	encInit(1);
	encInit(0);
	resetEncCount(0);
	resetEncCount(1);
	setConst('H', 45, 8, 2); //set the PID constants for high link
	setConst('L', 45, 8, 2); //set the PID constants for low link
	initTimer(1, CTC, 78);

	setPinsDir('B', 0, 1, 0);

	stopMotors();

	int butts = getPinsVal('B', 1, 0);
	while (butts) {
		if (timerFlag) {
			timerFlag = 0;
		}
		butts = getPinsVal('B', 1, 0);
	}

	robotHome();

	/* Values to print */
	int encVal = 0, potVal = 0, xVal = 0, yVal = 0, zVal = 0;
	encVal = encCount(0);
	potVal = calcPotAngle('L',getADC(2));
	xVal = getAccel(0);
	yVal = getAccel(1);
	zVal = getAccel(2);

	while(1) {
		if (timerFlag) {
			timerFlag = 0;
			gotoAngles(90,90);
			if (DEBUG_EN) {
				encVal = encCount(0);
				potVal = calcPotAngle('L',getADC(2));;
				xVal = getAccel(0);
				yVal = getAccel(1);
				zVal = getAccel(2);
				printf("%d, %d, %d, %d, %d\n", potVal, encVal, xVal, yVal, zVal);
			}
		}
	}
}

void lab4(){
	initRBELib();
	debugUSARTInit(115200);
	initSPI();

	initADC(LOW_POT);
	initADC(HIGH_POT);

	initADC(IR_ONE);
	initADC(IR_TWO);

	setConst('H', 45, 8, 2); //set the PID constants for high link
	setConst('L', 45, 8, 2); //set the PID constants for low link

	setServo(0,0);
	_delay_ms(10);
	setServo(2,0);

	stopMotors();

	int ir1val = 0;
	int ir2val = 0;

	while(1){

		ir1val = getIRval(IR_ONE);
		ir2val = getIRval(IR_TWO);

		if(ir1val < 180){
			setServo(0,0);
		}
		else if(ir2val < 140){
			setServo(0,180);
		}

		printf("IR 1: %d mm  IR 2: %d mm \r\n", ir1val, ir2val);

	}
}


ISR(TIMER1_COMPA_vect) {		// timer ISR, usable in all file functions
	timerFlag = 1;
}

int main(void) {
	switch(SIGN_OFF) {
	case 1:
		//signOff1();
		lab4();
		break;

	case 2:
		signOff2();
		break;

	case 3:
		signOff3();
		break;

	case 4:
		signOff4();
		break;
	}
	return 0;
}
