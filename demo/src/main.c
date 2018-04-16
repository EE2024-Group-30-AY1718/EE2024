/*****************************************************************************
 *   EE2024 Assignment 2    Lab Session W1, Lab Group 30
 *   Name: Julian Shoung; Matric No: A0159944M
 *   Name: Martyn Wong; Matric No: A0159915R
 *   Copyright(C) 2011, EE2024
 *   All rights reserved.
 *
 ******************************************************************************/
//LPC Libraries
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_i2c.h"
#include "lpc17xx_ssp.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_uart.h"

//Baseboard Libraries
#include "joystick.h"
#include "pca9532.h"
#include "acc.h"
#include "oled.h"
#include "rgb.h"
#include "led7seg.h"
#include "stdio.h"
#include "temp.h"
#include "light.h"

//C Libraries
#include <stdio.h>
#include <string.h>

#define NUM_HALF_PERIODS 340
#define TEMP_SCALAR_DIV10 1
#define ACC_THRESHOLD 0.4
#define RGB_RED   0x01
#define RGB_BLUE  0x02

//Set temperature threshold
#define TEMP_THRESHOLD_10C 300

const uint32_t lightLoLimit = 1;
const uint32_t lightHiLimit = 4000;

//Arrays Buffers for UART receive
char umsg[4] = {};
char submsg[4] = {};
char* sensor_readings[50]= {};
char* distance_reading[50]= {};
static char* msg = NULL;

int32_t xoff = 0;
int32_t yoff = 0;
int8_t x = 0;
int8_t y = 0;
int8_t z = 0;

uint8_t clearWarning = 1;

uint8_t data = 0;
uint32_t len = 0;


volatile uint32_t msTicks;

typedef enum {
	STATIONARY, COUNTDOWN, LAUNCH, RETURN
} system_mode_t;
volatile system_mode_t mode;

uint16_t luxLevels[17] = {0x0000,0x8000,0xC000,0xE000,0xF000,0xF800,0xFC00,0xFE00,0xFF00,0xFF80,0xFFC0,0xFFE0,0xFFF0,0xFFF8,0xFFFC,0xFFFE,0xFFFF};
uint8_t countdownNum[16] = {0xAA,0xA2,0x61,0xA6,0x23,0x28,0x38,0x20,0x7c,0x22,0x32,0x39,0x70,0xE0,0x7D,0x24};
char result[20];
char temp[20];
volatile int isOverheated = 0;
volatile int isOverheatedCountdown = 0;
volatile int isFirstClick = 1;
volatile int isClickedAgain = 0;
volatile int isVeered = 0;
volatile int isNearObs = 0;
volatile int firstEntry = 1;
volatile int obsFlag = 0;
volatile int32_t tempCount = 0;
volatile int32_t temperature = 0;
volatile int32_t light_val = 0;
volatile uint32_t startTime = 0;
volatile uint32_t endTime = 0;
volatile uint32_t startBlinkRed = 0;
volatile uint32_t endBlinkRed = 0;
volatile uint32_t startBlinkBlue = 0;
volatile uint32_t endBlinkBlue = 0;
volatile uint32_t startLaunchLoop = 0;
volatile uint32_t endLaunchLoop = 0;
volatile uint32_t currClick = 0;
volatile uint32_t initialClick = 0;
volatile uint8_t rgbRedStatus = 0x01;
volatile uint8_t rgbBlueStatus = 0x02;

void SysTick_Handler(void) {
	msTicks++;
}

uint32_t getMsTicks() {
	return msTicks;
}

