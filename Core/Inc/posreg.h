/*
 * posreg.h
 *
 *  Created on: 23.06.2020
 *      Author: Stunt GP
 */

#ifndef INC_POSREG_H_
#define INC_POSREG_H_

#include "main.h"

uint16_t EnkPosition[2];
float SPEED_FiltratedValue[2];
volatile int32_t ENK_speed[2];



void POSREG_EncoderStart();
void POSREG_EcoderRead();
float SPEED_getFiltrated_x(void);
float lowPassFilterSpeed_x(float in_0);
float SPEED_getFiltrated_y(void);
float lowPassFilterSpeed_y(float in_0);




#endif /* INC_POSREG_H_ */
