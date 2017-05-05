## Boat controller

The boat controller receives input from the radio and the accelerometer/gyroscope to control two rudders and two motor controllers. It also saves data in a file in CSV format on a microSD card formatted in FAT16.

<insert diagram>

<insert BoatController photos>

#### Components

|**name**|**description**|
|:------------:|:-------------------:|
|PIC24FJ128GB202|16-bit microcontroller|
|LD1117V3|5V to 3V3 linear converter|
|MPU6050|accerelometer/gyroscope (not present on the photo)|

#### Power

The card is powered by a 2000mAh Lithium-Ion battery that provides 3.7V. This is used to provide 3.3V to the microcontroller and the radio receptor, and 5V to the accelerometer and servos that control rudders.
The power switch is present on another part of the boat, easily accessible from the outside of the boat. This switch is connected to the power connector (jumper connecting two pins on the photo above).

#### LED indicator

The green LED is connected to pin RB13 of the microcontroller. During initialisation, the LED is turned on. If the initialisation fails, the LED blinks at a fast rate (5 times a second). If the initialisation is successful, the LED blinks at slow rate: on during 100ms, off during 1.9s.
