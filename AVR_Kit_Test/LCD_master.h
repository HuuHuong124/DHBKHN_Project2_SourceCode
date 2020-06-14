/*************************************************************************************/
/**************** Thu vien cua thay ko chay thi tham khao cai nay ********************/
/*********************** Doan Huu Huong 20166262 DTVT05 K61 **************************/
/*************************************************************************************/
//Tham khoa tai: https://www.electronicwings.com/avr-atmega/lcd16x2-interfacing-with-atmega16-32


#ifndef LCD_MASTER_H_
#define LCD_MASTER_H_

//#define F_CPU 8000000UL			
#include <avr/io.h>			
#include <util/delay.h>			

#define LCD_Data_Dir DDRC		
#define LCD_Command_Dir DDRD		
#define LCD_Data_Port PORTC		
#define LCD_Command_Port PORTD		
#define RS PD6				
#define RW PD5				
#define EN PD7				


void LCD_Command(unsigned char cmnd)
{
	LCD_Data_Port= cmnd;
	LCD_Command_Port &= ~(1<<RS);	
	LCD_Command_Port &= ~(1<<RW);	
	LCD_Command_Port |= (1<<EN);	
	_delay_us(1);
	LCD_Command_Port &= ~(1<<EN);
	_delay_ms(3);
}

void LCD_Char (unsigned char char_data)	
{
	LCD_Data_Port= char_data;
	LCD_Command_Port |= (1<<RS);	
	LCD_Command_Port &= ~(1<<RW);	
	LCD_Command_Port |= (1<<EN);	
	_delay_us(1);
	LCD_Command_Port &= ~(1<<EN);
	_delay_ms(1);
}

void LCD_Init (void)			
{
	LCD_Command_Dir = 0xFF;		
	LCD_Data_Dir = 0xFF;		
	_delay_ms(20);			
	
	LCD_Command (0x38);
	LCD_Command (0x0C);		
	LCD_Command (0x06);		
	LCD_Command (0x01);		
	LCD_Command (0x80);		
}

void LCD_String (char *str)		
{
	int i;
	for(i=0;str[i]!=0;i++)		
	{
		LCD_Char (str[i]);
	}
}

void LCD_String_xy (char row, char pos, char *str)
{
	if (row == 0 && pos<16)
	LCD_Command((pos & 0x0F)|0x80);	
	else if (row == 1 && pos<16)
	LCD_Command((pos & 0x0F)|0xC0);	
	LCD_String(str);		
}

void LCD_Clear()
{
	LCD_Command (0x01);		
	LCD_Command (0x80);		
	_delay_ms(10);
}

void test()
{

	LCD_Init();			
	LCD_Command(0x80);
	LCD_String("ElectronicWINGS");	
	LCD_Command(0xC0);		
	LCD_String("Hello World");	
	LCD_Command(0x1c);
	_delay_ms(300);
	LCD_Command(0x18);
	_delay_ms(300);
}



#endif /* LCD_MASTER_H_ */