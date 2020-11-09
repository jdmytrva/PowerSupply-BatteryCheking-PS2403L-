#ifndef _GENERAL_
#define _GENERAL_
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_exti.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_dma.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx.h"
#include "stm32f1xx_ll_gpio.h"
#include "lib.h"
#include "hd44780_driver.h"
#include "usart.h"
#include "Lib.h"
#include "Eeprom.h"
//


//#include "usart.h"







#define NUMBER_OF_CHANNELS 8
__IO uint16_t RegularConvData[NUMBER_OF_CHANNELS] ;

volatile uint16_t U_OUT;
volatile uint16_t U_PS;
volatile uint16_t U_IN;
volatile uint16_t U_Controller;
volatile uint16_t U_12V;
volatile int16_t Current;










struct StructCalibrationValuetoSaveInFlash
{
	uint32_t CRC_data;
	uint32_t CRC_AddresInEEprom;

	uint32_t Calibration0ValueForCurrent;
	uint32_t Calibration0ValueForCurrent_AddresInEEprom;

	uint32_t Calibration0ValueForCurrent1;
	uint32_t Calibration0ValueForCurrent1_AddresInEEprom;

	uint32_t CalibrationValueForCurrent;
	uint32_t CalibrationValueForCurrent_AddresInEEprom;

	uint32_t CalibrationValueForCurrent1;
	uint32_t CalibrationValueForCurrent1_AddresInEEprom;

	uint32_t CalibrationValueForVoltage;
	uint32_t CalibrationValueForVoltage_AddresInEEprom;

	uint32_t CalibrationValueForVoltage1;
	uint32_t CalibrationValueForVoltage1_AddresInEEprom;

	uint32_t CalibrationValueForVoltage2;
	uint32_t CalibrationValueForVoltage2_AddresInEEprom;

	uint32_t ResistanceComp_Ishunt_Wires;
	uint32_t ResistanceComp_Ishunt_Wires_AddresInEEprom;

	uint32_t ResistanceComp_MOSFET;
	uint32_t ResistanceComp_MOSFET_AddresInEEprom;
};

struct StructSettingsValuetoSaveInFlash
{
	uint32_t CRC_data;
	uint32_t CRC_AddresInEEprom;

	uint32_t Option1;
	uint32_t Option1_AddresInEEprom;
//
	uint32_t ChargeTime;
	uint32_t ChargeTime_AddresInEEprom;

	uint32_t LowVoltage;
	uint32_t LowVoltage_AddresInEEprom;

	uint32_t MaxVoltage;
	uint32_t MaxVoltage_AddresInEEprom;

	uint32_t Swing_Chrg_time;
	uint32_t Swing_Chrg_time_AddresInEEprom;

	uint32_t Swing_DChrg_time;
	uint32_t Swing_DChrg_time_AddresInEEprom;

	uint32_t ChargeAdapt;
	uint32_t ChargeAdapt_AddresInEEprom;

	uint32_t Option2;
	uint32_t Option2_AddresInEEprom;

};
struct StructValuetoSaveInFlashWhenPowerOFF
{
	uint32_t CRC_data;
	uint32_t CRC_AddresInEEprom;

	uint32_t BatteryCapacityDischargePreviousValue;
	uint32_t BatteryCapacityDischargePrevious_AddresInEEprom;

	uint32_t BatteryCapacityDischargeCurrent;
	uint32_t BatteryCapacityDischargeCurrent_AddresInEEprom;

	uint32_t OutState;
	uint32_t OutState_AddresInEEprom;
};


struct StructCalibrationValuetoSaveInFlash CalibrationData;
struct StructCalibrationValuetoSaveInFlash CalibrationDataForVerify;
struct StructCalibrationValuetoSaveInFlash CalibrationDataFactory;

struct StructSettingsValuetoSaveInFlash SettingsData;
struct StructSettingsValuetoSaveInFlash SettingsDataForVerify;
struct StructSettingsValuetoSaveInFlash SettingsDataFactory;

struct StructValuetoSaveInFlashWhenPowerOFF SaveDataWhenPowerOff;
struct StructValuetoSaveInFlashWhenPowerOFF SaveDataWhenPowerOffForVerify;
struct StructValuetoSaveInFlashWhenPowerOFF SaveDataWhenPowerOffFactory;


//ADC
volatile uint16_t U_OUT_ForSetResistance;
volatile int16_t Current_RAW;
volatile int16_t Current_load ;
volatile int16_t Current_Out ;
volatile int32_t Temperature;
volatile int32_t Rt;

volatile uint16_t  Count100mSecond;
volatile uint16_t  Count10mSecond;
volatile uint16_t  Count1000mSecond;
volatile uint16_t  CountAnymSecond;
volatile uint32_t  Seconds;

struct StructTemperatureLinearTable
{
	int16_t T;
	int16_t R;
};
struct StructTemperature_R_A_B
{
	int32_t R;
	int32_t A10000;
	int32_t B;
};
#define TLT 8
struct StructTemperatureLinearTable T_Table[TLT];
struct StructTemperature_R_A_B Temp_R_A_B[TLT];

#define MAX_LOG_ITEMS 24

struct StructLoggingData
{
	uint32_t RecordsQuantity;
	char Records[MAX_LOG_ITEMS][17];
};
struct StructLoggingData LoggingData;
void WriteInLOG(char  str[17]);
void WriteInLOGc(char  str [17],char c);


void InfoToUARTBeforeStart(void);

volatile uint32_t f1;
volatile uint32_t f2;
volatile uint32_t f3;
volatile uint32_t f4;
void delay_ms(volatile uint32_t value);
void delay_us(uint32_t us);

#define RX_BUFFER_SIZE 100 // for receive
#define TX_BUFFER_SIZE 1000 // for send
struct StructRX_TX_Buffer
{
	// Буфер на прием
	volatile uint8_t    rx_buffer[RX_BUFFER_SIZE];
	volatile uint16_t   rx_wr_index, //индекс хвоста буфера (куда писать данные)
	                    rx_rd_index, //индекс начала буфера (откуда читать данные)
	                    rx_counter; //количество данных в буфере
	volatile uint16_t    rx_buffer_overflow; //информация о переполнении буфера

	// Буфер на передачу
	volatile uint8_t   tx_buffer[TX_BUFFER_SIZE];
	volatile uint16_t  tx_wr_index, //индекс хвоста буфера (куда писать данные)
	                   tx_rd_index, //индекс начала буфера (откуда читать данные)
	                   tx_counter; //количество данных в буфере
	volatile uint16_t   tx_buffer_overflow; //информация о переполнении буфера
};
struct StructRX_TX_Buffer bufferUart1;


#endif
