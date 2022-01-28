/*
 * pwm.h
 *
 *  Created on: 23.06.2020
 *      Author: Stunt GP
 */

#ifndef INC_PWM_H_
#define INC_PWM_H_

#include "main.h"
#define PWM_max_duty 300
int16_t Speed[2];     //Predkosc na wyjsciu z regulatora
float Position[2];
float Position_reference_value[2];

int8_t Received[10];


volatile int16_t PID_reference_Value[2];
volatile int16_t PID_reference_Speed[2];
volatile int16_t PID_reference_Duty[2];
volatile int16_t PID_max_Speed;


void PWM_PrimarySet();
void PWM_Set();


int16_t PWM_PID_speed(float referenceSpeed, float actualSpeed);
int16_t PWM_P_position(float referencePosition, float actualPosition);
int16_t PWM_PID_ramp(float referenceSpeed);

int16_t PWM_PID_speed_y(float referenceSpeed, float actualSpeed);
int16_t PWM_P_position_y(float referencePosition, float actualPosition);
int16_t PWM_PID_ramp_y(float referenceSpeed);
void PID_ClearAll(void);

#endif /* INC_PWM_H_ */
