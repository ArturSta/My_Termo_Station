//Includes
#include "BME280.h"
#include "st7735.h"
#include "fonts.h"

//Typedefs
extern I2C_HandleTypeDef hi2c1;
//extern UART_HandleTypeDef huart2;

//Defines


//Variables
extern char str1[100];
BME280_CalibData_t CalibData;
int32_t temper_int;
float tf = 0.0f, pf = 0.0f, hf = 0.0f;// af = 0.0f;
uint32_t af = 0;

//Functions
void Error (void) {
  ST7735_FillScreen(ST7735_RED);
  ST7735_WriteString(TEMP_X_POSS, TEMP_Y_POSS, "BAROMETER", Font_16x26, ST7735_BLACK, ST7735_RED);
}

static void I2Cx_WriteData (uint16_t Addr, uint8_t Reg, uint8_t Value) {
  HAL_StatusTypeDef status = HAL_OK;
  status = HAL_I2C_Mem_Write(&hi2c1, Addr, (uint16_t)Reg, I2C_MEMADD_SIZE_8BIT, &Value, 1, 0x10000);
  if(status != HAL_OK) Error();
}

static uint8_t I2Cx_ReadData (uint16_t Addr, uint8_t Reg) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t value = 0;
  status = HAL_I2C_Mem_Read(&hi2c1, Addr, Reg, I2C_MEMADD_SIZE_8BIT, &value, 1, 0x10000);
  if(status != HAL_OK) Error();
  return value;
}

static void I2Cx_ReadData16 (uint16_t Addr, uint8_t Reg, uint16_t *Value) {
  HAL_StatusTypeDef status = HAL_OK;
  status = HAL_I2C_Mem_Read(&hi2c1, Addr, Reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*)Value, 2, 0x10000);
  if(status != HAL_OK) Error();
}

static void I2Cx_ReadData24 (uint16_t Addr, uint8_t Reg, uint32_t *Value) {
  HAL_StatusTypeDef status = HAL_OK;
  status = HAL_I2C_Mem_Read(&hi2c1, Addr, Reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*)Value, 3, 0x10000);
  if(status != HAL_OK) Error();
}

void BME280_WriteReg (uint8_t Reg, uint8_t Value) {
  I2Cx_WriteData(BME280_ADDRESS, Reg, Value);
}

uint8_t BME280_ReadReg(uint8_t Reg) {
  uint8_t res = I2Cx_ReadData(BME280_ADDRESS,Reg);
  return res;
}

void BME280_ReadReg_U16 (uint8_t Reg, uint16_t *Value) {
  I2Cx_ReadData16(BME280_ADDRESS,Reg,Value);
}

void BME280_ReadReg_S16 (uint8_t Reg, int16_t *Value) {
  I2Cx_ReadData16(BME280_ADDRESS,Reg, (uint16_t*) Value);
}

void BME280_ReadReg_BE_S16 (uint8_t Reg, int16_t *Value) {
  I2Cx_ReadData16(BME280_ADDRESS,Reg,(uint16_t*)Value);
  *(uint16_t *) Value = be16toword(*(uint16_t *) Value);
}

void BME280_ReadReg_U24 (uint8_t Reg, uint32_t *Value) {
  I2Cx_ReadData24(BME280_ADDRESS,Reg,Value);
  *(uint32_t *) Value &= 0x00FFFFFF;
}

void BME280_ReadReg_BE_U24 (uint8_t Reg, uint32_t *Value) {
  I2Cx_ReadData24(BME280_ADDRESS,Reg,Value);
  *(uint32_t *) Value = be24toword(*(uint32_t *) Value) & 0x00FFFFFF;
}