static void init_ssp(void)
{
	SSP_CFG_Type SSP_ConfigStruct;
	PINSEL_CFG_Type PinCfg;

	/*
	 * Initialize SPI pin connect
	 * P0.7 - SCK;
	 * P0.8 - MISO
	 * P0.9 - MOSI
	 * P2.2 - SSEL - used as GPIO
	 */
	PinCfg.Funcnum = 2;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = 0;
	PinCfg.Pinnum = 7;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 8;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 9;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Funcnum = 0;
	PinCfg.Portnum = 2;
	PinCfg.Pinnum = 2;
	PINSEL_ConfigPin(&PinCfg);

	SSP_ConfigStructInit(&SSP_ConfigStruct);

	// Initialize SSP peripheral with parameter given in structure above
	SSP_Init(LPC_SSP1, &SSP_ConfigStruct);

	// Enable SSP peripheral
	SSP_Cmd(LPC_SSP1, ENABLE);
}

static void init_i2c(void)
{
	PINSEL_CFG_Type PinCfg;

	/* Initialize I2C2 pin connect */
	PinCfg.Funcnum = 2;
	PinCfg.Pinnum = 10;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 11;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 5;
	PinCfg.Portnum = 2;
	PINSEL_ConfigPin(&PinCfg);

	// Initialize I2C peripheral
	I2C_Init(LPC_I2C2, 100000);

	/* Enable I2C1 operation */
	I2C_Cmd(LPC_I2C2, ENABLE);
}

static void init_GPIO(void) {
	// Initialize button, not necessary for SW4 as default
	PINSEL_CFG_Type PinCfg;

	PinCfg.Funcnum = 0;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = 1;
	PinCfg.Pinnum = 31;
	PINSEL_ConfigPin(&PinCfg);
	GPIO_SetDir(1, 1<<31, 0);
}

static void init_RGB(void) {
	PINSEL_CFG_Type PinCfg;

	PinCfg.Funcnum = 0;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = 2;
	PinCfg.Pinnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 1;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Portnum = 0;
	PinCfg.Pinnum = 26;
	PINSEL_ConfigPin(&PinCfg);
	GPIO_SetDir( 2, (1<<0), 1 );
	GPIO_SetDir( 0, (1<<26), 1 );
	GPIO_SetDir( 2, (1<<1), 1 );
}

void pinsel_uart3(void){
    PINSEL_CFG_Type PinCfg;
    PinCfg.Funcnum = 2;
    PinCfg.Pinnum = 0;
    PinCfg.Portnum = 0;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 1;
    PINSEL_ConfigPin(&PinCfg);
}

void setRGB(uint8_t ledMask) {
    if ((ledMask & RGB_RED) != 0) {
        GPIO_SetValue( 2, (1<<0));
    } else {
        GPIO_ClearValue( 2, (1<<0));
    }

    if ((ledMask & RGB_BLUE) != 0) {
        GPIO_SetValue( 0, (1<<26) );
    } else {
        GPIO_ClearValue( 0, (1<<26) );
    }
}

void int_temp_read(void) {
	if (tempCount == 0) {
		startTime = getMsTicks();
		endTime = 0;
		tempCount++;
	} else if (tempCount == 340) {
		endTime = getMsTicks();
		tempCount = 0;
	    if (endTime > startTime) {
	    	endTime = endTime - startTime;
	    } else {
	    	endTime = (0xFFFFFFFF - startTime + 1) + endTime; // In the case of overflow
	    }
		temperature = ( (1000*endTime) / (NUM_HALF_PERIODS*TEMP_SCALAR_DIV10) - 2731 );
		if ((mode == COUNTDOWN) && (temperature >= TEMP_THRESHOLD_10C) && !isOverheatedCountdown) {
			isOverheatedCountdown = 1;
		}
	} else {
		tempCount++;
	}
}

// Pre: Button has already been clicked once since entering Launch mode
// Checks if the click is within one second of the previous click
void return_check(void) {
	if (isClickedAgain) {
		currClick = getMsTicks();
		if (currClick > initialClick) {
			if (currClick - initialClick <= 1000) { // Clicked twice within 1 second, launch->return mode
				mode = RETURN;
				isFirstClick = 1;
				isClickedAgain = 0;
				oled_clearScreen(OLED_COLOR_BLACK);
				msg = "Entering RETURN Mode \r\n";
				UART_Send(LPC_UART3, (uint8_t *) msg, strlen(msg), BLOCKING);
			} else { // Set initial click time to the most recent click
				initialClick = currClick;
				isClickedAgain = 0;
			}
		}  else {
			currClick = (0xFFFFFFFF - initialClick + 1) + currClick; // In the case of overflow
		}
	}
}

