#include "i2c.h"

/*
	edit by link  2017-3-4
 */
uint8_t RxBuffer[15];
typedef struct
{
	uint8_t DeviceId;
	uint8_t Current_Mode;
	char Temperature;
	int MAG3110_XOut; 
	int MAG3110_YOut; 
	int MAG3110_ZOut; 	
}mag3110DataType;
mag3110DataType magData;

void mag3110_init()
{
	uint8_t ctrl2Config = CTRL_REG2_AUTO_MRST_EN_MARK;
	uint8_t ctrl1Config = 0xC9;
	uint8_t temp;
	// 读设备地址
	HAL_I2C_Mem_Read(&hi2c1, MAG3110_DEV_ADDR,MAG3110_WHO_AM_I,I2C_MEMADD_SIZE_8BIT, &magData.DeviceId, 1, 1000);
	// 配置Ctrl2
	if(HAL_I2C_Mem_Write(&hi2c1, MAG3110_DEV_ADDR, MAG3110_CTRL_REG2, I2C_MEMADD_SIZE_8BIT,&ctrl2Config,1, 1000) == HAL_OK)
	{
		HAL_I2C_Mem_Read(&hi2c1, MAG3110_DEV_ADDR,MAG3110_WHO_AM_I,I2C_MEMADD_SIZE_8BIT, &temp, 1, 1000);
		if(temp == CTRL_REG2_AUTO_MRST_EN_MARK)
    printf("\r\n Config2 OK \r\n");
	}
	// 配置Ctrl1
	if(HAL_I2C_Mem_Write(&hi2c1, MAG3110_DEV_ADDR, MAG3110_CTRL_REG1, I2C_MEMADD_SIZE_8BIT,&ctrl1Config,1, 1000) == HAL_OK)
	{
		HAL_I2C_Mem_Read(&hi2c1, MAG3110_DEV_ADDR,MAG3110_WHO_AM_I,I2C_MEMADD_SIZE_8BIT, &temp, 1, 1000);
		if(temp == 0xC9)
    printf("\r\n Config1 OK \r\n");
	}
	//读取当前的工作模式
	HAL_I2C_Mem_Read(&hi2c1, MAG3110_DEV_ADDR,MAG3110_WHO_AM_I,I2C_MEMADD_SIZE_8BIT, &magData.Current_Mode,1, 1000);
}



void ReadI2C(uint8_t REG_Address )
{
  uint8_t DR_status;
	HAL_I2C_Mem_Read(&hi2c1, MAG3110_DEV_ADDR,MAG3110_OUT_X_MSB,I2C_MEMADD_SIZE_8BIT, &DR_status, 1, 1000);
	if(DR_status == DR_STATUS_ALL_READY) // 所有数据都已经可以获取。
  HAL_I2C_Mem_Read(&hi2c1, MAG3110_DEV_ADDR,MAG3110_OUT_X_MSB,I2C_MEMADD_SIZE_8BIT, RxBuffer, sizeof(RxBuffer), 1000);//不清楚支不支持自增模式
	
	 magData.MAG3110_XOut= (RxBuffer[0]<<8) | RxBuffer[1];
   magData.MAG3110_YOut= (RxBuffer[2]<<8) | RxBuffer[3];
   magData.MAG3110_ZOut= (RxBuffer[4]<<8) | RxBuffer[5];
	
}