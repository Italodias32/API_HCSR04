/*
 * ***************************************************************
 *  API name    : HCSR04
 *  API files   : HCSR04.c, HCSR04.c
 *  file		: HCSR04.h
 *  authors		: Italo Dias, Sarah Carine
 *  university  : Federal University of Minas Gerais
 *  license     : GNU General Public License v3.0
 *  date        : 02/19/21
 *  modified	: 02/19/21
 *  This code implements a library for HCSR04
 *
 *  This API was developed as an assignment for Embedded Systems
 *  Programming class at the Federal University of Minas Gerais
 ***************************************************************
 *  SOFTWARE SETUP:
 *  Include HCSR04.h in main.c
 *
 ***************************************************************
 *  HARDWARE SETUP:
 *
 *  VSS = GND
 *  VDD = 5v
 *  Trigger = - Connect in Digital Pin - Output
 *  Echo = - Connect in Digital Pin - Input
 *****************************************************************
 */

#ifndef HCSR04_H
#define HCSR04_H

///************************************ Includes ****************************************/
//Header
#include "stm32f1xx_hal.h"
#include "main.h"

//Library
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

///************************************ Defines *****************************************/
#define microTIM TIM4
#define speedSound  0.0343/2
#define pass (void)0

///*********************************** Structures ***************************************/
typedef GPIO_TypeDef* GPIO_Port;
typedef uint16_t GPIO_Pin;
typedef struct{
	GPIO_Port* trig_port;
	GPIO_Pin trig_pin;

	GPIO_Port* echo_port;
	GPIO_Pin echo_pin;
}ultrasonic;

/*********************************** Public functions ***********************************/
ultrasonic HCSR04_generate(GPIO_Port port_trig[],GPIO_Pin pin_trig,GPIO_Port port_echo[],GPIO_Pin pin_echo);
void uDelay(uint32_t microSec);
void adjustment(float gainFactor);
float distance_cm(ultrasonic hcsr04);
float distance_m(ultrasonic hcsr04);
float distance_mm(ultrasonic hcsr04);
float distance_inch(ultrasonic hcsr04);
float forwardSpeed(ultrasonic hcsr04, float time);
float crossSpeed(ultrasonic hcsr04, float distance, float underLimit);
bool itsBetween(ultrasonic hcsr04, float underLimit, float upperLimit);

#endif /* HCSR04_H */
