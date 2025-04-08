/*
 * complemento.h
 *
 *  Created on: Apr 7, 2025
 *      Author: Andres
 */

#ifndef INC_COMPLEMENTO_H_
#define INC_COMPLEMENTO_H_

#include "main.h"

#define FILTER_LENGTH 10

int16_t aplicarFiltroFIR(int16_t rpm_actual);
void calibracion(void);

#endif /* INC_COMPLEMENTO_H_ */
