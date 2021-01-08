EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr User 5197 5118
encoding utf-8
Sheet 1 1
Title "MPU6050 wiring schematic"
Date "2021-01-08"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Sensor_Motion:MPU-6050 U1
U 1 1 5FF8C799
P 3650 2050
F 0 "U1" H 3650 1261 50  0001 C CNN
F 1 "MPU-6050" H 3650 1261 50  0000 C CNN
F 2 "Sensor_Motion:InvenSense_QFN-24_4x4mm_P0.5mm" H 3650 1250 50  0001 C CNN
F 3 "https://store.invensense.com/datasheets/invensense/MPU-6050_DataSheet_V3%204.pdf" H 3650 1900 50  0001 C CNN
	1    3650 2050
	1    0    0    -1  
$EndComp
$Comp
L MCU_Module:Arduino_Nano_v3.x A1
U 1 1 5FF8E62D
P 1750 1850
F 0 "A1" H 1750 761 50  0001 C CNN
F 1 "Arduino Nano" H 1750 761 50  0000 C CNN
F 2 "Module:Arduino_Nano" H 1750 1850 50  0001 C CIN
F 3 "http://www.mouser.com/pdfdocs/Gravitech_Arduino_Nano3_0.pdf" H 1750 1850 50  0001 C CNN
	1    1750 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	1850 2850 1850 3000
Wire Wire Line
	1850 3000 3650 3000
Wire Wire Line
	3650 3000 3650 2750
Wire Wire Line
	1850 850  1850 750 
Wire Wire Line
	1850 750  3750 750 
Wire Wire Line
	3750 750  3750 1350
Wire Wire Line
	2950 1850 2600 1850
Wire Wire Line
	2600 1850 2600 2350
Wire Wire Line
	2600 2350 2250 2350
Wire Wire Line
	2950 1750 2500 1750
Wire Wire Line
	2500 1750 2500 2250
Wire Wire Line
	2500 2250 2250 2250
$EndSCHEMATC
