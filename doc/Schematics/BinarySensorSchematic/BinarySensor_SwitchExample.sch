EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr User 5315 4331
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Switch:SW_Push_Dual SW1
U 1 1 5F6CB2D3
P 2600 1000
F 0 "SW1" H 2600 575 50  0000 C CNN
F 1 "Switch" H 2600 666 50  0000 C CNN
F 2 "" H 2600 1200 50  0001 C CNN
F 3 "~" H 2600 1200 50  0001 C CNN
	1    2600 1000
	-1   0    0    1   
$EndComp
$Comp
L Device:R R1
U 1 1 5F6D06CA
P 3200 1200
F 0 "R1" H 3130 1154 50  0000 R CNN
F 1 "10k" H 3130 1245 50  0000 R CNN
F 2 "" V 3130 1200 50  0001 C CNN
F 3 "~" H 3200 1200 50  0001 C CNN
	1    3200 1200
	-1   0    0    1   
$EndComp
Wire Wire Line
	3200 1000 3200 1050
$Comp
L MCU_Module:Arduino_Nano_v3.x A1
U 1 1 5F6C991A
P 2800 1950
F 0 "A1" V 2754 907 50  0000 R CNN
F 1 "Arduino Nano" V 2845 907 50  0000 R CNN
F 2 "Module:Arduino_Nano" H 2800 1950 50  0001 C CIN
F 3 "http://www.mouser.com/pdfdocs/Gravitech_Arduino_Nano3_0.pdf" H 2800 1950 50  0001 C CNN
	1    2800 1950
	0    1    1    0   
$EndComp
Wire Wire Line
	2800 1000 3200 1000
Wire Wire Line
	2400 1000 1800 1000
Wire Wire Line
	3200 1450 3200 1350
Wire Wire Line
	1800 1000 1800 1950
$EndSCHEMATC
