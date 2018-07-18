
#include <asf.h>
#include "lcd.h"

void extint_detection_callback(void)
{
	LCD_clearScreen(RED);
}

void configure_extint_channel(void)
{
	struct extint_chan_conf config_extint_chan;
	extint_chan_get_config_defaults(&config_extint_chan);
	config_extint_chan.gpio_pin = PIN_PB11A_EIC_EXTINT11;
	config_extint_chan.gpio_pin_mux = MUX_PB11A_EIC_EXTINT11;
	config_extint_chan.gpio_pin_pull = EXTINT_PULL_UP;
	config_extint_chan.detection_criteria = EXTINT_DETECT_BOTH;
	extint_chan_set_config(11, &config_extint_chan);

}
void configure_extint_callbacks(void)
{
	extint_register_callback(extint_detection_callback,
	11,
	EXTINT_CALLBACK_TYPE_DETECT);
	extint_chan_enable_callback(11,
	EXTINT_CALLBACK_TYPE_DETECT);
}


int main (void)
{
	system_init();
	configure_extint_channel();
	configure_extint_callbacks();
	delay_init();
	configure_port_pins();

	LCD_init();
	LCD_clearScreen(BLACK);

	/* Insert application code here, after the board has been initialized. */

	/* This skeleton code simply sets the LED to the state of the button. */
	while (1) {
		/* Is button pressed? */
		if (port_pin_get_input_level(BUTTON_0_PIN) == BUTTON_0_ACTIVE) {
			/* Yes, so turn LED on. */
			port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);

				
		} else {
			/* No, so turn LED off. */
			port_pin_set_output_level(LED_0_PIN, !LED_0_ACTIVE);
		}
	}
}
