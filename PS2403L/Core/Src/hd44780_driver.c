#include "hd44780_driver.h"

uint8_t pos_x = 0;
uint8_t pos_y = 0;

void strobe(void)
{
	LCD_PORT->BSRR=LCD_EN_BS;
	delay_us(2);
	LCD_PORT->BSRR=LCD_EN_BC;
	delay_us(2);
}

void lcd_init() {
	delay_ms(100);
	lcd_set_4bit_mode();
	lcd_set_state(LCD_ENABLE,CURSOR_ENABLE,NO_BLINK);
	lcd_clear();
	//lcd_send(0x06,COMMAND);
}

void lcd_set_user_char(uint8_t char_num, char * char_data) {
	uint8_t i;
	lcd_send(((1<<6) | (char_num * 8) ), COMMAND);
	for (i=0;i<=7;i++) {
		lcd_send(char_data[i],DATA);
	}
	lcd_send((1<<7), COMMAND);
}

void lcd_set_CGRAM_char(int8_t char_num, char * char_data) {
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


void lcd_out(char * txt,uint8_t TypeOfLCD) {
	uint8_t i=0;
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

	char sym[8];
	sym[0]=0b00100;
	sym[1]=0b10101;
	sym[2]=0b01110;
	sym[3]=0b00100;
	sym[4]=0b00000;
	sym[5]=0b00100;
	sym[6]=0b11111;
	sym[7]=0b11111;
	lcd_set_user_char(0,sym);//charge
	sym[0]=0b10101;
	sym[1]=0b01110;
	sym[2]=0b00100;
	sym[3]=0b00000;
	sym[4]=0b00000;
	sym[5]=0b00000;
	sym[6]=0b00100;
	sym[7]=0b11111;
	lcd_set_user_char(1,sym);//charge

	sym[0]=0b00000;
	sym[1]=0b00100;
	sym[2]=0b01110;
	sym[3]=0b10101;
	sym[4]=0b00000;
	sym[5]=0b00100;
	sym[6]=0b11111;
	sym[7]=0b11111;
	lcd_set_user_char(2,sym);//discharge

	sym[0]=0b00100;
	sym[1]=0b01110;
	sym[2]=0b10101;
	sym[3]=0b00100;
	sym[4]=0b00000;
	sym[5]=0b00000;
	sym[6]=0b00100;
	sym[7]=0b11111;
	lcd_set_user_char(3,sym);//discharge

	sym[0]=0b00100;
	sym[1]=0b01110;
	sym[2]=0b10101;
	sym[3]=0b00100;
	sym[4]=0b00100;
	sym[5]=0b10101;
	sym[6]=0b01110;
	sym[7]=0b00100;
	lcd_set_user_char(4,sym);//training

	sym[0]=0b00010;
	sym[1]=0b00111;
	sym[2]=0b00111;
	sym[3]=0b01010;
	sym[4]=0b01010;
	sym[5]=0b11100;
	sym[6]=0b11100;
	sym[7]=0b01000;
	lcd_set_user_char(5,sym);//swing

	sym[0]=0b01111;
	sym[1]=0b00101;
	sym[2]=0b00101;
	sym[3]=0b00101;
	sym[4]=0b00101;
	sym[5]=0b10101;
	sym[6]=0b01001;
	sym[7]=0b00000;
	lcd_set_user_char(6,sym);//Л

	sym[0]=0b10001;
	sym[1]=0b10001;
	sym[2]=0b10001;
	sym[3]=0b11111;
	sym[4]=0b00001;
	sym[5]=0b00001;
	sym[6]=0b00001;
	sym[7]=0b00000;
	lcd_set_user_char(7,sym);//Ч



}

char russ[]=
{
		65, //А 192
		3, //Б
		66, //В
		59, //Г
		63, //Д
		69, //Е
		93, //Ж
		51, //З
		123, //И 200
		64, //Й
		75, //К
		6, //Л
		77, //М
		72, //Н
		79, //О
		92, //П
		80, //Р
		67, //С
		84, //Т 210
		96, //У
		36, //Ф
		88, //Х
		5, //Ц
		7, //Ч
		91, //Ш
		95, //Щ
		3, //Ъ,Ї
		105, //Ы,І,73
		4, //Ь
		2, //Э
		1,//Ю
		0//Я 223
};

void LcdOutbyNumber(int8_t num,uint8_t Line)
{
	lcd_send(num,DATA);
}

void LcdOut(char * txt,uint8_t Line)
{
	//char i=0;

	while(*txt)
	{
		if(*txt==178)//І
		{
			lcd_send(73,DATA);
		}else if(*txt==175)//Ї
		{
			lcd_send(3,DATA);
		}else if(*txt=='Ю')//Ю
		{
			lcd_send(1,DATA);
		}else if(*txt==170)//Є
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
	//delay_ms(2);
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
	//delay_ms(1);
	//LCD_PORT->BSRR=LCD_EN_BC;

	strobe();
	delay_ms(1);

	LCD_PORT->BSRR=(LCD_DB7_BC | LCD_DB6_BC | LCD_DB5_BC | LCD_DB4_BC | LCD_CD_BC | LCD_EN_BC);
	LCD_PORT->BSRR=(LCD_DB5_BS);

	strobe();
	delay_ms(1);

	LCD_PORT->BSRR=(LCD_DB7_BC | LCD_DB6_BC | LCD_DB5_BC | LCD_DB4_BC | LCD_CD_BC | LCD_EN_BC);
	LCD_PORT->BSRR=(LCD_DB7_BS);

	strobe();
	delay_ms(1);

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
	delay_ms(2);
	if (dc) {
			LCD_PORT->BSRR=LCD_CD_BC;
		}

}


void PrintToLCD(char* str )
{
	LcdOut(str,2);
	//Send_String_to_UART(str);
}

void PrintToLCD_xy(char* str ,uint8_t x, uint8_t y)
{
	lcd_set_xy(x,y);

	LcdOut(str,2);
	//Send_String_to_UART(str);
}

void PrintToLCD_int32(int32_t x)
{
	PrintToLCD(itoa(x));
}
void PrintToLCD_int32_koma(int32_t x, uint8_t k)

{
	PrintToLCD(itoa_koma(x,k));
}
void InitLCD()
{



	lcd_init();
	lcd_clear();
	lcd_set_state(LCD_ENABLE, CURSOR_DISABLE, NO_BLINK);
}
