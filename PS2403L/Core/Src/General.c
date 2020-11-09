#include "General.h"
#define ADDRESS_FLASH_CALIBRATTION FLASH_BASE+1024*124
#define ADDRESS_FLASH_SETTINGS     FLASH_BASE+1024*123
#define ADDRESS_FLASH_WHEN_OFF     FLASH_BASE+1024*122


struct StructCalibrationValuetoSaveInFlash CalibrationData=
{
		0,
		ADDRESS_FLASH_CALIBRATTION,
		0,
		ADDRESS_FLASH_CALIBRATTION+4,
		0,
		ADDRESS_FLASH_CALIBRATTION+8,
		0,
		ADDRESS_FLASH_CALIBRATTION+12,
		0,
		ADDRESS_FLASH_CALIBRATTION+16,
		0,
		ADDRESS_FLASH_CALIBRATTION+20,
		0,
		ADDRESS_FLASH_CALIBRATTION+24,
		0,
		ADDRESS_FLASH_CALIBRATTION+28,
		0,
		ADDRESS_FLASH_CALIBRATTION+32,
		0,
		ADDRESS_FLASH_CALIBRATTION+36

};
struct StructCalibrationValuetoSaveInFlash CalibrationDataForVerify=
{
		0,
		ADDRESS_FLASH_CALIBRATTION,
		0,
		ADDRESS_FLASH_CALIBRATTION+4,
		0,
		ADDRESS_FLASH_CALIBRATTION+8,
		0,
		ADDRESS_FLASH_CALIBRATTION+12,
		0,
		ADDRESS_FLASH_CALIBRATTION+16,
		0,
		ADDRESS_FLASH_CALIBRATTION+20,
		0,
		ADDRESS_FLASH_CALIBRATTION+24,
		0,
		ADDRESS_FLASH_CALIBRATTION+28,
		0,
		ADDRESS_FLASH_CALIBRATTION+32,
		0,
		ADDRESS_FLASH_CALIBRATTION+36

};
struct StructCalibrationValuetoSaveInFlash CalibrationDataFactory=
{
		0,//CRC
		ADDRESS_FLASH_CALIBRATTION,
		0,//Calibration0ValueForCurrent
		ADDRESS_FLASH_CALIBRATTION+4,
		0,//Calibration0ValueForCurrent1
		ADDRESS_FLASH_CALIBRATTION+8,
		141,//CalibrationValueForCurrent Resistor 6.2k || 4.7k  1%
		ADDRESS_FLASH_CALIBRATTION+12,
		141,//CalibrationValueForCurrent1 Resistor 6.2k || 4.7k  1%
		ADDRESS_FLASH_CALIBRATTION+16,
		1109,//CalibrationValueForVoltage Resistor 8.2k 1%
		ADDRESS_FLASH_CALIBRATTION+20,
		1109,//CalibrationValueForVoltage1 Resistor 8.2k 1%
		ADDRESS_FLASH_CALIBRATTION+24,
		1109,//CalibrationValueForVoltage2 Resistor 8.2k 1%
		ADDRESS_FLASH_CALIBRATTION+28,
		135,//ResistanceComp_Ishunt_Wires mOm 116
		ADDRESS_FLASH_CALIBRATTION+32,
		34,//ResistanceComp_MOSFET mOm
		ADDRESS_FLASH_CALIBRATTION+36
};
//=================================================

struct StructSettingsValuetoSaveInFlash SettingsData=
{
		0,
		ADDRESS_FLASH_SETTINGS,
		0,
		ADDRESS_FLASH_SETTINGS+4,
		0,
		ADDRESS_FLASH_SETTINGS+8,
		0,
		ADDRESS_FLASH_SETTINGS+12,
		0,
		ADDRESS_FLASH_SETTINGS+16,
		0,
		ADDRESS_FLASH_SETTINGS+20,
		0,
		ADDRESS_FLASH_SETTINGS+24,
		0,
		ADDRESS_FLASH_SETTINGS+28,
		0,
		ADDRESS_FLASH_SETTINGS+32
};
struct StructSettingsValuetoSaveInFlash SettingsDataForVerify=
{
		0,
		ADDRESS_FLASH_SETTINGS,
		0,
		ADDRESS_FLASH_SETTINGS+4,
		0,
		ADDRESS_FLASH_SETTINGS+8,
		0,
		ADDRESS_FLASH_SETTINGS+12,
		0,
		ADDRESS_FLASH_SETTINGS+16,
		0,
		ADDRESS_FLASH_SETTINGS+20,
		0,
		ADDRESS_FLASH_SETTINGS+24,
		0,
		ADDRESS_FLASH_SETTINGS+28,
		0,
		ADDRESS_FLASH_SETTINGS+32
};

