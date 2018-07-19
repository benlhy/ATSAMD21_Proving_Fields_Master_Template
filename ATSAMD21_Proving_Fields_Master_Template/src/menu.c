

#include "menu.h"

#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4
#define CLICK 5

/*
if (statetext)
            {
                LCD_puts_f(statetext, 1);
                LCD_Colon(0);
                statetext = NULL;
            }
    
    
            input = getkey();           // Read buttons
    
    
            if (pStateFunc)
            {
                // When in this state, we must call the state function
                nextstate = pStateFunc(input);
            }
            else if (input != KEY_NULL)
            {
                // Plain menu, clock the state machine
                nextstate = StateMachine(state, input);
            }
    
            if (nextstate != state)
            {
                state = nextstate;
                for (i=0; menu_state[i].state; i++)
                {
                    if (menu_state[i].state == state)
                    {
                        statetext =  menu_state[i].pText;
                        pStateFunc = menu_state[i].pFunc;
                        break;
                    }
                }
            }


typedef struct
{
    unsigned char state;
    unsigned char input;
    unsigned char nextstate;
} MENU_NEXTSTATE;


typedef struct
{
    unsigned char state;
    char __flash *pText;
    char (*pFunc)(char input);
} MENU_STATE;

// Menu text
char MT_AVRBF[]                     = "AVR Butterfly";
char MT_TIME[]                      = "Time";
char MT_TIME_CLOCK[]                = "Clock";
char MT_TIME_CLOCK_ADJUST[]         = "Adjust Clock";
char MT_TIME_CLOCKFORMAT_ADJUST[]   = "Change Clock Format";
char MT_TIME_DATE[]                 = "Date";
char MT_TIME_DATE_ADJUST[]          = "Adjust Date";
char MT_TIME_DATEFORMAT_ADJUST[]    = "Change Date Format";
char MT_MUSIC[]                     = "Music";
char MT_VCARD[]                     = "Name";
char MT_ENTERNAME[]                 = "Enter name";
char MT_TEMPERATURE[]               = "Temperature";
char MT_VOLTAGE[]                   = "Voltage";
char MT_LIGHT[]                     = "Light";
char MT_OPTIONS[]                   = "Options";
char MT_OPTIONS_DISPLAY[]           = "Display";
char MT_OPTIONS_DISPLAY_CONTRAST[]  = "Adjust contrast";
char MT_OPTIONS_DISPLAY_SEG[]       = "Browse segments";
char MT_VCARD_DOWNLOAD_NAME[]       = "Download Name";
char MT_OPTIONS_BOOT[]              = "Bootloader";
char MT_OPTIONS_POWER_SAVE[]        = "Power Save Mode";
char MT_OPTIONS_AUTO_POWER_SAVE[]   = "Auto Power Save";

MENU_NEXTSTATE menu_nextstate[] = {
//  STATE                       INPUT       NEXT STATE
    {ST_AVRBF,                  KEY_UP,   ST_OPTIONS},
    {ST_AVRBF,                  KEY_RIGHT,   ST_AVRBF_REV},
    {ST_AVRBF,                  KEY_DOWN,  ST_TIME},

    {ST_AVRBF_REV,              KEY_LEFT,   ST_AVRBF},
    
    {ST_TIME,                   KEY_UP,   ST_AVRBF},
    {ST_TIME,                   KEY_RIGHT,   ST_TIME_CLOCK},
    {ST_TIME,                   KEY_LEFT,   ST_AVRBF},
    {ST_TIME,                   KEY_DOWN,  ST_MUSIC},
    
    {ST_TIME_CLOCK,             KEY_UP,   ST_TIME_DATE},
    {ST_TIME_CLOCK,             KEY_RIGHT,   ST_TIME_CLOCK_FUNC},
    {ST_TIME_CLOCK,             KEY_LEFT,   ST_TIME},
    {ST_TIME_CLOCK,             KEY_DOWN,  ST_TIME_DATE},

    {ST_TIME_CLOCK_ADJUST,      KEY_UP,   ST_TIME_CLOCKFORMAT_ADJUST}, 
    {ST_TIME_CLOCK_ADJUST,      KEY_ENTER,  ST_TIME_CLOCK_ADJUST_FUNC},
    {ST_TIME_CLOCK_ADJUST,      KEY_LEFT,   ST_TIME_CLOCK_FUNC},    
    {ST_TIME_CLOCK_ADJUST,      KEY_DOWN,  ST_TIME_CLOCKFORMAT_ADJUST}, 

    {ST_TIME_CLOCKFORMAT_ADJUST, KEY_UP,  ST_TIME_CLOCK_ADJUST},
    {ST_TIME_CLOCKFORMAT_ADJUST, KEY_ENTER, ST_TIME_CLOCKFORMAT_ADJUST_FUNC},
    {ST_TIME_CLOCKFORMAT_ADJUST, KEY_LEFT,  ST_TIME_CLOCK_FUNC},
    {ST_TIME_CLOCKFORMAT_ADJUST, KEY_DOWN, ST_TIME_CLOCK_ADJUST},

    {ST_TIME_DATE,              KEY_UP,   ST_TIME_CLOCK},
    {ST_TIME_DATE,              KEY_RIGHT,   ST_TIME_DATE_FUNC},
    {ST_TIME_DATE,              KEY_LEFT,   ST_TIME},
    {ST_TIME_DATE,              KEY_DOWN,  ST_TIME_CLOCK},

    {ST_TIME_DATE_ADJUST,       KEY_UP,   ST_TIME_DATEFORMAT_ADJUST},
    {ST_TIME_DATE_ADJUST,       KEY_ENTER,  ST_TIME_DATE_ADJUST_FUNC},
    {ST_TIME_DATE_ADJUST,       KEY_LEFT,   ST_TIME_DATE_FUNC},
    {ST_TIME_DATE_ADJUST,       KEY_DOWN,  ST_TIME_DATEFORMAT_ADJUST},
        
    {ST_TIME_DATEFORMAT_ADJUST, KEY_UP,   ST_TIME_DATE_ADJUST},
    {ST_TIME_DATEFORMAT_ADJUST, KEY_ENTER,  ST_TIME_DATEFORMAT_ADJUST_FUNC},
    {ST_TIME_DATEFORMAT_ADJUST, KEY_LEFT,   ST_TIME_DATE_FUNC},
    {ST_TIME_DATEFORMAT_ADJUST, KEY_DOWN,  ST_TIME_DATE_ADJUST},

    {ST_MUSIC,                  KEY_UP,   ST_TIME},
    {ST_MUSIC,                  KEY_RIGHT,   ST_MUSIC_SELECT},
    {ST_MUSIC,                  KEY_LEFT,   ST_AVRBF},
    {ST_MUSIC,                  KEY_DOWN,  ST_VCARD},

    {ST_SOUND_MUSIC,            KEY_RIGHT,   ST_MUSIC_SELECT},
    {ST_SOUND_MUSIC,            KEY_LEFT,   ST_MUSIC},

    {ST_VCARD,                  KEY_UP,   ST_MUSIC},
    {ST_VCARD,                  KEY_RIGHT,   ST_VCARD_FUNC},
    {ST_VCARD,                  KEY_LEFT,   ST_AVRBF},
    {ST_VCARD,                  KEY_DOWN,  ST_TEMPERATURE},
       
    {ST_VCARD_ENTER_NAME,       KEY_UP,     ST_VCARD_DOWNLOAD_NAME},
    {ST_VCARD_ENTER_NAME,       KEY_ENTER,    ST_VCARD_ENTER_NAME_FUNC},
    {ST_VCARD_ENTER_NAME,       KEY_LEFT,     ST_VCARD_FUNC},    
    {ST_VCARD_ENTER_NAME,       KEY_DOWN,    ST_VCARD_DOWNLOAD_NAME},
    
    {ST_VCARD_DOWNLOAD_NAME,    KEY_UP,     ST_VCARD_ENTER_NAME},
    {ST_VCARD_DOWNLOAD_NAME,    KEY_ENTER,    ST_VCARD_DOWNLOAD_NAME_FUNC},
    {ST_VCARD_DOWNLOAD_NAME,    KEY_LEFT,     ST_VCARD_FUNC},    
    {ST_VCARD_DOWNLOAD_NAME,    KEY_DOWN,    ST_VCARD_ENTER_NAME},    
    
    {ST_TEMPERATURE,            KEY_UP,   ST_VCARD},
    {ST_TEMPERATURE,            KEY_RIGHT,   ST_TEMPERATURE_FUNC},
    {ST_TEMPERATURE,            KEY_LEFT,   ST_AVRBF},
    {ST_TEMPERATURE,            KEY_DOWN,  ST_VOLTAGE},
    
    {ST_VOLTAGE,                KEY_UP,   ST_TEMPERATURE},
    {ST_VOLTAGE,                KEY_RIGHT,   ST_VOLTAGE_FUNC},
    {ST_VOLTAGE,                KEY_LEFT,   ST_AVRBF},
    {ST_VOLTAGE,                KEY_DOWN,  ST_LIGHT},
    
    {ST_LIGHT,                  KEY_UP,   ST_VOLTAGE},
    {ST_LIGHT,                  KEY_RIGHT,   ST_LIGHT_FUNC},
    {ST_LIGHT,                  KEY_LEFT,   ST_AVRBF},
    {ST_LIGHT,                  KEY_DOWN,  ST_OPTIONS},
    
    {ST_OPTIONS,                KEY_UP,   ST_LIGHT},
    {ST_OPTIONS,                KEY_RIGHT,   ST_OPTIONS_DISPLAY},
    {ST_OPTIONS,                KEY_LEFT,   ST_AVRBF},
    {ST_OPTIONS,                KEY_DOWN,  ST_AVRBF},
    
    {ST_OPTIONS_DISPLAY,        KEY_UP,   ST_OPTIONS_AUTO_POWER_SAVE},
    {ST_OPTIONS_DISPLAY,        KEY_RIGHT,   ST_OPTIONS_DISPLAY_CONTRAST},
    {ST_OPTIONS_DISPLAY,        KEY_LEFT,   ST_OPTIONS},
    {ST_OPTIONS_DISPLAY,        KEY_DOWN,  ST_OPTIONS_BOOT},
    
    {ST_OPTIONS_DISPLAY_CONTRAST, KEY_ENTER,    ST_OPTIONS_DISPLAY_CONTRAST_FUNC},
    {ST_OPTIONS_DISPLAY_CONTRAST, KEY_LEFT,     ST_OPTIONS_DISPLAY},

    {ST_OPTIONS_BOOT,             KEY_UP,     ST_OPTIONS_DISPLAY},
    {ST_OPTIONS_BOOT,             KEY_RIGHT,     ST_OPTIONS_BOOT_FUNC},
    {ST_OPTIONS_BOOT,             KEY_LEFT,     ST_OPTIONS},
    {ST_OPTIONS_BOOT,             KEY_DOWN,    ST_OPTIONS_POWER_SAVE},

    {ST_OPTIONS_POWER_SAVE,       KEY_UP,     ST_OPTIONS_BOOT},
    {ST_OPTIONS_POWER_SAVE,       KEY_RIGHT,     ST_OPTIONS_POWER_SAVE_FUNC},
    {ST_OPTIONS_POWER_SAVE,       KEY_LEFT,     ST_OPTIONS},
    {ST_OPTIONS_POWER_SAVE,       KEY_DOWN,    ST_OPTIONS_AUTO_POWER_SAVE},
        
    {ST_OPTIONS_AUTO_POWER_SAVE,  KEY_UP,     ST_OPTIONS_POWER_SAVE},
    {ST_OPTIONS_AUTO_POWER_SAVE,  KEY_RIGHT,     ST_OPTIONS_AUTO_POWER_SAVE_FUNC},
    {ST_OPTIONS_AUTO_POWER_SAVE,  KEY_LEFT,     ST_OPTIONS},
    {ST_OPTIONS_AUTO_POWER_SAVE,  KEY_DOWN,    ST_OPTIONS_DISPLAY},
        
    {0,                         0,          0}
};


MENU_STATE menu_state[] = {
//  STATE                               STATE TEXT                  STATE_FUNC
    {ST_AVRBF,                          MT_AVRBF,                   NULL},
    {ST_AVRBF_REV,                      NULL,                       Revision},
    {ST_TIME,                           MT_TIME,                    NULL},
    {ST_TIME_CLOCK,                     MT_TIME_CLOCK,              NULL},
    {ST_TIME_CLOCK_FUNC,                NULL,                       ShowClock},
    {ST_TIME_CLOCK_ADJUST,              MT_TIME_CLOCK_ADJUST,       NULL},
    {ST_TIME_CLOCK_ADJUST_FUNC,         NULL,                       SetClock},
    {ST_TIME_CLOCKFORMAT_ADJUST,        MT_TIME_CLOCKFORMAT_ADJUST, NULL},
    {ST_TIME_CLOCKFORMAT_ADJUST_FUNC,   NULL,                       SetClockFormat},
    {ST_TIME_DATE,                      MT_TIME_DATE,               NULL},
    {ST_TIME_DATE_FUNC,                 NULL,                       ShowDate},
    {ST_TIME_DATE_ADJUST,               MT_TIME_DATE_ADJUST,        NULL},
    {ST_TIME_DATE_ADJUST_FUNC,          NULL,                       SetDate},    
    {ST_TIME_DATEFORMAT_ADJUST,         MT_TIME_DATEFORMAT_ADJUST,  NULL},
    {ST_TIME_DATEFORMAT_ADJUST_FUNC,    NULL,                       SetDateFormat},     
    {ST_MUSIC,                          MT_MUSIC,                   NULL},
    {ST_MUSIC_SELECT,                   NULL,                       SelectSound},
    {ST_MUSIC_PLAY,                     NULL,                       Sound},
    {ST_VCARD,                          MT_VCARD,                   NULL},
    {ST_VCARD_FUNC,                     NULL,                       vCard},
    {ST_VCARD_ENTER_NAME,               MT_ENTERNAME,               NULL},
    {ST_VCARD_DOWNLOAD_NAME,            MT_VCARD_DOWNLOAD_NAME,     NULL},
    {ST_VCARD_ENTER_NAME_FUNC,          NULL,                       EnterName},
    {ST_VCARD_DOWNLOAD_NAME_FUNC,       NULL,                       RS232},
    {ST_TEMPERATURE,                    MT_TEMPERATURE,             NULL},
    {ST_TEMPERATURE_FUNC,               NULL,                       TemperatureFunc},
    {ST_VOLTAGE,                        MT_VOLTAGE,                 NULL},
    {ST_VOLTAGE_FUNC,                   NULL,                       VoltageFunc},
    {ST_LIGHT,                          MT_LIGHT,                   NULL},
    {ST_LIGHT_FUNC,                     NULL,                       LightFunc},
    {ST_OPTIONS,                        MT_OPTIONS,                 NULL},
    {ST_OPTIONS_DISPLAY,                MT_OPTIONS_DISPLAY,         NULL},
    {ST_OPTIONS_DISPLAY_CONTRAST,       MT_OPTIONS_DISPLAY_CONTRAST,NULL},
    {ST_OPTIONS_DISPLAY_CONTRAST_FUNC,  NULL,                       SetContrast},
    {ST_OPTIONS_BOOT,                   MT_OPTIONS_BOOT,            NULL},
    {ST_OPTIONS_BOOT_FUNC,              NULL,                       BootFunc},    
    {ST_OPTIONS_POWER_SAVE,             MT_OPTIONS_POWER_SAVE,      NULL},
    {ST_OPTIONS_POWER_SAVE_FUNC,        NULL,                       PowerSaveFunc},
    {ST_OPTIONS_AUTO_POWER_SAVE,        MT_OPTIONS_AUTO_POWER_SAVE, NULL},
    {ST_OPTIONS_AUTO_POWER_SAVE_FUNC,   NULL,                       AutoPower},

    {0,                                 NULL,                       NULL},
    
};
*/



