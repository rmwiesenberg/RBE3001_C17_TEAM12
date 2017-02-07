/*
 * motors.c
 *
 *  Created on: Jan 31, 2017
 *      Author: Rayyan
 */
#include <RBELib/RBELib.h>

void stopMotors(){
	int i_;
	for (i_ = 0; i_ < 4; i_++) {
		setDAC(i_, 0);
	}
}

/**
 * @brief Drive the arm to a desired angle
 *
 * @param lowerTheta The desired angle for the lower link.
 * @param upperTheta The desired angle for the upper link.
 *
 * @todo Make a way to drive the links to a desired angle.
 */
void gotoAngles(int lowerTheta, int upperTheta){
	int ang_L = getADC(2);
	int ang_H = getADC(3);

	unsigned short pid_H = calcPID('H', upperTheta, ang_H);
	unsigned short pid_L = calcPID('L', lowerTheta, ang_L);

	driveLink(0, pid_L);
	driveLink(1, pid_H);
}

/**
 * @brief Drive the end effector of the arm to a desired X and Y position in the workspace.
 *
 * @param x The desired x position for the end effector.
 * @param y The desired y position for the end effector.
 *
 * @todo Use kinematic equations to move the end effector to the desired position.
 */
void gotoXY(int x, int y){

}

/**
 * @brief Drive a link (upper or lower) in a desired direction.
 *
 * @param link Which link to control.
 * @param dir Which way to drive the link.
 *
 * @todo Create a way to drive either link in any direction.
 */
void driveLink(int link, int dir){
	//upperlink
	if(link == 1){
		if(dir >= 0){
			if (dir > 4095) {
				dir = 4095;
			}
			setDAC(2, 0);
			setDAC(3, dir);
		}
		if(dir < 0){
			if (dir < -4095) {
				dir = -4095;
			}
			setDAC(2, dir*-1);
			setDAC(3, 0);
		}
	}
	//lowerlink
	if(link == 0){
			if(dir >= 0){
				if (dir > 4095) {
					dir = 4095;
				}
				setDAC(0, dir);
				setDAC(1, 0);
			}
			if(dir < 0){
				if (dir < -4095) {
					dir = -4095;
				}
				setDAC(0, 0);
				setDAC(1, dir*-1);
			}
		}


}

/**
 * @brief Drive the arm to a "home" position using the potentiometers.  This
 * should be called before using the encoders and just goes to a default position.
 * Once this has been called once, you can initialize/clear the encoders.
 *
 * @todo Drive the arm to a known position using the potentiometers.
 */
void homePos(){

}
