/*
 ******************************************************************************************************************************
 *  API name    : HCSR04
 *  API files   : HCSR04.c, HCSR04.h
 *  file		: HCSR04.c
 *  authors		: Italo Dias, Sarah Oliveira
 *  university  : Federal University of Minas Gerais
 *  license     : GNU General Public License v3.0
 *  date        : 02/19/21
 *  modified	: 02/19/21
 *  This code implements a library for HCSR04
 *
 *  This API was developed as an assignment for Embedded Systems
 *  Programming class at the Federal University of Minas Gerais
 *
 ******************************************************************************************************************************
 *  References:
 *  Tutorial 37 - HC-SR05 Ultrasonic Sensor
 *  user Github: MYaqoobEmbedded and Kefirr Grzegorz
 *  authors:
 *  link code: https://github.com/MYaqoobEmbedded/STM32-Tutorials/tree/master/Tutorial%2037%20-%20HC-SR05%20Ultrasonic%20Sensor
 *
 *  LCD_20x4
 *  user Github: elisabacelar and Bloq96
 *  authors: Elisa Bacelar and Gabriel Araújo
 *  link code: https://github.com/elisabacelar/LCD_20x4
 *
 *  Display_API
 *  user Github: vcaitite and AmandaARTeixeira
 *  authors: Vitor Gabrie Reis Caitite and Amanda Alkmim Rezende Teixeira
 *  link code: https://github.com/vcaitite/Display_API
 *
 ******************************************************************************************************************************
 *  SOFTWARE SETUP:
 *  Include HCSR04.h in main.c
 *
******************************************************************************************************************************
 *  HARDWARE SETUP:
 *  VSS = GND
 *  VDD = 5v
 *  Trigger = - Connected in Digital Pin - Output
 *  Echo = - Connected in Digital Pin - Input
 *
 ******************************************************************************************************************************
 */
#include "HCSR04.h"

///*************************************************** Global Variables *******************************************************/
float gain = 2.8;

///************************************************** Function definitions ****************************************************/
///********************************************** Static function definition **************************************************/
/**
  * @brief Microsecond delay
  * @param[microSec] Microsecond delay time
  * @retval None
  */
void uDelay(uint32_t microSec){
	if(microSec < 2){
		microSec = 2;
	}
	microTIM->ARR = microSec - 1;
	microTIM->EGR = 1;
	microTIM->SR &= ~1;
	microTIM->CR1 |= 1;
	while((microTIM->SR&0x0001) != 1);
	microTIM->SR &= ~(0x0001);
}

/**
  * @brief Creates a configuration for HCSR04 sensor
  * @param[port_trig] Trig port configuration
  * @param[pin_trig] Trig pin configuration
  * @param[port_echo] Echo port configuration
  * @param[pin_echo] Echo pin configuration
  * @retval An object ultrasonic
  */
ultrasonic HCSR04_generate(GPIO_Port port_trig[],GPIO_Pin pin_trig,GPIO_Port port_echo[],GPIO_Pin pin_echo){
	ultrasonic hcsr04;
	hcsr04.trig_port = port_trig;
	hcsr04.trig_pin = pin_trig;
	hcsr04.echo_port = port_echo;
	hcsr04.echo_pin = pin_echo;
	return hcsr04;
}

/**
  * @brief Adjusts the gain for fine adjustments during execution
  * @param[gainFactor] Gain value for adjustment
  * @retval None
  */
void adjustment(float gainFactor){
	gain = gain*gainFactor;
}

/**
  * @brief Indicates the distance value from an object in cm
  * @param[hcsr04] Ultrasonic object
  * @retval Distance in cm
  */
float distance_cm(ultrasonic* hcsr04){
	uint32_t cont = 0;

	HAL_GPIO_WritePin(hcsr04->trig_port, hcsr04->trig_pin, GPIO_PIN_RESET);
	uDelay(3);

	HAL_GPIO_WritePin(hcsr04->trig_port, hcsr04->trig_pin, GPIO_PIN_SET);
	uDelay(10);
	HAL_GPIO_WritePin(hcsr04->trig_port, hcsr04->trig_pin, GPIO_PIN_RESET);

	while(HAL_GPIO_ReadPin(hcsr04->echo_port, hcsr04->echo_pin) == GPIO_PIN_RESET);

	while(HAL_GPIO_ReadPin(hcsr04->echo_port, hcsr04->echo_pin) == GPIO_PIN_SET)
	{
		 cont++;
		 uDelay(2);
	};

	return (cont + 0.0f)*gain*speedSound;
}

/**
  * @brief Indicates the distance value from an object in m
  * @param[hcsr04] Ultrasonic object
  * @retval Distance in m
  */
float distance_m(ultrasonic* hcsr04){
	return distance_cm(hcsr04)/100;
}

/**
  * @brief Indicates the distance value from an object in mm
  * @param[hcsr04] Ultrasonic object
  * @retval Distance in mm
  */
float distance_mm(ultrasonic* hcsr04){
	return distance_cm(hcsr04)*10;
}

/**
  * @brief Indicates the distance value from an object in inches
  * @param[hcsr04] Ultrasonic object
  * @retval Distance in inches
  */
float distance_inch(ultrasonic* hcsr04){
	return distance_cm(hcsr04)/2.54;
}

/**
  * @brief Indicates the speed relative to the sensor direction
  * @param[hcsr04] Ultrasonic object
  * @param[time] Time interval for speed calculation in s
  * @retval Float indicating the speed at which the object approaches or moves away from the sensor
  */
float forwardSpeed(ultrasonic* hcsr04, float time){
	float x_i = distance_m(hcsr04);
	int delta_t = (int)1000*time;
	HAL_Delay(delta_t);
	float x_f = distance_m(hcsr04);
	return (x_f-x_i)/time;
}

/**
  * @brief Indicates the speed of occurrence of 2 consecutive events
  * @param[hcsr04] Ultrasonic object
  * @param[distance] Route size for speed calculation in m
  * @param[underLimit] Minimum distance value detected in cm
  * @retval Float with the detection speed value of two following events
  */
float crossSpeed(ultrasonic* hcsr04, float distance, float underLimit){
	int flag = 0;
	int cont = 0;
	while(1){
		if((distance_cm(hcsr04) > underLimit) & (flag == 0)){
			pass;
		}else if((distance_cm(hcsr04) < underLimit) & (flag == 0)){
			flag = 1;
		}else if((distance_cm(hcsr04) > underLimit) & (flag == 1)){
			cont++;
			uDelay(2);
		}else if((distance_cm(hcsr04) < underLimit) & (flag == 1)){
			break;
		}
	}
	return distance/((cont + 0.0f)*gain);
}

/**
  * @brief Indicates whether an object is within a distance range
  * @param[hcsr04] Ultrasonic object
  * @param[underLimit] Under limit of the distance range in cm
  * @param[upperLimit] Upper limit of the distance range in cm
  * @retval Boolean indicating whether the object is within the limits
  */
bool itsBetween(ultrasonic* hcsr04, float underLimit, float upperLimit){
	if((distance_cm(hcsr04) > underLimit) & (distance_cm(hcsr04) > upperLimit)){
		return true;
	}
	return false;
}
