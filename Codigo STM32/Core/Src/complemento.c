/*
 * complemento.c
 *
 *  Created on: Apr 7, 2025
 *      Author: Andres
 */


#include "complemento.h"

extern TIM_HandleTypeDef htim4;

float fir_coeffs[FILTER_LENGTH] = {0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1};
float rpm_buffer[FILTER_LENGTH];
int buffer_index = 0;

int16_t aplicarFiltroFIR(int16_t rpm_actual) {
    rpm_buffer[buffer_index] = rpm_actual;
    buffer_index = (buffer_index + 1) % FILTER_LENGTH;

    float filtered_rpm = 0.0;
    for (int i = 0; i < FILTER_LENGTH; i++) {
        int idx = (buffer_index - i + FILTER_LENGTH) % FILTER_LENGTH;
        filtered_rpm += fir_coeffs[i] * rpm_buffer[idx];
    }
    return filtered_rpm;
}

void calibracion() {
    int vrb = 0;
    int abc;
    __HAL_TIM_SET_COUNTER(&htim4, 0);
    abc = __HAL_TIM_GET_COUNTER(&htim4);
    while (abc >= 0) {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 1);

        while ((abc = __HAL_TIM_GET_COUNTER(&htim4)) >= 2) {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 0);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 1);

            while ((abc = __HAL_TIM_GET_COUNTER(&htim4)) >= 4) {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 0);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1);

                while ((abc = __HAL_TIM_GET_COUNTER(&htim4)) >= 6) {
                    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0);
                    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 1);

                    if ((abc = __HAL_TIM_GET_COUNTER(&htim4)) >= 8) {
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 0);
                        vrb = 1;
                        break;
                    }
                }
                if (vrb == 1) break;
            }
            if (vrb == 1) break;
        }
        if (vrb == 1) break;
    }
}
