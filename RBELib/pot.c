/**
 * File pot.c
 *
 * @brief The source file and functions for the potentiometers.
 * @details  Use these functions to read the values from the pots.
 *
 * @author Ben Titus
 * @date 22 Jan, 2017
 */


int potAngle(int pot) {
	return (270 / (pot/1023)) + 0.5;
}


int potVolts(int pot) {
	return (5000 / (pot/1023)) + 0.5;
}
