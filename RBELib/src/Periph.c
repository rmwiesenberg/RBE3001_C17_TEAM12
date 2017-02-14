/**
 * @file Periph.c
 *
 * @brief The header file and function prototypes for the peripherals (IR, encoder and accelerometer).
 * @details Each of these functions is for controlling the peripheral devices of the arm.
 * @author Rayyan Khan, Ben Titus, Ryan Wiesenberg
 * @date February 11, 2017
 */


#include <RBELib/RBELib.h>
#include <avr/io.h>


#define G_CONV 1 // Convert ADC value to milli-Gs

/**
 * @brief Find the acceleration in the given axis (X, Y, Z).
 * @param  axis The axis that you want to get the measurement of.
 * @return gVal Value of  acceleration.
 *
 * @todo Create a function that is able to find the acceleration of a given axis.
 */
int getAccel(int axis) {
	PORTC &= ~(1 << PC7);

	unsigned int in = 0;

	const char cmd = ((1<<4) | (1<<3) | axis) << 2;


	spiTransceive(cmd);

	unsigned char high = spiTransceive(0);
	unsigned char low = spiTransceive(0);

	in = ((int)high << 4) | ((low >> 4) & 0x0F);

	PORTC |= (1 << PC7);

	return in;
//
//	DDRCbits._P5 = 1;
//	PORTCbits._P5 = 1;
//	int axVal, refVal, gVal;
//
//	BYTE refOUT = 0;
//	refOUT = (1 << 4)
//			| (1 << 3)
//			| (3 << 0);
//	refOUT = 0b00110110;
//
//	//set channel
//	BYTE OUT = 0;
//	OUT = (1 << 4)     // Start bit
//			| (1 << 3) // Set to single ended mode
//			| ((axis) << 0);  // Set channel
//
//	BYTE high, low;
//
//	refVal = 0;
//	axVal = 0;
//
//	PORTCbits._P5 = 0;
//
//	spiTransceive(refOUT);
//	high = spiTransceive(0);
//	low = spiTransceive(0);
//
//	PORTCbits._P5 = 1;
//	printf("high: %d  low: %d  ", high, low);
//
//	_delay_ms(1);
//
//	refVal = (high << 8) | (low);
//
//	PORTCbits._P5 = 0;
//
//	//shift out control bits
//	spiTransceive(OUT);
//	high = spiTransceive(0);
//	low = spiTransceive(0);
//
//	PORTCbits._P5 = 1;
//	printf("high: %d  low: %d  ", high, low);
//
//	axVal = (high << 8) | (low);
//
//	if (axVal >= refVal) {
//		gVal = (axVal - refVal) * G_CONV;
//	} else {
//		gVal = -((refVal - axVal) * G_CONV);
//	}
//
//
//
//	printf("refVal: %d  axVal %d:  ", refVal, axVal);
//
//	SPI_MISO = 0;
//	SPI_MOSI = 0;
//	return gVal;
}

/**
 * @brief Read an IR sensor and calculate the distance of the block.
 * @param  chan The port that the IR sensor is on.
 * @return value The distance the block is from the sensor.
 *
 * @todo Make a function that is able to get the ADC value of the IR sensor.
 */
int IRDist(int chan) {
	return 0;
}

/**
 * @brief Initialize the encoders with the desired settings.
 * @param chan Channel to initialize.
 *
 * @todo Make a function that can setup both encoder chips on the board.
 */
void encInit(int chan) {

}

/**
 * @brief Reset the current count of the encoder ticks.
 * @param chan The channel to clear.
 *
 * @todo Clear the encoder count (set to 0).
 */
void resetEncCount(int chan) {

}

/**
 * @brief Finds the current count of one of the encoders.
 * @param  chan Channel that the encoder is on that you would like to read.
 * @return count The current count of the encoder.
 *
 * @todo Find the current encoder ticks on a given channel.
 */
int encCount(int chan) {
		DDRC |= (1 << DDC5) | (1 << DDC4);				// set Encoder CS1 and Encoder CS0 to output mode

		switch(chan) {
		case 0:
			PORTC |= (1 << PC5);
			PORTC &= ~(1 << PC5);

			/* Setup */
			spiTransceive(0x88);
			spiTransceive(0x03);

			// reset SPI read cycle
			PORTC |= (1 << PC5);
			PORTC &= ~(1 << PC5);

			spiTransceive(0x90);
			spiTransceive(0x12);

			// reset SPI read cycle
			PORTC |= (1 << PC5);
			PORTC &= ~(1 << PC5);

			/* Get Counts */
			spiTransceive(0x60);

			int rec = spiTransceive(0xFF);
			rec = (rec & 0xFF) << 8;
			rec += spiTransceive(0xFF);

			PORTC |= (1 << PC5);

			return rec;

			break;
		case 2:
			//fill this
			return rec;
			break;

		default:
			return -1;
			break;
		}
//	}
}

