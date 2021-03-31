EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr User 6299 5906
encoding utf-8
Sheet 1 1
Title "ArduForge Battery Indicator Example Schematics"
Date "2021-03-30"
Rev ""
Comp "CrossForge"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Module:Arduino_Nano_v3.x A?
U 1 1 6062C0D8
P 5150 2850
F 0 "A?" H 5150 1669 50  0001 C CNN
F 1 "Arduino Nano" H 5150 1761 50  0000 C CNN
F 2 "Module:Arduino_Nano" H 5150 2850 50  0001 C CIN
F 3 "http://www.mouser.com/pdfdocs/Gravitech_Arduino_Nano3_0.pdf" H 5150 2850 50  0001 C CNN
	1    5150 2850
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 6062EBF5
P 4150 3100
F 0 "R?" H 4220 3146 50  0001 L CNN
F 1 "47k" H 4220 3100 50  0000 L CNN
F 2 "" V 4080 3100 50  0001 C CNN
F 3 "~" H 4150 3100 50  0001 C CNN
	1    4150 3100
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 6062F427
P 4150 2550
F 0 "R?" H 4220 2596 50  0001 L CNN
F 1 "22k" H 4220 2550 50  0000 L CNN
F 2 "" V 4080 2550 50  0001 C CNN
F 3 "~" H 4150 2550 50  0001 C CNN
	1    4150 2550
	1    0    0    -1  
$EndComp
$Comp
L power:+9V #PWR?
U 1 1 6062FEE7
P 3650 2900
F 0 "#PWR?" H 3650 2750 50  0001 C CNN
F 1 "+9V" V 3665 3028 50  0000 L CNN
F 2 "" H 3650 2900 50  0001 C CNN
F 3 "" H 3650 2900 50  0001 C CNN
	1    3650 2900
	0    -1   -1   0   
$EndComp
$Comp
L power:-9V #PWR?
U 1 1 60630C63
P 3650 2750
F 0 "#PWR?" H 3650 2625 50  0001 C CNN
F 1 "-9V" V 3665 2878 50  0000 L CNN
F 2 "" H 3650 2750 50  0001 C CNN
F 3 "" H 3650 2750 50  0001 C CNN
	1    3650 2750
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3650 2750 3650 2300
Wire Wire Line
	3650 2300 4150 2300
Wire Wire Line
	4150 2300 4150 2400
Wire Wire Line
	3650 2900 3650 3250
Wire Wire Line
	3650 3250 4150 3250
Wire Wire Line
	4150 1850 4150 2300
Connection ~ 4150 2300
Wire Wire Line
	5050 1850 4150 1850
Wire Wire Line
	4150 2700 4150 2850
Wire Wire Line
	4650 2850 4150 2850
Connection ~ 4150 2850
Wire Wire Line
	4150 2850 4150 2950
$Comp
L RF_Module:ESP32-WROOM-32 U?
U 1 1 6062D1F2
P 1400 2400
F 0 "U?" H 1400 3981 50  0001 C CNN
F 1 "ESP32-WROOM-32" H 1400 3889 50  0000 C CNN
F 2 "RF_Module:ESP32-WROOM-32" H 1400 900 50  0001 C CNN
F 3 "https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32_datasheet_en.pdf" H 1100 2450 50  0001 C CNN
	1    1400 2400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 60649E96
P 2400 2500
F 0 "R?" H 2470 2546 50  0001 L CNN
F 1 "47k" H 2470 2500 50  0000 L CNN
F 2 "" V 2330 2500 50  0001 C CNN
F 3 "~" H 2400 2500 50  0001 C CNN
	1    2400 2500
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 60649E9C
P 2400 3050
F 0 "R?" H 2470 3096 50  0001 L CNN
F 1 "22k" H 2470 3050 50  0000 L CNN
F 2 "" V 2330 3050 50  0001 C CNN
F 3 "~" H 2400 3050 50  0001 C CNN
	1    2400 3050
	-1   0    0    1   
$EndComp
$Comp
L power:+9V #PWR?
U 1 1 60649EA2
P 2900 2700
F 0 "#PWR?" H 2900 2550 50  0001 C CNN
F 1 "+9V" V 2915 2828 50  0000 L CNN
F 2 "" H 2900 2700 50  0001 C CNN
F 3 "" H 2900 2700 50  0001 C CNN
	1    2900 2700
	0    1    1    0   
$EndComp
$Comp
L power:-9V #PWR?
U 1 1 60649EA8
P 2900 2850
F 0 "#PWR?" H 2900 2725 50  0001 C CNN
F 1 "-9V" V 2915 2978 50  0000 L CNN
F 2 "" H 2900 2850 50  0001 C CNN
F 3 "" H 2900 2850 50  0001 C CNN
	1    2900 2850
	0    1    1    0   
$EndComp
Wire Wire Line
	2900 2850 2900 3300
Wire Wire Line
	2900 3300 2400 3300
Wire Wire Line
	2400 3300 2400 3200
Wire Wire Line
	2900 2700 2900 2350
Wire Wire Line
	2900 2350 2400 2350
Wire Wire Line
	2400 3800 2400 3300
Connection ~ 2400 3300
Wire Wire Line
	1400 3800 2400 3800
Wire Wire Line
	2400 2750 2400 2650
Wire Wire Line
	2400 2900 2400 2750
Connection ~ 2400 2750
Wire Wire Line
	2100 2750 2400 2750
Wire Wire Line
	2100 2750 2100 3300
Wire Wire Line
	2100 3300 2000 3300
$EndSCHEMATC
