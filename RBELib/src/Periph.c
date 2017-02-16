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

	unsigned short in = 0;

	BYTE out;

	switch (axis) {
	case 0:
		out = 0b01100000;
		break;

	case 1:
		out = 0b01100100;
		break;

	case 2:
		out = 0b01101000;
		break;

	default:
		out = 0b01101100;
		break;
	}

	PORTCbits._P0 = 0;

	spiTransceive(out);

	unsigned char high = spiTransceive(0x00);
	unsigned char low = spiTransceive(0x00);

	PORTCbits._P0 = 1;

	in = (high << 4) | (low >> 4);

	return in;
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
	switch(chan) {
	// Lower Motor Encoder
	case 0:
		PORTCbits._P4 = 1;

		// Setup encoder 0
		PORTCbits._P5 = 1;
		PORTCbits._P5 = 0;

		/* Write to MDR0 Register (10001000)
		 * B7-B6: 10 - Write
		 * B5-B3: 001 - MDR0 register select
		 * B2-B0: 000 - Don't Care */
		spiTransceive(0x88);

		/* Setup MDR0 Register (00000011)
		 * B7: 0 - Clock divide = 1
		 * B6: 0 - Asynch Index
		 * B5-B4: 00 - Disable Index
		 * B3-B2: 00 - Free running count mode
		 * B1-B0: 11 - x4 quadrature (4 counts per cycle)*/
		spiTransceive(0x03);

		// reset SPI read cycle
		PORTCbits._P5 = 1;
		PORTCbits._P5 = 0;

		/* Write to MDR1 Register (10010000)
		 * B7-B6: 10 - Write
		 * B5-B3: 010 - MDR1 register select
		 * B2-B0: 000 - Don't Care */
		spiTransceive(0x90);

		/* Setup MDR1 Register (00010010)
		 * B7: 0 - NOP (no flag on CY)
		 * B6: 0 - NOP (no flag on BW)
		 * B5: 0 - NOP (no flag on CMP)
		 * B4: 1 - Flag on IDX
		 * B3: 0 - Not used
		 * B2: 0 - Enable counting
		 * B1-B0: 10 - 2-byte counter */
		spiTransceive(0x12);

		// reset SPI read cycle
		PORTCbits._P5 = 1;
		break;

	case 1:
		PORTCbits._P5 = 1;

		// Setup encoder 1
		PORTCbits._P4 = 1;
		PORTCbits._P4 = 0;

		/* Write to MDR0 Register (10001000)
		 * B7-B6: 10 - Write
		 * B5-B3: 001 - MDR0 register select
		 * B2-B0: 000 - Don't Care */
		spiTransceive(0x88);

		/* Setup MDR0 Register (00000011)
		 * B7: 0 - Clock divide = 1
		 * B6: 0 - Asynch Index
		 * B5-B4: 00 - Disable Index
		 * B3-B2: 00 - Free running count mode
		 * B1-B0: 11 - x4 quadrature (4 counts per cycle)*/
		spiTransceive(0x03);

		// reset SPI read cycle
		PORTCbits._P4 = 1;
		PORTCbits._P4 = 0;

		/* Write to MDR1 Register (10010000)
		 * B7-B6: 10 - Write
		 * B5-B3: 010 - MDR1 register select
		 * B2-B0: 000 - Don't Care */
		spiTransceive(0x90);

		/* Setup MDR1 Register (00010010)
		 * B7: 0 - NOP (no flag on CY)
		 * B6: 0 - NOP (no flag on BW)
		 * B5: 0 - NOP (no flag on CMP)
		 * B4: 1 - Flag on IDX
		 * B3: 0 - Not used
		 * B2: 0 - Enable counting
		 * B1-B0: 10 - 2-byte counter */
		spiTransceive(0x12);

		// reset SPI read cycle
		PORTCbits._P4 = 1;
		break;
	}
}

/**
 * @brief Reset the current count of the encoder ticks.
 * @param chan The channel to clear.
 *
 * @todo Clear the encoder count (set to 0).
 */
void resetEncCount(int chan) {
	switch(chan) {
	case 0:
		PORTCbits._P5 = 1;
		PORTCbits._P5 = 0;
		/* Clear CNTR register (00100000)
		 * B7-B6: 00 - CLR command
		 * B5-B3: 100 - CNTR register select
		 * B2-B0: 000 - Don't case */
		spiTransceive(0x20);
		PORTCbits._P5 = 1;
		break;

	case 1:
		PORTCbits._P4 = 1;
		PORTCbits._P4 = 0;
		/* Clear CNTR register (00100000)
		 * B7-B6: 00 - CLR command
		 * B5-B3: 100 - CNTR register select
		 * B2-B0: 000 - Don't case */
		spiTransceive(0x20);
		PORTCbits._P4 = 1;
		break;
	}
}

/**
 * @brief Finds the current count of one of the encoders.
 * @param  chan Channel that the encoder is on that you would like to read.
 * @return count The current count of the encoder.
 *
 * @todo Find the current encoder ticks on a given channel.
 */
int encCount(int chan) {
	BYTE high = 0, low = 0;
	int ret = 0;

		switch(chan) {
		case 0:
			// reset SPI read cycle
			PORTCbits._P5 = 1;
			PORTCbits._P5 = 0;

			/* Get Counts (01100000)
			 * B7-B6: 01 - Read register
			 * B5-B3: 100 - CNTR register select
			 * B2-B0: 000 - Don't Care */
			spiTransceive(0x60);

			/* Read upper byte from register
			 * B7-B1: 00000000 - Don't care
			 */
			high = spiTransceive(0x00);

			/* Read lower byte from register
			 * B7-B1: 00000000 - Don't care
			 */
			low = spiTransceive(0x00);

			PORTCbits._P5 = 1;

			ret = (high << 8) | low;
			return ret;
			break;

		// Upper Motor Encoder
		case 1:
			// reset SPI read cycle
			PORTCbits._P4 = 1;
			PORTCbits._P4 = 0;

			/* Get Counts (01100000)
			 * B7-B6: 01 - Read register
			 * B5-B3: 100 - CNTR register select
			 * B2-B0: 000 - Don't Care */
			spiTransceive(0x60);

			/* Read upper byte from register
			 * B7-B1: 00000000 - Don't care */
			high = spiTransceive(0x00);

			/* Read lower byte from register
			 * B7-B1: 00000000 - Don't care */
			low = spiTransceive(0x00);

			PORTCbits._P4 = 1;

			ret = (high << 8) | low;
			return ret;
			break;

		default:
			return -1;
			break;
		}
//	}
}

