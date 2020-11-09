#ifndef _MNU_
#define _MNU_
//#include "stm32f1xx.h"
#include "MicroMenu2_1.h"
void MenuCalibrationWriteToFlash_Enter(Key_Pressed_t key);
void MenuSettingsWriteToFlash_Enter(Key_Pressed_t key);
void MenuSettingsBatteryType_Enter(Key_Pressed_t key);
void MenuDischarge_Enter(Key_Pressed_t key);
void MenuCharge_Enter(Key_Pressed_t key);
void MenuLog_Enter();
void MenuSettingsChargeTime_Enter(void);
void MenuSettingsLowVolt_Enter(void);
void MenuSettingsMaxVolt_Enter(void);
void MenuSettingsSwngChrgTime_Enter(void);
void MenuSettingsSwngDChrgTime_Enter(void);
void MenuSettingsChargeAddapt_Enter(void);
void MenuOption_Enter(Key_Pressed_t key);
void MenuOption_Enter1(Key_Pressed_t key);
//       NAME          NEXT,       PREV        PARENT,       CHILD       EnterCallback        SelectCallback         Text
 MENU_ITEM(Menu_1,     Menu_2,     Menu_10,    NULL_MENU,   Menu_1_1,   NULL,                NULL,               "Settings        ");
 MENU_ITEM(Menu_2,     Menu_3,     Menu_1,     NULL_MENU,   Menu_2_1,   NULL,                NULL,               "PowerSupply     ");
 MENU_ITEM(Menu_3,     Menu_4,     Menu_2,     NULL_MENU,   Menu_3_1,   NULL,                NULL,               "Load            ");
 MENU_ITEM(Menu_4,     Menu_5,     Menu_3,     NULL_MENU,   Menu_4_1,   NULL,                NULL,               "Charge CC CV    ");
 MENU_ITEM(Menu_5,     Menu_6,     Menu_4,     NULL_MENU,   Menu_5_1,   NULL,                NULL,               "Charge addapt   ");
 MENU_ITEM(Menu_6,     Menu_6a,     Menu_5,     NULL_MENU,   Menu_6_1,   NULL,                NULL,              "DisCharge       ");
 MENU_ITEM(Menu_6a,    Menu_7,     Menu_6,     NULL_MENU,   Menu_6_1a,   NULL,                NULL,              "CheckingCapacity");
 MENU_ITEM(Menu_7,     Menu_8,     Menu_6a,     NULL_MENU,   Menu_7_1,   NULL,                NULL,              "Training        ");
 MENU_ITEM(Menu_8,     Menu_8a,     Menu_7,     NULL_MENU,   Menu_8_1,   NULL,                NULL,              "Swing           ");
 MENU_ITEM(Menu_8a,    Menu_9,     Menu_8,     NULL_MENU,   Menu_8a_1,   NULL,                NULL,               "Batt. Simulation");
 MENU_ITEM(Menu_9,     Menu_11,    Menu_8a,     NULL_MENU,   Menu_9_1,   NULL,                NULL,               "DIAGNOSTIC      ");

 MENU_ITEM(Menu_11,     Menu_10,    Menu_9,     NULL_MENU,   Menu_11_1, NULL,                NULL,               "LOG             ");

 MENU_ITEM(Menu_10,    Menu_1,     Menu_11,     NULL_MENU,   Menu_10_1,  NULL,                NULL,               "CALIBRATION     ");

