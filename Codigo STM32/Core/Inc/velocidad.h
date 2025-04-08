/*
 * velocidad.h
 *
 *  Created on: Apr 7, 2025
 *      Author: Andres
 */

#ifndef INC_VELOCIDAD_H_
#define INC_VELOCIDAD_H_

#include "main.h"
#include <math.h>

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim3;

extern uint8_t set;
extern float pulsos;
extern float pulsos_2;
extern int16_t rpm;
extern int16_t rpm_2;
extern float pulsos_ant;
extern float pulsos_act;
extern float pulsos_ant_2;
extern float pulsos_act_2;
extern int32_t contador_actual;
extern int32_t contador_actual_2;
extern int16_t rpm_f;
extern float diametro_rueda;
extern int16_t velocidad_mm_s;
extern int16_t velocidad_rad_s;
extern int32_t val_pwm;
extern int16_t pwm_mm;

void vel(int16_t rpm_s);
void calculo_rpm(void);
void conversor(float volt);
void conversor_mm_s(int32_t velocidad);


#endif /* INC_VELOCIDAD_H_ */
