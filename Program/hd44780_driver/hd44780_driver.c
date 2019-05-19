#include "hd44780_driver.h"

uint8_t pos_x = 0;
uint8_t pos_y = 0;


void DelaymSec(volatile uint16_t Count)//mSec
{
	volatile uint32_t tmpvar;
	for (tmpvar=Count*2000;tmpvar!=0;tmpvar--);
}

void DelayuSec(volatile uint16_t Count)//uSec
{
	volatile uint32_t tmpvar;
	for (tmpvar=Count*2;tmpvar!=0;tmpvar--);
}

void strobe(void)
{
	LCD_PORT->BSRR=LCD_EN_BS;
	DelayuSec(2);
	LCD_PORT->BSRR=LCD_EN_BC;
	DelayuSec(2);
}

void lcd_init() {
	DelaymSec(100);
	lcd_set_4bit_mode();
	lcd_set_state(LCD_ENABLE,CURSOR_ENABLE,NO_BLINK);
	lcd_clear();
	//lcd_send(0x06,COMMAND);
}

void lcd_set_user_char(uint8_t char_num, uint8_t * char_data) {
	uint8_t i;
	lcd_send(((1<<6) | (char_num * 8) ), COMMAND);
	for (i=0;i<=7;i++) {
		lcd_send(char_data[i],DATA);
	}
	lcd_send((1<<7), COMMAND);
}

void lcd_set_CGRAM_char(uint8_t char_num, uint8_t * char_data) {
	uint8_t i;
	lcd_send(((1<<6) | (char_num * 8) ), COMMAND);
	for (i=0;i<=7;i++) {
		lcd_send(char_data[i],DATA);
	}
	lcd_send((1<<7), COMMAND);
}
void lcd_set_xy(uint8_t x, uint8_t y)  {
	pos_x = x;
	pos_y = y;
	if (y==0) {
		lcd_send( ((1<<7) | (x)),COMMAND);
	} else {
		lcd_send( ((3<<6) | (x)),COMMAND);
	}

}


void lcd_out(char * txt,char TypeOfLCD) {
	char i=0;
	while(*txt)
	{
		lcd_send(*txt,DATA);
		if ((TypeOfLCD ==1)&&(i==7) )
			lcd_set_xy(0,1);
		txt++;
		i++;
	}
}

void SetSymbols()
{
	uint8_t sym[8];
	sym[0]=0b00100;
	sym[1]=0b10101;
	sym[2]=0b01110;
	sym[3]=0b00100;
	sym[4]=0b00000;
	sym[5]=0b00100;
	sym[6]=0b11111;
	sym[7]=0b11111;
	lcd_set_user_char(0,&sym);//charge
	sym[0]=0b10101;
	sym[1]=0b01110;
	sym[2]=0b00100;
	sym[3]=0b00000;
	sym[4]=0b00000;
	sym[5]=0b00000;
	sym[6]=0b00100;
	sym[7]=0b11111;
	lcd_set_user_char(1,&sym);//charge

	sym[0]=0b00000;
	sym[1]=0b00100;
	sym[2]=0b01110;
	sym[3]=0b10101;
	sym[4]=0b00000;
	sym[5]=0b00100;
	sym[6]=0b11111;
	sym[7]=0b11111;
	lcd_set_user_char(2,&sym);//discharge

	sym[0]=0b00100;
	sym[1]=0b01110;
	sym[2]=0b10101;
	sym[3]=0b00100;
	sym[4]=0b00000;
	sym[5]=0b00000;
	sym[6]=0b00100;
	sym[7]=0b11111;
	lcd_set_user_char(3,&sym);//discharge

	sym[0]=0b00100;
	sym[1]=0b01110;
	sym[2]=0b10101;
	sym[3]=0b00100;
	sym[4]=0b00100;
	sym[5]=0b10101;
	sym[6]=0b01110;
	sym[7]=0b00100;
	lcd_set_user_char(4,&sym);//training

	sym[0]=0b00010;
	sym[1]=0b00111;
	sym[2]=0b00111;
	sym[3]=0b01010;
	sym[4]=0b01010;
	sym[5]=0b11100;
	sym[6]=0b11100;
	sym[7]=0b01000;
	lcd_set_user_char(5,&sym);//swing

	sym[0]=0b01111;
	sym[1]=0b00101;
	sym[2]=0b00101;
	sym[3]=0b00101;
	sym[4]=0b00101;
	sym[5]=0b10101;
	sym[6]=0b01001;
	sym[7]=0b00000;
	lcd_set_user_char(6,&sym);//Ë

	sym[0]=0b10001;
	sym[1]=0b10001;
	sym[2]=0b10001;
	sym[3]=0b11111;
	sym[4]=0b00001;
	sym[5]=0b00001;
	sym[6]=0b00001;
	sym[7]=0b00000;
	lcd_set_user_char(7,&sym);//×



}

