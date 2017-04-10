#include "i2c.h"
#include "arm_math.h"
#include "math.h"
/*
	edit by link  2017-3-4
	
	mag3110 和 FXOS8700 的lsb都是 0.1ut 都一样
 */
 

uint8_t RxBuffer[15];
magMeterDataType magData;

uint32_t magSpeedCount = 0;

int16_t 		XMoni;
int16_t 		YMoni;
int16_t 		ZMoni; 
float guassxPre[2];

int rollMoni;
//int16_t 		XMoniG;
//int16_t 		YMoniG;
//int16_t 		ZMoniG;

typedef struct 
{
	float x;
	float y;
	float z;
}magDataType;
typedef struct
{
	float x;
	float y;
	float z;
}magNedDataType;


magDataType magGaussRaw;
magDataType magGauss;
magDataType magNed = {0.317112,-0.019685,0.416822};

float x_axis[3] = {1.0f,0.0f,0.0f};
float y_axis[3] = {0.0f,1.0f,0.0f};
float z_axis[3] = {0.0f,0.0f,1.0f};
// moni variable
int mag_angle_moni;
int mag_speed_moni;
int angle_diff_moni;

float pitch = PI/2; // 弹头朝上
float yaw = 0;
float roll;
int rollMoni;
float mag_angle_out;
float mag_angle_out_pre;
float mag_speed_out_w = 0;
float mag_speed_out_r;
float mag_angle_out_180;
float mag_angle_out_180_filter;
float angle_diff;
float mag_speed_out_Hz;
int angleEncoderMoni;
extern uint16_t encoderCout;
//矫正的参数-不加隔离
float magOffet[3] = {0.847542354616265,0.357417131466831,0.640276215182707};
float B[6] = {0.994810461526874,-0.0135080951916163,-0.0124830589881432,1.00610989378119,0.00997875294844941,0.999546613353286};
//void mag3110_init()
//{
//	uint8_t ctrl2Config = CTRL_REG2_AUTO_MRST_EN_MARK;
//	uint8_t ctrl1Config = 0x01; //0x09 
//	uint8_t temp;
//	// 读设备地址
//	HAL_I2C_Mem_Read(&hi2c1, MAG3110_DEV_READ,MAG3110_WHO_AM_I,I2C_MEMADD_SIZE_8BIT, &magData.DeviceId, 1, 1000);
//	// 配置Ctrl2
//	if(HAL_I2C_Mem_Write(&hi2c1, MAG3110_DEV_WRITE, MAG3110_CTRL_REG2, I2C_MEMADD_SIZE_8BIT,&ctrl2Config,1, 1000) == HAL_OK)
//	{
//		HAL_I2C_Mem_Read(&hi2c1, MAG3110_DEV_READ,MAG3110_CTRL_REG2,I2C_MEMADD_SIZE_8BIT, &temp, 1, 1000);
//		if(temp == CTRL_REG2_AUTO_MRST_EN_MARK)
//    printf("\r\n Config2 OK \r\n");
//	}
//	// 配置Ctrl1
//	if(HAL_I2C_Mem_Write(&hi2c1, MAG3110_DEV_WRITE, MAG3110_CTRL_REG1, I2C_MEMADD_SIZE_8BIT,&ctrl1Config,1, 1000) == HAL_OK)
//	{
//		HAL_I2C_Mem_Read(&hi2c1, MAG3110_DEV_READ,MAG3110_CTRL_REG1,I2C_MEMADD_SIZE_8BIT, &temp, 1, 1000);
//		if(temp == 0x09)
//    printf("\r\n Config1 OK \r\n");
//	}
//	//读取当前的工作模式
//	HAL_I2C_Mem_Read(&hi2c1, MAG3110_DEV_READ,MAG3110_SYSMOD,I2C_MEMADD_SIZE_8BIT, &magData.Current_Mode,1, 1000);
//}