void BME280_ReadCoefficients (void) {
  BME280_ReadReg_U16(BME280_REGISTER_DIG_T1,&CalibData.dig_T1);
	//Prints for debug
//  sprintf(str1, "DIG_T1: %urn", CalibData.dig_T1);
//  ST7735_WriteString(0, 40, str1, Font_7x10, ST7735_WHITE, ST7735_BLACK);
  BME280_ReadReg_S16(BME280_REGISTER_DIG_T2,&CalibData.dig_T2);
//  sprintf(str1, "DIG_T2: %drn", CalibData.dig_T2);
//  ST7735_WriteString(0, 50, str1, Font_7x10, ST7735_WHITE, ST7735_BLACK);
  BME280_ReadReg_S16(BME280_REGISTER_DIG_T3,&CalibData.dig_T3);
//  sprintf(str1, "DIG_T3: %drn", CalibData.dig_T3);
//  ST7735_WriteString(0, 60, str1, Font_7x10, ST7735_WHITE, ST7735_BLACK);
  BME280_ReadReg_U16(BME280_REGISTER_DIG_P1,&CalibData.dig_P1);
//  sprintf(str1, "DIG_P1: %urn", CalibData.dig_P1);
//  ST7735_WriteString(0, 70, str1, Font_7x10, ST7735_WHITE, ST7735_BLACK);
  BME280_ReadReg_S16(BME280_REGISTER_DIG_P2,&CalibData.dig_P2);
//  sprintf(str1, "DIG_P2: %drn", CalibData.dig_P2);
//  ST7735_WriteString(0, 80, str1, Font_7x10, ST7735_WHITE, ST7735_BLACK);
  BME280_ReadReg_S16(BME280_REGISTER_DIG_P3,&CalibData.dig_P3);
//  sprintf(str1, "DIG_P3: %drn", CalibData.dig_P3);
//  ST7735_WriteString(0, 90, str1, Font_7x10, ST7735_WHITE, ST7735_BLACK);
  BME280_ReadReg_S16(BME280_REGISTER_DIG_P4,&CalibData.dig_P4);
//  sprintf(str1, "DIG_P4: %drn", CalibData.dig_P4);
//	ST7735_WriteString(0, 100, str1, Font_7x10, ST7735_WHITE, ST7735_BLACK);
  BME280_ReadReg_S16(BME280_REGISTER_DIG_P5,&CalibData.dig_P5);
//  sprintf(str1, "DIG_P5: %drn", CalibData.dig_P5);
//	ST7735_WriteString(0, 110, str1, Font_7x10, ST7735_WHITE, ST7735_BLACK);
  BME280_ReadReg_S16(BME280_REGISTER_DIG_P6,&CalibData.dig_P6);
//  sprintf(str1, "DIG_P6: %drn", CalibData.dig_P6);
//	ST7735_WriteString(0, 120, str1, Font_7x10, ST7735_WHITE, ST7735_BLACK);
  BME280_ReadReg_S16(BME280_REGISTER_DIG_P7,&CalibData.dig_P7);
//  sprintf(str1, "DIG_P7: %drn", CalibData.dig_P7);
//	ST7735_WriteString(0, 130, str1, Font_7x10, ST7735_WHITE, ST7735_BLACK);
  BME280_ReadReg_S16(BME280_REGISTER_DIG_P8,&CalibData.dig_P8);
//  sprintf(str1, "DIG_P8: %drn", CalibData.dig_P8);
//	ST7735_WriteString(0, 140, str1, Font_7x10, ST7735_WHITE, ST7735_BLACK);
  BME280_ReadReg_S16(BME280_REGISTER_DIG_P9,&CalibData.dig_P9);
//  sprintf(str1, "DIG_P9: %drn", CalibData.dig_P9);
//	ST7735_WriteString(0, 150, str1, Font_7x10, ST7735_WHITE, ST7735_BLACK);
  CalibData.dig_H1 = BME280_ReadReg(BME280_REGISTER_DIG_H1);
//  sprintf(str1, "DIG_H1: %drn", CalibData.dig_H1);
  BME280_ReadReg_S16(BME280_REGISTER_DIG_H2,&CalibData.dig_H2);
//  sprintf(str1, "DIG_H2: %drn", CalibData.dig_H2);
  CalibData.dig_H3 = BME280_ReadReg(BME280_REGISTER_DIG_H3);
//  sprintf(str1, "DIG_H3: %drn", CalibData.dig_H3);
  CalibData.dig_H4 = (BME280_ReadReg(BME280_REGISTER_DIG_H4) << 4) | (BME280_ReadReg(BME280_REGISTER_DIG_H4+1) & 0xF);
//  sprintf(str1, "DIG_H4: %drn", CalibData.dig_H4);;
  CalibData.dig_H5 = (BME280_ReadReg(BME280_REGISTER_DIG_H5+1) << 4) | (BME280_ReadReg(BME280_REGISTER_DIG_H5) >> 4);
//  sprintf(str1, "DIG_H5: %drn", CalibData.dig_H5);
  CalibData.dig_H6 = (int8_t)BME280_ReadReg(BME280_REGISTER_DIG_H6);
//  sprintf(str1, "DIG_H6: %drn", CalibData.dig_H3);
}

