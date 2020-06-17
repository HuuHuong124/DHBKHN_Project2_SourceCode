/*
 * thu_vien_rieng.h
 *
 * Created: 3/15/2020 9:05:49 AM
 * Author: Doan Huu Huong 20166262
 *         Dien tu 05 K61
 */ 


#ifndef THU_VIEN_RIENG_H_
#define THU_VIEN_RIENG_H_
uint8_t pre =8; // xung nhip bang 8MHz
#include <util/delay.h>
#include <avr/interrupt.h>
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

void delay_ms(unsigned int count) // Ham delay nay toi viet lai
{
	unsigned int i;
	count = pre*count; // pre = 8 do PRE 8MHz
	for(i=0;i<count;i++){
		_delay_ms(1);
	}
}

void DELAY_MS(unsigned int mili_count)// Ham delay nay cua thay ko chay
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
	
	LCD_Init();			/* Khoi tao LCD */
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
// ham chay chu LCD duoc viet Lai
void LCD_SHIFT(unsigned char shift)
{
	unsigned char  i;
	for(i=0;i<shift;i++)
	{
		LCD_Command(0x1c);/* dich LCD */
	}	
}

// Bai tap luyen tap cuoi ki, doc cam bien bui min SDS011 hien thi ra LCD va UART
// SDS011 la cam bien do bui PM2.5 va PM10, giao thuc ket noi la UART 9600
// Vi ATmega16 chi co 1 UART nen toi chia ra Rx de doc cam bien, Tx de gui du lieu di
void get_SDS011()
{
	UART_INIT(51, 8, 0 ,1);
	char sdsPm25;
	char sdsPm10;
	uint8_t UARTprint[30];
	LCD_Init();			
	LCD_Clear();
	LCD_Command(0x80);
	LCD_String("Pm2.5 = ---- ug");
	LCD_Command(0xC0);
	LCD_String("Pm 10 = ---- ug");
	while(1)
	{
		//UART_TRAN_STR("-------Lay du lieu------  ");
		uint8_t sdsData = 0;
		uint8_t i = 0;
		uint8_t sdsDust[10] = {0};
		uint8_t sdsChecksum = 0;
		while ((UCSRA &(1<<RXC)) != 0)
		{
			sdsData = UDR;
			delay_ms(2);// d?i l?y d? li?u
			if(sdsData == 0xAA) //head1 ok
			{
				sdsDust[0] =  sdsData;
				sdsData = UDR;
				if(sdsData == 0xc0)//head2 ok
				{
					sdsDust[1] =  sdsData;
					sdsChecksum = 0;
					for(i=0;i < 6;i++)//lay data
					{
						sdsDust[i+2] = USART_ReceiveByte();
						sdsChecksum += sdsDust[i+2];
					}
					sdsDust[8] = USART_ReceiveByte();
					sdsDust[9] = USART_ReceiveByte();
					
					//debug du lieu nhan duoc
					/*
					sprintf(UARTprint,"%5x",sdsDust[0]);
					UART_TRAN_STR(UARTprint);
					sprintf(UARTprint,"%5x",sdsDust[1]);
					UART_TRAN_STR(UARTprint);
					sprintf(UARTprint,"%5x",sdsDust[2]);
					UART_TRAN_STR(UARTprint);
					sprintf(UARTprint,"%5x",sdsDust[3]);
					UART_TRAN_STR(UARTprint);
					sprintf(UARTprint,"%5x",sdsDust[4]);
					UART_TRAN_STR(UARTprint);
					sprintf(UARTprint,"%5x",sdsDust[5]);
					UART_TRAN_STR(UARTprint);
					sprintf(UARTprint,"%5x",sdsDust[6]);
					UART_TRAN_STR(UARTprint);
					sprintf(UARTprint,"%5x",sdsDust[7]);
					UART_TRAN_STR(UARTprint);
					sprintf(UARTprint,"%5x",sdsDust[8]);
					UART_TRAN_STR(UARTprint);	
					sprintf(UARTprint,"%5x",sdsDust[9]);
					UART_TRAN_STR(UARTprint);
					UART_TRAN_BYTE(13);
					UART_TRAN_BYTE(10);
					sprintf(UARTprint,"%5x",sdsChecksum);
					UART_TRAN_STR(UARTprint);
					UART_TRAN_BYTE(13);
					UART_TRAN_BYTE(10);
					*/
					if(sdsChecksum == sdsDust[8])  //ok
					{
						sdsPm25 = (sdsDust[3]*256 + sdsDust[2])/10;
						sdsPm10 = (sdsDust[5]*256 + sdsDust[4])/10;
						
						// hien thi ra UART
						sprintf(UARTprint,"%4d",sdsPm25);
						UART_TRAN_STR("Pm2.5 = ");
						UART_TRAN_STR(UARTprint);
						UART_TRAN_BYTE(13);
						UART_TRAN_BYTE(10);
						// hien thi ra LCD
						LCD_Command(0x88);
						LCD_String(UARTprint);
						
						sprintf(UARTprint,"%4d",sdsPm10);
						UART_TRAN_STR("Pm10  = ");
						UART_TRAN_STR(UARTprint);
						UART_TRAN_BYTE(13);
						UART_TRAN_BYTE(10);
						LCD_Command(0xC8);
						LCD_String(UARTprint);
						delay_ms(1000);
					}
				}
			}
		}
	}
}


#endif /* THU_VIEN_RIENG_H_ */