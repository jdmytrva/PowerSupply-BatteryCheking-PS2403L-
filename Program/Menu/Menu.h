#include <stm32f10x.h>
#ifndef _MNU_
#define _MNU_

void MenuEEpromWrite_Enter(Key_Pressed_t key);

 //       NAME          NEXT,       PREV        PARENT,       CHILD       EnterCallback        SelectCallback         Text
 MENU_ITEM(Menu_1,     Menu_2,     Menu_10,    NULL_MENU,   Menu_1_1,   NULL,                NULL,               "Settings        ");
 MENU_ITEM(Menu_2,     Menu_3,     Menu_1,     NULL_MENU,   Menu_2_1,   NULL,                NULL,               "PowerSupply     ");
 MENU_ITEM(Menu_3,     Menu_4,     Menu_2,     NULL_MENU,   Menu_3_1,   NULL,                NULL,               "Load            ");
 MENU_ITEM(Menu_4,     Menu_5,     Menu_3,     NULL_MENU,   Menu_4_1,   NULL,                NULL,               "Charge CC CV    ");
 MENU_ITEM(Menu_5,     Menu_6,     Menu_4,     NULL_MENU,   Menu_5_1,   NULL,                NULL,               "Charge addapt   ");
 MENU_ITEM(Menu_6,     Menu_7,     Menu_5,     NULL_MENU,   Menu_6_1,   NULL,                NULL,               "DisCharge       ");
 MENU_ITEM(Menu_7,     Menu_8,     Menu_6,     NULL_MENU,   Menu_7_1,   NULL,                NULL,               "Training        ");
 MENU_ITEM(Menu_8,     Menu_9,     Menu_7,     NULL_MENU,   Menu_8_1,   NULL,                NULL,               "Swing           ");
 MENU_ITEM(Menu_9,     Menu_10,    Menu_8,     NULL_MENU,   Menu_9_1,   NULL,                NULL,               "DIAGNOSTIC      ");
 MENU_ITEM(Menu_10,    Menu_1,     Menu_9,     NULL_MENU,   Menu_10_1,  NULL,                NULL,               "CALIBRATION     ");

//MENU Settings
 //       NAME          NEXT,       PREV        PARENT,       CHILD       EnterCallback        SelectCallback         Text
 MENU_ITEM(Menu_1_1,   Menu_1_2,   Menu_1_7,   NULL_MENU,   Menu_1_1_1,   NULL,                NULL,               "Charge time, h  ");
 MENU_ITEM(Menu_1_2,   Menu_1_3,   Menu_1_1,   NULL_MENU,   Menu_1_2_1,   NULL,                NULL,               "Low Voltage     ");
 MENU_ITEM(Menu_1_3,   Menu_1_4,   Menu_1_2,   NULL_MENU,   Menu_1_3_1,   NULL,                NULL,               "Max Voltage     ");
 MENU_ITEM(Menu_1_4,   Menu_1_5,   Menu_1_3,   NULL_MENU,   Menu_1_4_1,   NULL,                NULL,               "Swing Charg Time");
 MENU_ITEM(Menu_1_5,   Menu_1_6,   Menu_1_4,   NULL_MENU,   Menu_1_5_1,   NULL,                NULL,               "SwngDisChrg Time");
 MENU_ITEM(Menu_1_6,   Menu_1_7,   Menu_1_5,   NULL_MENU,   Menu_1_6_1,   NULL,                NULL,               "Charge addapt, %");
 MENU_ITEM(Menu_1_7,   Menu_1_1,   Menu_1_6,   NULL_MENU,   Menu_1,      NULL,                NULL,               "OK to exit      ");

 //MENU  CALIBRATION
 //       NAME          NEXT,       PREV        PARENT,       CHILD       EnterCallback        SelectCallback         Text
 MENU_ITEM(Menu_10_1,  Menu_10_2,  Menu_10_10, NULL_MENU,   Menu_10_1_1,   NULL,                NULL,               "CURRENT Out to 0");
 MENU_ITEM(Menu_10_2,  Menu_10_3,  Menu_10_1,  NULL_MENU,   Menu_10_2_1,   NULL,                NULL,               "CURRENT Load to0");
 MENU_ITEM(Menu_10_3,  Menu_10_4,  Menu_10_2,  NULL_MENU,   Menu_10_3_1,   NULL,                NULL,               "CURRENT Out     ");
 MENU_ITEM(Menu_10_4,  Menu_10_5,  Menu_10_3,  NULL_MENU,   Menu_10_4_1,   NULL,                NULL,               "CURRENT Load    ");
 MENU_ITEM(Menu_10_5,  Menu_10_6,  Menu_10_4,  NULL_MENU,   Menu_10_5_1,   NULL,                NULL,               "Voltage PS      ");
 MENU_ITEM(Menu_10_6,  Menu_10_7,  Menu_10_5,  NULL_MENU,   Menu_10_6_1,   NULL,                NULL,               "Voltage Out     ");
 MENU_ITEM(Menu_10_7,  Menu_10_8,  Menu_10_6,  NULL_MENU,   Menu_10_7_1,   NULL,                NULL,               "Voltage In      ");
 MENU_ITEM(Menu_10_8,  Menu_10_9,  Menu_10_7,  NULL_MENU,   Menu_10_8_1,   NULL,                NULL,               "Resist comp 5V1A");
 MENU_ITEM(Menu_10_9,  Menu_10_10, Menu_10_8,  NULL_MENU,   Menu_10_9_1,   NULL,                NULL,               "Back to Factory ");
 MENU_ITEM(Menu_10_10, Menu_10_1,  Menu_10_9,  NULL_MENU,   Menu_10,       NULL,                  NULL,             "OK to exit      ");

