#ifndef RTC_H_
#define RTC_H_

//Includes
#include "st7735.h"
#include "fonts.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rtc.h"
#include <string.h>
#include "stdio.h"

//Defines
#define RTC_BCKP_RGSTR   0x509C
#define BTN_A_PORT       GPIOC
#define BTN_A_PIN        GPIO_PIN_13
#define BTN_B_PORT       GPIOB
#define BTN_B_PIN        GPIO_PIN_8
#define BTN_A_Read       HAL_GPIO_ReadPin(BTN_A_PORT, BTN_A_PIN)
#define BTN_B_Read       HAL_GPIO_ReadPin(BTN_B_PORT, BTN_B_PIN)

//Variables


//Function prototypes
void set_time (void);
void get_time (void);
bool configure_time (void);

#endif /* RTC_H_ */
