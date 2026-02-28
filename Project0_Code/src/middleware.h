/*
 * middleware.h
 *
 *  Created on: Feb 19, 2026
 *      Author: jackh
 */

#ifndef MIDDLEWARE_H_
#define MIDDLEWARE_H_

#include <stdint.h>

#define SR_DATA_PIN GPIO_Pin_6

void shiftByte(uint8_t value);
void shiftBit(uint8_t bit);
void clearShiftRegister(void);

void enableClocks(void);
void ADC_GPIO_Initialization(void);
void ShiftReg_GPIO_Init(void);
void ADC_Initialization(void);


#endif /* MIDDLEWARE_H_ */