//MENU Settings
 //       NAME          NEXT,       PREV        PARENT,       CHILD       EnterCallback        SelectCallback         Text
 MENU_ITEM(Menu_1_1,   Menu_1_1a,  Menu_1_7,   Menu_1,   Menu_1_1_1,   NULL,                NULL,              "01:Charge time  ");
 MENU_ITEM(Menu_1_1a,   Menu_1_2,  Menu_1_1,   Menu_1,   Menu_1_1_1a,   NULL,                NULL,             "02:Battery type ");
 MENU_ITEM(Menu_1_2,   Menu_1_3,   Menu_1_1a,  Menu_1,   Menu_1_2_1,   NULL,                NULL,               "03:Low Voltage  ");
 MENU_ITEM(Menu_1_3,   Menu_1_4,   Menu_1_2,   Menu_1,   Menu_1_3_1,   NULL,                NULL,               "04:Max Voltage  ");
 MENU_ITEM(Menu_1_4,   Menu_1_5,   Menu_1_3,   Menu_1,   Menu_1_4_1,   NULL,                NULL,               "05:Swing Chrg Tm");
 MENU_ITEM(Menu_1_5,   Menu_1_6,   Menu_1_4,   Menu_1,   Menu_1_5_1,   NULL,                NULL,               "06:SwngDisChrg T");
 MENU_ITEM(Menu_1_6,   Menu_1_S,   Menu_1_5,   Menu_1,   Menu_1_6_1,   NULL,                NULL,               "07:Charge addapt");
 MENU_ITEM(Menu_1_S,   Menu_1_SO,  Menu_1_6,   Menu_1,   Menu_1_S_1,   NULL,                NULL,               "08:Save Menu Pos");
 MENU_ITEM(Menu_1_SO,  Menu_1_7,   Menu_1_S,   Menu_1,   Menu_1_SO_1,  NULL,                NULL,               "09:Out after ON ");
 MENU_ITEM(Menu_1_7,   Menu_1_1,   Menu_1_SO,  Menu_1,   Menu_1,       NULL,                NULL,               "OK to exit      ");

 //MENU  CALIBRATION
 //       NAME          NEXT,       PREV        PARENT,       CHILD       EnterCallback        SelectCallback         Text
 MENU_ITEM(Menu_10_1,  Menu_10_2,  Menu_10_10, Menu_10,   Menu_10_1_1,   NULL,                NULL,               "CURRENT Out to 0");
 MENU_ITEM(Menu_10_2,  Menu_10_3,  Menu_10_1,  Menu_10,   Menu_10_2_1,   NULL,                NULL,               "CURRENT Load to0");
 MENU_ITEM(Menu_10_3,  Menu_10_4,  Menu_10_2,  Menu_10,   Menu_10_3_1,   NULL,                NULL,               "CURRENT Out     ");
 MENU_ITEM(Menu_10_4,  Menu_10_5,  Menu_10_3,  Menu_10,   Menu_10_4_1,   NULL,                NULL,               "CURRENT Load    ");
 MENU_ITEM(Menu_10_5,  Menu_10_6,  Menu_10_4,  Menu_10,   Menu_10_5_1,   NULL,                NULL,               "Voltage PS      ");
 MENU_ITEM(Menu_10_6,  Menu_10_7,  Menu_10_5,  Menu_10,   Menu_10_6_1,   NULL,                NULL,               "Voltage Out     ");
 MENU_ITEM(Menu_10_7,  Menu_10_8,  Menu_10_6,  Menu_10,   Menu_10_7_1,   NULL,                NULL,               "Voltage In      ");
 MENU_ITEM(Menu_10_8,  Menu_10_9,  Menu_10_7,  Menu_10,   Menu_10_8_1,   NULL,                NULL,               "Resist comp 5V1A");
 MENU_ITEM(Menu_10_9,  Menu_10_10, Menu_10_8,  Menu_10,   Menu_10_9_1,   NULL,                NULL,               "Back to Factory ");
 MENU_ITEM(Menu_10_10, Menu_10_1,  Menu_10_9,  Menu_10,   Menu_10,       NULL,                  NULL,             "OK to exit      ");

