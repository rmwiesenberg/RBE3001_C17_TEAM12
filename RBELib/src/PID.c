/*
 * PID.c
 *
 *  Created on: Aug 24, 2016
 *      Author: joest
 */


#include "RBELib\RBELib.h"
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
//long pidErrorSum = 0;
long pidLastError_L = 0;
long pidPrevLastError_L = 0;
long pidLastError_H = 0;
long pidPrevLastError_H = 0;
long pidPrev_L = 0;
long pidPrev_H = 0;
float pidTd_H = 0;
float pidTi_inv_H = 0;
float pidTd_L = 0;
float pidTi_inv_L = 0;
pidConst pidConsts;
void setConst(char link, float Kp, float Ki, float Kd)
{
	switch(link)
	{
	case 'H':
		pidConsts.Kp_H = Kp;
		pidConsts.Ki_H = Ki;
		pidConsts.Kd_H = Kp;
		pidTd_H = Kd / Kp;
		pidTi_inv_H = (Ki / Kp);
		break;
	case 'L':
		pidConsts.Kp_L = Kp;
		pidConsts.Ki_L = Ki;
		pidConsts.Kd_L = Kd;
		pidTd_L = Kd / Kp;
		pidTi_inv_L = (Ki / Kp);
		break;
	}

}

/**
 * @brief Calculate the PID value.
 * @param  link Which link to calculate the error for (Use 'H' and 'L').
 * @param setPoint The desired position of the link.
 * @param actPos The current position of the link.
 *
 * @todo Make a function to calculate the PID value for a link.
 */
signed int calcPID(char link, int setPoint, int actPos)
{
	long error = actPos - setPoint;
	long pidValue = 0;
	if (link == 'L')
	{
		pidValue = pidPrev_L + pidConsts.Kp_L * ((1 + pidTi_inv_L + pidTd_L) * error  + (-1 - 2 * pidTd_L) * pidLastError_L + pidTd_L * pidPrevLastError_L);
		pidPrev_L = pidValue;
		pidPrevLastError_L = pidLastError_L;
		pidLastError_L = error;


	}
	else
	{
		pidValue = pidPrev_H + pidConsts.Kp_H * ((1 + pidTi_inv_H + pidTd_H) * error  + (-1 - 2 * pidTd_H) * pidLastError_H + pidTd_H * pidPrevLastError_H);
		pidPrev_H = pidValue;
		pidPrevLastError_H = pidLastError_H;
		pidLastError_H = error;
	}
	if(pidValue>4000) pidValue=4000;
	if(pidValue<-4000) pidValue=-4000;
	return pidValue;
}
