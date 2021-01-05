EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr User 5236 4331
encoding utf-8
Sheet 1 1
Title "MF52 Example Schematic"
Date "2020-08-18"
Rev ""
Comp "ArduForge"
Comment1 "Value of R2 should match TH1's R25 value."
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Device:R R2
U 1 1 5F3BA920
P 3000 2150
F 0 "R2" V 2793 2150 50  0000 C CNN
F 1 "10k" V 2884 2150 50  0000 C CNN
F 2 "" V 2930 2150 50  0001 C CNN
F 3 "~" H 3000 2150 50  0001 C CNN
	1    3000 2150
	0    -1   -1   0   
$EndComp
$Comp
L Device:Thermistor_NTC TH1
U 1 1 5F3BBAAA
P 2100 2150
F 0 "TH1" V 1810 2150 50  0000 C CNN
F 1 "MF52 NTC Thermistor" V 1901 2150 50  0000 C CNN
F 2 "" H 2100 2200 50  0001 C CNN
F 3 "~" H 2100 2200 50  0001 C CNN
	1    2100 2150
	0    -1   -1   0   
$EndComp
$Comp
L MCU_Module:Arduino_UNO_R3 A?
U 1 1 5F3C2FF7
P 2300 1300
F 0 "A?" H 2300 19  50  0001 C CNN
F 1 "Arduino Uno" V 2300 1550 50  0000 R CNN
F 2 "Module:Arduino_UNO_R3" H 2300 1300 50  0001 C CIN
F 3 "https://www.arduino.cc/en/Main/arduinoBoardUno" H 2300 1300 50  0001 C CNN
	1    2300 1300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3400 1400 3400 2150
Wire Wire Line
	3400 2150 3150 2150
Wire Wire Line
	1900 2150 1950 2150
Wire Wire Line
	1900 1800 1900 2150
Wire Wire Line
	2650 2150 2650 2500
Wire Wire Line
	2650 2500 3700 2500
Wire Wire Line
	3700 2500 3700 650 
Wire Wire Line
	3700 650  2300 650 
Wire Wire Line
	2300 650  2300 800 
Wire Wire Line
	2250 2150 2650 2150
Connection ~ 2650 2150
Wire Wire Line
	2650 2150 2850 2150
$EndSCHEMATC
