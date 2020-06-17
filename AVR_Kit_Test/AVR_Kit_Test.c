/*
 * AVR_Kit_Test.c
 * Created: 13/6/2020 11:21:54 PM
 * Author: Doan Huu Huong 20166262
 *         Dien tu 05 K61
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define FRE 8
#include "hunget_uart.h"
#include "hunget_adc.h"
#include "hunget_lcd.h"
#include "LCD_master.h"

unsigned char  push_button = 0;
#include "thu_vien_rieng.h"

int main(void)
{	
	/* Day la ham chinh, chay module nao thi bo dau // truoc cam ham trong module day */
	
	/************************** 3.2 Dieu khien cong ra so *****************************/
		//INIT();
		//PORT();
		
	/********************* 3.3 Doc trang thai Logic dau vao so ************************/	
		//PB_2_LED();
		
	/******************** 3.4 Do dien ap tuong tu, hien thi LCD ***********************/
		//ADC_2_LCD();
		
    /******************** 3.5 Giao tiep may tinh qua chuan UART ***********************/
		//UART();
		
	/**********************************************************************************
	 * Bai tap luyen tap cuoi ki, doc cam bien bui min SDS011 hien thi ra LCD va UART *
	 * SDS011 la cam bien do bui PM2.5 va PM10, giao thuc ket noi la UART 9600        *
	 * Vi ATmega16 chi co 1 UART nen chia ra Rx de doc cam bien, Tx de gui du lieu    */
		get_SDS011();
	
	return 0;
}