struct StructSettingsValuetoSaveInFlash SettingsDataFactory=
{
		0,//CRC
		ADDRESS_FLASH_SETTINGS,
		1,//option1 -  Menu Position
		ADDRESS_FLASH_SETTINGS+4,
		15,//Charge time
		ADDRESS_FLASH_SETTINGS+8,
		1050,//LowVoltage
		ADDRESS_FLASH_SETTINGS+12,
		1450,//MaxVoltage
		ADDRESS_FLASH_SETTINGS+16,
		10,//Swing_Chrg_time
		ADDRESS_FLASH_SETTINGS+20,
		10,//Swing_DChrg_time
		ADDRESS_FLASH_SETTINGS+24,
		6,//ChargeAdapt %
		ADDRESS_FLASH_SETTINGS+28,
		1,//Out after powerUp option. 1 - OFF
		ADDRESS_FLASH_SETTINGS+32

};
//=====================================
struct StructValuetoSaveInFlashWhenPowerOFF SaveDataWhenPowerOff=
{
		0,
		ADDRESS_FLASH_WHEN_OFF,
		0,
		ADDRESS_FLASH_WHEN_OFF+4,
		0,
		ADDRESS_FLASH_WHEN_OFF+8,
		0,
		ADDRESS_FLASH_WHEN_OFF+12
};

struct StructValuetoSaveInFlashWhenPowerOFF SaveDataWhenPowerOffForVerify=
{
		0,
		ADDRESS_FLASH_WHEN_OFF,
		0,
		ADDRESS_FLASH_WHEN_OFF+4,
		0,
		ADDRESS_FLASH_WHEN_OFF+8,
		0,
		ADDRESS_FLASH_WHEN_OFF+12
};

struct StructValuetoSaveInFlashWhenPowerOFF SaveDataWhenPowerOffFactory=
{
		0,//CRC
		ADDRESS_FLASH_WHEN_OFF,
		0,//BatteryCapacityDischargePreviousValue
		ADDRESS_FLASH_WHEN_OFF+4,
		0,//BatteryCapacityDischargeCurrent
		ADDRESS_FLASH_WHEN_OFF+8,
		0,//Out State. 0 - OFF, 1 - ON
		ADDRESS_FLASH_WHEN_OFF+12
};


volatile uint16_t U_OUT_ForSetResistance=0;
volatile int16_t Current_load = 0;
volatile int16_t Current_Out = 0;

struct StructTemperatureLinearTable T_Table []=
{
		{1,31000},
		{10,20000},
		{25,10000},//Temperature, Resistance
		{40,4300},
		{60,3000},
		{80,1950},
		{100,900},
		{150,500}
};
void Generation_Coefficients_R_A_B()
{
	int8_t i;
	for (i=0;i<(TLT-1);i++)
	{
		Temp_R_A_B[i].R = T_Table[i].R;
		Temp_R_A_B[i].A10000 = (T_Table[i+1].T - T_Table[i].T)*10000/(T_Table[i+1].R - T_Table[i].R);
		Temp_R_A_B[i].B = T_Table[i].T - Temp_R_A_B[i].A10000 * T_Table[i].R/10000;
	}
	Temp_R_A_B[i].R = T_Table[i].R;
/*
	for (i=0; i<(TLT-1);i++)
	{
		Print_to_USART1_d(Temp_R_A_B[i].R,"R:",0);
	    Print_to_USART1_d(Temp_R_A_B[i].A10000,"A10000:",0);
	    Print_to_USART1_d(Temp_R_A_B[i].B,"B:",0);
	    Print_to_USART1_d(Temp_R_A_B[i].A10000*Temp_R_A_B[i].R/10000+Temp_R_A_B[i].B,"T0:",0);
	    int32_t c;
	    c=(Temp_R_A_B[i].R-Temp_R_A_B[i+1].R)/2+Temp_R_A_B[i+1].R;
	    Print_to_USART1_d(c,"c= ",0);
	    Print_to_USART1_d(Temp_R_A_B[i].A10000*c/10000+Temp_R_A_B[i].B,"T:",0);
	}
*/
}
int32_t GetTemperature(int32_t Rtemp)
{
	if (Rtemp>Temp_R_A_B[0].R) return -10;

	int8_t i;
	for (i=0; i<(TLT-1);i++)
	{
		if (Rtemp <= Temp_R_A_B[i].R  && Rtemp > Temp_R_A_B[i+1].R)
		{
			return Rtemp*Temp_R_A_B[i].A10000/10000+Temp_R_A_B[i].B;

		}
	}
	return 200;
}