void send_sensor_readings(void) {
	sprintf(sensor_readings, "RPT Temp : %.2f; ACC X : %.2f, Y : %.2f \r\n", temperature/10.0, x/64.0, y/64.0);
	UART_Send(LPC_UART3, (uint8_t *)sensor_readings, strlen(sensor_readings), BLOCKING);
}

void send_distance_reading(void) {
	sprintf(distance_reading,"RPT Obstacle Distance: %d \r\n", light_val);
	UART_Send(LPC_UART3, (uint8_t *)distance_reading, strlen(distance_reading), BLOCKING);
}

//UART interrupt handler
void UART3_IRQHandler (void) {
	 //UART3_StdIntHandler();

	 if( ((LPC_UART3->IIR & 0xE) == 0b0100) || ((LPC_UART3->IIR & 0xE) == 0b1100) ) {
		 UART_Receive(LPC_UART3, &data, 1, NONE_BLOCKING);
		 if (data){

			 //If input is not enter or a null character
			 if ( data != '\r' && data !='\n'){
				 umsg[len] = data;
				 len++;
			 }
			 //If user has pressed enter
			 if ( data == '\r'){

				 //Sets 4th character to be null
				 umsg[3] = '\0';

				 //Check for RPT message
				 if (strcmp(umsg,"RPT") == 0) {

					 if (mode == LAUNCH) {
						 send_sensor_readings();
					 }
					 if (mode == RETURN) {
						 send_distance_reading();
					 }
				 }
				 len = 0;
			 }
		 }
		 data = 0;
	 } else  {
		 //Clear interrupt and data
		 UART_Receive(LPC_UART3, &data, 1, NONE_BLOCKING);
		 data = 0;
	 }
}

void init_uart(void){
	UART_CFG_Type uartCfg;
    uartCfg.Baud_rate = 115200;
    uartCfg.Databits = UART_DATABIT_8;
    uartCfg.Parity = UART_PARITY_NONE;
    uartCfg.Stopbits = UART_STOPBIT_1;
    //pin select for uart3;
    pinsel_uart3();
    //supply power & setup working parameters for uart3
    UART_Init(LPC_UART3, &uartCfg);
    //enable transmit for uart3
    UART_TxCmd(LPC_UART3, ENABLE);

	UART_FIFO_CFG_Type FFConfig;

	UART_FIFOConfigStructInit(&FFConfig);
	FFConfig.FIFO_DMAMode = DISABLE;
	FFConfig.FIFO_ResetRxBuf = ENABLE;
	FFConfig.FIFO_ResetTxBuf = ENABLE;
	FFConfig.FIFO_Level = UART_FIFO_TRGLEV2;
	LPC_UART3->FCR = 1;

	UART_FIFOConfig(LPC_UART3, &FFConfig);
    UART_SetupCbs(LPC_UART3, 0, (void *)UART3_IRQHandler);
}

// EINT3 Interrupt Handler
void EINT3_IRQHandler(void) {

	//SW3
	if ((LPC_GPIOINT->IO2IntStatF>>10)& 0x1) {
		if (mode == STATIONARY) {
			mode = COUNTDOWN;
		} else if (mode == LAUNCH) {
	    	if (isFirstClick) {
	    		initialClick = getMsTicks();
		    	isFirstClick = 0;
	    	} else {
	    		isClickedAgain = 1;
	    	}
	    } else if (mode == RETURN) {
	    	mode = STATIONARY;
	    	firstEntry = 1;
	    }
		LPC_GPIOINT->IO2IntClr = 1<<10;
	}

	//Temperature
	if ((LPC_GPIOINT->IO0IntStatF>>2)& 0x1) {
		int_temp_read();
		LPC_GPIOINT->IO0IntClr = 1<<2;
	}
}

