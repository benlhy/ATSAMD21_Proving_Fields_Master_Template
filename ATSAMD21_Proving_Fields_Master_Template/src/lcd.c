// functions to operate the ST7735 on the PIC32
// adapted from https://github.com/sumotoy/TFT_ST7735
// and https://github.com/adafruit/Adafruit-ST7735-Library


// B8 is turned into SDI1 but is not used or connected to anything

#include "lcd.h"

#define MAX_X 160
#define MAX_Y 128

#define ST77XX_MADCTL_MY  0x80
#define ST77XX_MADCTL_MX  0x40
#define ST77XX_MADCTL_MV  0x20
#define ST77XX_MADCTL_ML  0x10
#define ST77XX_MADCTL_RGB 0x00
#define ST77XX_MADCTL  0x36

void LCD_rotate(uint8_t m){
	uint8_t madctl = 0;
	uint8_t rotation = m % 4; // can't be higher than 3

	switch (rotation) {
		case 0:

			madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MY | ST77XX_MADCTL_RGB;
			

		break;
		case 1:

			madctl = ST77XX_MADCTL_MY | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB;
			
		case 2:
		
			madctl = ST77XX_MADCTL_RGB;

		break;
		case 3:
		
			madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB;
			
		break;
	}
	LCD_command(ST77XX_MADCTL);
	LCD_data(madctl);
}


void configure_port_pins(void)
{
	struct port_config config_port_pin;
	port_get_config_defaults(&config_port_pin);
	config_port_pin.direction = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(PIN_PA16, &config_port_pin);
}


void swap_num(short* a,short* b){
	 short t = *a; 
	 *a = *b; 
	 *b = t;
}

