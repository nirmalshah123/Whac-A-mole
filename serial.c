#include <at89c5131.h>

bit tx_complete = 0,rx_complete = 0; //Bit flags for interrupts

/**********************************************************
   uart_init(): 
	Initialization function to be completed
	Initializes UART peripheral for 8-bit transfer, 
	1 start and 1 stop bits. 
	
	Please write TH1 value for required baud rate
***********************************************************/	
void uart_init(void)
{
	TMOD=0x20;			//Configure Timer 1 in Mode 2
	TH1=-13;					//Load TH1 to obtain require Baudrate (Refer Serial.pdf for calculations)
	SCON=0x50;			//Configure UART peripheral for 8-bit data transfer 
	TR1=1;								//Start Timer 1
	ES =1;									//Enable Serial Interrupt
	EA =1;									//Enable Global Interrupt
}


/**********************************************************
   transmit_char(<unsigned char ch>): 
	Transmits a character using UART
***********************************************************/	
void transmit_char(unsigned char ch)
{
	SBUF=ch;				//Load data in SBUF
	while(!tx_complete); 			//Wait for tx_complete flag (interrupt to complete)
	tx_complete = 0;		//Clear tx_complete flag 
}


/**********************************************************
   transmit_string(<String pointer>): 
	Transmit a string using UART
***********************************************************/	
void transmit_string(unsigned char *s)
{
	while(*s != 0)
	{
			transmit_char(*s++);
	}
}


/**********************************************************
   receive_char(): 
	Receives a character through UART. Returns a 
	character.
***********************************************************/		



/**********************************************************
   Serial_ISR(): 
	Interrupt service routine for UART interrupt.
	Determines whether it is a transmit or receive
	interrupt and raise corresponding flag.
	Transmit or receive functions (defined above) monitor
	for these flags to check if data transfer is done.
***********************************************************/	
