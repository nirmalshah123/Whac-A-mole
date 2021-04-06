 
#include <at89c5131.h>
#include "lcd.h"		//Header file with LCD interfacing functions
#include "serial.c"	//C file with UART interfacing functions

int count=0;
unsigned char stop = 0;
unsigned int max=0,score=0;

void score_card()
{
	unsigned char score_hundreds,score_tens,score_units;
	unsigned char max_hundreds,max_tens,max_units;
	unsigned int max_temp;
	char score_string[]="Score: ";				//String for printing out score
	char max_string[]="High Score: ";
	if(score>max)													//Checking if our score is greater than the max score or not
		max=score;
	max_temp = max;												//temperory variable to save max score
	lcd_cmd(0x01);												//Clearing the screen
	lcd_cmd(0x80);												//Moving the cursor to the start
	lcd_write_string(score_string);				//Printing out the string
	score_units = score%10;								//Finding out the units, tens, hundreds place
	score_tens		 = score/10;
	score_hundreds	 = score_tens/10;
	score_tens     = score_tens%10;
	lcd_write_char(score_hundreds + 0x30);	//Printing the found out digits by adding 0x30 to get ascii code of each digits
	lcd_write_char(score_tens + 0x30);
	lcd_write_char(score_units + 0x30);
	lcd_cmd(0xc0);
	
	lcd_write_string(max_string);					//Printing out the max string
	max_units = max_temp%10;							//Finding out the digits of the max 
	max_tens		 = max_temp/10;
	max_hundreds	 = max_tens/10;
	max_tens     = max_tens%10;
	
	
	lcd_write_char(max_hundreds + 0x30);	//Printing them out
	lcd_write_char(max_tens + 0x30);
	lcd_write_char(max_units + 0x30);
	msdelay(3000);												//Giving a delay of 3 seconds
	lcd_cmd(0x01);												//Clearing the screen
	score=0;															//Reseting the score to 0
	
}
void whacamole(unsigned char start)			//Whacamole function to print the strings "m" and "*"
{
	char star[]="********";							
	unsigned char m_location,i;
	if(start<=0x07)												//If the position of m is less than 7 then it's on 1st row. We need to print the string now
	{
		lcd_cmd(0x80);											//Cursor to row 1 and column 1
		msdelay(4);
		m_location = 0x80 + start;					//Assigning the location of m
		for(i=0x80;i<=0x87;i++)							//Printing the string which would contain "m" and "*"
		{
			if(i!=m_location)
			{
	
				lcd_write_char('*');
				msdelay(4);
			}
			else
			{
				lcd_write_char('m');
				msdelay(4);
			}
			
		}
		lcd_cmd(0xc0);
		lcd_write_string(star);
	}
	
	else if(start>=0x08)										//If start is greater than 8 than m is on 2nd line
	{
		lcd_cmd(0x80);												//Printing the string star on the 1st line
		lcd_write_string(star);
		lcd_cmd(0xc0);
		msdelay(4);
		m_location = 0xc0 + start-0x08;
			for(i=0xc0;i<=0xc7;i++)
		{
			if(i!=m_location)
			{
				lcd_write_char('*');
				msdelay(4);
			}
			else
			{
				lcd_write_char('m');
				msdelay(4);
			}
		}
	}
}

void timer_isr() interrupt 1						//Timer intterput used for running the code for 10 seconds
{

		count = count + 1;									//Increasing the count by 1
		TR0 =0 ;														// Initialising the values of TR =0 
		TH0 = 0x3c;													//Setting the value to run the T1 timer for 50,000 times
		TL0 = 0xb0;							
		if(count - 400 ==0)								//If count = 4000 then it would have run for 10 seconds 
		{			
			stop = 1;													//setting stop to 1, meaning we have completed our 10 seconds
			P3_0 = 1;
		}
		TR0 = 1;														//Again starting the timer
	
}




void start(void)												// This function would print the starting things of the game such as "Get Ready" and 2 rows of "********"
{
			
			char get_ready[]="Get Ready";
			char star[]="********";
			lcd_cmd(0x01);										//Clearing the screen
			lcd_cmd(0x84);										//Move cursor to Row 1 column 4
			msdelay(4);
			lcd_write_string(get_ready);			//Writing the string "Get Ready"
			msdelay(2000);										//Giving a delay of 2 seconds
			lcd_cmd(0x01);										//Clear LCD screen
			msdelay(4);								
			lcd_cmd(0x84);										//Move cursor to Row 1 column 4
			lcd_write_string(star);						//Printing the string star 
			lcd_cmd(0xC4);										//Move cursor to Row 2 column 4
			lcd_write_string(star);						//Printing the string star
			msdelay(3000);										//Giving a delay of 3 seconds
			lcd_cmd(0x01);										//Clearing the screen
			msdelay(4);
}



void main()
{		
		unsigned char B=2;									//Initial position of m
		unsigned char m_loc=B,ch,B1,temp_B1;
		char map[]={'q','w','e','r','t','y','u','i','a','s','d','f','g','h','j','k'};
		char score_string[]="Score: ";
		char max_string[]="High Score:";
		
		lcd_init();
		uart_init();
		transmit_string("Loading the game\r\n");

		TH0 = 0x3c;										//Setting the value to run the T1 timer for 50,000 times
		TL0 = 0xb0;
		start();											//Calling start function to print "Get Ready"
		TR0=1;												//Starting the timer T0
		while(1)
		{
				whacamole(m_loc);					//Calling the function to print the string which would have m

				if(stop!=1)								//If we have not completed 10 seconds, then only accept the input from the user
				{
					ch = receive_char();		//Recieving a character
					transmit_char(ch);			//This line is for testing purposes, to check which character is typed or not
			
					
						if(map[m_loc] == ch)	//If the location in the map matches with our character then we have found correct match
						{
								score = score + 1;								//Adding it to our score
								temp_B1 = (B>>3 ^ (B & 0x01))<<3;	//Calculating (b3 XOR b0,0,0,0)
								B1 = temp_B1 + (B>>1);						//Our next state = (b3 XOR b0,b3,b2,b1)
								m_loc = (B + B1)%16;							//Finding out the position of m_loc = (B+B1)%16
								B = B1;														//Storing the next state 
						}
				}
				else											//This is where we would run if we have completed 10 seconds
				{	
					TR0=0;									//Stops the timer
					score_card();						//Call the function score_card to display the results
					count=0;								//Resting the value of count=0
					stop=0;									//Reseting the value of stop=0
					TH0 = 0x3c;							//Initialising the timer values
					TL0 = 0xb0;
					start();								//Calling the start functiob
					TR0=1;									//Starting the timer
				}
		}		
}