void BME280_SetStandby (uint8_t tsb) {
  uint8_t reg;
  reg = BME280_ReadReg(BME280_REG_CONFIG) & ~BME280_STBY_MSK;
  reg |= tsb & BME280_STBY_MSK;
  BME280_WriteReg(BME280_REG_CONFIG,reg);
}

void BME280_SetFilter (uint8_t filter) {
  uint8_t reg;
  reg = BME280_ReadReg(BME280_REG_CONFIG) & ~BME280_FILTER_MSK;
  reg |= filter & BME280_FILTER_MSK;
  BME280_WriteReg(BME280_REG_CONFIG,reg);
}

void BME280_SetOversamplingTemper (uint8_t osrs) {
  uint8_t reg;
  reg = BME280_ReadReg(BME280_REG_CTRL_MEAS) & ~BME280_OSRS_T_MSK;
  reg |= osrs & BME280_OSRS_T_MSK;
  BME280_WriteReg(BME280_REG_CTRL_MEAS,reg);
}

void BME280_SetOversamplingPressure (uint8_t osrs) {
  uint8_t reg;
  reg = BME280_ReadReg(BME280_REG_CTRL_MEAS) & ~BME280_OSRS_P_MSK;
  reg |= osrs & BME280_OSRS_P_MSK;
  BME280_WriteReg(BME280_REG_CTRL_MEAS,reg);
}

void BME280_SetOversamplingHum (uint8_t osrs) {
  uint8_t reg;
  reg = BME280_ReadReg(BME280_REG_CTRL_HUM) & ~BME280_OSRS_H_MSK;
  reg |= osrs & BME280_OSRS_H_MSK;
  BME280_WriteReg(BME280_REG_CTRL_HUM,reg);
  //The 'ctrl_hum' register needs to be written
  //after changing 'ctrl_hum' for the changes to become effwctive.
  reg = BME280_ReadReg(BME280_REG_CTRL_MEAS);
  BME280_WriteReg(BME280_REG_CTRL_MEAS,reg);
}

void BME280_SetMode (uint8_t mode) {
  uint8_t reg;
  reg = BME280_ReadReg(BME280_REG_CTRL_MEAS) & ~BME280_MODE_MSK;
  reg |= mode & BME280_MODE_MSK;
  BME280_WriteReg(BME280_REG_CTRL_MEAS,reg);
}

uint8_t BME280_ReadStatus (void) {
  //clear unuset bits
  uint8_t res = BME280_ReadReg(BME280_REGISTER_STATUS)&0x09;
  return res;
}

