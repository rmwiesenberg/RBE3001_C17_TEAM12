#include <RBELib/RBELib.h>

#include <avr/io.h>


//character for receiving serial data
int lowADC = 0;
int highADC=1023;


int main(void)
{
	  //Enable printf() and setServo()
	  initRBELib();
	  DDRB = 0xFF; //Set Port as output

	  debugUSARTInit(115200);

	  	  while(1)
	  	  {

	  		PORTB = 0xFF; //Turn port on
	  		_delay_ms(500); //Delay .5 sec
	  		PORTB = 0x00; //Turn port off
	  		_delay_ms(500); //Delay .5 sec

	  	  }
}
