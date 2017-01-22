/**
 * file ADC.c
 *
 * @brief The source file and functions for the ADC.
 * @details For single ended conversion, the ADC value an be found from the voltage using:
 * @f[ \frac {V_{in} * 1024}{V_{ref}} @f]
 *
 * @author Ben Titus
 * @date 22 Jan, 2017
 */

void initADC(int channel) {
	switch (channel) {
	case 1:
		ADMUX = 0x01;
		break;

	case 2:
		ADMUX = 0x02;
		break;

	case 3:
		ADMUX = 0x03;
		break;

	case 4:
		ADMUX = 0x04;
		break;

	case 5:
		ADMUX = 0x05;
		break;

	case 6:
		ADMUX = 0x06;
		break;

	case 7:
		ADMUX = 0x07;
		break;

	default:
		ADMUX = 0x00;
		break;
	}
}

/**
 * @brief Disables ADC functionality and clears any saved values (globals).
 *
 * @param channel  The ADC channel to disable.
 *
 * @todo Create the corresponding function to clear the last ADC
 * calculation register and disconnect the input to the ADC if desired.
 */
void clearADC(int channel) {
	ADCSRA &= 0x7F;
}

/**
 * @brief Run a conversion on and get the analog value from one ADC
 * channel if using polling.
 *
 * @param channel  The ADC channel to run a conversion on.
 * @return adcVal The 8-10 bit value returned by the ADC
 * conversion.  The precision depends on your settings and
 * how much accuracy you desire.
 *
 * @todo Create the corresponding function to obtain the value of the
 * last calculation if you are using polling.
 */
unsigned short getADC(int channel) {

}

/**
 * @brief Change the channel the ADC is sampling if using interrupts.
 *
 * @param channel  The ADC channel to switch to.
 *
 * @todo Create a way to switch ADC channels if you are using interrupts.
 */
void changeADC(int channel) {
	ADMUX &= 0b11100000;
	ADMUX |= channel;
}
