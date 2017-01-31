/**
 * @file Lib.h
 *
 * @brief Header file to store structs that hold data and functions for those data structs
 * @author Rayyan Khan, Ben Titus, Ryan Wiesenberg
 * @date January 28, 2017
 */
#ifndef VALS_H_
#define VALS_H_


typedef struct {
	unsigned short adcVal;
	int angle;
	int mVolt;
} PotVal;

void setPotVal(unsigned short adc);

void printPotVal(PotVal aVal);

#endif