int main (void) {
    init_uart();

    init_i2c();
    init_ssp();
    init_GPIO();
    init_RGB();

    pca9532_init();
    joystick_init();
    acc_init();
    oled_init();
    led7seg_init();

    temp_init(&getMsTicks);
    light_enable();
    light_setRange(LIGHT_RANGE_4000);

    // Enable GPIO Interrupt P2.10 (SW3)
    LPC_GPIOINT->IO2IntEnF |= 1<<10;

    // Enable GPIO Interrupt P0.2 (Temp)
    LPC_GPIOINT->IO0IntEnF |= 1<<2;

    // Enable EINT3 interrupt
    NVIC_ClearPendingIRQ(EINT3_IRQn);
    NVIC_EnableIRQ(EINT3_IRQn);

    //Enable Interrupt for UART3
    NVIC_ClearPendingIRQ(UART3_IRQn);
    UART_IntConfig(LPC_UART3, UART_INTCFG_RBR, ENABLE);
    NVIC_EnableIRQ(UART3_IRQn);


    //Setup SysTick Timer to interrupt at 1 msec intervals
    if (SysTick_Config(SystemCoreClock / 1000)) {
    	while(1);
    }

    /*
     * Assume base board in zero-g position when reading first value.
     */
    acc_read(&x, &y, &z);
    xoff = 0-x;
    yoff = 0-y;
    //zoff = 64-z;

    oled_clearScreen(OLED_COLOR_BLACK);

    while (1) {
     /* ####### Modes  ###### */
    	switch(mode) {
    		//Features: Temperature, Launch countdown
			case STATIONARY:
				if (firstEntry) {
					pca9532_setLeds(0, 0xffff);
					oled_clearScreen(OLED_COLOR_BLACK);
					msg = "Entering STATIONARY Mode \r\n";
					UART_Send(LPC_UART3, (uint8_t *) msg, strlen(msg), BLOCKING);
					firstEntry = 0;
				}
				isNearObs = 0;
				led7seg_setChar(countdownNum[0],TRUE);
				if (!isOverheated) {
					sprintf(result,"STATIONARY");
					oled_putString(0, 0, (uint8_t *)result,OLED_COLOR_WHITE, OLED_COLOR_BLACK );
				}
				break;
			case COUNTDOWN:
				oled_clearScreen(OLED_COLOR_BLACK);
				sprintf(result,"Entering LAUNCH Mode");
				oled_putString(0, 0, (uint8_t *)result,OLED_COLOR_WHITE, OLED_COLOR_BLACK );

				int count = 0;

				while(1) {
					led7seg_setChar(countdownNum[count],TRUE);
					Timer0_Wait(200); // Reduced for quick testing, usually 1000
					count++;
					if (isOverheatedCountdown) { // Temp warning, ABORT launch
						mode = STATIONARY;
						isOverheatedCountdown = 0;
						oled_clearScreen(OLED_COLOR_BLACK);
						break;
					} else if (count == 15) { // Finished countdown, no longer stationary
						mode = LAUNCH;
						oled_clearScreen(OLED_COLOR_BLACK);
						msg = "Entering LAUNCH Mode \r\n";
						UART_Send(LPC_UART3, (uint8_t *) msg, strlen(msg), BLOCKING);
						break;
					}
				}
				break;

			/*Features: temperature, acc readings;
			  blink red and blue LED for warning;
			  send UART for warnings within 1s
			  send UART every 10s */
			case LAUNCH:
				led7seg_setChar(countdownNum[15],TRUE);
				if (!isOverheated) { // stop it from overwriting the temp too high
					sprintf(result,"LAUNCH");
					oled_putString(0, 0, (uint8_t *)result,OLED_COLOR_WHITE, OLED_COLOR_BLACK );
				}
				acc_read(&x, &y, &z);

				//Absolute function for negative value
				x = abs(x)+xoff;
				y = abs(y)+yoff;

				sprintf(result,"X: %.2fg\n", x/64.0);
				oled_putString(0, 24, (uint8_t *)result,OLED_COLOR_WHITE, OLED_COLOR_BLACK );
				sprintf(result,"Y: %.2fg\n", y/64.0);
				oled_putString(0, 36, (uint8_t *)result,OLED_COLOR_WHITE, OLED_COLOR_BLACK );
				if (!isVeered && ((x/64.0 >= ACC_THRESHOLD) | (y/64.0 >= ACC_THRESHOLD))) {
					isVeered = 1;
					startBlinkBlue = getMsTicks();
					msg = "VEER OFF COURSE \r\n";
					UART_Send(LPC_UART3, (uint8_t *)msg , strlen(msg), BLOCKING);
				}

				//Send temp and acc reading every 10 seconds
				/*endLaunchLoop = getMsTicks();
				if (endLaunchLoop - startLaunchLoop >= 10000) {
					sprintf(result, "UPDATE Temp : %.2f; ACC X : %.2f, Y : %.2f \r\n", temperature/10.0, x/64.0, y/64.0);
					msg = result;
					UART_Send(LPC_UART3, (uint8_t *)msg , strlen(msg), BLOCKING);
					startLaunchLoop = endLaunchLoop;
				}*/

				return_check();
				break;

			//Show obstacle distance
			case RETURN:
				if (isOverheated) {
					setRGB(0);
					isOverheated = 0;
					oled_clearScreen(OLED_COLOR_BLACK);
				}
				if (isVeered) {
					setRGB(0);
					isVeered = 0;
					oled_clearScreen(OLED_COLOR_BLACK);
				}
				led7seg_setChar(countdownNum[15],TRUE);
				sprintf(result,"RETURN");
				oled_putString(0, 0, (uint8_t *)result,OLED_COLOR_WHITE, OLED_COLOR_BLACK );
		    	light_val = light_read();
		    	//sprintf(result,"Light: %d lux", light_val);
		    	sprintf(result,"Obstacle");
		    	oled_putString(0, 12, (uint8_t *)result,OLED_COLOR_WHITE, OLED_COLOR_BLACK );
		    	sprintf(result,"Distance: %d", light_val);
		    	oled_putString(0, 24, (uint8_t *)result,OLED_COLOR_WHITE, OLED_COLOR_BLACK );
		    	if (light_val > 3000) {
		    		if (!isNearObs) {
		    			msg = "OBSTACLE NEAR \r\n";
		    			UART_Send(LPC_UART3, (uint8_t *)msg , strlen(msg), BLOCKING);
		    			obsFlag = 1;
		    		}
		    		isNearObs = 1;
		    	} else {
		    		if (isNearObs) {
		    			msg = "OBSTACLE AVOIDED \r\n";
		    			UART_Send(LPC_UART3, (uint8_t *)msg , strlen(msg), BLOCKING);
		    			obsFlag = 1;
		    		}
		    		isNearObs = 0;
		    	}
		    	light_val = (light_val/3892.0) * 17.0;
				pca9532_setLeds(luxLevels[light_val], 0xffff);

//				uint8_t data[10];
//				uint32_t len = 0;
//				uint8_t line[64];

				//Receive and Send up to 3 letters to the terminal
//				UART_Receive(LPC_UART3, data, 3, BLOCKING);
//				data[3] = '\0';
//				printf("%s\n", data);
//				data[3] = '\n';
//				data[4] = '\0';
//				UART_Send(LPC_UART3, data, strlen(data), BLOCKING);

				break;
			default:
				break;
    	}

        /* ####### Warnings ###### */
        /* # */

		if ((mode != RETURN) && (temperature >= TEMP_THRESHOLD_10C) && !isOverheated) {
        	rgbBlueStatus = RGB_BLUE; // veer -> overheat
			isOverheated = 1;
			oled_clearScreen(OLED_COLOR_BLACK);
			startBlinkRed = getMsTicks();
			msg = "TEMP TOO HIGH \r\n";
			UART_Send(LPC_UART3, (uint8_t *)msg , strlen(msg), BLOCKING);
		}

		if (isOverheated && isVeered) {
    		endBlinkBlue = getMsTicks();
    		if (endBlinkBlue - startBlinkBlue >= 333) {
    			startBlinkBlue = endBlinkBlue;
    			rgbBlueStatus = ~rgbBlueStatus;
    			rgbBlueStatus &= 0x03; // Only keep 2 LSB
    		}
    		clearWarning = (GPIO_ReadValue(1) >> 31) & 0x01;
            if (clearWarning == 0) {
            	rgbBlueStatus = RGB_BLUE;
            	isOverheated = 0;
            	isVeered = 0;
            	setRGB(0);
				oled_clearScreen(OLED_COLOR_BLACK);
            } else {
				setRGB(rgbBlueStatus);
            	sprintf(result, "TEMP TOO HIGH");
        		oled_putString(0, 0, (uint8_t *)result,OLED_COLOR_WHITE, OLED_COLOR_BLACK );
            	sprintf(result, "VEER OFF COURSE");
        		oled_putString(0, 48, (uint8_t *)result,OLED_COLOR_WHITE, OLED_COLOR_BLACK );
            }
		}
		else if (isOverheated) {
    		endBlinkRed = getMsTicks();
    		if (endBlinkRed - startBlinkRed >= 333) {
    			startBlinkRed = endBlinkRed;
    			rgbRedStatus ^= RGB_RED;
    		}
    		clearWarning = (GPIO_ReadValue(1) >> 31) & 0x01;
            if (clearWarning == 0 && temperature < TEMP_THRESHOLD_10C) {
            	rgbRedStatus = RGB_RED;
            	isOverheated = 0;
            	setRGB(0);
				oled_clearScreen(OLED_COLOR_BLACK);
            } else {
				setRGB(rgbRedStatus);
            	sprintf(result, "TEMP TOO HIGH");
        		oled_putString(0, 0, (uint8_t *)result,OLED_COLOR_WHITE, OLED_COLOR_BLACK );
            }
    	}
		else if (isVeered) {
    		endBlinkBlue = getMsTicks();
    		if (endBlinkBlue - startBlinkBlue >= 333) {
    			startBlinkBlue = endBlinkBlue;
    			rgbBlueStatus ^= RGB_BLUE;
    		}
    		clearWarning = (GPIO_ReadValue(1) >> 31) & 0x01;
            if (clearWarning == 0) {
            	rgbBlueStatus = RGB_BLUE;
            	isVeered = 0;
            	setRGB(0);
				oled_clearScreen(OLED_COLOR_BLACK);
            } else {
				setRGB(rgbBlueStatus);
            	sprintf(result, "VEER OFF COURSE");
        		oled_putString(0, 48, (uint8_t *)result,OLED_COLOR_WHITE, OLED_COLOR_BLACK );
            }
		}

		if (isNearObs && obsFlag) {
        	sprintf(result, "OBSTACLE NEAR");
    		oled_putString(0, 48, (uint8_t *)result,OLED_COLOR_WHITE, OLED_COLOR_BLACK );
    		obsFlag = 0;
		} else if (!isNearObs && obsFlag){
        	sprintf(result, "                    ");
    		oled_putString(0, 48, (uint8_t *)result,OLED_COLOR_WHITE, OLED_COLOR_BLACK );
    		obsFlag = 0;
		}

		if (mode != RETURN) {
			sprintf(result, "Temp: %.2fC\n", temperature/10.0);
			oled_putString(0, 12, (uint8_t *)result,OLED_COLOR_WHITE, OLED_COLOR_BLACK );
		}


        Timer0_Wait(1);
    }


}

void check_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while(1);
}

