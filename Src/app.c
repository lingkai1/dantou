/* USER CODE BEGIN 0 */
/*
	edit by link 2017-1-9
*/

#include "main.h"
#include "stm32f4xx_hal.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
/*local variable*/
uint32_t ADC_Value[sampleNum];
uint32_t L1,L2,L3,L0;
uint32_t lightSum3;
uint32_t lightSum2;
uint32_t lightSum1;
uint8_t countMax = 0;
uint8_t maxIndexPre;
uint8_t maxIndex;
uint32_t maxStay=0;
int maxStayPre = 0;
uint8_t preIndex;
int8_t Direction = 1;
float speed;
float speedPre;
int speedMoni = 0;
typedef struct
{
	uint8_t index[4];
	uint32_t data[4];
	
}Light;

Light LightData;
Light LightDataSort;
void Bubble_sort(Light *L)
{
	int i,j,temp;
	 for(i = 0; i < 4; i++)
    {
        for(j = 0; j  < 4-i-1; j++)
        {
            if(L->data[j] < L->data[j + 1])
            {
                temp = L->data[j];
                L->data[j] = L->data[j + 1];
                L->data[j + 1] = temp;
							
							  temp = L->index[j];
                L->index[j] = L->index[j + 1];
                L->index[j + 1] = temp;
            }		
        }
    }
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
 if (htim->Instance == htim3.Instance)/*100ns interupt*/
  {
		 
	int i =0;
	int dev=1;

	uint32_t sum0 = 0;
	uint32_t sum1 = 0;
	uint32_t sum2 = 0;
	uint32_t sum3 = 0;
	
	if(maxStay<0xFFFFFFFF)
	  maxStay ++;	
	else
		maxStay = 0xFFFFFFFF;
		
	for(i = 0; i < sampleNum; i++)
	{
		switch (i%4)
		{
			case 0: sum0 += ADC_Value[i]; break;
			case 1: sum1 += ADC_Value[i]; break;
			case 2:	sum2 += ADC_Value[i]; break;
			case 3:	sum3 += ADC_Value[i]; break;
			}
	}
	dev = sampleNum/4;
	L0 = sum0/dev;
	L1 = sum1/dev;
	L2 = sum2/dev;
	L3 = sum3/dev;		 

	 
	LightData.index[0] = 0;
	LightData.index[1] = 1;
  LightData.index[2] = 2;
  LightData.index[3] = 3;	 
	
	LightData.data[0] = L0;
	LightData.data[1] = L1;
	LightData.data[2] = L2;
	LightData.data[3] = L3;	 
	

	LightDataSort = LightData;
	Bubble_sort(&LightDataSort);
	 
	lightSum3 =  LightData.data[0]+LightData.data[1]+LightData.data[2];
	lightSum2 =  LightData.data[0]+LightData.data[1];
	lightSum1 =  LightData.data[0];	 
	
	if(maxIndex == LightDataSort.index[0]-1 || (LightDataSort.index[0] == 0 && maxIndex == 3))
	{
		Direction = 1;
	}	
	else if (maxIndex == LightDataSort.index[0]+1 || (LightDataSort.index[0] == 3 && maxIndex == 0))
	{
		Direction = -1;
	}
	else
	{
		
	}
	
	
	
	
	if (Direction == 1)
	{	
		if(LightDataSort.index[0]==0)  //��ǰ������������һ������  �ݶ�Ϊ��
		{
			preIndex = 3;
		}
		else
		{
			preIndex = LightDataSort.index[0]-1;
		}
	}
	else
	{
			if(LightDataSort.index[0]==3)  //��ǰ������������һ������  �ݶ�Ϊ��
		{
			preIndex = 0;
		}
		else
		{
			preIndex = LightDataSort.index[0]+1;
		}
	}
	
	//if (speed / maxStay)
	if(maxStay < maxStayPre)  //��û�м�⵽��һ���Ƶ�ʱ�� �ٶ�Ĭ�Ϻ���һ֡һ��
		speed = speedPre;
	else 											// ��⵽�ٶ��Ѿ�������һ֡���ٶ���  �����¼����ٶ�
		speed = 1 / ((float)maxStay*4.0*0.001) * 60 * Direction;
	if ((LightDataSort.data[0] > 2 * LightData.data[preIndex]) && LightDataSort.index[0] != maxIndex) // ��������ˣ��ж�����������һ�������ĵ�ѹ3���������Σ��ж����µķ�ֵ
		countMax++;
	if (countMax>2)  //�������� ȷ����һ������ĵ���
	{
		maxIndex = LightDataSort.index[0];
		speed = 1 / ((float)maxStay*4.0*0.001) * 60 * Direction;	 // (r/s)         get speed	
		speedPre = speed; 
		maxStayPre = maxStay;
		maxStay = 0;
		countMax = 0;
	}
	maxIndexPre = LightData.index[0];
 }
	//HAL_UART_Transmit(&huart1, (uint8_t *)&maxIndex, 1, 0xFFFF);
	speedMoni = (int)(speed * 1000);
 if (htim->Instance == htim4.Instance) //1ms interupt
 {
	 
	 HAL_UART_Transmit(&huart1, (uint8_t *)&speed, sizeof(speed), 0xFFFF);
	 
	 
 }
 
}

/* USER CODE END 0 */


