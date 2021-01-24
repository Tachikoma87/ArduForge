# ArduForge

ArduForge is a collection of C++ classes for various common, popular devices used within the Arduino environment. All classes feature extensive in-code documentation, examples, and wiring schematics. I hope this will make it easy to understand how to use the classes and devices. Due to the thorough documentation, this project may also be suitable for teaching electronics to novice users in electronics or students in general.

# List of devices ready to use
* BatteryIndicatorNiMH - Measure voltage and remaining charge of a battery
* BinarySensor - debounced binary sensor such as a switch or hall sensor
* CC1101 - popular mid-range ISM-band radio transceiver
* HCSR04 - cheap ultrasonic distance measurement device for a distance up to 4 meter
* MF52 - cheap and popular thermistor
* MPU6050 - very common 6-axis (3 DOF acceleration, 3 DOF rotation) inertial measurement unit (IMU)
* RGBLed - Lightweight class to control an RGB Led with common cathode
* RotaryEncoder - tracks the rotation of a shaft and usually comes with a switch

# Documentation
The online documentation can be accessed at https://crossforge.github.io/ArduForge/doc/html/index.html

# Examples
All examples use the serial port with a speed setting of 115200 baud.

# Project structure
The *./lib* directory contains finished classes ready to use as well as examples and the wiring schematics. The *./src* directory contains the working files and prototypes not yet tested or finished. 
