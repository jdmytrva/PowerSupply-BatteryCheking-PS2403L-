#include <stm32f10x.h>
#ifndef _MNU_
#define _MNU_


#define NULL 0

//TypeOfMenu
#define OPTION 1
#define VALUE 2
#define MENU 3
#define CHARGE 4
#define DISCHARGE 5
#define TRAINING 6
#define DIAGNOSTIC 7
#define POWERSUPPLY 8
#define CHARGE_TIME 9
#define LOW_VOLTAGE 10
#define MAX_VOLTAGE 11
#define SWING_CHRG_TIME 12
#define SWING_DCHRG_TIME 13
#define SWING 14
#define CURRENTSET0 15
#define CURRENT1SET0 16
#define CURRENTOUT 17
#define CURRENTLOAD 18
#define VOLTAGE 19
#define VOLTAGE1 20
#define VOLTAGEIN 21
#define BACKTOFACTORY 22
#define LOAD 23
#define CHARGE_ADAPT 24
#define CHARGE_ADAPT_MENU 25
#define RESISTANCE_COMP 26











//SelectValues
#define SET 1
#define UNSET 2



struct menu_item
{
        char Name[17];
        int16_t SelectValues;
        char TypeOfMenu;
        struct  menu_item       *Parent;
        struct  menu_item       *Child;
        struct  menu_item       *Prev;
        struct  menu_item       *Next;
};

#define SIZE_OF_STRUCT 59
struct menu_item Menu[SIZE_OF_STRUCT];


#endif
