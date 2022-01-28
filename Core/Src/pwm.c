/*
 * pwm.c
 *
 *  Created on: 23.06.2020
 *      Author: Stunt GP
 */

#include "pwm.h"

void PWM_PrimarySet(){

	//MOTOR 1
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET); // do dzia³ania mostka H
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);   //   nr 1


	//MOTOR 2
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET); // do dzia³ania mostka H
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);   //   nr 2

	//MOTOR 3
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET); // do dzia³ania mostka H
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);   //   nr 3

	//SERVO na PB0 ENK 3A
	HAL_TIMEx_PWMN_Start(&htim8, TIM_CHANNEL_2);

}

void PWM_Set()            // Ustawienie Wypelnienia
{
	//MOTOR 1 na plytce
	if (Speed[0] > 0) {
		TIM1->CCR1 = Speed[0];
		TIM1->CCR2 = 0;
	} else {
		TIM1->CCR1 = 0;
		TIM1->CCR2 = (-1) * Speed[0];
	}
	//MOTOR 2 na plytce
	if (Speed[1] > 0) {
		TIM1->CCR3 = Speed[1];
		TIM1->CCR4 = 0;
	} else {
		TIM1->CCR3 = 0;
		TIM1->CCR4 = (-1) * Speed[1];
	}

	//MOTOR 3 na plytce
	if (Speed[2] > 0) {
		TIM2->CCR1 = Speed[2];
		TIM2->CCR2 = 0;
	} else {
		TIM2->CCR1 = 0;
		TIM2->CCR2 = (-1) * Speed[2];
	}
}





volatile float PID_speed_Kp = 2;
volatile float PID_speed_KiTs = 0.0001;

volatile float integrator_speed[2] = {0,0};
volatile float error_speed[2]= {0,0};

int16_t PWM_PID_speed(float referenceSpeed, float actualSpeed) {
	error_speed[0] = referenceSpeed - actualSpeed;
	integrator_speed[0] += (PID_speed_KiTs * error_speed[0]);
	int16_t out = (int16_t) (PID_speed_Kp * error_speed[0] + integrator_speed[0]);

	if (referenceSpeed > 0) {
		if (out >= PWM_max_duty) {
			integrator_speed[0] -= (PID_speed_KiTs * error_speed[0]);
			out=PWM_max_duty;
		} else if (out <= 0) {
			out = 0;
		}
	} else if (referenceSpeed < 0) {
		if (out >= 0) {
			out = 0;
		} else if (out <= -PWM_max_duty) {
			integrator_speed[0] -= (PID_speed_KiTs * error_speed[0]);
			out=-PWM_max_duty;
		}
	} else if (referenceSpeed == 0) {
		out = 0;
		integrator_speed[0] = 0;
	}

	return out;
}


const int PID_speed_ramp = PID_SPEED_RAMP;

int16_t PID_ramp_out[2] = {0,0};

int16_t PWM_PID_ramp(float referenceSpeed) {
	static int16_t n = 0;
	static int16_t i=0;
	i++;
	if(i>=10){
		n++;
		i=0;
		if (n >= PID_speed_ramp) {
			if (referenceSpeed > PID_ramp_out[0]) {
				PID_ramp_out[0]++;
			}
			if (referenceSpeed < PID_ramp_out[0]) {
				PID_ramp_out[0]--;
			}
			n = 0;
		}
	}
	return PID_ramp_out[0];
}


volatile float PID_position_Kp = 1;
volatile float PID_position_KiTs = 0.001;
volatile float integrator_position[2] = {0,0};
volatile float error_position[2]= {0,0};
int16_t PWM_P_position(float referencePosition, float actualPosition) {
	error_position[0] = referencePosition - actualPosition;
	//integrator_position += (PID_position_KiTs * error_position);
	integrator_position[0]=0;
	int16_t out = (int16_t) (PID_position_Kp * error_position[0] + integrator_position[0]);

	if (referencePosition > 0) {
		if (out >= PWM_max_duty) {
	//		integrator_position -= (PID_position_KiTs * error_position);
			out=PWM_max_duty;
		} //else if (out <= 0) {
		//	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_14); // zmieñ stan diody
		//	out = 0;
		//}
	} else if (referencePosition < 0) {
		if (out >= 0) {
			//out = 0;
		} else if (out <= -PWM_max_duty) {
		//	integrator_position -= (PID_position_KiTs * error_position);
			out=-PWM_max_duty;
		}
	} else if (referencePosition == 0) {
		out = 0;
		integrator_position[0] = 0;
	}

	return out;
}

///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// drugi silnik //////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////



int16_t PWM_PID_speed_y(float referenceSpeed, float actualSpeed) {
	error_speed[1] = referenceSpeed - actualSpeed;
	integrator_speed[1] += (PID_speed_KiTs * error_speed[1]);
	int16_t out = (int16_t) (PID_speed_Kp * error_speed[1] + integrator_speed[1]);

	if (referenceSpeed > 0) {
		if (out >= PWM_max_duty) {
			integrator_speed[1] -= (PID_speed_KiTs * error_speed[1]);
			out=PWM_max_duty;
		} else if (out <= 0) {
			out = 0;
		}
	} else if (referenceSpeed < 0) {
		if (out >= 0) {
			out = 0;
		} else if (out <= -PWM_max_duty) {
			integrator_speed[1] -= (PID_speed_KiTs * error_speed[1]);
			out=-PWM_max_duty;
		}
	} else if (referenceSpeed == 0) {
		out = 0;
		integrator_speed[1] = 0;
	}

	return out;
}


int16_t PWM_PID_ramp_y(float referenceSpeed) {
	static int16_t n = 0;
	static int16_t i=0;
	i++;
	if(i>=10){
		n++;
		i=0;
		if (n >= PID_speed_ramp) {
			if (referenceSpeed > PID_ramp_out[1]) {
				PID_ramp_out[1]++;
			}
			if (referenceSpeed < PID_ramp_out[1]) {
				PID_ramp_out[1]--;
			}
			n = 0;
		}
	}
	return PID_ramp_out[1];
}



int16_t PWM_P_position_y(float referencePosition, float actualPosition) {
	error_position[1] = referencePosition - actualPosition;
	//integrator_position += (PID_position_KiTs * error_position);
	integrator_position[1]=0;
	int16_t out = (int16_t) (PID_position_Kp * error_position[1] + integrator_position[1]);

	if (referencePosition > 0) {
		if (out >= PWM_max_duty) {
	//		integrator_position -= (PID_position_KiTs * error_position);
			out=PWM_max_duty;
		} //else if (out <= 0) {
		//	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_14); // zmieñ stan diody
		//	out = 0;
		//}
	} else if (referencePosition < 0) {
		if (out >= 0) {
			//out = 0;
		} else if (out <= -PWM_max_duty) {
		//	integrator_position -= (PID_position_KiTs * error_position);
			out=-PWM_max_duty;
		}
	} else if (referencePosition == 0) {
		out = 0;
		integrator_position[1] = 0;
	}

	return out;
}





void PID_ClearAll(void) {
	PID_ramp_out[0] = 0;
	integrator_speed[0] = 0;
	PID_reference_Value[0] = 0;
	PID_reference_Speed[0] = 0;
	PID_reference_Duty[0] = 0;

	PID_ramp_out[1] = 0;
	integrator_speed[1] = 0;
	PID_reference_Value[1] = 0;
	PID_reference_Speed[1] = 0;
	PID_reference_Duty[1] = 0;
}