//PowerSupply
 MENU_ITEM(Menu_2_1,     NULL_MENU,     NULL_MENU,     Menu_2,   Menu_2,   NULL,                NULL,              "                ");
 //Load
 MENU_ITEM(Menu_3_1,     NULL_MENU,     NULL_MENU,     Menu_3,   Menu_3,   NULL,                NULL,               "                ");
 //Charge CC CV
 MENU_ITEM(Menu_4_1,     NULL_MENU,     NULL_MENU,     Menu_4,   Menu_4,   NULL,                MenuCharge_Enter,               "                ");
 //Charge addapt
 MENU_ITEM(Menu_5_1,     NULL_MENU,     NULL_MENU,     Menu_5,   Menu_5,   NULL,                MenuCharge_Enter,               "                ");
 //DisCharge
 MENU_ITEM(Menu_6_1,     NULL_MENU,     NULL_MENU,     Menu_6,   Menu_6,   NULL,                MenuDischarge_Enter,"                ");
 //Checking Capaciti
  MENU_ITEM(Menu_6_1a,     NULL_MENU,     NULL_MENU,     Menu_6a,   Menu_6a,   NULL,             NULL,"                ");
 //Training
 MENU_ITEM(Menu_7_1,     NULL_MENU,     NULL_MENU,     Menu_7,   Menu_7,   NULL,                NULL,               "                ");
 //Swing
 MENU_ITEM(Menu_8_1,     NULL_MENU,     NULL_MENU,     Menu_8,   Menu_8,   NULL,                NULL,               "                ");

 //Batt. Simulation
 MENU_ITEM(Menu_8a_1,     NULL_MENU,     NULL_MENU,     Menu_8a,   Menu_8a,   NULL,                NULL,               "                ");

 //DIAGNOSTIC
 MENU_ITEM(Menu_9_1,     NULL_MENU,    NULL_MENU,     Menu_9,   Menu_9,   NULL,                NULL,               "                ");

 //LOG
 MENU_ITEM(Menu_11_1,     NULL_MENU,    NULL_MENU,     Menu_11,   Menu_11,   NULL,                MenuLog_Enter,             "                ");



 //CURRENT Out to 0
 MENU_ITEM(Menu_10_1_1,  NULL_MENU,  NULL_MENU, Menu_10_1,   Menu_10_1,   NULL,                MenuCalibrationWriteToFlash_Enter,               "                ");
 //CURRENT Load to0
 MENU_ITEM(Menu_10_2_1,  NULL_MENU,  NULL_MENU,  Menu_10_2,   Menu_10_2,   NULL,                MenuCalibrationWriteToFlash_Enter,               "                ");
 //CURRENT Out
 MENU_ITEM(Menu_10_3_1,  NULL_MENU,  NULL_MENU,  Menu_10_3,   Menu_10_3,   NULL,                MenuCalibrationWriteToFlash_Enter,               "                ");
 //CURRENT Load
 MENU_ITEM(Menu_10_4_1,  NULL_MENU,  NULL_MENU,  Menu_10_4,   Menu_10_4,   NULL,                MenuCalibrationWriteToFlash_Enter,               "                ");
 //Voltage PS
 MENU_ITEM(Menu_10_5_1,  NULL_MENU,  NULL_MENU,  Menu_10_5,   Menu_10_5,   NULL,                MenuCalibrationWriteToFlash_Enter,               "                ");
 //Voltage Out
 MENU_ITEM(Menu_10_6_1,  NULL_MENU,  NULL_MENU,  Menu_10_6,   Menu_10_6,   NULL,                MenuCalibrationWriteToFlash_Enter,               "                ");
 //Voltage In
 MENU_ITEM(Menu_10_7_1,  NULL_MENU,  NULL_MENU,  Menu_10_7,   Menu_10_7,   NULL,                MenuCalibrationWriteToFlash_Enter,               "");
 //Resist comp 5V1A
 MENU_ITEM(Menu_10_8_1,  NULL_MENU,  NULL_MENU,  Menu_10_8,   Menu_10_8,   NULL,                MenuCalibrationWriteToFlash_Enter,               "");
//Back to Factory
 MENU_ITEM(Menu_10_9_1,  NULL_MENU, NULL_MENU,  Menu_10_9,   Menu_10_9,   NULL,                NULL,               "");




 //          NAME          NEXT,       PREV        PARENT,       CHILD      EnterCallback                    SelectCallback         Text
 //Charge time, h
 MENU_ITEM(Menu_1_1_1,   NULL_MENU,   NULL_MENU,   Menu_1_1,   Menu_1_1,   NULL,    MenuSettingsWriteToFlash_Enter,                   "                ");
 //Battery type
  MENU_ITEM(Menu_1_1_1a,   NULL_MENU,   NULL_MENU,   Menu_1_1a,   Menu_1_1a,   NULL,    MenuSettingsBatteryType_Enter,                   "                ");
 //Low Volt, mV*10
 MENU_ITEM(Menu_1_2_1,   NULL_MENU,   NULL_MENU,   Menu_1_2,   Menu_1_2,   NULL,       MenuSettingsWriteToFlash_Enter,               "                ");
 //Max Volt, mV*10
 MENU_ITEM(Menu_1_3_1,   NULL_MENU,   NULL_MENU,   Menu_1_3,   Menu_1_3,   NULL,       MenuSettingsWriteToFlash_Enter,               "                ");
 //Swng Chrg Time,s
 MENU_ITEM(Menu_1_4_1,   NULL_MENU,   NULL_MENU,   Menu_1_4,   Menu_1_4,   NULL,  MenuSettingsWriteToFlash_Enter,               "                ");
 //SwngDChrg Time,s
 MENU_ITEM(Menu_1_5_1,   NULL_MENU,   NULL_MENU,   Menu_1_5,   Menu_1_5,   NULL, MenuSettingsWriteToFlash_Enter,               "                ");
 //Charge addapt, %
 MENU_ITEM(Menu_1_6_1,   NULL_MENU,   NULL_MENU,   Menu_1_6,   Menu_1_6,   NULL,  MenuSettingsWriteToFlash_Enter,               "                ");
//SaveMenuPos  OFF
 MENU_ITEM(Menu_1_S_1,   NULL_MENU,   NULL_MENU,   Menu_1_S,   Menu_1_S,   NULL,  MenuOption_Enter,               "                ");
 //09:Out after powerUp
 MENU_ITEM(Menu_1_SO_1,   NULL_MENU,   NULL_MENU,   Menu_1_SO,   Menu_1_SO,   NULL,  MenuOption_Enter1,               "                ");


#endif
