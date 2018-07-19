
#include <asf.h>
#include <stdio.h>
#include "lcd.h"
#include "menu.h"

volatile uint8_t count;
volatile uint8_t selection;
volatile uint8_t menuIndex;
volatile uint8_t update_flag;

void extint_down_callback(void)
{
	count--;
	update_flag = 1;
    delay_ms(200);
}
void extint_up_callback(void)
{
	count++;
	update_flag = 1;
	delay_ms(200);
}

void extint_click_callback(void)
{	
	selection = count;
	count = 0;
	update_flag = 1;
	delay_ms(200);
}

void configure_extint_channel(void)
{
	struct extint_chan_conf config_extint_chan;
	extint_chan_get_config_defaults(&config_extint_chan);
	config_extint_chan.gpio_pin = PIN_PB11A_EIC_EXTINT11;
	config_extint_chan.gpio_pin_mux = MUX_PB11A_EIC_EXTINT11;
	config_extint_chan.gpio_pin_pull = EXTINT_PULL_UP;
	config_extint_chan.detection_criteria = EXTINT_DETECT_RISING;
	extint_chan_set_config(11, &config_extint_chan);
	extint_chan_get_config_defaults(&config_extint_chan);
	config_extint_chan.gpio_pin = PIN_PB10A_EIC_EXTINT10;
	config_extint_chan.gpio_pin_mux = MUX_PB10A_EIC_EXTINT10;
	config_extint_chan.gpio_pin_pull = EXTINT_PULL_UP;
	config_extint_chan.detection_criteria = EXTINT_DETECT_RISING;
	extint_chan_set_config(10, &config_extint_chan);
	extint_chan_get_config_defaults(&config_extint_chan);
	config_extint_chan.gpio_pin = PIN_PA09A_EIC_EXTINT9;
	config_extint_chan.gpio_pin_mux = MUX_PA09A_EIC_EXTINT9;
	config_extint_chan.gpio_pin_pull = EXTINT_PULL_UP;
	config_extint_chan.detection_criteria = EXTINT_DETECT_RISING;
	extint_chan_set_config(9, &config_extint_chan);

}
void configure_extint_callbacks(void)
{
	extint_register_callback(extint_down_callback, 11, EXTINT_CALLBACK_TYPE_DETECT);
	extint_chan_enable_callback(11,EXTINT_CALLBACK_TYPE_DETECT);
	extint_register_callback(extint_up_callback, 10, EXTINT_CALLBACK_TYPE_DETECT);
	extint_chan_enable_callback(10,EXTINT_CALLBACK_TYPE_DETECT);
	extint_register_callback(extint_click_callback, 9, EXTINT_CALLBACK_TYPE_DETECT);
	extint_chan_enable_callback(9,EXTINT_CALLBACK_TYPE_DETECT);
}




int main (void)
{

	uint8_t output[30];
	system_init();
	configure_extint_channel();
	configure_extint_callbacks();
	delay_init();
	configure_port_pins();

	LCD_init();
	LCD_clearScreen(BLACK);
	sprintf(output,"Welcome back!");
	drawString(8,8,output,WHITE,BLACK);
	sprintf(output,"Proving Fields v0.1");
	drawString(8,16,output,WHITE,BLACK);
	LCD_drawXLine(26,WHITE);
	printMenu(0,1,0);
	count = 0;
	update_flag = 0;
	selection = 0;
	menuIndex = 1;
	uint8_t limit = 3;


	/* Insert application code here, after the board has been initialized. */

	/* This skeleton code simply sets the LED to the state of the button. */
	while (1) {

		/* Is button pressed? */
		if (update_flag){
			if (count<0){
				count = 0;
			}
			else if (count>limit){
				count = limit;
			}
			
			if (menuIndex==1 && selection == 2){
				menuIndex = 2;
				selection = 0;
				limit = 2;
				LCD_clearScreen(BLACK);
				sprintf(output,"Welcome back!");
				drawString(8,8,output,WHITE,BLACK);
				sprintf(output,"Proving Fields v0.1");
				drawString(8,16,output,WHITE,BLACK);
				LCD_drawXLine(26,WHITE);
			}
			else if (menuIndex==2 && selection == 2){
				menuIndex = 1;
				selection = 0;
				limit = 3;
				LCD_clearScreen(BLACK);
				sprintf(output,"Welcome back!");
				drawString(8,8,output,WHITE,BLACK);
				sprintf(output,"Proving Fields v0.1");
				drawString(8,16,output,WHITE,BLACK);
				LCD_drawXLine(26,WHITE);
			}
			printMenu(count,menuIndex,selection);
			update_flag = 0;
		}
// 		if (port_pin_get_input_level(BUTTON_0_PIN) == BUTTON_0_ACTIVE) {
// 			/* Yes, so turn LED on. */
// 			
// 			
// 			uint8_t val = (count % 4)-1;
// 			printMenu(val,3);
// 			delay_ms(100);
// 
// 				
// 		} else {
// 			/* No, so turn LED off. */
// 			port_pin_set_output_level(LED_0_PIN, !LED_0_ACTIVE);
// 		}
	}
}
