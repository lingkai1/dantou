#ifndef __8700CQ_H
#define __8700CQ_H

// FXOS8700CQ I2C address
#define FXOS8700CQ_SLAVE_ADDR 0x1E // with pins SA0=0, SA1=0
#define FXOS8700CQ_READ FXOS8700CQ_SLAVE_ADDR<<1|0x01
#define FXOS8700CQ_WRITE FXOS8700CQ_SLAVE_ADDR<<1|0x00
// FXOS8700CQ internal register addresses
#define FXOS8700CQ_STATUS 0x00
#define FXOS8700CQ_WHOAMI 0x0D
#define FXOS8700CQ_ACCELEROMETOR_XYZ_DATA_CFG 0x0E			///useless
#define FXOS8700CQ_CTRL_REG1 0x2A            //
#define FXOS8700CQ_CTRL_REG4 0x2D  						// interrupt enable
#define FXOS8700CQ_CTRL_REG5 0x2E 						// interrupt map
#define FXOS8700CQ_M_CTRL_REG1 0x5B  // output data selection  0x00 ODR:800 standy     0x01,activeMode 
#define FXOS8700CQ_M_CTRL_REG2 0x5C

#define M_DR_STATUS 0x32 

#define M_DATA_ALL_READY 0x08 //0b00001000

#define FXOS8700CQ_WHOAMI_VAL 0xC7
#define SYSMOD 0x0B										// read only
#define M_OUT_X_MSB 0x33//MSB of 16-bit magnetic data for X-axis
#define M_OUT_X_LSB 0x34//LSB of 16-bit magnetic data for X-axis
#define M_OUT_Y_MSB 0x35//MSB of 16-bit magnetic data for Y-axis
#define M_OUT_Y_LSB 0x36//LSB of 16-bit magnetic data for Y-axis
#define M_OUT_Z_MSB 0x37//MSB of 16-bit magnetic data for Z-axis
#define M_OUT_Z_LSB 0x38//LSB of 16-bit magnetic data for Z-axis
#define CMP_X_MSB 0x39// [13:8] of integrated X-axis accerleration data
#define CMP_X_LSB 0x3A// [7:0] of integrated X-axis accerleration data
#define CMP_Y_MSB 0x3B// [13:8] of integrated Y-axis accerleration data
#define CMP_Y_LSB 0x3C// [7:0] of integrated Y-axis accerleration data
#define CMP_Z_MSB 0x3D// [13:8] of integrated Z-axis accerleration data
#define CMP_Z_LSB 0x3E// [7:0] of integrated Z-axis accerleration data
#define M_OFF_X_MSB 0x3F// MSB of magnetometer of X-axis offset
#define M_OFF_X_LSB 0x40// LSB of magnetometer of X-axis offset
#define M_OFF_Y_MSB 0x41// MSB of magnetometer of Y-axis offset
#define M_OFF_Y_LSB 0x42// LSB of magnetometer of Y-axis offset
#define M_OFF_Z_MSB 0x43// MSB of magnetometer of Z-axis offset
#define M_OFF_Z_LSB 0x44// LSB of magnetometer of Z-axis offset
#define MAX_X_MSB 0x45// Magnetometer X-axis maximum value MSB
#define MAX_X_LSB 0x46// Magnetometer X-axis maximum value LSB
#define MAX_Y_MSB 0x47// Magnetometer Y-axis maximum value MSB
#define MAX_Y_LSB 0x48// Magnetometer Y-axis maximum value LSB
#define MAX_Z_MSB 0x49// Magnetometer Z-axis maximum value MSB
#define MAX_Z_LSB 0x4A// Magnetometer Z-axis maximum value LSB
#define MIN_X_MSB 0x4B// Magnetometer X-axis minimum value MSB
#define M_CTRL_REG1 0x5B // 0x11
												 // 0 
												 // ODR = 800 OSR ÎÞÐ§
typedef struct
{
	uint8_t DeviceId;
	uint8_t Current_Mode;
	char Temperature;
	short X; 
	short Y; 
	short Z; 	
}magMeterDataType; 
#define MAG_ONLY_MODE 0x11
void FXO8700_init(void);
void ReadI2C(uint8_t REG_Address );

#endif