void WriteInLOG(char  str [17])
{
	static uint16_t i_log=0;
	uint8_t i;
	uint8_t j;

	char *number;
	number = itoa(LoggingData.RecordsQuantity);


	for(i=0;number[i]!='\0';i++)
	{
		LoggingData.Records[LoggingData.RecordsQuantity][i] = number[i];
	}
	LoggingData.Records[LoggingData.RecordsQuantity][i] = ':';
	i++;

	for(j=0;str[j]!='\0';i++,j++)
	{
		if (i<16) LoggingData.Records[LoggingData.RecordsQuantity][i] = str[j];
		else
		{
			LoggingData.Records[LoggingData.RecordsQuantity][i] = '\0';
			break;
		}
	}
	for (;i<16;i++)
	{
		LoggingData.Records[LoggingData.RecordsQuantity][i] = ' ';
	}
	logDebugD("ii: ",i,0);
	LoggingData.Records[LoggingData.RecordsQuantity][i] = '\0';

	LoggingData.RecordsQuantity++;
	if (LoggingData.RecordsQuantity>=(MAX_LOG_ITEMS))
	{
		for (i=0;i<MAX_LOG_ITEMS-10;i++)
		{
			number = itoa(i);
			uint8_t k;
			for(k=0;number[k]!='\0';k++)
			{
				LoggingData.Records[i][k] = number[k];
			}
			LoggingData.Records[i][k] = ':';
			for (j=k+1;LoggingData.Records[i][j]!='\0';j++)
			{
				if (i>=10)
					LoggingData.Records[i][j] = LoggingData.Records[i+10][j];
				else
					LoggingData.Records[i][j] = LoggingData.Records[i+10][j+1];
			}
			j--;
			for (;j<16;j++)
			{
				LoggingData.Records[i][j] = ' ';
			}
			LoggingData.Records[i][j] = '\0';
		}

		LoggingData.RecordsQuantity=MAX_LOG_ITEMS-10;
	}
	flash_write_block();
	logDebugD("Q: ",LoggingData.RecordsQuantity,0);
}

void WriteInLOGc(char  str [17],char c)
{
	static uint16_t i_log=0;
	uint8_t i;
	uint8_t j;

	char *number;
	number = itoa(LoggingData.RecordsQuantity);


	for(i=0;number[i]!='\0';i++)
	{
		LoggingData.Records[LoggingData.RecordsQuantity][i] = number[i];
	}
	LoggingData.Records[LoggingData.RecordsQuantity][i] = c;
	i++;

	for(j=0;str[j]!='\0';i++,j++)
	{
		if (i<16) LoggingData.Records[LoggingData.RecordsQuantity][i] = str[j];
		else
		{
			LoggingData.Records[LoggingData.RecordsQuantity][i] = '\0';
			break;
		}
	}
	for (;i<16;i++)
	{
		LoggingData.Records[LoggingData.RecordsQuantity][i] = ' ';
	}
	logDebugD("ii: ",i,0);
	LoggingData.Records[LoggingData.RecordsQuantity][i] = '\0';

	LoggingData.RecordsQuantity++;
	if (LoggingData.RecordsQuantity>=(MAX_LOG_ITEMS))
	{
		for (i=0;i<MAX_LOG_ITEMS-10;i++)
		{
			number = itoa(i);
			uint8_t k;
			for(k=0;number[k]!='\0';k++)
			{
				LoggingData.Records[i][k] = number[k];
			}
			LoggingData.Records[i][k] = c;
			for (j=k+1;LoggingData.Records[i][j]!='\0';j++)
			{
				if (i>=10)
					LoggingData.Records[i][j] = LoggingData.Records[i+10][j];
				else
					LoggingData.Records[i][j] = LoggingData.Records[i+10][j+1];
			}
			j--;
			for (;j<16;j++)
			{
				LoggingData.Records[i][j] = ' ';
			}
			LoggingData.Records[i][j] = '\0';
		}

		LoggingData.RecordsQuantity=MAX_LOG_ITEMS-10;
	}
	flash_write_block();
	logDebugD("Q: ",LoggingData.RecordsQuantity,0);
}



