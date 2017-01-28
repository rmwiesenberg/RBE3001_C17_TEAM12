/**
 * @file Lib.h
 *
 * @brief Header file to store structs that hold data and functions for those data structs
 * @author Rayyan Khan, Ben Titus, Ryan Wiesenberg
 * @date January 28, 2017
 */
#ifndef VALS_H_
#define VALS_H_


typedef struct potVal {
	unsigned short adcVal;
	int angle = (adcVal * 250 / 1023) - 83;
	int mVolt = adcVal * 5000 / 1023;
}potVal;

void printPotVal(potVal aVal);

#endif
