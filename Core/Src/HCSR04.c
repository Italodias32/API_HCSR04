#include "HCSR04.h"

ultrasonic HCSR04_generate(GPIO_Port port_trig[],GPIO_Pin pin_trig,GPIO_Port port_echo[],GPIO_Pin pin_echo){
	ultrasonic hcsr04;
	hcsr04.trig_port = port_trig;
	hcsr04.trig_pin = pin_trig;
	hcsr04.echo_port = port_echo;
	hcsr04.echo_pin = pin_echo;
	return hcsr04;
}

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

float distance_cm(ultrasonic hcsr04){
	uint32_t cont = 0;

	HAL_GPIO_WritePin(hcsr04.trig_port, hcsr04.trig_pin, GPIO_PIN_RESET);
	uDelay(2);

	HAL_GPIO_WritePin(hcsr04.trig_port, &hcsr04.trig_pin, GPIO_PIN_SET);
	uDelay(10);
	HAL_GPIO_WritePin(hcsr04.trig_port, hcsr04.trig_pin, GPIO_PIN_RESET);

	while(HAL_GPIO_ReadPin(hcsr04.echo_port,hcsr04.echo_pin) == GPIO_PIN_RESET);

	while(HAL_GPIO_ReadPin(hcsr04.echo_port,hcsr04.echo_pin) == GPIO_PIN_SET)
	{
		 cont++;
		 uDelay(2);
	};

	return (cont + 0.0f)*2.8*speedSound;
}

float distance_m(ultrasonic hcsr04){
	return distance_cm(hcsr04)/100;
}

float distance_mm(ultrasonic hcsr04){
	return distance_cm(hcsr04)*10;
}

float forwardSpeed(ultrasonic hcsr04, float time){
	float x_i = distance_m(hcsr04);
	int delta_t = (int)1000*time;
	HAL_Delay(delta_t);
	float x_f = distance_m(hcsr04);
	return (x_f-x_i)/time;
}

float crossSpeed(ultrasonic hcsr04, float distance, float underLimit){
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
	return distance/((cont + 0.0f)*2.8);
}

bool itsBetween(ultrasonic hcsr04, float underLimit, float upperLimit){
	if((distance_cm(hcsr04) > underLimit) & (distance_cm(hcsr04) > upperLimit)){
		return true;
	}
	return false;
}