void FXO8700_init()
{
	uint8_t crtl1 = 0x00;
	uint8_t magctrl1 = 0x1D;
	uint8_t magctrl2 = 0x00;
	uint8_t temp;
		// 读设备地址
	HAL_I2C_Mem_Read(&hi2c1, FXOS8700CQ_READ,FXOS8700CQ_WHOAMI,I2C_MEMADD_SIZE_8BIT, &magData.DeviceId, 1, 1000);
//	配置Ctrl1
	if(HAL_I2C_Mem_Write(&hi2c1, FXOS8700CQ_WRITE, FXOS8700CQ_CTRL_REG1, I2C_MEMADD_SIZE_8BIT,&crtl1,1, 1000) == HAL_OK)
	{
		HAL_I2C_Mem_Read(&hi2c1, FXOS8700CQ_READ,FXOS8700CQ_CTRL_REG1,I2C_MEMADD_SIZE_8BIT, &temp, 1, 1000);
		if(temp == 0x00)
    printf("\r\n Config1 OK \r\n");
	}
	
		//	配置m_Ctrl1  0x11  禁用加速度计  0x1D 4倍采样率
	if(HAL_I2C_Mem_Write(&hi2c1, FXOS8700CQ_WRITE, FXOS8700CQ_M_CTRL_REG1, I2C_MEMADD_SIZE_8BIT,&magctrl1,1, 1000) == HAL_OK)
	{
		HAL_I2C_Mem_Read(&hi2c1, FXOS8700CQ_READ,FXOS8700CQ_M_CTRL_REG1,I2C_MEMADD_SIZE_8BIT, &temp, 1, 1000);
		if(temp == 0x1D)
    printf("\r\n Config1 OK \r\n");
	}
	
			//	配置m_Ctrl2 0x00 
 	if(HAL_I2C_Mem_Write(&hi2c1, FXOS8700CQ_WRITE, FXOS8700CQ_M_CTRL_REG2, I2C_MEMADD_SIZE_8BIT,&magctrl2,1, 1000) == HAL_OK)
	{
		HAL_I2C_Mem_Read(&hi2c1, FXOS8700CQ_READ,FXOS8700CQ_M_CTRL_REG2,I2C_MEMADD_SIZE_8BIT, &temp, 1, 1000);
		if(temp == 0x00)
    printf("\r\n Config1 OK \r\n");
	}
	crtl1 = 0x15; //  active 开始采集 0x01 800 0x05 400    0x15
	// 重新配置 ctr1 开始采集数据
		if(HAL_I2C_Mem_Write(&hi2c1, FXOS8700CQ_WRITE, FXOS8700CQ_CTRL_REG1, I2C_MEMADD_SIZE_8BIT,&crtl1,1, 1000) == HAL_OK)
	{
		HAL_I2C_Mem_Read(&hi2c1, FXOS8700CQ_READ,FXOS8700CQ_CTRL_REG1,I2C_MEMADD_SIZE_8BIT, &temp, 1, 1000);
		if(temp == 0x01)
    printf("\r\n Config1 OK \r\n");
	}
	
}



