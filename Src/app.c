/* USER CODE BEGIN 0 */
/*
	edit by link 2017-1-9
*/

#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "app.h"
#include "math.h"
#include "MAG3110.h"

extern uint32_t magSpeedCount;
/*local variable*/



uint32_t ADC_Value[sampleNum];
uint32_t L1,L2,L3,L0;
uint8_t countMax = 0;
uint8_t maxIndexPre; // ��һ��AD���ֵ���ɼ����ݽ��
uint8_t maxIndex;		//��ǰADֵ�����жϽ��
uint32_t maxStay=0;	//���ֵͣ����ʱ��
int maxStayPre = 0;// ��һ�����ֵͣ����ʱ��
uint8_t preIndex;   //��ǰ�ƺŵ���һ���ƺţ���3�ŵ� ���ϸ��ƺ���2��0
int8_t Direction = 1; //ת������ -1������ -1������
float speed;					//��ת�ٶ�
float speedPre;				//�ϴ��ٶ�
int speedMoni = 0;
uint32_t test = 0;   // use to test
uint32_t topTenMax[10] = {0,0,0,0,0,0,0,0,0,0};
uint32_t topTenMin[10] = {0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff};
uint32_t maxAD; //��������ADֵ��������Сֵ������������һ�εƵ����ݹ�һ��,ȡtopTenMax�ľ�ֵ��
uint32_t minAD;
uint8_t maxAdIndex;  //maxAd��Ӧ��Index
float angleRaw;
float angleRaw180;
float angle;
float lambda;
float angle180;
float angle180Pre=0;


extern float mag_speed_out_r;
extern float mag_angle_out_180;
extern uint32_t magSpeedCount;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
//	if(GPIO_Pin == ENCO_Pin)
//	{
//		if(HAL_GPIO_ReadPin(Direction_GPIO_Port,Direction_Pin) == GPIO_PIN_SET)
//		{
//			encoderCout ++;
//		}
//		else
//		{
//			encoderCout --;
//		}
//	}
//	if(GPIO_Pin == EncoderZero_Pin)
//	{
//		
//		encoderCout = 0;	
//	}
}	
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
 if (htim->Instance == htim2.Instance)/* 1ms interupt*/
 {
		// 	timer3DataProcess();
			magSpeedCount ++;
//			DataTranmit();
 }
	//HAL_UART_Transmit(&huart1, (uint8_t *)&maxIndex, 1, 0xFFFF);
	
 if (htim->Instance == htim4.Instance) //100ns interupt
 {	 	
	 //timer4DataTranmit();
	 		// 	timer3DataProcess();
			magSpeedCount ++;
			DataTranmit();
 }
}
void DataTranmit()
{
	uint8_t data[10];
	typedef union Rx_Dat
	{
		float x[2];
		uint8_t y[8];
	}Rx_Dat;
	
	Rx_Dat My_Rx_Dat;
	My_Rx_Dat.x[0] = mag_speed_out_r;
	My_Rx_Dat.x[1] = mag_angle_out_180;
	
//	
//	
//	
//	My_Rx_Dat.ad[0] = LightData.data[0];
//	My_Rx_Dat.ad[1] = LightData.data[1];
//	My_Rx_Dat.ad[2] = LightData.data[2];
//	My_Rx_Dat.ad[3] = LightData.data[3];
	
	
	data[0] = 0xAA;
	data[1] = 0x55;
	data[2] = My_Rx_Dat.y[0];	
	data[3] = My_Rx_Dat.y[1];	
	data[4] = My_Rx_Dat.y[2];	
	data[5] = My_Rx_Dat.y[3];	
	data[6] = My_Rx_Dat.y[4];	
	data[7] = My_Rx_Dat.y[5];	
	data[8] = My_Rx_Dat.y[6];	
	data[9] = My_Rx_Dat.y[7];	
	
	HAL_UART_Transmit(&huart1, (uint8_t *)data, sizeof(data), 0xFFFF);

}