void InfoToUARTBeforeStart(void)
{

	logInfoD("CRC(Calibration) =",CalibrationData.CRC_data,0);
	logInfoD("Calibration0ValueForCurrent =",CalibrationData.Calibration0ValueForCurrent ,0);
	logInfoD("Calibration0ValueForCurrent1 =",CalibrationData.Calibration0ValueForCurrent1 ,0);
	logInfoD("CalibrationValueForCurrent =",CalibrationData.CalibrationValueForCurrent ,0);
	logInfoD("CalibrationValueForCurrent1 =",CalibrationData.CalibrationValueForCurrent1 ,0);
	logInfoD("CalibrationValueForVoltage =",CalibrationData.CalibrationValueForVoltage ,0);
	logInfoD("CalibrationValueForVoltage1 =",CalibrationData.CalibrationValueForVoltage1 ,0);
	logInfoD("CalibrationValueForVoltage2 =",CalibrationData.CalibrationValueForVoltage2 ,0);
	logInfoD("ResistanceComp_Ishunt_Wires =",CalibrationData.ResistanceComp_Ishunt_Wires ,0) ;
	logInfoD("ResistanceComp_MOSFET =",CalibrationData.ResistanceComp_MOSFET ,0) ;
	delay_ms(50);
	logInfoD("CRC(SettingsData) =",SettingsData.CRC_data,0);
	logInfoD("Option1(menu position) =",SettingsData.Option1 ,0);
	logInfoD("ChargeTime =",SettingsData.ChargeTime ,0);
	logInfoD("LowVoltage =",SettingsData.LowVoltage ,0);
	logInfoD("MaxVoltage =",SettingsData.MaxVoltage ,0);
	logInfoD("ChargeAdapt =",SettingsData.ChargeAdapt ,0);
	logInfoD("Swing_Chrg_time =",SettingsData.Swing_Chrg_time ,0);
	logInfoD("Swing_DChrg_time =",SettingsData.Swing_DChrg_time ,0);
	logInfoD("Option2(OUT after powerUp) =",SettingsData.Option2 ,0);
	delay_ms(50);

	logInfoD("CRC(SaveDataWhenPowerOff) =",SaveDataWhenPowerOff.CRC_data,0);
	logInfoD("BatteryCapacityDischargePreviousValue =",SaveDataWhenPowerOff.BatteryCapacityDischargePreviousValue ,0);
	logInfoD("BatteryCapacityDischargeCurrent =",SaveDataWhenPowerOff.BatteryCapacityDischargeCurrent ,0);
	logInfoD("OutState = ",SaveDataWhenPowerOff.OutState ,0);

	logInfoD("SystemCoreClock: ", SystemCoreClock, 0);
	delay_ms(50);
}
void delay_us(uint32_t us)
{
	volatile uint32_t n = 0;
	if (us==1) n=1;//в тактах процесора
	if (us==2) n=2;//в тактах процесора
	if (us==3) n=18;//в тактах процесора
	if (us > 3)n=(SystemCoreClock/1000000)*(us-3);//в тактах процесора
	//uint32_t n=(SystemCoreClock/1000000)*us;//в тактах процесора
    volatile  uint32_t n0=SysTick->VAL;// Начальное значение счётчика SysTick.
    volatile uint32_t np=n0;  // Предыдущее значение счётчика.
    volatile int32_t nc;// Текущее значение счётчика SysTick.

    // Выполняем цикл до тех пор, пока не пройдёт заданное количество тактов процессора.
    do{
    	nc=SysTick->VAL;
        // Проверка на переполнение, корректировка на модуль пересчёта в случае переполнения.
        if(nc>=np)
            n0+=SysTick->LOAD+1;
        np=nc;
    }while(n0-nc<n);
}
// t1=SysTick->VAL;
// t2=SysTick->VAL;

void delay_ms(volatile uint32_t value)
{
	while(value>0)
	{
		delay_us(1000);
		value--;
	}
}

//For
volatile uint32_t f1=0;
volatile uint32_t f2=0;
volatile uint32_t f3=0;
volatile uint32_t f4=0;

volatile uint16_t  Count100mSecond = 0;
volatile uint16_t  Count10mSecond = 0;
volatile uint16_t  Count1000mSecond = 0;
volatile uint16_t  CountAnymSecond = 0;
volatile uint32_t  Seconds = 0;

struct StructRX_TX_Buffer bufferUart1 =
{
	{},
	0,
	0,
	0,
	0,
	{},
	0,
	0,
	0,
	0
};
