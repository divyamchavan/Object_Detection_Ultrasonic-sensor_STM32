# Object_Detection_Ultrasonic-sensor_STM32

__Greetings Everyone__

__Goal__: In this project I am trying to measure the distance of the object from the ultrasonic sensor and display the distance on an 16*2 Lcd using I2C communication.

__Ultrasonic Sensor__ : HC-SR04 module - It can detects objects in the range 2cm to 400cm.

__Pin connections__

|HC-SR04|STM32F401RE|Description|
|-------|-------|----|
|VCC|5V|3.3v ~ 5V|
|Trigger|PA5|Triggering Input Pin. 10uS TTL Pulses|
|Echo|PA6|TTL Logic Output Pin. Proportional to distance|
|GND|GND pin|

__Working__  
•	Echo pin is connected to GPIO port pin which is configured as input.  
•	Trig pin is connected to GPIO port pin which is configured as output. A pulse of 10uS is sent to the trigger pin to send out 8-cycles burst of ultrasound at 40KHz and raise the echo pin. 
•	If there are any objects in the range of sensor the sound is reflected back and sensed by the sensor. This puts the echo pin to low.  
•	The time (T) between the sending and receiving of ultrasound wave gives the time taken by the wave to travel to and forth.  
•	A timer configured in input capture mode and edge triggered (both falling and rising) is used. Timer value is captured when the signal is sent(T1) and received (T2)  
•	Hence, Time taken(T) = T2 – T1.  
•	Distance = (T / 2) * sound velocity (340m/s). Time is halved to consider only one way travel.  


__Timing Diagaram__ 

![US_timing](https://github.com/divyamchavan/Object_Detection_Ultrasonic-sensor_STM32/assets/121415464/3fe6febb-84ac-42e2-82d9-6cef09b44cc4)

__Displaying the distance on LCD using I2C protocol__  

__LCD__ : 1602 LCD with piggyback I2C Module PCF8574T
![LCD_I2C](https://github.com/divyamchavan/Object_Detection_Ultrasonic-sensor_STM32/assets/121415464/010043a8-317b-42b2-b696-5d5d4de9a750)

|I2C Module|STM32F401RE
|-----|------|
|GND|GND|
|VCC|VCC|
|SCL|PB6|
|SDA|PB7





 
