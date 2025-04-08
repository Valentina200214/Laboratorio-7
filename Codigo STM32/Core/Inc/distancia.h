/*
 * distancia.h
 *
 *  Created on: Apr 7, 2025
 *      Author: Andres
 */

#ifndef INC_DISTANCIA_H_
#define INC_DISTANCIA_H_

#include "main.h"

void promedio_adc(void);
void calculo_mm(void);
float interpolar_distancia(uint16_t adc_recibido, uint16_t *adc_tabla, float *distancias, int num_puntos);
void control_distancia(uint16_t distancia_objetivo_mm);

extern int16_t pwm_mm;

#endif /* INC_DISTANCIA_H_ */
