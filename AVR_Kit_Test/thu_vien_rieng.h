/*
 * thu_vien_rieng.h
 *
 * Created: 3/15/2020 9:05:49 AM
 * Author: Doan Huu Huong 20166262
 *         Dien tu 05 K61
 */ 


#ifndef THU_VIEN_RIENG_H_
#define THU_VIEN_RIENG_H_
#include <util/delay.h>
void INIT();
void PORT();
void LED7_OUT(uint8_t num);
void delay_ms(unsigned int count);
void DELAY_MS(unsigned int mili_count);
void PB_2_LED();
uint8_t PB_CHECK();
void ADC_2_LCD();
void UART();
void LCD4_DIS_SHIFT(unsigned char lcd4_direct, unsigned char lcd4_step);
void INIT()
{
	//Khoi tao trang thai output cho cac chan noi toi cac led don
	DDRD |= 0xFF;
	PORTD |= 0xFF;

	//LED 7 thanh
	DDRC |= 0xFF;
	PORTC |= 0xFF;
	
	// Khoi tao input cho dau vao ADC
	//DDRA = 0x00;
	//PORTA = 0x00;
	
	//ADC_PRES(128);
	//ADC_AVCC();
	//ADC_IN(0);
}
	
void PORT()
{
	unsigned char led_shift = 255;
	unsigned char led_7_count = 0; 
	
	while(1)
	{
		PORTD = led_shift;
		if(led_shift != 0)
			led_shift = led_shift << 1;
		else
			led_shift = 255;
			
		LED7_OUT(led_7_count);
		
		PORTC ^= (1 << PC3);
		led_7_count = led_7_count+1;
		if (led_7_count > 9)
		{
			led_7_count = 0;
		}
		delay_ms(1000);
	}
}
void LED7_OUT(uint8_t num)
{
	uint8_t temp = PORTC;
	temp &= 0B00001000;
	switch(num)
	{
		case 0: temp |= 0B10000000; break;
		case 1: temp |= 0B11100011; break;
		case 2: temp |= 0B01000100; break;
		case 3: temp |= 0B01000001; break;
		case 4: temp |= 0B00100011; break;
		case 5: temp |= 0B00010001; break;
		case 6: temp |= 0B00010000; break;
		case 7: temp |= 0B11000011; break;
		case 8: temp |= 0B00000000; break;
		case 9: temp |= 0B00000001; break;
	}
	PORTC = temp;
}
void delay_ms(unsigned int count)
{
	unsigned int i;
	for(i=0;i<count;i++){
		_delay_ms(8);
	}
}

void DELAY_MS(unsigned int mili_count)// Ham delay nay ko chay
{
	unsigned int i,j;
	mili_count = mili_count * FRE;
	for (i = 0; i<mili_count; i++)
		{
			for(j = 0; j < 53; j++)
			{
			//NULL
			}
		}
}
void PB_2_LED()
{
	DDRD |= 0xFF;
	PORTD |= 0xFF;
	DDRC |= 0xFF;
	PORTC |= 0xFF;
	while(1)
	{
		push_button = PB_CHECK();
		LED7_OUT(push_button);
		switch(push_button)
		{
			case 1: PORTD = 0b11111100; break;	
			case 2: PORTD = 0b11110011; break;	
			case 3: PORTD = 0b11001111; break;
			case 4: PORTD = 0b00111111; break;			
			default: PORTD = 0xFF;	
		}
	}
}
uint8_t PB_CHECK(push_button)
{
	if ((PINB & 0x0F) != 0x0F)
	{
		if(!(PINB & (1<<PB0)))
			return 1;
		
		if(!(PINB & (1<<PB1)))
			return 2;
			
		if(!(PINB & (1<<PB2)))
			return 3;
			
		if(!(PINB & (1<<PB3)))
			return 4;
	}
	return push_button;
}
void ADC_2_LCD()
{
	DDRA = 0x00;
	PORTA = 0x00;
		
	ADC_PRES(128);
	ADC_AVCC();
	ADC_IN(0);
	
	DDRD |= (1<<PD5);
	PORTD &= ~(1<<PD5);
	PORTC |= 0x0F;
	
	LCD_Init();			
	LCD_Clear();
	LCD_Command(0x81);
	LCD_String("by: Huu Huong");
	
	LCD_Command(0xC0);
	LCD_String("ADC: 0000/1024");
	char LCDprint[30];
	while(1)
	{
		ADC_STA_CONVERT();
		LCD_Command(0xC5);
		sprintf(LCDprint,"%4d",ADC);
		LCD_String(LCDprint);
		delay_ms(500);
	}
}
void UART()
{
	UART_INIT(51, 8, 0 ,1);
	DDRD |= (1<PD5);
	PORTD &= ~(1<<PD5);
	PORTC |= 0x0F;
	
	LCD_Init();			/* Initialize LCD */
	LCD_Clear();
	LCD_Command(0x80);
	LCD_String("Huu Huong, 20166262, DT05-k61");
	LCD_Command(0xC0);
	LCD_String("Vien DTVT, Truong DHBK Ha Noi");
	
	DELAY_MS(1000);
	
	while(1)
	{
		UART_TRAN_STR("Doan Huu Huong, 20166262, DTVT05-K61");
		UART_TRAN_BYTE(13);
		UART_TRAN_BYTE(10);
		
		UART_TRAN_STR("Vien DTVT, Truong DHBK Ha Noi");
		UART_TRAN_BYTE(13);
		UART_TRAN_BYTE(10);
		
		UART_TRAN_BYTE(13);
		UART_TRAN_BYTE(10);
		
		delay_ms(1000);
		
		LCD_SHIFT(36);
	}
}
void LCD4_DIS_SHIFT(unsigned char lcd4_direct, unsigned char lcd4_step)
{
	unsigned char i;
	if(lcd4_direct == 0)
		for(i = 0; i< lcd4_step; i++)
			LCD4_OUT_CMD(0x1C);
	else
		for(i = 0; i< lcd4_step; i++)
			LCD4_OUT_CMD(0x18);
}
void LCD_SHIFT(unsigned char shift)
{
	unsigned char  i;
	for(i=0;i<shift;i++)
	{
		LCD_Command(0x1c);/* shift entire display right */
	}	
}
#endif /* THU_VIEN_RIENG_H_ */