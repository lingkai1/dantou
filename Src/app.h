#ifndef __APP_H
#define __APP_H
#include "stm32f4xx_hal.h"
void timer4DataTranmit(void);
void timer3DataProcess(void); //����ת�٣�����ADֵ�Ը����ƽ�����������LightData��LightDataSort;
uint32_t sumArray(uint32_t *in, uint8_t len);
void Bubble_sort_all(uint32_t *in, uint8_t len);
void findTenMaxAndMinAD(void);
void findAngel(void);
#endif
