/**
 * @file vals.h
 *
 * @brief Header file to store structs that hold data and functions for those data structs
 * @author Rayyan Khan, Ben Titus, Ryan Wiesenberg
 * @date January 28, 2017
 */
#ifndef VALS_H_
#define VALS_H_



typedef struct{
	unsigned short adcVal;
	unsigned int angle;
	unsigned int mVolt;
} PotVal;

/**
 * Takes in a PotVal struct and prints out its fields
 * @param aVal PotVal to print out
 */
void printPotVal(PotVal aVal);

/**
 * Initializes the values for a PotVal struct
 * @param pot PotVal to initialize
 * @param link 'H' or 'L' for Upper or Lower link, respectively
 * @param adc ADC value to set the other values
 */
void setPotVal(PotVal* pot, char link, int adc);

#endif
