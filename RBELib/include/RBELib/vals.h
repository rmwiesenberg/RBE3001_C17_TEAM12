/**
 * @file vals.h
 *
 * @brief Header file to store structs that hold data and functions for those data structs
 * @author Rayyan Khan, Ben Titus, Ryan Wiesenberg
 * @date January 28, 2017
 */
#ifndef VALS_H_
#define VALS_H_

//defines for setting PotVal struct
#define H_OFFSET 80    // high link angle offset
#define H_SCALE 265.0  // high link angle scale factor
#define L_OFFSET 77    // low link angle offset
#define L_SCALE 250.0  // low link angle scale factor

#define CUR_SCALE 4.88


typedef struct{
	unsigned short adcVal;
	unsigned int mAmp;
} CurVal;

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

int getLinkAngle(char link);


/**
 * Takes in a CurVal struct and prints out its fields
 * @param aVal CurVal to print out
 */
void printCurVal(CurVal aVal);

/**
 * Initializes the values for a CurVal struct
 * @param cur CurVal struct to initialize
 * @param adc ADC value read in from the current sense pins
 */
void setCurVal(CurVal* cur, int adc);

#endif
