/*
 * velocidad.c
 *
 *  Created on: Apr 7, 2025
 *      Author: Andres
 */


#include "velocidad.h"
#include "complemento.h"
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

void vel(int16_t rpm_s) {
    velocidad_mm_s = (3.1416 * diametro_rueda * rpm_s) / 60.0;
    velocidad_rad_s = (2 * 3.1416 * rpm_s) / 60.0;
}

void calculo_rpm() {
    if (set == 1) {
        __HAL_TIM_SET_COUNTER(&htim1, 0);
        set = 0;
    }

    if (__HAL_TIM_GET_COUNTER(&htim1) >= 200) {
        __HAL_TIM_SET_COUNTER(&htim1, 0);

        contador_actual = (int32_t)__HAL_TIM_GET_COUNTER(&htim5);
        contador_actual_2 = (int32_t)__HAL_TIM_GET_COUNTER(&htim3);
        pulsos_act = fabsf((float)contador_actual) / 1431.0f; // Usamos fabsf para float
        pulsos_act_2 = fabsf((float)contador_actual_2) / 1431.0f;
        pulsos = pulsos_act - pulsos_ant;
        pulsos_2 = pulsos_act_2 - pulsos_ant_2;
        pulsos_ant = pulsos_act;
        pulsos_ant_2 = pulsos_act_2;

        float temp_rpm = pulsos * 600.0f;
		rpm = (int16_t)fabsf(temp_rpm);
		rpm_2 = (int16_t)fabsf(pulsos_2 * 600.0f);

        if (rpm < 0) rpm = 0;
        rpm_f = aplicarFiltroFIR(rpm);
        vel(rpm_f);
    }
}

void conversor(float volt) {
    if (volt < 0) {
        volt = 0;
    } else if (volt > 7.5) {
        volt = 7.5;
    } else {
        val_pwm = (int32_t)((volt * (-100)) / (7.5));
    }
}

void conversor_mm_s(int32_t velocidad) {
    if (velocidad > 744) {
        velocidad = 744;
    }
    pwm_mm = ((velocidad * 100) / 744) * (-1);
}
