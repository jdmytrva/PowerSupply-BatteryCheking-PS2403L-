#ifndef _HD44780_DRIVER_
#define _HD44780_DRIVER_
#include "Lib.h"
#include "General.h"
/////should be defined by user /////
#define LCD_PORT GPIOB
#define LCD_CD 15
#define LCD_EN 14
#define LCD_DB4 13
#define LCD_DB5 12
#define LCD_DB6 11
#define LCD_DB7 10
///////////////////////////////////

extern char russ[];


typedef enum {
	COMMAND = 0,
	DATA = !COMMAND
} dat_or_comm;

typedef enum {
	LCD_DISABLE = 0,
	LCD_ENABLE = !LCD_DISABLE
} lcd_state;

typedef enum {
	CURSOR_DISABLE = 0,
	CURSOR_ENABLE = !CURSOR_DISABLE
} cursor_state;

typedef enum {
	NO_BLINK = 0,
	BLINK = !NO_BLINK
} cursor_mode;

typedef enum SpecialSymbols {

    CHARGE_H=1,
    DISCHARGE_l=2,
    DISCHARGE_H=3,
    TRAINING_ = 4

}SpecialChars ;

#define LCD_EN_BS (1 << LCD_EN)
#define LCD_CD_BS (1 << LCD_CD)
#define LCD_DB4_BS (1 << LCD_DB4)
#define LCD_DB5_BS (1 << LCD_DB5)
#define LCD_DB6_BS (1 << LCD_DB6)
#define LCD_DB7_BS (1 << LCD_DB7)

#define LCD_EN_BC (1 << (LCD_EN+16))
#define LCD_CD_BC (1 << (LCD_CD+16))
#define LCD_DB4_BC (1 << (LCD_DB4+16))
#define LCD_DB5_BC (1 << (LCD_DB5+16))
#define LCD_DB6_BC (1 << (LCD_DB6+16))
#define LCD_DB7_BC (1 << (LCD_DB7+16))

#if LCD_EN > 7
	#define LCD_EN_H  (1 << (LCD_EN-8) * 4)
	#define C_LCD_EN_H (7 << ((LCD_EN-8) * 4 + 1))
#else
	#define LCD_EN_L (1 << LCD_EN * 4)
	#define C_LCD_EN_L (7 << (LCD_EN  * 4 + 1))
#endif

#if LCD_CD > 7
	#define LCD_CD_H  (1 << (LCD_CD-8) * 4)
	#define C_LCD_CD_H  (7 << ((LCD_CD-8) * 4 + 1))
#else
	#define LCD_CD_L (1 << LCD_CD * 4)
	#define C_LCD_CD_L (7 << (LCD_CD * 4 + 1))
#endif

#if LCD_DB4 > 7
	#define DB4_H  (1 << (LCD_DB4-8) * 4)
	#define C_DB4_H  (7 << ((LCD_DB4-8) * 4 + 1))
#else
	#define DB4_L (1 << LCD_DB4 * 4)
	#define C_DB4_L  (7 << (LCD_DB4 * 4 + 1))
#endif

#if LCD_DB5 > 7
	#define DB5_H  (1 << (LCD_DB5-8) * 4)
	#define C_DB5_H  (7 << ((LCD_DB5-8) * 4 + 1))
#else
	#define DB5_L (1 << LCD_DB5 * 4)
	#define C_DB5_L  (7 << (LCD_DB5 * 4 + 1))
#endif

#if LCD_DB6 > 7
	#define DB6_H  (1 << (LCD_DB6-8) * 4)
	#define C_DB6_H  (7 << ((LCD_DB6-8) * 4 + 1))
#else
	#define DB6_L (1<< LCD_DB6 * 4)
	#define C_DB6_L  (7 << (LCD_DB6 * 4 + 1))
#endif

#if LCD_DB7 > 7
	#define DB7_H  (1 << (LCD_DB7-8) * 4)
	#define C_DB7_H  (7 << ((LCD_DB7-8) * 4 + 1))
#else
	#define DB7_L (1 << LCD_DB7 * 4)
	#define C_DB7_L  (7 << (LCD_DB7 * 4 + 1))
#endif


#ifndef DB4_H
	#define DB4_H 0
#endif

#ifndef DB5_H
	#define DB5_H 0
#endif

#ifndef DB6_H
	#define DB6_H 0
#endif

#ifndef DB7_H
	#define DB7_H 0
#endif

#ifndef DB4_L
	#define DB4_L 0
#endif

#ifndef DB5_L
	#define DB5_L 0
#endif

#ifndef DB6_L
	#define DB6_L 0
#endif

#ifndef DB7_L
	#define DB7_L 0
#endif

#ifndef LCD_CD_L
	#define LCD_CD_L 0
#endif

#ifndef LCD_CD_H
	#define LCD_CD_H 0
#endif

#ifndef LCD_EN_L
	#define LCD_EN_L 0
#endif

#ifndef LCD_EN_H
	#define LCD_EN_H 0
#endif

////////////////////////

#ifndef C_DB4_H
	#define C_DB4_H 0
#endif

#ifndef C_DB5_H
	#define C_DB5_H 0
#endif

#ifndef C_DB6_H
	#define C_DB6_H 0
#endif

#ifndef C_DB7_H
	#define C_DB7_H 0
#endif

#ifndef C_DB4_L
	#define C_DB4_L 0
#endif

#ifndef C_DB5_L
	#define C_DB5_L 0
#endif

#ifndef C_DB6_L
	#define C_DB6_L 0
#endif

#ifndef C_DB7_L
	#define C_DB7_L 0
#endif


#ifndef C_LCD_CD_L
	#define C_LCD_CD_L 0
#endif

#ifndef C_LCD_CD_H
	#define C_LCD_CD_H 0
#endif

#ifndef C_LCD_EN_L
	#define C_LCD_EN_L 0
#endif

#ifndef C_LCD_EN_H
	#define C_LCD_EN_H 0
#endif

#define LCD_PORT_CRH_S (DB4_H | DB5_H | DB6_H | DB7_H | LCD_EN_H | LCD_CD_H)
#define LCD_PORT_CRL_S (DB4_L | DB5_L | DB6_L | DB7_L | LCD_EN_L | LCD_CD_L)

#define LCD_PORT_CRH_C (C_DB4_H | C_DB5_H | C_DB6_H | C_DB7_H | C_LCD_EN_H | C_LCD_CD_H)
#define LCD_PORT_CRL_C (C_DB4_L | C_DB5_L | C_DB6_L | C_DB7_L | C_LCD_EN_L | C_LCD_CD_L)


void lcd_set_CGRAM_char(int8_t char_num, char * char_data);
void lcd_init();
void lcd_send(uint8_t byte, dat_or_comm dc);
void lcd_set_4bit_mode(void);
void lcd_set_state(lcd_state state, cursor_state cur_state, cursor_mode cur_mode);
void lcd_clear(void);
void lcd_out(char * txt,uint8_t TypeOfLCD);
void LcdOut(char * txt,uint8_t TypeOfLCD);
void LcdOutbyNumber(int8_t num,uint8_t Line);
void lcd_set_xy(uint8_t x, uint8_t y);
void lcd_set_user_char(uint8_t char_num, char * char_data);
void PrintToLCD(char* str);
void PrintToLCD_int32(int32_t x);
void PrintToLCD_int32_koma(int32_t x, uint8_t k);
void SetSymbols();
void strobe(void);
void PrintToLCD_xy(char* str ,uint8_t x, uint8_t y);
//void SetSymbolsLevel(uint8_t Level[]);

void InitLCD();
#endif
