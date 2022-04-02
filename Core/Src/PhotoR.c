//Includes
#include "RTC.h"

//Defines
#define too_low_light "$"
#define very_low_light "%"
#define low_light "&"
#define mid_light "'"
#define enough_light "\("
#define SIGN_X_POSS 111
#define SIGN_Y_POSS 0

//Typedefs
extern ADC_HandleTypeDef hadc1;

//Variables
extern char str1[100];
uint32_t photo_val = 0;

//Functions
void PhotoR_handle (void) {
	HAL_ADC_Start(&hadc1);
	photo_val = HAL_ADC_GetValue(&hadc1);
	if (photo_val >= 3900) {
		ST7735_WriteString(SIGN_X_POSS, SIGN_Y_POSS, too_low_light, myFont_16x18, ST7735_WHITE, ST7735_BLACK);
	}
	if (photo_val < 3900 && photo_val >= 3400) {
		ST7735_WriteString(SIGN_X_POSS, SIGN_Y_POSS, very_low_light, myFont_16x18, ST7735_WHITE, ST7735_BLACK);
	}
	if (photo_val < 3400 && photo_val >= 2900) {
		ST7735_WriteString(SIGN_X_POSS, SIGN_Y_POSS, low_light, myFont_16x18, ST7735_WHITE, ST7735_BLACK);
	}
	if (photo_val < 2900 && photo_val >= 2350) {
		ST7735_WriteString(SIGN_X_POSS, SIGN_Y_POSS, mid_light, myFont_16x18, ST7735_WHITE, ST7735_BLACK);
	}
	if (photo_val < 2350) {
		ST7735_WriteString(SIGN_X_POSS, SIGN_Y_POSS, enough_light, myFont_16x18, ST7735_WHITE, ST7735_BLACK);
	}
	//Debug print below
	//sprintf(str1, "%d", photo_val);
	//ST7735_WriteString(0, 70, str1, Font_7x10, ST7735_WHITE, ST7735_BLACK);
	photo_val = 0;
}
