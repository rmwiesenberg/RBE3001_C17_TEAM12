/**
 * @file curSense.h
 *
 * @brief Current sensing functions
 *
 * @author Rayyan Khan, Ben Titus, Ryan Wiesenberg
 * @date February 5, 2017
 */

#ifndef CURSENSE_H_
#define CURSENSE_H_

/**
 * Reads and returns the value of the current sense pin for high or low link
 * @param link Link to read current of
 * @return Value of the ADC on the current sense pin
 */
unsigned short getCurrent(char link);


#endif
