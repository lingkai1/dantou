/* USER CODE BEGIN 0 */
/*
	edit by link 2017-1-9
*/

#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "app.h"
#include "math.h"
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
typedef struct
{
	uint8_t index[4];
	uint32_t data[4];
	
}Light;

Light LightData;
Light LightDataSort;
/**********�����е�ADֵ���д�С����*************/
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

/**********���������������**********/
void Bubble_sort_all(uint32_t *in, uint8_t len)
{
	int i,j,temp;
	 for(i = 0; i < len; i++)
    {
        for(j = 0; j  < len-i-1; j++)
        {
            if(in[j] < in[j + 1])
            {
                temp = in[j];
                in[j] = in[j + 1];
                in[j + 1] = temp;
							
//							  temp = L->index[j];
//                L->index[j] = L->index[j + 1];
//                L->index[j + 1] = temp;
            }		
        }
    }
}
//////////////////////////////////////////////////////////////////////////
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
 if (htim->Instance == htim3.Instance)/* 1ms interupt*/
 {
		 timer3DataProcess();
		 timer4DataTranmit();
 }
	//HAL_UART_Transmit(&huart1, (uint8_t *)&maxIndex, 1, 0xFFFF);
	
 if (htim->Instance == htim4.Instance) //10ms interupt
 {
	 //timer4DataTranmit();
 }
 
}
void timer4DataTranmit()
{
	uint8_t data[10];
	typedef union Rx_Dat
	{
	uint8_t y[8];
	uint16_t ad[4];
	}Rx_Dat;
	
	Rx_Dat My_Rx_Dat;
	
	
	
	My_Rx_Dat.ad[0] = LightData.data[0];
	My_Rx_Dat.ad[1] = LightData.data[1];
	My_Rx_Dat.ad[2] = LightData.data[2];
	My_Rx_Dat.ad[3] = LightData.data[3];
	
	
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
//////////////////////////////////////////////////////
void timer3DataProcess()
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
	///////////////////////////////////////////////////	
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
	/////////////////////////////////////////////////////////

	LightDataSort = LightData;
	Bubble_sort(&LightDataSort);
	//��10������10����С��ADֵ
	findTenMaxAndMinAD();
	 
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
		speed = 1 / ((float)maxStay*4.0*0.001)  * Direction;
	if ((LightDataSort.data[0] > 2 * LightData.data[preIndex]) && LightDataSort.index[0] != maxIndex) // ��������ˣ��ж�����������һ�������ĵ�ѹ3���������Σ��ж����µķ�ֵ
		countMax++;
	if (countMax>2)  //�������� ȷ����һ������ĵ���
	{
		
		speed = 1 / ((float)maxStay*4.0*0.001) * Direction;	 // (r/s)         get speed	
		speedPre = speed; 
		maxStayPre = maxStay;
		maxStay = 0;
		countMax = 0;
		maxAD = sumArray(topTenMax,10)/10;
		minAD = sumArray(topTenMin,10)/10;
		maxAdIndex = maxIndex;
		maxIndex = LightDataSort.index[0];
		memset(topTenMax,0x00,sizeof(topTenMax));
		memset(topTenMin,0xFF,sizeof(topTenMin));
		//findADmax()
	}
	
	findAngel();
	maxIndexPre = LightData.index[0];
	speedMoni = (int)(speed * 1000);
}

uint32_t sumArray(uint32_t *in, uint8_t len)
{
	uint32_t sum = 0;
	uint8_t i ;
	for(i=0; i<len; i++)
	{
		sum += in[i];
	}
	
	return sum; 
}

void findTenMaxAndMinAD()
{
	//findMax
	if(LightDataSort.data[0] > topTenMax[9]) //��ǰ�����������������С��
	{
		topTenMax[9] = LightDataSort.data[0];
		Bubble_sort_all(topTenMax, 10);	
	}else{}
	
	if(LightDataSort.data[3] < topTenMin[0])//��ǰ��СС����С�����е����
	{
		topTenMin[0] = LightDataSort.data[3];
		Bubble_sort_all(topTenMin, 10);	
	}else{}
	
}
/* USER CODE END 0 */
void findAngel()
{
	lambda = (float)LightDataSort.data[0] / (float)maxAD;
	angleRaw = sqrt(-2*0.4028*log(lambda));	
	if((Direction == 1 && maxAdIndex != LightDataSort.index[0]) || (Direction == -1 && maxAdIndex == LightDataSort.index[0])) // ����ת�٣�����ǵ�ǰ���������Ƕ�,����ת������ǵ�ǰ
	{
		angle = angleRaw;
		//if(maxAdIndex != ) 
	}
	else																										// �����������������Ƕȡ�
	{
		angle = -angleRaw;
	}
	
		angle180 = angle/3.1416*180;
		angleRaw180 = angleRaw/3.1416*180;
	
		angle180 = LightDataSort.index[0]*90 + angle180;
	
		if(angle180>=360)
			angle180 -= 360;
		else if (angle180 < 0)
			angle180 += 360;
		angle180 = 0.92*angle180Pre+0.08*angle180;
		angle180Pre = angle180;
}

