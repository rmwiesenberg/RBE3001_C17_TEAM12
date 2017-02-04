/**
 * @file Lib.h
 *
 * @brief Source file to store structs that hold data and functions for those data structs
 * @author Rayyan Khan, Ben Titus, Ryan Wiesenberg
 * @date January 28, 2017
 */

#include <RBELib/RBELib.h>



void printPotVal(PotVal aVal) {
	printf("adcVal: %d, angle: %d, mVolt: %d\n\r", aVal.adcVal, aVal.angle, aVal.mVolt);
}

void setPotVal(PotVal* pot, char link, int adc) {

	pot->adcVal = adc;


	//set values for high link
	if (link == 'H') {
		pot->mVolt = (int) (adc * (5000. / 1023.));
		pot->angle = (int) ((adc * (265. / 1023.))-83);
		printf("adcVal: %d, angle: %d, mVolt: %d ", pot->adcVal, pot->angle, pot->mVolt);
	}

	// set value for low link
	if (link == 'L') {
		pot->angle = (adc * (250. / 1023.)) - 90;
		pot->mVolt = adc * (5000. / 1023.);
	}

	printf("pot->angle: %d ", pot->angle);
//	if (pot->angle <= 0) {
//		pot->angle = 1;
//	}
}
