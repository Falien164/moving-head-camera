/*
 * posreg.c
 *
 *  Created on: 23.06.2020
 *      Author: Stunt GP
 */

#include "posreg.h"

void POSREG_EncoderStart() {
	// Encoder 1

	__HAL_TIM_ENABLE_IT(&htim3, TIM_IT_CC1);
	__HAL_TIM_ENABLE_IT(&htim3, TIM_IT_CC2);
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);

	// Encoder 2
	__HAL_TIM_ENABLE_IT(&htim4, TIM_IT_CC1);
	__HAL_TIM_ENABLE_IT(&htim4, TIM_IT_CC2);
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);

	// Encoder 3
	//HAL_TIM_Encoder_Start(&htim8, TIM_CHANNEL_3);
	//HAL_TIM_Encoder_Start(&htim8, TIM_CHANNEL_2);
}

void POSREG_EcoderRead() {
	// Encoder 1
	EnkPosition[1] = (TIM3->CNT);
	// Encoder 2
	EnkPosition[0] = (TIM4->CNT);

	// Encoder 3
	//EnkPosition[2] = (TIM8->CNT);
}

void TIM3_Callback(void) {
	if (__HAL_TIM_GET_FLAG(&htim3, TIM_FLAG_CC1) != RESET) {
		if (__HAL_TIM_GET_IT_SOURCE(&htim3, TIM_IT_CC1) != RESET) {

			__HAL_TIM_CLEAR_IT(&htim3, TIM_IT_CC1);
			if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == GPIO_PIN_SET) {
				ENK_speed[1] = HAL_TIM_ReadCapturedValue(&htim3,
				TIM_CHANNEL_1);
			} else {
				ENK_speed[1]= -HAL_TIM_ReadCapturedValue(&htim3,
				TIM_CHANNEL_1);
			}
		}
	}
	if (__HAL_TIM_GET_FLAG(&htim3, TIM_FLAG_CC3) != RESET) {
		if (__HAL_TIM_GET_IT_SOURCE(&htim3, TIM_IT_CC3) != RESET) {
			__HAL_TIM_CLEAR_IT(&htim3, TIM_IT_CC3);
			ENK_speed[1] = 0;
		}
	}
}

void TIM4_Callback(void) {
	if (__HAL_TIM_GET_FLAG(&htim4, TIM_FLAG_CC1) != RESET) {
		if (__HAL_TIM_GET_IT_SOURCE(&htim4, TIM_IT_CC1) != RESET) {
			__HAL_TIM_CLEAR_IT(&htim4, TIM_IT_CC1);
			if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == GPIO_PIN_SET) {
				ENK_speed[0] = HAL_TIM_ReadCapturedValue(&htim4,
				TIM_CHANNEL_1);
			} else {
				ENK_speed[0]= -HAL_TIM_ReadCapturedValue(&htim4,
				TIM_CHANNEL_1);
			}
		}
	}
	if (__HAL_TIM_GET_FLAG(&htim4, TIM_FLAG_CC3) != RESET) {
		if (__HAL_TIM_GET_IT_SOURCE(&htim4, TIM_IT_CC3) != RESET) {
			__HAL_TIM_CLEAR_IT(&htim4, TIM_IT_CC3);
			ENK_speed[0] = 0;
		}
	}
}


const float a1s = 1;
const float a2s = -0.700207538209710;
const float b1s = 0.149896230895145;
const float b2s = 0.149896230895145;
float lowPassFilterSpeed_x(float in_0) {
	static float in_1 = 0;
	static float out_1 = 0;
	float out_0;
	if (in_0 != 0) {
		out_0 = b1s * in_0 + b2s * in_1 - a2s * out_1;
		in_1 = in_0;
		out_1 = out_0;
		return out_0;
	} else {
		in_1 = 0;
		out_1 = 0;
		return 0;
	}
}




float SPEED_getFiltrated_x(void) {
	static float raw_speed;
	if (ENK_speed[0] != 0) {
		raw_speed = 120000 / ENK_speed[0];
	} else {
		raw_speed = 0;
	}
	SPEED_FiltratedValue[0] = lowPassFilterSpeed_x(raw_speed);
	if(SPEED_FiltratedValue[0] <40 && SPEED_FiltratedValue[0] >-40)
		SPEED_FiltratedValue[0]=0;
	return SPEED_FiltratedValue[0];
}


float lowPassFilterSpeed_y(float in_0) {
	static float in_1 = 0;
	static float out_1 = 0;
	float out_0;
	if (in_0 != 0) {
		out_0 = b1s * in_0 + b2s * in_1 - a2s * out_1;
		in_1 = in_0;
		out_1 = out_0;
		return out_0;
	} else {
		in_1 = 0;
		out_1 = 0;
		return 0;
	}
}
float SPEED_getFiltrated_y(void) {
	static float raw_speed;
	if (ENK_speed[1] != 0) {
		raw_speed = 100000 / ENK_speed[1];
	} else {
		raw_speed = 0;
	}
	SPEED_FiltratedValue[1] = lowPassFilterSpeed_y(raw_speed);
	if(SPEED_FiltratedValue[1] <40 && SPEED_FiltratedValue[1] >-40)
		SPEED_FiltratedValue[1]=0;
	return SPEED_FiltratedValue[1];
}


