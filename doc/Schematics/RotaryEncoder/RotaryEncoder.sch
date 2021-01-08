EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr User 5197 4724
encoding utf-8
Sheet 1 1
Title "Rotary encoder wiring schematic"
Date "2021-01-08"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Module:Arduino_Nano_v3.x A1
U 1 1 5FF89716
P 2750 1750
F 0 "A1" H 2750 661 50  0000 C CNN
F 1 "Arduino Nano" H 2750 570 50  0000 C CNN
F 2 "Module:Arduino_Nano" H 2750 1750 50  0001 C CIN
F 3 "http://www.mouser.com/pdfdocs/Gravitech_Arduino_Nano3_0.pdf" H 2750 1750 50  0001 C CNN
	1    2750 1750
	1    0    0    -1  
$EndComp
$Comp
L Device:Rotary_Encoder_KY-040 U?
U 1 1 5FF93BA2
P 1600 1350
F 0 "U?" H 1600 1650 50  0001 C CNN
F 1 "Rotary Encoder KY 040" V 1950 950 50  0000 L CNN
F 2 "" H 1600 1650 50  0001 C CNN
F 3 "" H 1600 1650 50  0001 C CNN
	1    1600 1350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2850 750  2850 600 
Wire Wire Line
	2850 600  2150 600 
Wire Wire Line
	1750 1550 2100 1550
Wire Wire Line
	2100 1550 2100 1350
Wire Wire Line
	2100 1350 2250 1350
Wire Wire Line
	1750 1450 2250 1450
Wire Wire Line
	1750 1350 1850 1350
Wire Wire Line
	1850 1350 1850 1650
Wire Wire Line
	1850 1650 2200 1650
Wire Wire Line
	2200 1650 2200 1550
Wire Wire Line
	2200 1550 2250 1550
Wire Wire Line
	1750 1250 2150 1250
Wire Wire Line
	2150 1250 2150 600 
$Comp
L power:GND #PWR?
U 1 1 5FF9C6CD
P 1800 1150
F 0 "#PWR?" H 1800 900 50  0001 C CNN
F 1 "GND" V 1805 1022 50  0000 R CNN
F 2 "" H 1800 1150 50  0001 C CNN
F 3 "" H 1800 1150 50  0001 C CNN
	1    1800 1150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1800 1150 1750 1150
$EndSCHEMATC
