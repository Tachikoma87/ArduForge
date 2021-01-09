EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr User 5197 4724
encoding utf-8
Sheet 1 1
Title "HC-SR04 wiring schematic"
Date "2021-01-09"
Rev ""
Comp "ArduForge"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Device:HC-SR04 U?
U 1 1 5FF9DBC0
P 1500 2000
F 0 "U?" H 1533 2325 50  0001 C CNN
F 1 "HC-SR04" H 1500 2200 50  0000 C CNN
F 2 "" H 1500 2200 50  0001 C CNN
F 3 "" H 1500 2200 50  0001 C CNN
	1    1500 2000
	1    0    0    -1  
$EndComp
$Comp
L MCU_Module:Arduino_Nano_v3.x A?
U 1 1 5FF9E4D3
P 3050 1750
F 0 "A?" H 3050 661 50  0001 C CNN
F 1 "Arduino Nano" H 3050 661 50  0000 C CNN
F 2 "Module:Arduino_Nano" H 3050 1750 50  0001 C CIN
F 3 "http://www.mouser.com/pdfdocs/Gravitech_Arduino_Nano3_0.pdf" H 3050 570 50  0001 C CNN
	1    3050 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 2200 1850 2200
Wire Wire Line
	1850 2200 1850 2750
Wire Wire Line
	1850 2750 3050 2750
Wire Wire Line
	3250 750  3250 650 
Wire Wire Line
	3250 650  1800 650 
Wire Wire Line
	1800 650  1800 1900
Wire Wire Line
	1800 1900 1700 1900
Wire Wire Line
	1700 2000 2250 2000
Wire Wire Line
	2250 2000 2250 1550
Wire Wire Line
	2250 1550 2550 1550
Wire Wire Line
	1700 2100 2350 2100
Wire Wire Line
	2350 2100 2350 1650
Wire Wire Line
	2350 1650 2550 1650
$EndSCHEMATC
