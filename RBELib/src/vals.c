/**
 * @file Lib.h
 *
 * @brief Source file to store structs that hold data and functions for those data structs
 * @author Rayyan Khan, Ben Titus, Ryan Wiesenberg
 * @date January 28, 2017
 */

#include <RBELib/RBELib.h>

int getLinkAngle(char link){
	int angle = 0;
	if (link == 'H') {
		angle = ((getADC(3) * (H_SCALE / 1023.))-H_OFFSET);
	}
	if (link == 'L') {
		angle = ((getADC(2) * (H_SCALE / 1023.))-H_OFFSET);
	}

	return angle;
}

void setPotVal(struct Motor* motor, char link, int adc) {

	motor->adcPot = adc;


	//set values for high link
	if (link == 'H') {
		motor->mVolt = (int) (adc * (5000. / 1023.));
		motor->angle = (int) ((adc * (H_SCALE / 1023.))-H_OFFSET);
//		printf("adcVal: %d, angle: %d, mVolt: %d ", pot->adcVal, pot->angle, pot->mVolt);
	}

	// set value for low link
	if (link == 'L') {
		motor->angle = (int) ((adc * (L_SCALE / 1023.)) - L_OFFSET);
		motor->mVolt = (int) (adc * (5000. / 1023.));
	}

//	printf("pot->angle: %d ", pot->angle);
//	if (pot->angle <= 0) {
//		pot->angle = 1;
//	}
}


void setCurVal(struct Motor* motor, int adc) {
	motor->adcCur = adc;
	motor->mAmp = (adc-512)*CUR_SCALE;
}

void printMotor(struct Motor motor) {
	printf("adcPot: %d angle: %d mVolt:%d adcCur: %d mAmp: %d", motor.adcPot, motor.angle, motor.mVolt, motor.adcCur, motor.mAmp);
}