char russ[30]=
{
		65, //À 192
		3, //Á
		66, //Â
		59, //Ã
		63, //Ä
		69, //Å
		93, //Æ
		51, //Ç
		123, //È 200
		64, //É
		75, //Ê
		6, //Ë
		77, //Ì
		72, //Í
		79, //Î
		92, //Ï
		80, //Ð
		67, //Ñ
		84, //Ò 210
		96, //Ó
		36, //Ô
		88, //Õ
		5, //Ö
		7, //×
		91, //Ø
		95, //Ù
		3, //Ú,¯
		105, //Û,²,73
		4, //Ü
		2, //Ý
		1,//Þ
		0//ß 223
};

void LcdOutbyNumber(uint8_t num,char Line)
{
	lcd_send(num,DATA);
}

void LcdOut(char * txt,char Line)
{
	//char i=0;

	while(*txt)
	{
		if(*txt==178)//²
		{
			lcd_send(73,DATA);
		}else if(*txt==175)//¯
		{
			lcd_send(3,DATA);
		}else if(*txt=='Þ')//Þ
		{
			lcd_send(1,DATA);
		}else if(*txt==170)//ª
		{
			lcd_send(2,DATA);
		}else if(*txt<192)
		{
			lcd_send(*txt,DATA);
		}else
		{
			lcd_send(russ[*txt-192],DATA);
		}

		txt++;
		pos_x++;
		if ((pos_y == 0 )&&(pos_x == 8) &&(Line ==2))
		{
			lcd_set_xy(0,1);
			pos_x = 0;
			pos_y = 1;
		}
		if ((pos_y == 1 )&&(pos_x == 8) )
		{
			//lcd_set_xy(0,0);
			//pos_x = 0;
			//pos_y = 0;
		}


	}
}

void lcd_clear(void) {
	lcd_send(0x01,COMMAND);
	//DelaymSec(2);
}

void lcd_set_state(lcd_state state, cursor_state cur_state, cursor_mode cur_mode)  {
	if (state==LCD_DISABLE)  {
		lcd_send(0x08,COMMAND);
	} else {
		if (cur_state==CURSOR_DISABLE) {
			if (cur_mode==NO_BLINK)  {
				lcd_send(0x0C,COMMAND);
			} else {
				lcd_send(0x0D,COMMAND);
			}
		} else  {
			if (cur_mode==NO_BLINK)  {
				lcd_send(0x0E,COMMAND);
			} else {
				lcd_send(0x0F,COMMAND);
			}
		}
	}
}

void lcd_set_4bit_mode(void) {


	LCD_PORT->BSRR=(LCD_DB7_BC | LCD_DB6_BC | LCD_DB5_BC | LCD_DB4_BC | LCD_CD_BC | LCD_EN_BC);
	LCD_PORT->BSRR=(LCD_DB5_BS);

	//LCD_PORT->BSRR=LCD_EN_BS;
	//DelaymSec(1);
	//LCD_PORT->BSRR=LCD_EN_BC;

	strobe();
	DelaymSec(1);

	LCD_PORT->BSRR=(LCD_DB7_BC | LCD_DB6_BC | LCD_DB5_BC | LCD_DB4_BC | LCD_CD_BC | LCD_EN_BC);
	LCD_PORT->BSRR=(LCD_DB5_BS);

	strobe();
	DelaymSec(1);

	LCD_PORT->BSRR=(LCD_DB7_BC | LCD_DB6_BC | LCD_DB5_BC | LCD_DB4_BC | LCD_CD_BC | LCD_EN_BC);
	LCD_PORT->BSRR=(LCD_DB7_BS);

	strobe();
	DelaymSec(1);

}

void lcd_send(uint8_t byte, dat_or_comm dc)  {

	LCD_PORT->BSRR=(LCD_DB7_BC | LCD_DB6_BC | LCD_DB5_BC | LCD_DB4_BC | LCD_CD_BC | LCD_EN_BC);

	if (dc) {
		LCD_PORT->BSRR=LCD_CD_BS;
	}

	if (byte & 0x10) {
		LCD_PORT->BSRR=LCD_DB4_BS;
	}
	if (byte & 0x20) {
		LCD_PORT->BSRR=LCD_DB5_BS;
	}
	if (byte & 0x40) {
		LCD_PORT->BSRR=LCD_DB6_BS;
	}
	if (byte & 0x80) {
		LCD_PORT->BSRR=LCD_DB7_BS;
	}

	strobe();


	LCD_PORT->BSRR=(LCD_DB7_BC | LCD_DB6_BC | LCD_DB5_BC | LCD_DB4_BC );

	if (byte & 0x01) {
		LCD_PORT->BSRR=LCD_DB4_BS;
	}
	if (byte & 0x02) {
		LCD_PORT->BSRR=LCD_DB5_BS;
	}
	if (byte & 0x04) {
		LCD_PORT->BSRR=LCD_DB6_BS;
	}
	if (byte & 0x08) {
		LCD_PORT->BSRR=LCD_DB7_BS;
	}



	strobe();
	DelaymSec(2);
	if (dc) {
			LCD_PORT->BSRR=LCD_CD_BC;
		}

}


void PrintOnLCD_(uint8_t* str,char line )
{


	LcdOut(str,line);
	//Send_String_to_UART(str);
}

void PrintOnLCD_xy_(uint8_t* str ,uint8_t x, uint8_t y)
{
	lcd_set_xy(x,y);

	LcdOut(str,2);
	//Send_String_to_UART(str);
}