// 
// void printNodeMenu(NODE curr_node){
// 	char output[30];
// 	uint8_t pos;
// 	for(int i = 0; i< curr_node.numSubMenu;i++){
// 		pos = 20+i*10;
// 		if (i!=curr_node.current_pointer){
// 			drawString(8, pos,curr_node.submenu[i]->nodeName,WHITE,BLACK);
// 		}
// 		else (i!=curr_node.current_pointer){
// 			drawString(8, pos,curr_node.submenu[i]->nodeName,BLACK,WHITE);
// 		}
// 	}
// 	
// }



void printMenu(uint8_t pos,uint8_t menuIndex, uint8_t selection){
	char output[30];
	char* menu[4];
	menu[0] = "Option 1";
	menu[1] = "Option 2";
	menu[2] = "Option 3";
	menu[3] = "Option 4";
	char* submenu1[3]; // linked to option 2
	submenu1[0] = "Option 1";
	submenu1[1] = "Option 2";
	submenu1[2] = "Back";

	
	switch(menuIndex){
		case 1:
			for(int i = 0; i<4;i++){
				uint8_t position = 30+i*10;
				if (i!=pos){
					drawString(8, position,menu[i],WHITE,BLACK);
				}
				else {
					drawString(8, position,menu[i],BLACK,WHITE);
				}
			}
		break;
		case 2:
			for(int i = 0; i<3;i++){
				uint8_t position = 30+i*10;
				if (i!=pos){
					drawString(8, position,submenu1[i],WHITE,BLACK);
				}
				else {
					drawString(8, position,submenu1[i],BLACK,WHITE);
				}
			}
		break;
	}


}