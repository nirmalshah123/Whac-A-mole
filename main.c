 
#include <at89c5131.h>
#include "lcd.h"		//Header file with LCD interfacing functions
#include "serial.c"	//C file with UART interfacing functions

unsigned int count=0,temp_count=0;
unsigned char max=0,score=0;


void score_card()
{
	char score_string[]="Score: ";
	char max_string[]="High Score:";
	if(score>max)
		max=score;
	lcd_cmd(0x01);
	lcd_cmd(0x80);
	lcd_write_string(score_string);
	lcd_write_char(score + 0x30);
	lcd_cmd(0xc0);
	lcd_write_string(max_string);
	lcd_write_char(max + 0x30);
	msdelay(3000);
	lcd_cmd(0x01);
	score=0;
	
}

void timer_isr() interrupt 1
{

		count++;
		TR0 =0 ;
		TH0 = 0x3c;										//Setting the value to run the T1 timer for 50,000 times
		TL0 = 0xb0;
		if(count ==4000)
		{			
			temp_count=count;
		}TR0 = 1;
	
}

void init_whacamole(unsigned char start)
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


void start(void)
{
			
			char get_ready[]="Get Ready";
			char star[]="********";
	
			lcd_cmd(0x84);	//Move cursor to Row 1 column 2
			msdelay(4);
			lcd_write_string(get_ready);
			msdelay(2000);
			lcd_cmd(0x01);	//Clear LCD screen
			msdelay(4);
			lcd_cmd(0x84);	//Move cursor to Row 1 column 2
			lcd_write_string(star);
			lcd_cmd(0xC4);	//Move cursor to Row 2 column 2
			lcd_write_string(star);
			msdelay(3000);
			lcd_cmd(0x01);
			msdelay(4);
}
unsigned char receive_char(void)
{
	unsigned char ch = 0;
/*	while(!rx_complete)				//Wait for rx_complete(interrupt to complete)
	{
		if(count == 200)
		{
			temp_count=count;
			TR0=0;
			SBUF = 'm';
			break;
		}
	}*/
	rx_complete = 0;
	ch = SBUF;					//Read data from SBUF
	return ch;					//Return read character
}

void serial_ISR(void) interrupt 4
{
		if(TI==1)			//check whether TI is set
		{
			TI = 0;			//Clear TI flag
			tx_complete = 1;	//Set tx_complete flag indicating interrupt completion
		}
		else if(RI==1)			//check whether RI is set
		{
			RI = 0;			//Clear RI flag
			rx_complete = 1;	//Set rx_complete flag indicating interrupt completion
		}
}

void main()
{		
		unsigned char B=5;
		unsigned char m_loc=B,i,ch,B1,temp_B1;
		char map[]={'q','w','e','r','t','y','u','i','a','s','d','f','g','h','j','k'};
			char score_string[]="Score: ";
	char max_string[]="High Score:";
		
		PSL=0;
		PT0L=1;
		
		lcd_init();
	uart_init();
	transmit_string("Loading the game\r\n");
		ET0 = 1;
				count=0;
				TH0 = 0x3c;										//Setting the value to run the T1 timer for 50,000 times
				TL0 = 0xb0;
			TR0 = 1;
		while(1)
		{
				init_whacamole(m_loc);
			
			if(temp_count!=4000)
			{
				ch = receive_char();
				transmit_char(ch);
			

				for(i=0;i<16;i++)
			{
				if(map[i] == ch)
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
			{score_card();
				count=0;
				temp_count=0;
				TH0 = 0x3c;
				TL0 = 0xb0;
				TR0=1;
			}
		

	}
		
}

