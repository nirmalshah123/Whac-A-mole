#include <at89c5131.h>

void whacamole(char start)
{
	char m='m',star='*';
	char m_location,i;
	lcd_cmd(0x01);
	msdelay(4);
	if(start<=7)
	{
		m_location = 0x80 + start;
		for(i=0x80;i<=0x87;i++)
		{
			if(i!=m_location)
			{
				lcd_write_char(star);
			}
			else
			{
				lcd_write_char(m);
			}
		}
	}
	
	else
	{
		m_location = 0xc0 + start-8;
			for(i=0xc0;i<=0xc7;i++)
		{
			if(i!=m_location)
			{
				lcd_write_char(star);
			}
			else
			{
				lcd_write_char(m);
			}
		}
	}
	
}

void main(void)
{
	
}