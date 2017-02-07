/**
 * @file Lib.h
 *
 * @brief Source file to store structs that hold data and functions for those data structs
 * @author Rayyan Khan, Ben Titus, Ryan Wiesenberg
 * @date January 28, 2017
 */

#include <RBELib/RBELib.h>



void printPotVal(PotVal aVal) {
	printf("adcVal: %d, angle: %d, mVolt: %d ", aVal.adcVal, aVal.angle, aVal.mVolt);
}

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

void setPotVal(PotVal* pot, char link, int adc) {

	pot->adcVal = adc;


	//set values for high link
	if (link == 'H') {
		pot->mVolt = (int) (adc * (5000. / 1023.));
		pot->angle = (int) ((adc * (H_SCALE / 1023.))-H_OFFSET);
//		printf("adcVal: %d, angle: %d, mVolt: %d ", pot->adcVal, pot->angle, pot->mVolt);
	}

	// set value for low link
	if (link == 'L') {
		pot->angle = (int) ((adc * (L_SCALE / 1023.)) - L_OFFSET);
		pot->mVolt = (int) (adc * (5000. / 1023.));
	}

//	printf("pot->angle: %d ", pot->angle);
//	if (pot->angle <= 0) {
//		pot->angle = 1;
//	}
}


void printCurVal(CurVal aVal) {
	printf("adcVal: %d mAmp: %d", aVal.adcVal, aVal.mAmp);
}

void setCurVal(CurVal* cur, int adc) {
	cur->adcVal = adc;
	cur->mAmp = (adc-2048)*CUR_SCALE;
}
