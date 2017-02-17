/**
 * @file PID.h
 *
 * @brief The header file for PID constants and calculations.
 * @details Sets the PID constants and calculate the PID value.
 * @author Eric Willcox
 * @date August 17 2013
 */

#include <RBELib/RBELib.h>
#define NUMERRS 32
#define SAMPLEFREQ 100

pidConst pidConsts;

/**
 * @brief Sets the Kp, Ki, and Kd values for 1 link.
 * @details to set the values, use the following style
 * @code pidConst.Kp = 1.3; @endcode
 * @param link The link you want to set the values for (H or L).
 * @param Kp Proportional value.
 * @param Ki Integral value.
 * @param Kd Derivative value.
 *
 * @todo Create a function to the the PID constants for a given link.
 */
void setConst(char link, float Kp, float Ki, float Kd) {
	if (link == 'H') {
		pidConsts.Kp_H = Kp;
		pidConsts.Ki_H = Ki;
		pidConsts.Kd_H = Kd;
	}

	if (link == 'L') {
		pidConsts.Kp_L = Kp;
		pidConsts.Ki_L = Ki;
		pidConsts.Kd_L = Kd;
	}
}

signed int pastErr_H[NUMERRS];
signed int pastErr_L[NUMERRS];
signed int pastPID[NUMERRS];

/**
 * @brief Calculate the PID value.
 * @param  link Which link to calculate the error for (Use 'U' and 'L').
 * @param setPoint The desired position of the link.
 * @param actPos The current position of the link.
 *
 * @todo Make a function to calculate the PID value for a link.
 */
signed int calcPID(char link, int setPoint, int actPos) {

	// calc current error
	int error = setPoint - actPos;
	int errSum = 0;
	int i = 0;
	long pVal = 0;
	long iVal = 0;
	long dVal = 0;

	//pVal + iVal + dVal for high link
	if (link == 'H') {

		// put current error into array and shift down
		for (i = NUMERRS; i > 0; i--) {
			errSum += pastErr_H[i];
			pastErr_H[i-1] = pastErr_H[i];
		}
		// Finish up for loop
		errSum += error;
		pastErr_H[0] = error;

		pVal = pidConsts.Kp_H * error;
		iVal = pidConsts.Ki_H * errSum / SAMPLEFREQ;
		dVal = pidConsts.Kd_H * (error - pastErr_H[1]) * SAMPLEFREQ;
	}

	//pVal + iVal + dVal for lower link
	if (link == 'L') {

		// sum past errors
		// put current error into array and shift down
		for (i = NUMERRS; i > 0; i--) {
			errSum += pastErr_L[i];
			pastErr_L[i-1] = pastErr_L[i];
		}
		// finish up for loop
		errSum += error;
		pastErr_L[0] = error;

		// calc values according to equation
		pVal = pidConsts.Kp_L * error;
		iVal = pidConsts.Ki_L * errSum / SAMPLEFREQ;
		dVal = pidConsts.Kd_L * (error - pastErr_L[1]) * SAMPLEFREQ;
	}


	if (pVal >= 4096) {
		pVal = 4095;
	} else if (pVal <= -4096) {
		pVal = -4095;
	}
	if (iVal >= 4096) {
		iVal = 4095;
	} else if (iVal <= -4096) {
		iVal = -4095;
	}
	if (dVal >= 4096) {
		dVal = 4095;
	} else if (dVal <= -4096) {
		dVal = -4095;
	}

	//printf("pVal: %ld iVal: %ld dVal: %ld ", pVal, iVal, dVal);
	return pVal + iVal + dVal;
}
