EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr User 5197 4921
encoding utf-8
Sheet 1 1
Title "HM-10 Wiring Schematic"
Date "2021-01-11"
Rev ""
Comp "ArduForge"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Module:Arduino_Nano_v3.x A?
U 1 1 5FFC3671
P 3700 1800
F 0 "A?" H 3700 711 50  0001 C CNN
F 1 "Arduino Nano" H 3700 711 50  0000 C CNN
F 2 "Module:Arduino_Nano" H 3700 1800 50  0001 C CIN
F 3 "http://www.mouser.com/pdfdocs/Gravitech_Arduino_Nano3_0.pdf" H 3700 1800 50  0001 C CNN
	1    3700 1800
	1    0    0    -1  
$EndComp
$Comp
L Device:HM-10 U?
U 1 1 5FFCB57A
P 1700 2000
F 0 "U?" H 1725 2365 50  0001 C CNN
F 1 "HM-10" H 1725 2273 50  0000 C CNN
F 2 "" H 1700 2250 50  0001 C CNN
F 3 "" H 1700 2250 50  0001 C CNN
	1    1700 2000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5FFCEA47
P 2800 1400
F 0 "R?" V 2593 1400 50  0001 C CNN
F 1 "220" V 2685 1400 50  0000 C CNN
F 2 "" V 2730 1400 50  0001 C CNN
F 3 "~" H 2800 1400 50  0001 C CNN
	1    2800 1400
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 5FFCF543
P 2350 1400
F 0 "R?" V 2143 1400 50  0001 C CNN
F 1 "470" V 2235 1400 50  0000 C CNN
F 2 "" V 2280 1400 50  0001 C CNN
F 3 "~" H 2350 1400 50  0001 C CNN
	1    2350 1400
	0    1    1    0   
$EndComp
Wire Wire Line
	2050 2000 2550 2000
Wire Wire Line
	2550 2000 2550 1400
Wire Wire Line
	2500 1400 2550 1400
Connection ~ 2550 1400
Wire Wire Line
	2550 1400 2650 1400
Wire Wire Line
	2050 2100 2650 2100
Wire Wire Line
	3700 2800 1250 2800
Wire Wire Line
	2200 1400 1250 1400
Wire Wire Line
	1100 750  3900 750 
Wire Wire Line
	3900 750  3900 800 
Wire Wire Line
	1400 1900 1250 1900
Connection ~ 1250 1900
Wire Wire Line
	1250 1900 1250 2800
Wire Wire Line
	1400 2000 1100 2000
Wire Wire Line
	1100 2000 1100 750 
Wire Wire Line
	2950 1400 3200 1400
Wire Wire Line
	3200 1500 2650 1500
Wire Wire Line
	2650 1500 2650 2100
Wire Wire Line
	2050 1900 3100 1900
Wire Wire Line
	3100 1900 3100 1600
Wire Wire Line
	3100 1600 3200 1600
Wire Wire Line
	1250 1400 1250 1900
$EndSCHEMATC
