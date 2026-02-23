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
	RCC_AHB1PeriphClockCmd(RCC_AHB1_Periph_GPIOC, ENABLE); // set for the shift registers now
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
static void ShiftReg_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_ShiftRegisterReset;
	GPIO_InitTypeDef GPIO_ShiftRegisterClock;
	GPIO_InitTypeDef GPIO_ShiftRegisterData;

	GPIO_ShiftRegisterReset.GPIO_Pin = GPIO_Pin_8;
	GPIO_ShiftRegisterReset.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_ShiftRegisterReset.GPIO_OType = GPIO_OType_PP;
	GPIO_ShiftRegisterReset.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_ShiftRegisterReset.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_ShiftRegisterClock.GPIO_Pin = GPIO_Pin_7;
	GPIO_ShiftRegisterClock.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_ShiftRegisterClock.GPIO_OType = GPIO_OType_PP;
	GPIO_ShiftRegisterClock.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_ShiftRegisterClock.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_ShiftRegisterData.GPIO_Pin = GPIO_Pin_6;
	GPIO_ShiftRegisterData.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_ShiftRegisterData.GPIO_OType = GPIO_OType_PP;
	GPIO_ShiftRegisterData.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_ShiftRegisterData.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOC, &GPIO_ShiftRegisterReset);
	GPIO_Init(GPIOC, &GPIO_ShiftRegisterClock);
	GPIO_Init(GPIOC, &GPIO_ShiftRegisterData);
}

static void ADC__Initialization(){
	ADC_InitTypeDef ADC_InitStruct;
	ADC_StructInit(&ADC_InitStruct);
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;

	ADC_Init(ADC1,&ADC_InitStruct);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_3, 1, ADC_SampleTime_3Cycles); // try different vals
	ADC_Cmd(ADC1, ENABLE);
}


static void Clear_Shift_Register()
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_8);
	for (volatile int i = 0; i < 50; i++);
	GPIO_SetBits(GPIOC, GPIO_Pin_8);
}
static void Pulse_Clock(void)
{
	GPIO_SetBits(GPIOC, SR_CLOCK_PIN);
	for (volatile int i = 0; i<50; i++);
	GPIO_ResetBits(GPIOC, SR_CLOCK_PIN);

}
static void shiftByte(uint8_t value)
{
	for (int i = 7; i >= 0; i--)
	{
		shiftBit((value >> i) & 1);
	}
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

	Pusle_Clock();
}






