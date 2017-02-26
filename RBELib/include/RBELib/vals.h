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
#define H_OFFSET 88    // high link angle offset
#define H_SCALE 270.0  // high link angle scale factor
#define L_OFFSET 87    // low link angle offset
#define L_SCALE 250.0  // low link angle scale factor

#define CUR_SCALE 4.8828// 5000 / 1024

#define DAC_SCALE 1.2207 // 5000 / 4096

#define PI 3.14159265


/**
 * Struct to hold useful data for a link motor
 */
struct Motor{
	int adcPot;
	int angle;
	int mVolt;
	int dacVal;
	int dacVolt;
	int adcCur;
	int mAmp;
	int encVal;
};


/**
 * Calculates tip position when given the angles of both links and prints out (x,y) coords
 * @param mL Motor struct containing lower link data
 * @param mH Motor struct containing upper link data
 */
void calcTipPos(struct Motor mL, struct Motor mH);


/**
 * Initializes the values for a PotVal struct
 * @param pot PotVal to initialize
 * @param link 'H' or 'L' for Upper or Lower link, respectively
 * @param adc ADC value to set the other values
 */
void setPotVal(struct Motor* motor, char link, int adc);


/**
 * Returns the angle of the desired link, H or L
 * @param link Link (H, or L) to return angle of
 */
int getLinkAngle(char link);


/**
 * Initializes the values for a CurVal struct
 * @param cur CurVal struct to initialize
 * @param adc ADC value read in from the current sense pins
 */
void setCurVal(struct Motor* motor, int adc);


/**
 * Takes in a CurVal struct and prints out its fields
 * @param aVal CurVal to print out
 */
void printMotor(struct Motor motor);


/**
 * Takes in Motor struct and sets the DAC output voltage
 * @param motor Motor to set value for
 * @param dac DAC output value
 */
void setMotorVolt(struct Motor* motor, int dac);


/**
 * Takes in Motor struct and sets the encoder value
 * @param motor Motor to set value for
 * @param enc Encoder tick value to set
 */
void setEncVal(struct Motor* motor, int enc);


/**
 * Takes in ADC value and returns scaled pot value
 * @param link Link to calculate value for
 * @param adc ADC value to scale
 */
int calcPotAngle(char link, int adc);

float getIRval(int pin);

#endif
