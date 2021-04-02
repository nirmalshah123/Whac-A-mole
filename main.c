 
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
	if(start<=0x07)
	{
		lcd_cmd(0x80);
		msdelay(4);
		m_location = 0x80 + start;
		for(i=0x80;i<=0x87;i++)
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
	
	else if(start>=0x08)
	{
		lcd_cmd(0x80);
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
		if(count - 4000 ==0)								//If count = 4000 then it would have run for 10 seconds 
		{			
			stop = 1;													//setting stop to 1, meaning we have completed our 10 seconds
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
		unsigned char B=9;									//Initial position of m
		unsigned char m_loc=B,i,ch,B1,temp_B1;
		char map[]={'q','w','e','r','t','y','u','i','a','s','d','f','g','h','j','k'};
		char score_string[]="Score: ";
		char max_string[]="High Score:";
		
		lcd_init();
		uart_init();
		transmit_string("Loading the game\r\n");

		TH0 = 0x3c;										//Setting the value to run the T1 timer for 50,000 times
		TL0 = 0xb0;
		//start();
		TR0=1;
		while(1)
		{
				whacamole(m_loc);

				if(stop!=1)
				{
					ch = receive_char();
					transmit_char(ch);
			
					for(i=0;i<16;i++)
					{
						if(map[m_loc] == ch)
						{	
							if(m_loc == i)
							{
								score = score + 1;
								lcd_write_char(score + 0x30);
								msdelay(4);
								temp_B1 = (B>>3 ^ (B & 0x01))<<3;
								B1 = temp_B1 + (B>1);
								m_loc = (B + B1)%16;
								B = B1;
							}
						}
					}
				}
				else
				{	
					TR0=0;
					score_card();
					count=0;
					stop=0;
					TH0 = 0x3c;
					TL0 = 0xb0;
					//start();
					TR0=1;
				}
		}		
}

