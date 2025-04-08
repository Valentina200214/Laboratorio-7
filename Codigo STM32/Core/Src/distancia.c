/*
 * distancia.c
 *
 *  Created on: Apr 7, 2025
 *      Author: Andres
 */


#include "distancia.h"
#include "velocidad.h"
#include "comunicacion.h"
#include "motorDriver.h"
#include "complemento.h"

#define ADC_BUFFER_SIZE 10

extern uint16_t adcValue[5];
extern uint16_t adcMM[5];
extern float adcVoltaje[5];
extern float adc_avg[5];
extern uint16_t adc_buffer[5][ADC_BUFFER_SIZE];
extern uint8_t adc_buffer_index;
extern uint8_t muestras;

extern float distancias_mm[];
extern int num_puntos;
extern uint16_t adc_sensor1[];
extern uint16_t adc_sensor2[];
extern uint16_t adc_sensor3[];
extern uint16_t adc_sensor4[];

extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim2;

extern int32_t contador;
extern uint32_t tiempo;
extern int16_t rpm_f;
extern int16_t velocidad_mm_s;
extern int16_t velocidad_rad_s;

void promedio_adc(void) {
    for (int i = 0; i < 5; i++) {
        adc_buffer[i][adc_buffer_index] = adcValue[i];
    }

    adc_buffer_index++;
    muestras++;

    if (muestras >= ADC_BUFFER_SIZE) {
        for (int i = 0; i < 5; i++) {
            uint32_t sum = 0;
            for (int j = 0; j < ADC_BUFFER_SIZE; j++) {
                sum += adc_buffer[i][j];
            }
            adc_avg[i] = (float)sum / ADC_BUFFER_SIZE;
            adcVoltaje[i] = (adc_avg[i] * 3.3) / 4095.0;
        }

        adc_buffer_index = 0;
        muestras = 0;
    }
}

void calculo_mm() {
    adcMM[0] = (uint16_t)interpolar_distancia((uint16_t)adc_avg[0], adc_sensor1, distancias_mm, num_puntos);
    adcMM[1] = (uint16_t)interpolar_distancia((uint16_t)adc_avg[1], adc_sensor2, distancias_mm, num_puntos);
    adcMM[2] = (uint16_t)interpolar_distancia((uint16_t)adc_avg[2], adc_sensor3, distancias_mm, num_puntos);
    adcMM[3] = (uint16_t)interpolar_distancia((uint16_t)adc_avg[3], adc_sensor4, distancias_mm, num_puntos);
}

float interpolar_distancia(uint16_t adc_recibido, uint16_t *adc_tabla, float *distancias, int num_puntos) {
    if (adc_recibido >= adc_tabla[0]) {
        return distancias[0];
    }
    if (adc_recibido <= adc_tabla[num_puntos - 1]) {
        return distancias[num_puntos - 1];
    }

    for (int i = 0; i < num_puntos - 1; i++) {
        if (adc_recibido <= adc_tabla[i] && adc_recibido >= adc_tabla[i + 1]) {
            float x0 = adc_tabla[i];
            float x1 = adc_tabla[i + 1];
            float y0 = distancias[i];
            float y1 = distancias[i + 1];

            float m = (y1 - y0) / (x1 - x0);
            float b = y0 - m * x0;
            return m * adc_recibido + b;
        }
    }

    return distancias[num_puntos - 1];
}

void control_distancia(uint16_t distancia_objetivo_mm) {
    const uint8_t TOLERANCIA_MM = 2;

    int16_t distancia_actual;
    int8_t velocidad_pwm;
    uint8_t bandera_movimiento = 1;

    while (bandera_movimiento) {
        distancia_actual = (adcMM[0] + adcMM[1] + adcMM[2] + adcMM[3]) / 4;

        int16_t error_distancia = distancia_objetivo_mm - distancia_actual;

        if (abs(error_distancia) <= TOLERANCIA_MM) {
            velocidad_pwm = 0;
            motores(velocidad_pwm, velocidad_pwm);
            if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == 0){
            	bandera_movimiento = 0;
            }
        } else {
            if (error_distancia > 0) {
                conversor_mm_s(-400);
                velocidad_pwm = pwm_mm;
                motores(velocidad_pwm, velocidad_pwm);
            } else {
                conversor_mm_s(400);
                velocidad_pwm = pwm_mm;
                motores(velocidad_pwm, velocidad_pwm);
            }
        }

        calculo_rpm();
        contador = ((int32_t)__HAL_TIM_GET_COUNTER(&htim5)) / 1431;
        tiempo = (__HAL_TIM_GET_COUNTER(&htim2) / 2);
        calibracion();
        promedio_adc();
        calculo_mm();
        datos_enviar(rpm_f, contador, tiempo, velocidad_mm_s, velocidad_rad_s,
                     adcValue[0], adcValue[1], adcValue[2], adcValue[3],
                     adcVoltaje[0], adcVoltaje[1], adcVoltaje[2], adcVoltaje[3],
                     adcMM[0], adcMM[1], adcMM[2], adcMM[3]);

    }
}