void ReadI2C(uint8_t REG_Address )
{
  uint8_t M_DR_status;
	float dotResult_x;
	float absResult_x;
	float temp1;
	float temp2;
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MAG3110///////////////////////////////////////////////////////////////////////////////////////////////
//  这里 MAG3110  的xy是反的
//////////////////////////////////////////////////////////////////////////////////////////////////////////	
//	HAL_I2C_Mem_Read(&hi2c1, MAG3110_DEV_READ,MAG3110_DR_STATUS,I2C_MEMADD_SIZE_8BIT, &DR_status, 1, 1000);
//	if(DR_status && DR_STATUS_ALL_READY) 
//	{
//		//HAL_I2C_Mem_Read(&hi2c1, MAG3110_DEV_READ,MAG3110_OUT_X_MSB,I2C_MEMADD_SIZE_8BIT, RxBuffer, sizeof(RxBuffer), 1000);//不清楚支不支持自增模式
//		HAL_I2C_Mem_Read(&hi2c1, MAG3110_DEV_READ,MAG3110_OUT_X_MSB,I2C_MEMADD_SIZE_8BIT, RxBuffer, 6, 1000);
	
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// FXOS8700CQ/////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////	
	HAL_I2C_Mem_Read(&hi2c1, FXOS8700CQ_READ,M_DR_STATUS,I2C_MEMADD_SIZE_8BIT, &M_DR_status, 1, 1000);	
	if(M_DR_status && M_DATA_ALL_READY)
	{
		HAL_I2C_Mem_Read(&hi2c1, FXOS8700CQ_READ,M_OUT_X_MSB,I2C_MEMADD_SIZE_8BIT, RxBuffer, 6, 1000);
		magData.X= (RxBuffer[0]<<8) | RxBuffer[1];
		magData.Y= (RxBuffer[2]<<8) | RxBuffer[3];
		magData.Z= (RxBuffer[4]<<8) | RxBuffer[5];
		// 此处进行坐标转换 北东地 ,x y互换就成标准的右手系了
		XMoni = magData.X;
		YMoni = magData.Y;
		ZMoni = magData.Z;
	//  0.1uT/bits ~~~ GAUSS /10000 *10
		magGaussRaw.x = (float)magData.X * 0.001f;
		magGaussRaw.y = (float)magData.Y * 0.001f;
		magGaussRaw.z = (float)magData.Z * 0.001f;
	
		magDataType magTemp;
	
	  magTemp.x = magGaussRaw.x - magOffet[0];
    magTemp.y = magGaussRaw.y - magOffet[1];
    magTemp.z = magGaussRaw.z - magOffet[2];
	
    magGauss.x = B [0]*magTemp.x + B [1]*magTemp.y + B [2]*magTemp.z;
    magGauss.y = B [1]*magTemp.x + B [3]*magTemp.y + B [4]*magTemp.z;
    magGauss.z = B [2]*magTemp.x + B [4]*magTemp.y + B [5]*magTemp.z;
			
		dotResult_x = magGauss.x*x_axis[0] +magGauss.z*x_axis[2];
		arm_sqrt_f32(magGauss.x*magGauss.x + magGauss.z*magGauss.z,&absResult_x);
		mag_angle_out = acos(dotResult_x/absResult_x);
			// naive 过零点检测 正弦波频率
		if(magGauss.x > 0 && guassxPre < 0)
		{
			// 直接输出频率
			mag_speed_out_Hz = 1/(magSpeedCount * 0.001);
		}			
			
		if(magGauss.z >= 0 ) //第一第二象限
		{
			}
		else if(magGauss.z < 0) // 三四象限。
			mag_angle_out = 2*PI - mag_angle_out;		
		
		mag_angle_out_180 = mag_angle_out * (180/PI);
		
		if(mag_angle_out_180 > 360)
			mag_angle_out_180 -= 360;
		if(mag_angle_out_180 < 0)
			mag_angle_out_180 += 360;
		mag_angle_moni = 1000*mag_angle_out_180;
		
		if((mag_angle_out_180 <320 && mag_angle_out_180>40) && (mag_angle_out_pre <320 && mag_angle_out_pre>40))
		{
//			  xt(i) = a*xt(i-1) + b*ut(i-1);
//				yt(i-1) = c*xt(i-1) + d*ut(i-1);
//			mag_speed_out_r = -2.56 * mag_speed_out_w + 1.6 * mag_angle_out_180;
//			mag_speed_out_w = 0.98019867f * mag_speed_out_w + 0.01237582918 * mag_angle_out_180;
				mag_speed_out_r = mag_speed_out_r * ((0.0625-0.0125)/0.0625) + 1/0.0625 * (mag_angle_out_180 - mag_angle_out_pre);
				
		}
		//mag_speed_out_r = mag_speed_out_r * ((0.0625-0.0125)/0.0625) + 1/0.0625 * (mag_angle_out_180 - mag_angle_out_pre);
		//mag_speed_out_r = mag_speed_out_r * exp(-0.0125/0.125) + 0.125 * (1 - exp(-0.0125/0.125)) * mag_angle_out_180;
	
		
//		进行俯仰角计算 
		temp1 = - arm_sin_f32(yaw)* magNed.x + arm_cos_f32(yaw) * magNed.y; 
		temp2 = arm_cos_f32(yaw)*arm_sin_f32(pitch)*magNed.x +
													arm_sin_f32(yaw)*arm_sin_f32(pitch)*magNed.y + arm_cos_f32(pitch) * magNed.z;
		
		
		roll = atan2((temp2 * magGauss.y-temp1 * magGauss.z),(temp1 * magGauss.y + temp2 * magGauss.z));
		rollMoni = (roll * (180.0f/PI)) + 180;
		mag_angle_out_pre = mag_angle_out_180;
		
		//angle_diff = angle_diff * (180/PI);
//		if(angle_diff > 120.0f)
//		{
//			angle_diff = 360 - angle_diff;
//		}
//		else if(angle_diff <	-120.0f)
//		{
//			angle_diff = -angle_diff - 360;
//		}	
//		if(magSpeedCount < 100) magSpeedCount = 100;
//		mag_speed_out_w = angle_diff/(magSpeedCount * 0.0001);		// 角速度输出。
//		magSpeedCount = 0;
//		mag_speed_out_r = 2*PI/mag_speed_out_w;
//		
		mag_speed_moni = mag_speed_out_r * 1000;
		angleEncoderMoni = (float)encoderCout/512 * 360; 	
//		angle_diff_moni = angle_diff * 1000;
	}		
}
