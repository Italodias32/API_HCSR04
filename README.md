# API_HCSR04

This is a HC-SR04 API for STM32 using STM32F103RB. 

Developed by Italo Dias and Sarah Oliveira

<div align="center"><img src="https://user-images.githubusercontent.com/38631264/108587390-05ee6280-7332-11eb-86ab-a64ab0032239.jpeg" width="450"></div>

## Introduction
This is an API developed for the HC-SR04 sensor, the work was developed on the Nucleo Board STM32f103RBT6. The API is ready for use on other Boards in the family, however the user must change the HCSR04.h file, placing the include relative to the board used.

This API was developed during the Embedded Systems Programming course, taught by Prof. Ricardo Duarte, at the Federal University of Minas Gerais in February 2021. All API development was done with assistance.


```c
#define microTIM TIM4
```

The code is organized into two folders. The first one, Library, contains the files HCSR04.h and HCSR04.c. The second one, Example, contains an example developed using the OLED Display, based on Alexandre Lutsai's function library, available at this link: [stm32libs](https://github.com/SL-RU/stm32libs).


## API Fucctions
- ### ultrasonic HCSR04_generate(GPIO_Port port_trig[],GPIO_Pin pin_trig,GPIO_Port port_echo[],GPIO_Pin pin_echo);

This library creates the HCSR04 sensor module. As parameters, are passed the port type and the port number of Trig and Echo, in this order. Then, the function returns a structure variable of type ultrasonic, which contains the necessary parameters for using the library.

- ### void uDelay(uint32_t microSec);
This function creates a time delay in microseconds using the Timer chosen by the programmer for the API. Its use is necessary to validate the other functions of the code.
The HAL_Delay () function receives integer parameters corresponding to the time in milliseconds. However, to work with smaller units, it may be necessary to use the uDelay function, since the sensor works with values ​​in the microsecond range.
The Function is based on the method used in the following reference, the credits relating to the creation of the method belonging to the author:
[Tutorial 37 - HC-SR05 Ultrasonic Sensor](https://github.com/MYaqoobEmbedded/STM32-Tutorials/tree/master/Tutorial%2037%20-%20HC-SR05%20Ultrasonic%20Sensor)

- ### void adjustment(float gainFactor);
This function changes the global gain variation, in order to obtain greater measurement accuracy, and the user can configure this parameter with a potentiometer, for example.

- ### float distance_cm(ultrasonic* hcsr04);
This function receives the value read by the ultrasonic sensor and based on this value it executes the measurement routine. First, the Trig has a high pulse for a period of 10us, to trigger the system. Then, the sensor sends 8 pulses of 25us and the code waits for the Echo value to return. When the Echo is high, a counter is started using the uDelay() function. The function returns the distance by multiplying the time obtained from the Echo pulse by the speed of sound divided by 2.

- ### float distance_m(ultrasonic* hcsr04);
This function receives the value read by the ultrasonic sensor, and in order to obtain the distance between an object and the sensor in meters, divides the return of the distance_cm function by 100.

- ### float distance_mm(ultrasonic* hcsr04);
This function receives the value read by the ultrasonic sensor, and in order to obtain the distance between an object and the sensor in millimeters, multiplies by 10 the return of the distance_cm function.

- ### float distance_inch(ultrasonic* hcsr04);
This function receives the value read by the ultrasonic sensor, and in order to obtain the distance between an object and the sensor in inches, divides by 2.54 the return of the distance_cm function.

- ### float forwardSpeed(ultrasonic* hcsr04, float time);
This function calculates the instantaneous speed of an object that approaches or moves away from the sensor. First, it receives the parameter hcsr04 read by the ultrasonic sensor, a time value that can be understood as a time distance. The Sensor reads the distance at the first instant, waits for a time equal to the time variable, then it reads the distance value again. By calculating the difference between the positions and dividing the result by time, an estimate of the instantaneous speed in meters per second is obtained.
In addition to calculating speed, the function would be useful, for example, in control systems, however it is advisable to use different boards or MCUs for the controller and instrumentation, mainly for the use of this function, which causes a pause in the process, due delay.

- ### float crossSpeed(ultrasonic* hcsr04, float distance, float underLimit);
This function calculates the speed at which an object crosses the sensor twice in a row. First the object crosses the sensor, starting a counter, then the object crosses the sensor again, paralyzing the counter. The time difference is taken and, based on the distance of the route, the speed is obtained.
An application example for this function would be to run on circular tracks, such as athletics tracks. Initially the corridor crosses the sensor, initializing the counter, and soon after, when crossing the sensor again, the function returns the average speed of the corridor in the course.

- ### bool itsBetween(ultrasonic* hcsr04, float underLimit, float upperLimit);
This function receives the value read by the ultrasonic sensor, hcsr04, a lower distance value and a higher value. Then, it returns true if the value is within these limits or false, otherwise.

<div align="center"><img src="https://user-images.githubusercontent.com/38631264/108588036-dccfd100-7335-11eb-977a-b0776d8ae880.gif" width=600></div>

## Example
In the main.c file include the HCSR04.h file
```c
#include "HCSR04.h"
```
Initialize the sensor
```c
ultrasonic hcsr04 = HCSR04_generate(GPIOA, GPIO_PIN_9, GPIOA, GPIO_PIN_8);
```
Use the distance value to turn on a led
```c
 distance = distance_cm(&hcsr04);
 if(distance < 10){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,GPIO_PIN_SET);
	HAL_Delay(5000);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,GPIO_PIN_RESET);
 }
```
## References
[Tutorial 37 - HC-SR05 Ultrasonic Sensor](https://github.com/MYaqoobEmbedded/STM32-Tutorials/tree/master/Tutorial%2037%20-%20HC-SR05%20Ultrasonic%20Sensor) - by: MYaqoobEmbedded and Kefirr Grzegorz

[LCD_20x4](https://github.com/elisabacelar/LCD_20x4) - by: elisabacelar and Bloq96

[Display_API](https://github.com/vcaitite/Display_API) - by: vcaitite and AmandaARTeixeira

[stm32libs](https://github.com/SL-RU/stm32libs) - by: Alexander Lutsai - SL-RU
