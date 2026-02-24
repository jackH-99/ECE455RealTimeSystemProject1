 #include <stdint.h>
#include <stdio.h>
#include "stm32f4_discovery.h"
#include "stm32f4xx_gpio.h"

#define SR_DATA_PIN GPIO_Pin_6
#define SR_CLOCK_PIN GPIO_Pin_7
#define SR_CLEAR_PIN GPIO_Pin_8
#define TL_RED 0x80
#define TL_AMBER 0x40
#define TL_GREEN 0x20
#define TL_OFF 0x00

static void shiftByte(uint8_t value);
static void shiftBit(uint8_t bit);


static void enableClocks(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1_Periph_GPIOB, ENABLE); // set for the shift registers now
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); // don't know if this is right for the ADC
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

}

static void ADC_GPIO_Initialization(){
	GPIO_InitTypeDef ADC_GPIO;
	ADC_GPIO.GPIO_Pin = GPIO_Pin_3;
	ADC_GPIO.GPIO_Mode = GPIO_Mode_AN;
	ADC_GPIO.GPIO_PuPd = GPIO_PuPd_NOPULL;
	ADC_GPIO.GPIO_OType = GPIO_OType_PP;
	ADC_GPIO.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &ADC_GPIO);



}
/*
static void LED_GPIO_Initialization(){
	GPIO_InitTypeDef red_Light;

	red_Light.GPIO_Pin = GPIO_Pin_0;
	red_Light.GPIO_Mode = GPIO_Mode_OUT;
	red_Light.GPIO_OType = GPIO_OType_PP;
	red_Light.GPIO_Speed = GPIO_Speed_50MHz;
	red_Light.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_InitTypeDef amber_Ligth;

	amber_Light.GPIO_Pin = GPIO_Pin_1;
	amber_Light.GPIO_Mode = GPIO_Mode_OUT;
	amber_Light.GPIO_OType = GPIO_OType_PP;
	amber_Light.GPIO_Speed = GPIO_Speed_50MHz;
	amber_Light.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_InitTypeDef green_Ligth;

	green_Light.GPIO_Pin = GPIO_Pin_2;
	green_Light.GPIO_Mode = GPIO_Mode_OUT;
	green_Light.GPIO_OType = GPIO_OType_PP;
	green_Light.GPIO_Speed = GPIO_Speed_50MHz;
	green_Light.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOC, &redLight_GPIO);
	GPIO_Init(GPIOC, &amberLight_GPIO);
	GPIO_Init(GPIOC, &greenLight_GPIO);
}
*/
static void ShiftReg_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_ShiftRegisterReset1;
	GPIO_InitTypeDef GPIO_ShiftRegisterClock1;
	GPIO_InitTypeDef GPIO_ShiftRegisterData1;

	GPIO_InitTypeDef GPIO_ShiftRegisterReset2;
	GPIO_InitTypeDef GPIO_ShiftRegisterClock2;
	GPIO_InitTypeDef GPIO_ShiftRegisterData2;

	GPIO_InitTypeDef GPIO_ShiftRegisterReset3;
	GPIO_InitTypeDef GPIO_ShiftRegisterClock3;
	GPIO_InitTypeDef GPIO_ShiftRegisterData3;



	GPIO_ShiftRegisterReset1.GPIO_Pin = GPIO_Pin_8;
	GPIO_ShiftRegisterReset1.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_ShiftRegisterReset1.GPIO_OType = GPIO_OType_PP;
	GPIO_ShiftRegisterReset1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_ShiftRegisterReset1.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_ShiftRegisterClock1.GPIO_Pin = GPIO_Pin_7;
	GPIO_ShiftRegisterClock1.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_ShiftRegisterClock1.GPIO_OType = GPIO_OType_PP;
	GPIO_ShiftRegisterClock1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_ShiftRegisterClock1.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_ShiftRegisterData1.GPIO_Pin = GPIO_Pin_6;
	GPIO_ShiftRegisterData1.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_ShiftRegisterData1.GPIO_OType = GPIO_OType_PP;
	GPIO_ShiftRegisterData1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_ShiftRegisterData1.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_ShiftRegisterReset2.GPIO_Pin = GPIO_Pin_11;
	GPIO_ShiftRegisterReset2.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_ShiftRegisterReset2.GPIO_OType = GPIO_OType_PP;
	GPIO_ShiftRegisterReset2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_ShiftRegisterReset2.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_ShiftRegisterClock2.GPIO_Pin = GPIO_Pin_13;
	GPIO_ShiftRegisterClock2.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_ShiftRegisterClock2.GPIO_OType = GPIO_OType_PP;
	GPIO_ShiftRegisterClock2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_ShiftRegisterClock2.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_ShiftRegisterData2.GPIO_Pin = GPIO_Pin_15;
	GPIO_ShiftRegisterData2.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_ShiftRegisterData2.GPIO_OType = GPIO_OType_PP;
	GPIO_ShiftRegisterData2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_ShiftRegisterData2.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_ShiftRegisterReset3.GPIO_Pin = GPIO_Pin_12;
	GPIO_ShiftRegisterReset3.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_ShiftRegisterReset3.GPIO_OType = GPIO_OType_PP;
	GPIO_ShiftRegisterReset3.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_ShiftRegisterReset3.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_ShiftRegisterClock3.GPIO_Pin = GPIO_Pin_14;
	GPIO_ShiftRegisterClock3.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_ShiftRegisterClock3.GPIO_OType = GPIO_OType_PP;
	GPIO_ShiftRegisterClock3.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_ShiftRegisterClock3.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_ShiftRegisterData3.GPIO_Pin = GPIO_Pin_10;
	GPIO_ShiftRegisterData3.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_ShiftRegisterData3.GPIO_OType = GPIO_OType_PP;
	GPIO_ShiftRegisterData3.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_ShiftRegisterData3.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOC, &GPIO_ShiftRegisterReset1);
	GPIO_Init(GPIOC, &GPIO_ShiftRegisterClock1);
	GPIO_Init(GPIOC, &GPIO_ShiftRegisterData1);

	GPIO_Init(GPIOB, &GPIO_ShiftRegisterReset2);
	GPIO_Init(GPIOB, &GPIO_ShiftRegisterClock2);
	GPIO_Init(GPIOB, &GPIO_ShiftRegisterData2);

	GPIO_Init(GPIOB, &GPIO_ShiftRegisterReset3);
	GPIO_Init(GPIOB, &GPIO_ShiftRegisterClock3);
	GPIO_Init(GPIOB, &GPIO_ShiftRegisterData3);
}

static void ADC__Initialization(){
	ADC_InitTypeDef ADC_InitStruct;
	ADC_StructInit(&ADC_InitStruct);
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;

	ADC_Init(ADC1,&ADC_InitStruct);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_3, 1, ADC_SampleTime_3Cycles); // try different vals
	ADC_Cmd(ADC1, ENABLE);
}

static void shiftBit(uint8_t bit)
{
	if (bit)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_6);
	}
	else
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_6);
	}

	GPIO_SetBits(GPIOC, SR_CLOCK_PIN);
	for (volatile int i = 0; i<50; i++);
	GPIO_ResetBits(GPIOC, SR_CLOCK_PIN);
}