float BME280_ReadTemperature (void) {
  float temper_float = 0.0f;
	uint32_t temper_raw;
	int32_t val1, val2;
	BME280_ReadReg_BE_U24(BME280_REGISTER_TEMPDATA,&temper_raw);
	temper_raw >>= 4;
	//Debug prints
//	sprintf(str1, "Temperature RAW:  0x%08X", temper_raw);
//	ST7735_WriteString(0, 60, str1, Font_7x10, ST7735_WHITE, ST7735_BLACK);
	val1 = ((((temper_raw>>3) - ((int32_t)CalibData.dig_T1 <<1))) * ((int32_t)CalibData.dig_T2)) >> 11;
	val2 = (((((temper_raw>>4) - ((int32_t)CalibData.dig_T1)) * ((temper_raw>>4) - ((int32_t)CalibData.dig_T1))) >> 12)
				 * ((int32_t)CalibData.dig_T3)) >> 14;
	temper_int = val1 + val2;
	temper_float = ((temper_int * 5 + 128) >> 8);
	temper_float /= 100.0f;
	//temper_float = (temper_float - 32) * (5/9);
  return temper_float;
}

float BME280_ReadPressure (void) {
  float press_float = 0.0f;
	uint32_t press_raw;
	uint32_t pres_int;
	int64_t val1, val2, p;
	BME280_ReadTemperature(); // must be done first to get t_fine
	BME280_ReadReg_BE_U24(BME280_REGISTER_PRESSUREDATA,&press_raw);
	press_raw >>= 4;
	val1 = ((int64_t) temper_int) - 128000;
	val2 = val1 * val1 * (int64_t)CalibData.dig_P6;
	val2 = val2 + ((val1 * (int64_t)CalibData.dig_P5) << 17);
	val2 = val2 + ((int64_t)CalibData.dig_P4 << 35);
	val1 = ((val1 * val1 * (int64_t)CalibData.dig_P3) >> 8) + ((val1 * (int64_t)CalibData.dig_P2) << 12);
	val1 = (((((int64_t)1) << 47) + val1)) * ((int64_t)CalibData.dig_P1) >> 33;
	if (val1 == 0) {
		return 0; // avoid exception caused by division by zero
	}
	p = 1048576 - press_raw;
	p = (((p << 31) - val2) * 3125) / val1;
	val1 = (((int64_t)CalibData.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
	val2 = (((int64_t)CalibData.dig_P8) * p) >> 19;
	p = ((p + val1 + val2) >> 8) + ((int64_t)CalibData.dig_P7 << 4);
	pres_int = ((p >> 8) * 1000) + (((p & 0xff) * 390625) / 100000);
	press_float = pres_int / 100.0f;
  return press_float;
}

float BME280_ReadHumidity (void) {
  float hum_float = 0.0f;
	int16_t hum_raw;
	int32_t hum_raw_sign, v_x1_u32r;
	BME280_ReadTemperature(); // must be done first to get t_fine
	BME280_ReadReg_BE_S16(BME280_REGISTER_HUMIDDATA,&hum_raw);
	hum_raw_sign = ((int32_t)hum_raw)&0x0000FFFF;
	//sprintf(str1, "Humidity RAW: 0x%08X\r\n",hum_raw_sign);
	//Debug print below
//	sprintf(str1, "Humidity RAW: 0x%08X\r\n", hum_raw);
//	ST7735_WriteString(0, 60, str1, Font_7x10, ST7735_WHITE, ST7735_BLACK);
	v_x1_u32r = (temper_int - ((int32_t)76800));
	v_x1_u32r = (((((hum_raw_sign << 14) - (((int32_t)CalibData.dig_H4) << 20) -
								(((int32_t)CalibData.dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) *
								(((((((v_x1_u32r * ((int32_t)CalibData.dig_H6)) >> 10) *
								(((v_x1_u32r * ((int32_t)CalibData.dig_H3)) >> 11) + ((int32_t)32768))) >> 10) +
								((int32_t)2097152)) * ((int32_t)CalibData.dig_H2) + 8192) >> 14));
	v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
								((int32_t)CalibData.dig_H1)) >> 4));
	v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
	v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;
	hum_float = (v_x1_u32r>>12);
	hum_float /= 1024.0f;
  return hum_float;
}

float BME280_ReadAltitude (float seaLevel) {
  float att = 0.0f;
	float atm = BME280_ReadPressure();
	att = 44330.0 * (1.0 - pow(atm / seaLevel, 0.1903));
  return att;
}

void BME280_Init (void) {
  uint8_t    value = 0;
	HAL_Delay(25);
  //uint32_t   value32 = 0; Debug value
	value = BME280_ReadReg(BME280_REG_ID);
	//ST7735_WriteString(0, 50, "Debug val:", Font_7x10, ST7735_WHITE, ST7735_BLACK);
	//sprintf(str1, "%02X", value);
	//ST7735_WriteString(0, 65, str1, Font_7x10, ST7735_WHITE, ST7735_BLACK);
	if (value != BME280_ID) {
		Error();
		return;
	}
	BME280_WriteReg(BME280_REG_SOFTRESET,BME280_SOFTRESET_VALUE);
	while (BME280_ReadStatus() & BME280_STATUS_IM_UPDATE);  // Waiting for bit set
	BME280_ReadCoefficients();
	BME280_SetStandby(BME280_STBY_500); //Change stand by timeout here if needed
	BME280_SetFilter(BME280_FILTER_4);  //Change filter coefficient here if needed
	BME280_SetOversamplingTemper(BME280_OSRS_T_x4);
	BME280_SetOversamplingPressure(BME280_OSRS_P_x2);
	BME280_SetOversamplingHum(BME280_OSRS_H_x1);
	//Debug prints
//	value32 = BME280_ReadReg(BME280_REG_CTRL_MEAS);
//	sprintf(str1, "Measurements status: %04X", value32);
//	ST7735_FillScreen(ST7735_BLACK);
//	ST7735_WriteString(0, 40, str1, Font_7x10, ST7735_WHITE, ST7735_BLACK);
//	sprintf(str1, "Temperature: %s   Pressure: %s",
//  (value32 & BME280_OSRS_T_MSK) ? "ON" : "OFF",
//  (value32 & BME280_OSRS_P_MSK) ? "ON" : "OFF");
//	ST7735_WriteString(0, 40, str1, Font_7x10, ST7735_WHITE, ST7735_BLACK);
	BME280_SetMode(BME280_MODE_NORMAL);
}

void Show_BME_Values (void) {
	tf = BME280_ReadTemperature();
	sprintf(str1, "%.1f*C", tf);
	ST7735_WriteString(TEMP_VAL_X_POSS, TEMP_VAL_Y_POSS, str1, Font_11x18, ST7735_WHITE, ST7735_BLACK);
	pf = BME280_ReadPressure();
	sprintf(str1, "%.2f hPa", pf/1000.0f);
	ST7735_WriteString(PRES1_VAL_X_POSS, PRES1_VAL_Y_POSS, str1, Font_11x18, ST7735_WHITE, ST7735_BLACK);
	sprintf(str1, "%.2f mmHg", pf * 0.000750061683f);
	ST7735_WriteString(PRES2_VAL_X_POSS, PRES2_VAL_Y_POSS, str1, Font_11x18, ST7735_WHITE, ST7735_BLACK);
	af = BME280_ReadAltitude(SEALEVELPRESSURE_PA)/100;
	sprintf(str1, "%d", af);
	if (af < 1000) {
		ST7735_WriteString(ALT_VAL_X_POSS, ALT_VAL_Y_POSS, str1, Font_11x18, ST7735_WHITE, ST7735_BLACK);
	} else {
		ST7735_WriteString(ALT_VAL_X_POSS, ALT_VAL_Y_POSS, str1, Font_7x10, ST7735_WHITE, ST7735_BLACK);
	}
	hf = BME280_ReadHumidity();
	sprintf(str1, "%.1f%%", hf);
	ST7735_WriteString(HUM_VAL_X_POSS, HUM_VAL_Y_POSS, str1, Font_11x18, ST7735_WHITE, ST7735_BLACK);
}
