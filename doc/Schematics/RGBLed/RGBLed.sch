EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr User 5197 5118
encoding utf-8
Sheet 1 1
Title "RGB Led Wiring"
Date "2021-01-23"
Rev ""
Comp "ArduForge"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Device:LED_RCGB D?
U 1 1 600C87E3
P 1550 1900
F 0 "D?" H 1550 2397 50  0001 C CNN
F 1 "LED RGB" H 1550 2305 50  0000 C CNN
F 2 "" H 1550 1850 50  0001 C CNN
F 3 "~" H 1550 1850 50  0001 C CNN
	1    1550 1900
	1    0    0    -1  
$EndComp
$Comp
L MCU_Module:Arduino_Nano_v3.x A?
U 1 1 600CA41F
P 3350 1900
F 0 "A?" H 3350 811 50  0001 C CNN
F 1 "Arduino Nano" H 3350 811 50  0000 C CNN
F 2 "Module:Arduino_Nano" H 3350 1900 50  0001 C CIN
F 3 "http://www.mouser.com/pdfdocs/Gravitech_Arduino_Nano3_0.pdf" H 3350 1900 50  0001 C CNN
	1    3350 1900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 600CD54E
P 2300 1600
F 0 "R?" V 2093 1600 50  0001 C CNN
F 1 "220" V 2185 1600 50  0000 C CNN
F 2 "" V 2230 1600 50  0001 C CNN
F 3 "~" H 2300 1600 50  0001 C CNN
	1    2300 1600
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 600CE088
P 2300 1900
F 0 "R?" V 2093 1900 50  0001 C CNN
F 1 "330" V 2185 1900 50  0000 C CNN
F 2 "" V 2230 1900 50  0001 C CNN
F 3 "~" H 2300 1900 50  0001 C CNN
	1    2300 1900
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 600CE84B
P 2300 2200
F 0 "R?" V 2093 2200 50  0001 C CNN
F 1 "330" V 2185 2200 50  0000 C CNN
F 2 "" V 2230 2200 50  0001 C CNN
F 3 "~" H 2300 2200 50  0001 C CNN
	1    2300 2200
	0    1    1    0   
$EndComp
Wire Wire Line
	2850 1800 2700 1800
Wire Wire Line
	2700 1800 2700 1600
Wire Wire Line
	2700 1600 2450 1600
Wire Wire Line
	2850 1900 2450 1900
Wire Wire Line
	2150 1900 1750 1900
Wire Wire Line
	2150 1600 1750 1600
Wire Wire Line
	1750 1600 1750 1700
Wire Wire Line
	1750 2100 1750 2200
Wire Wire Line
	1750 2200 2150 2200
Wire Wire Line
	2850 2200 2450 2200
Wire Wire Line
	1350 1900 1350 2900
Wire Wire Line
	1350 2900 3350 2900
$EndSCHEMATC