//PowerSupply
 MENU_ITEM(Menu_2_1,     NULL_MENU,     NULL_MENU,     NULL_MENU,   Menu_2,   NULL,   NULL,              "                ");
 //Load
 MENU_ITEM(Menu_3_1,     NULL_MENU,     NULL_MENU,     NULL_MENU,   Menu_3,   NULL,                NULL,               "                ");
 //Charge CC CV
 MENU_ITEM(Menu_4_1,     NULL_MENU,     NULL_MENU,     NULL_MENU,   Menu_4,   NULL,                NULL,               "                ");
 //Charge addapt
 MENU_ITEM(Menu_5_1,     NULL_MENU,     NULL_MENU,     NULL_MENU,   Menu_5,   NULL,                NULL,               "                ");
 //DisCharge
 MENU_ITEM(Menu_6_1,     NULL_MENU,     NULL_MENU,     NULL_MENU,   Menu_6,   NULL,                NULL,               "                ");
 //Training
 MENU_ITEM(Menu_7_1,     NULL_MENU,     NULL_MENU,     NULL_MENU,   Menu_7,   NULL,                NULL,               "                ");
 //Swing
 MENU_ITEM(Menu_8_1,     NULL_MENU,     NULL_MENU,     NULL_MENU,   Menu_8,   NULL,                NULL,               "                ");
 //DIAGNOSTIC
 MENU_ITEM(Menu_9_1,     NULL_MENU,    NULL_MENU,     NULL_MENU,   Menu_9,   NULL,                NULL,               "                ");


 //CURRENT Out to 0
 MENU_ITEM(Menu_10_1_1,  NULL_MENU,  NULL_MENU, NULL_MENU,   Menu_10_1,   NULL,                MenuEEpromWrite_Enter,               "                ");
 //CURRENT Load to0
 MENU_ITEM(Menu_10_2_1,  NULL_MENU,  NULL_MENU,  NULL_MENU,   Menu_10_2,   NULL,                MenuEEpromWrite_Enter,               "                ");
 //CURRENT Out
 MENU_ITEM(Menu_10_3_1,  NULL_MENU,  NULL_MENU,  NULL_MENU,   Menu_10_3,   NULL,                MenuEEpromWrite_Enter,               "                ");
 //CURRENT Load
 MENU_ITEM(Menu_10_4_1,  NULL_MENU,  NULL_MENU,  NULL_MENU,   Menu_10_4,   NULL,                MenuEEpromWrite_Enter,               "                ");
 //Voltage PS
 MENU_ITEM(Menu_10_5_1,  NULL_MENU,  NULL_MENU,  NULL_MENU,   Menu_10_5,   NULL,                MenuEEpromWrite_Enter,               "                ");
 //Voltage Out
 MENU_ITEM(Menu_10_6_1,  NULL_MENU,  NULL_MENU,  NULL_MENU,   Menu_10_6,   NULL,                MenuEEpromWrite_Enter,               "                ");
 //Voltage In
 MENU_ITEM(Menu_10_7_1,  NULL_MENU,  NULL_MENU,  NULL_MENU,   Menu_10_7,   NULL,                MenuEEpromWrite_Enter,               "");
 //Resist comp 5V1A
 MENU_ITEM(Menu_10_8_1,  NULL_MENU,  NULL_MENU,  NULL_MENU,   Menu_10_8,   NULL,                MenuEEpromWrite_Enter,               "");
//Back to Factory
 MENU_ITEM(Menu_10_9_1,  NULL_MENU, NULL_MENU,  NULL_MENU,   Menu_10_9,   NULL,                NULL,               "");


 void MenuSettingsChargeTime_Enter(void);
 void MenuSettingsLowVolt_Enter(void);
 void MenuSettingsMaxVolt_Enter(void);
 void MenuSettingsSwngChrgTime_Enter(void);
 void MenuSettingsSwngDChrgTime_Enter(void);
 void MenuSettingsChargeAddapt_Enter(void);

 //          NAME          NEXT,       PREV        PARENT,       CHILD      EnterCallback                    SelectCallback         Text
 //Charge time, h
 MENU_ITEM(Menu_1_1_1,   NULL_MENU,   NULL_MENU,   NULL_MENU,   Menu_1_1,   NULL,    MenuEEpromWrite_Enter,                   "                ");
 //Low Volt, mV*10
 MENU_ITEM(Menu_1_2_1,   NULL_MENU,   NULL_MENU,   NULL_MENU,   Menu_1_2,   NULL,       MenuEEpromWrite_Enter,               "                ");
 //Max Volt, mV*10
 MENU_ITEM(Menu_1_3_1,   NULL_MENU,   NULL_MENU,   NULL_MENU,   Menu_1_3,   NULL,       MenuEEpromWrite_Enter,               "                ");
 //Swng Chrg Time,s
 MENU_ITEM(Menu_1_4_1,   NULL_MENU,   NULL_MENU,   NULL_MENU,   Menu_1_4,   NULL,  MenuEEpromWrite_Enter,               "                ");
 //SwngDChrg Time,s
 MENU_ITEM(Menu_1_5_1,   NULL_MENU,   NULL_MENU,   NULL_MENU,   Menu_1_5,   NULL, MenuEEpromWrite_Enter,               "                ");
 //Charge addapt, %
 MENU_ITEM(Menu_1_6_1,   NULL_MENU,   NULL_MENU,   NULL_MENU,   Menu_1_6,   NULL,  MenuEEpromWrite_Enter,               "                ");



#endif