void LCD_drawLine(short x0,short y0,short x1,short y1,short c){
	    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        swap_num(&x0, &y0);
        swap_num(&x1, &y1);
    }

    if (x0 > x1) {
        swap_num(&x0, &x1);
        swap_num(&y0, &y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x0<=x1; x0++) {
        if (steep) {
            LCD_drawPixel(y0, x0, c);
        } else {
            LCD_drawPixel(x0, y0, c);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

void drawPoints(char* points,int c){
	for (int i = 0;i<MAX_Y;i++){
		LCD_drawPixel(points[i],i,c);
	}
}

//draws line at y
void LCD_drawYLine(short y,int c){
	int i;
	for(i=0;i<MAX_Y;i++){
		LCD_drawPixel(y,i,c);
	}
}

// draws line at x
void LCD_drawXLine(short x,int c){
	int i;
	for(i=0;i<MAX_X;i++){
		LCD_drawPixel(i,x,c);
	}
}

/*
 * len goes from -50 to 50
 */
void drawYBar(short x, short y, short h, int len, int c1, int c2){
    int i;
    // if len is negative, start drawing from negative to center
    if (len < 0){
        drawProgressBarVert(x, y, h, 50+len, c2, 50, c1);// flip the colors
        drawProgressBarVert(x,y+50, h,0,c1,50,c2);
    }
    else{
        drawProgressBarVert(x,y,h,0,c1,50,c2);
        drawProgressBarVert(x,y+50,h,len,c1,50,c2);
    }
}


/*
 * len goes from -50 to 50
 */
void drawXBar(short x, short y, short h, int len, int c1, int c2){
    
    // if len is negative, start drawing from negative to center
    if (len < 0){
        drawProgressBar(x, y, h, 50+len, c2, 50, c1);// flip the colors
        drawProgressBar(x+50,y, h,0,c1,50,c2);
    }
    else{
        drawProgressBar(x,y,h,0,c1,50,c2);
        drawProgressBar(x+50,y,h,len,c1,50,c2);
    }
}

void drawProgressBarVert(short x, short y, short h, int len1, short c1, int len2, short c2){
    int i;
    for (i=0;i<len1;i++){
        int j;
        for (j=0;j<h;j++){
            LCD_drawPixel(x+j,y+i,c1);
        }
    }
    for (i=len1;i<len2;i++){
        int j;
        for (j=0;j<h;j++){
            LCD_drawPixel(x+j,y+i,c2);
        }
    }
}

void drawProgressBar(short x, short y, short h, int len1, short c1, int len2, short c2){
    int i;
    for (i=0;i<len1;i++){
        int j;
        for (j=0;j<h;j++){
            LCD_drawPixel(x+i,y+j,c1);
        }
    }
    for (i=len1;i<len2;i++){
        int j;
        for (j=0;j<h;j++){
            LCD_drawPixel(x+i,y+j,c2);
        }
    }
}

void drawString(short x, short y, char* str, short fg, short bg){
    int i = 0;
    while(str[i]){
        drawChar(x+5*i,y,str[i],fg,bg);
        i++;
    }
}


void drawChar(short x, short y, unsigned char c, short fg, short bg){
    char row = c - 0x20;
    int i;
    if ((MAX_X-x>7)&&(MAX_Y-y>7)){
        for(i=0;i<5;i++){
            char pixels = ASCII[row][i]; // so we have a list of pixies to go through
            int j;
            for(j=0;j<8;j++){
                if ((pixels>>j)&1==1){
                    LCD_drawPixel(x+i,y+j,fg);
                }
                else {
                    LCD_drawPixel(x+i,y+j,bg);
                }
            }
        }
    }
}
// SPI uses SERCOM0
// Configuration A
// 0 MOSI : PA04
// 1 SCK  : PA05
// 2 SS   : PA17
// 3 DC   : PA16

#define DAT_PIN PIN_PA16
#define SLAVE_SELECT_PIN PIN_PA17
#define CONF_MASTER_MUX_SETTING SPI_SIGNAL_MUX_SETTING_A
#define CONF_MASTER_PINMUX_PAD0 PINMUX_PA04D_SERCOM0_PAD0
#define CONF_MASTER_PINMUX_PAD1 PINMUX_PA05D_SERCOM0_PAD1
#define CONF_MASTER_PINMUX_PAD2 PINMUX_UNUSED
#define CONF_MASTER_PINMUX_PAD3 PINMUX_UNUSED
#define CONF_MASTER_SPI_MODULE SERCOM0

struct spi_module spi_master_instance;
struct spi_slave_inst slave;


void configure_spi_master(void)
{
	struct spi_config config_spi_master;
	struct spi_slave_inst_config slave_dev_config;
	/* Configure and initialize software device instance of peripheral slave */
	spi_slave_inst_get_config_defaults(&slave_dev_config);
	slave_dev_config.ss_pin = SLAVE_SELECT_PIN;
	spi_attach_slave(&slave, &slave_dev_config);
	/* Configure, initialize and enable SERCOM SPI module */
	spi_get_config_defaults(&config_spi_master);
	/*config_spi_master.transfer_mode = SPI_TRANSFER_MODE_1;*/
	config_spi_master.mux_setting = CONF_MASTER_MUX_SETTING;
	config_spi_master.pinmux_pad0 = CONF_MASTER_PINMUX_PAD0;
	config_spi_master.pinmux_pad1 = CONF_MASTER_PINMUX_PAD1;
	config_spi_master.pinmux_pad2 = CONF_MASTER_PINMUX_PAD2;
	config_spi_master.pinmux_pad3 = CONF_MASTER_PINMUX_PAD3;
	config_spi_master.mode_specific.master.baudrate =  12000000; //12MHz  = 400ns per signal
	spi_init(&spi_master_instance, CONF_MASTER_SPI_MODULE, &config_spi_master);
	spi_enable(&spi_master_instance);
}


void spi_io(unsigned char o) {
	spi_write(&spi_master_instance,o);
}

void LCD_command(unsigned char com) {
	port_pin_set_output_level(DAT_PIN,false);
	spi_select_slave(&spi_master_instance, &slave, true);
	spi_io(com);
	spi_select_slave(&spi_master_instance, &slave, false);
}

void LCD_data(unsigned char dat) {
	port_pin_set_output_level(DAT_PIN,true);
	spi_select_slave(&spi_master_instance, &slave, true);
	spi_io(dat);
	spi_select_slave(&spi_master_instance, &slave, false);
}

void LCD_data16(unsigned short dat) {
	port_pin_set_output_level(DAT_PIN,true);
	spi_select_slave(&spi_master_instance, &slave, true);
	spi_io(dat>>8);
	spi_io(dat);
	spi_select_slave(&spi_master_instance, &slave, false);
}




void LCD_init() {
	configure_spi_master();
	delay_ms(1);
  LCD_command(ST7735_SWRESET);//software reset
  delay_ms(50);
	LCD_command(ST7735_SLPOUT);//exit sleep
	delay_ms(5);
	LCD_command(ST7735_FRMCTR1);//Frame Rate Control (In normal mode/Full colors)
	LCD_data(0x01);
	LCD_data(0x2C);
	LCD_data(0x2D);
	delay_ms(1);
	LCD_command(ST7735_FRMCTR2);//Frame Rate Control (In normal mode/Full colors)
	LCD_data(0x01);
	LCD_data(0x2C);
	LCD_data(0x2D);
	delay_ms(1);
	LCD_command(ST7735_FRMCTR3);//Frame Rate Control (In normal mode/Full colors)
	LCD_data(0x01);
	LCD_data(0x2C);
	LCD_data(0x2D);
	LCD_data(0x01);
	LCD_data(0x2C);
	LCD_data(0x2D);
	delay_ms(1);
	LCD_command(ST7735_INVCTR);//display inversion
	LCD_data(0x07);
	delay_ms(1);
	LCD_command(ST7735_PWCTR1);
	LCD_data(0x0A);//4.30 - 0x0A
	LCD_data(0x02);//0x05
	LCD_data(0x84);//added auto mode
	delay_ms(1);
	LCD_command(ST7735_PWCTR2);
	LCD_data(0xC5);
	delay_ms(1);
	LCD_command( ST7735_PWCTR3);
	LCD_data(0x0A);
	LCD_data(0x00);
	delay_ms(1);
	LCD_command( ST7735_PWCTR4);
	LCD_data(0x8A);
	LCD_data(0x2A);
	delay_ms(1);
	LCD_command( ST7735_PWCTR5);
	LCD_data(0x8A);
	LCD_data(0xEE);
	delay_ms(1);
	LCD_command(ST7735_VMCTR1);
	LCD_data(0x0E);
	delay_ms(1);
	LCD_command(ST7735_INVOFF);
	LCD_command(ST7735_MADCTL);
	LCD_data(0xC8);
	delay_ms(1);
	LCD_command(ST7735_COLMOD);
	LCD_data(0x05);
	delay_ms(1);
	LCD_command(ST7735_CASET);
	LCD_data(0x00);
	LCD_data(0x00);
	LCD_data(0x00);
	LCD_data(0x7F);
	delay_ms(1);
	LCD_command(ST7735_RASET);
	LCD_data(0x00);
	LCD_data(0x00);
	LCD_data(0x00);
	LCD_data(0x9F);
	delay_ms(1);
	LCD_command(ST7735_GMCTRP1);
	LCD_data(0x02);
	LCD_data(0x1C);
	LCD_data(0x07);
	LCD_data(0x12);
	LCD_data(0x37);
	LCD_data(0x32);
	LCD_data(0x29);
	LCD_data(0x2D);
	LCD_data(0x29);
	LCD_data(0x25);
	LCD_data(0x2B);
	LCD_data(0x39);
	LCD_data(0x00);
	LCD_data(0x01);
	LCD_data(0x03);
	LCD_data(0x10);
	delay_ms(1);
	LCD_command(ST7735_GMCTRN1);
	LCD_data(0x03);
	LCD_data(0x1D);
	LCD_data(0x07);
	LCD_data(0x06);
	LCD_data(0x2E);
	LCD_data(0x2C);
	LCD_data(0x29);
	LCD_data(0x2D);
	LCD_data(0x2E);
	LCD_data(0x2E);
	LCD_data(0x37);
	LCD_data(0x3F);
	LCD_data(0x00);
	LCD_data(0x00);
	LCD_data(0x02);
	LCD_data(0x10);
	delay_ms(1);
	LCD_command(ST7735_NORON);
	delay_ms(10);
	LCD_command(ST7735_DISPON);
	delay_ms(100);
	LCD_command(ST7735_MADCTL); // rotation
    LCD_data(MADCTL_MY | MADCTL_MV | MADCTL_RGB);
}

void LCD_drawPixel(unsigned short x, unsigned short y, unsigned short color) {
  // check boundary
  LCD_setAddr(x,y,x+1,y+1);
  LCD_data16(color);
}

void LCD_setAddr(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1) {
  LCD_command(ST7735_CASET); // Column
  LCD_data16(x0);
	LCD_data16(x1);
	LCD_command(ST7735_RASET); // Page
	LCD_data16(y0);
	LCD_data16(y1);
	LCD_command(ST7735_RAMWR); // Into RAM
}

void LCD_clearScreen(unsigned short color) {
  int i;
  LCD_setAddr(0,0,_GRAMWIDTH,_GRAMHEIGH);
	for (i = 0;i < _GRAMSIZE; i++){
		LCD_data16(color);
	}
}