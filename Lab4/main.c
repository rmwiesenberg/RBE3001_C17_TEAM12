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

void readit(int* theta){
	unsigned char inchar;
	int stage = 0;

	while(stage != 2){
		inchar = getCharDebug();
		if(inchar == ' ' || inchar == '\n'){
			stage++;
		} else {
			theta[stage] += inchar - 48;
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

	int pid_h = 0; //PID output to motor
	int pid_l = 0;

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
	int potval[2] = {90,0};
	while(1){
		switch(state){
		case 0: // find position and velocity of block
			potValH = getADC(HIGH_POT);
			potValL = getADC(LOW_POT);
			setPotVal(&motorH, 'H', potValH);
			setPotVal(&motorL, 'L', potValL);
			pid_l = calcPID('L', 90, motorL.angle);
			pid_h = calcPID('H', 0+90, motorH.angle);

			ir1val = IRDist(IR_ONE);
			if (ir1val < 150){
				printf("%d", ir1val);
				state = 1;
			}
		break;
		case 1: // Get
			readit(potval);
			state = 2;
		break;
		case 2:
			ir2val = IRDist(IR_TWO);
			if(ir2val < 150){
				state = 3;
			}
		break;
		case 3:
			potValH = getADC(HIGH_POT);
			potValL = getADC(LOW_POT);
			setPotVal(&motorH, 'H', potValH);
			setPotVal(&motorL, 'L', potValL);
			pid_l = calcPID('L', potval[0], motorL.angle);
			pid_h = calcPID('H', potval[1]+90, motorH.angle);

			if (pid_l < 300 && pid_h < 300){
				setServo(0,255);
				state = 4;
			}
		break;
		case 4:
			potValH = getADC(HIGH_POT);
			potValL = getADC(LOW_POT);
			setPotVal(&motorH, 'H', potValH);
			setPotVal(&motorL, 'L', potValL);
			pid_l = calcPID('L', 90, motorL.angle);
			pid_h = calcPID('H', -90+90, motorH.angle);

			if (pid_l < 300 && pid_h < 300){
				setServo(0,255);
				state = 5;
			}
		break;
		case 5:
			curValH = getADC(HIGH_CUR);
			setCurVal(&motorH, curValH);

			if(motorH.mAmp < 100){
				potValH = getADC(HIGH_POT);
				potValL = getADC(LOW_POT);
				setPotVal(&motorH, 'H', potValH);
				setPotVal(&motorL, 'L', potValL);
				pid_l = calcPID('L', 90, motorL.angle);
				pid_h = calcPID('H', 90+90, motorH.angle);
			} else {

			}

			if (pid_l < 300 && pid_h < 300){
				setServo(0,0);
				state = 0;
			}
		break;
		}
	}
}


ISR(TIMER1_COMPA_vect) {		// timer ISR, usable in all file functions
	timerFlag = 1;
}

int main(void) {
	lab4();

}
