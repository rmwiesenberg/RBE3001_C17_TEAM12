/**
 * @file Lib.h
 *
 * @brief Source file to store structs that hold data and functions for those data structs
 * @author Rayyan Khan, Ben Titus, Ryan Wiesenberg
 * @date January 28, 2017
 */

#include "vals.h"

void setPotVal(PotVal pot, unsigned short adc) {

	pot->adcVal = adc;
	pot->angle = (adcVal * 250 / 1023) - 83,
	pot->mVolt = adcVal * 5000 / 1023;
}

void printPotVal(potVal aVal) {
	calcPotVals()
	printf("%d, %d, %d\n\r", adcIn, angle, mVolt);
}
