//Includes
#include "RTC.h"

//Defines
#define MORNING "!"
#define DAY " "
#define EVENING "\""
#define NIGHT "#"
#define SIGN_X_POSS 0
#define SIGN_Y_POSS 0
#define TIME_X_POSS 22
#define TIME_Y_POSS 0
#define DATE_X_POSS 28
#define DATE_Y_POSS 26
#define CFG_X_POSS  0
#define CFG_Y_POSS  26

//Typedefs
extern RTC_HandleTypeDef hrtc;
extern TIM_HandleTypeDef htim3;

//Variables
extern char str1[100];

//Functions
void delay_ms(uint16_t au16_ms) {
    while(au16_ms > 0)
    {
    htim3.Instance->CNT = 0;
    au16_ms--;
    while (htim3.Instance->CNT < 1000);
    }
}

void RTC_error (void) {
  ST7735_WriteString(TIME_X_POSS, TIME_Y_POSS, "RTC ERROR", Font_7x10, ST7735_BLACK, ST7735_RED);
}

void set_time (void) {
	RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
	sTime.Hours = 0x19;
  sTime.Minutes = 0x10;
  sTime.Seconds = 0x10;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK) {
    RTC_error();
  }
  sDate.WeekDay = RTC_WEEKDAY_THURSDAY;
  sDate.Month = RTC_MONTH_MARCH;
  sDate.Date = 0x31;
  sDate.Year = 0x22;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK) {
    RTC_error();
  }
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, RTC_BCKP_RGSTR);  // BCKP register
}

void get_time (void) {
	char time[50];
	RTC_TimeTypeDef gTime;
  RTC_DateTypeDef gDate;
	HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);  // Get time
	HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);  // Get date
	if (gTime.Hours >= 6 && gTime.Hours < 12) {
		ST7735_WriteString(SIGN_X_POSS, SIGN_Y_POSS, MORNING, myFont_16x18, ST7735_YELLOW, ST7735_BLACK);
	}
	if (gTime.Hours >= 12 && gTime.Hours < 18) {
		ST7735_WriteString(SIGN_X_POSS, SIGN_Y_POSS, DAY, myFont_16x18, ST7735_YELLOW, ST7735_BLACK);
	}
	if (gTime.Hours >= 18 && gTime.Hours <= 23) {
		ST7735_WriteString(SIGN_X_POSS, SIGN_Y_POSS, EVENING, myFont_16x18, ST7735_ORANGE, ST7735_BLACK);
	}
	if (gTime.Hours >= 0 && gTime.Hours < 6) {
		ST7735_WriteString(SIGN_X_POSS, SIGN_Y_POSS, NIGHT, myFont_16x18, ST7735_WHITE, ST7735_BLACK);
	}
	//Display time
	sprintf(time, "%02d:%02d", gTime.Hours, gTime.Minutes);
	ST7735_WriteString(TIME_X_POSS, TIME_Y_POSS, time, Font_16x26, ST7735_WHITE, ST7735_BLACK);
	sprintf(time, "%02d/%02d/%02d", gDate.Month, gDate.Date, 2000 + gDate.Year);
	ST7735_WriteString(DATE_X_POSS, DATE_Y_POSS, time, Font_7x10, ST7735_WHITE, ST7735_BLACK);
}

bool configure_time (void) {
	bool RetVal = false;
	bool configuring = false;
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};
	RTC_TimeTypeDef gTime;
	RTC_DateTypeDef gDate;
	int press = 0;
		while (!configuring) {
			get_time();
			switch (press) {  // Delay in switch cases needed to ensure unexpexted switches
				case 0:
					ST7735_WriteString(CFG_X_POSS, CFG_Y_POSS, "cfgM", Font_7x10, ST7735_WHITE, ST7735_BLACK);
					if (BTN_A_Read == 1) {
						sTime.Minutes++;
						delay_ms(1000);
						if (sTime.Minutes == 0x60) {
							sTime.Minutes = 0x00;
							if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK) {
								RTC_error();
							}
						}
						if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK) {
							RTC_error();
						}
					}
					delay_ms(15000); //Delay needed to protect early switch
					if (BTN_B_Read == 1) {
						press++;
						delay_ms(15000);
					}
					break;
				case 1:
					ST7735_WriteString(CFG_X_POSS, CFG_Y_POSS, "cfgH", Font_7x10, ST7735_WHITE, ST7735_BLACK);
					if (BTN_A_Read == 1) {
						sTime.Hours++;
						delay_ms(1000);
						if (sTime.Hours == 0x24) {
							sTime.Hours = 0x00;
							if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK) {
								RTC_error();
							}
						}
						if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK) {
							RTC_error();
						}
					}
					delay_ms(15000); //Delay needed to protect early switch
					if (BTN_B_Read == 1) {
						press++;
						delay_ms(15000);
					}
					break;
				case 2:
					ST7735_WriteString(CFG_X_POSS, CFG_Y_POSS, "cfgY", Font_7x10, ST7735_WHITE, ST7735_BLACK);
					if (BTN_A_Read == 1) {
						sDate.Year++;
						delay_ms(1000);
						if (sDate.Year == 100) {
							sDate.Year = 0x00;
							if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK) {
								RTC_error();
							}
						}
						if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK) {
							RTC_error();
						}
					}
					delay_ms(15000); //Delay needed to protect early switch
					if (BTN_B_Read == 1) {
						press++;
						delay_ms(15000);
					}
					break;
				case 3:
					ST7735_WriteString(CFG_X_POSS, CFG_Y_POSS, "cfgD", Font_7x10, ST7735_WHITE, ST7735_BLACK);
					if (BTN_A_Read == 1) {
						sDate.Date++;
						delay_ms(1000);
						if (sDate.Date == 32) {
							sDate.Date = 0x00;
							if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK) {
								RTC_error();
							}
						}
						if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK) {
							RTC_error();
						}
					}
					delay_ms(15000); //Delay needed to protect early switch
					if (BTN_B_Read == 1) {
						press++;
						delay_ms(15000);
					}
					break;
				case 4:
					ST7735_WriteString(CFG_X_POSS, CFG_Y_POSS, "cfgD", Font_7x10, ST7735_WHITE, ST7735_BLACK);
					if (BTN_A_Read == 1) {
						sDate.Month++;
						delay_ms(1000);
						if (sDate.Month == 13) {
							sDate.Month = 0x01;
							if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK) {
								RTC_error();
							}
						}
						if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK) {
							RTC_error();
						}
					}
					delay_ms(15000); //Delay needed to protect early switch
					if (BTN_B_Read == 1) {
						press++;
						delay_ms(15000);
					}
					break;
//				case 5: //Use this case if weekday is needed
//					ST7735_WriteString(CFG_X_POSS, CFG_Y_POSS, "cfgW", Font_7x10, ST7735_WHITE, ST7735_BLACK);
//					if (BTN_A_Read == 1) {
//						sDate.WeekDay++;
//						delay_ms(1000);
//						if (sDate.WeekDay == 8) {
//							sDate.WeekDay = 0x01;
//							if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK) {
//								RTC_error();
//							}
//						}
//						if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK) {
//							RTC_error();
//						}
//					}
//					delay_ms(15000); //Delay needed to protect early switch
//					if (BTN_B_Read == 1) {
//						press++;
//						delay_ms(15000);
//					}
//					break;
					case 5:
						ST7735_WriteString(CFG_X_POSS, CFG_Y_POSS, "    ", Font_7x10, ST7735_WHITE, ST7735_BLACK);
						configuring = true;
					break;
			}
		}
		RetVal = true;
	return RetVal;
}
