# API_HCSR04

This is an HC-SR04 API for STM32 using STM32F103RB and developed in STMCubeIDE1.3.0

Developed by Italo Dias and Sarah Oliveira

<div align="center"><img src="https://user-images.githubusercontent.com/38631264/108587390-05ee6280-7332-11eb-86ab-a64ab0032239.jpeg" width="450"></div>

## Introduction
This is an API developed for the HC-SR04 sensor, the work was developed on the Nucleo Board STM32f103RBT6. The API is ready for use on other Boards in the family, however the user must change the HCSR04.h file, placing the include relative to the board used.

This API was developed during the Embedded Systems Programming course, taught by Prof. Ricardo Duarte, at the Federal University of Minas Gerais in February 2021. All API development was done with assistance.

About the API, it is important to comment that it was developed using Timer TIM4, internal clock of the STM32F103RB Core. If it is necessary to change the Timer or set a new clock, change it in: HCSR04.h. The frequency was set at 64MHz. To change it, make the gain adjustment to suit the new frequency, it can be done manually in the code of the HCSR04.c file or at run time, using the "adjustment (float gainFactor)" function.

```c
#define microTIM TIM4
```

The code is organized into two folders. The first one, Library, contains the files HCSR04.h and HCSR04.c. The second one, Example, contains an example developed using the OLED Display, based on Alexandre Lutsai's function library, available at this link: [stm32libs](https://github.com/SL-RU/stm32libs). In example available, another important observation is about the Trig and Echo pins, which were defined in the application as PA9 and PA8, respectively. However, they can be changed, as long as they are digital. Trig was defined as output and Echo as input.

## API Fucctions
- ### ultrasonic HCSR04_generate(GPIO_Port port_trig[],GPIO_Pin pin_trig,GPIO_Port port_echo[],GPIO_Pin pin_echo);

This function creates the HCSR04 sensor module. As parameters, are passed the port type and the port number of Trig and Echo, in this order. Then, the function returns a structure variable of type ultrasonic, which contains the necessary parameters for using the library.

- ### void uDelay(uint32_t microSec);
This function creates a time delay in microseconds using the Timer chosen by the programmer for the API. Its use is necessary to validate the other functions of the code.
The HAL_Delay() function receives integer parameters corresponding to the time in milliseconds. However, to work with smaller units, it may be necessary to use the uDelay function, since the sensor works with values ​​in the microsecond range.
The Function is based on the method used in the following reference, the credits relating to the creation of the method belonging to the author:
[Tutorial 37 - HC-SR05 Ultrasonic Sensor](https://github.com/MYaqoobEmbedded/STM32-Tutorials/tree/master/Tutorial%2037%20-%20HC-SR05%20Ultrasonic%20Sensor)

- ### void adjustment(float gainFactor);
This function changes the global gain variation, in order to obtain greater measurement accuracy, and the user can configure this parameter with a potentiometer, for example.

- ### float distance_cm(ultrasonic* hcsr04);
This function receives the value read by the ultrasonic sensor and based on this value it executes the measurement routine. First, the Trig has a high pulse for a period of 10us, to trigger the system. Then, the sensor sends 8 pulses of 40kHz and the code waits for the Echo value to return. When the Echo is high, a counter is started using the uDelay() function. The function returns the distance by multiplying the time obtained from the Echo pulse by the speed of sound divided by 2.

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
1. Start a project Select a trigger pin as an output (in this case GPIOA - GPIO_PIN_9) and an echo pin as an input (in this case GPIOA - GPIO_PIN_8), a third output pin is chosen and connected to an LED (in this case GPIOA - GPIO_PIN_6).

2. Set the clock to the desired frequency (in this case TIM4 at 64MHz).

3. Include the HCSR04.h and HCSR04.c files in the Inc and Src folders, respectively.

<br/>
<div align="center"><img src="https://user-images.githubusercontent.com/38631264/108608116-813c2c80-73a3-11eb-8825-de269c9e06a9.png" width=450></div>
<br/>

4. In the main.c file include the HCSR04.h file:
```c
#include "HCSR04.h"
```
5. Initialize the sensor:
```c
ultrasonic hcsr04 = HCSR04_generate(GPIOA, GPIO_PIN_9, GPIOA, GPIO_PIN_8);
```
6. Use the distance value to turn on a led:
```c
while(1){
 	distance = distance_cm(&hcsr04);
 	if(distance < 10){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,GPIO_PIN_SET);
		HAL_Delay(1000);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,GPIO_PIN_RESET);
 	}
 }
```
7. Test the application:
<br/>
<div align="center"><img src="https://user-images.githubusercontent.com/38631264/108607643-569ca480-73a0-11eb-9e34-5deca4364a70.gif" width=600></div>
<br/>
 

## References
[Tutorial 37 - HC-SR05 Ultrasonic Sensor](https://github.com/MYaqoobEmbedded/STM32-Tutorials/tree/master/Tutorial%2037%20-%20HC-SR05%20Ultrasonic%20Sensor) - by: MYaqoobEmbedded and Kefirr Grzegorz

[LCD_20x4](https://github.com/elisabacelar/LCD_20x4) - by: elisabacelar and Bloq96

[Display_API](https://github.com/vcaitite/Display_API) - by: vcaitite and AmandaARTeixeira

[stm32libs](https://github.com/SL-RU/stm32libs) - by: Alexander Lutsai - SL-RU
