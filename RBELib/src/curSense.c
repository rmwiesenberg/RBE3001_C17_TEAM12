/**
 * @file curSense.h
 *
 * @brief Current sensing functions
 *
 * @author Rayyan Khan, Ben Titus, Ryan Wiesenberg
 * @date February 5, 2017
 */

#include <RBELib/RBELib.h>

unsigned short getCurrent(char link) {
	if (link == 'H') {
		return getADC(1);
	}
	if (link == 'L') {
		return getADC(0);
	}

	return 0;
}
