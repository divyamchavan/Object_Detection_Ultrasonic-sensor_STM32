# Object_Detection_Ultrasonic-sensor_STM32

__Greetings Everyone__

__Goal__: In this project I am trying to measure the distance of the object from the ultrasonic sensor and display the distance on an 16*2 Lcd using I2C communication.

__Ultrasonic Sensor__ : HC-SR04 module - It can detects objects in the range 2cm to 400cm.

__Pin connections__

|HC-SR04|STM32F401RE|Description|
|-------|-------|
|VCC|5V|3.3v ~ 5V|
|Trigger|PA5|Triggering Input Pin. 10uS TTL Pulses|
|Echo|PA6|TTL Logic Output Pin. Proportional to distance|
|GND|GND pin|

![US_timing](https://github.com/divyamchavan/Object_Detection_Ultrasonic-sensor_STM32/assets/121415464/3fe6febb-84ac-42e2-82d9-6cef09b44cc4)

 
