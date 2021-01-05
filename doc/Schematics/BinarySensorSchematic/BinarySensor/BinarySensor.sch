EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr User 6496 5118
encoding utf-8
Sheet 1 1
Title "Binary Sensor Example Schematics"
Date ""
Rev ""
Comp "CrossForge"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Switch:SW_Push_Dual SW1
U 1 1 5F6CB2D3
P 1900 1650
F 0 "SW1" H 1900 1225 50  0000 C CNN
F 1 "Switch" H 1900 1316 50  0000 C CNN
F 2 "" H 1900 1850 50  0001 C CNN
F 3 "~" H 1900 1850 50  0001 C CNN
	1    1900 1650
	-1   0    0    1   
$EndComp
$Comp
L Device:R R1
U 1 1 5F6D06CA
P 2500 1950
F 0 "R1" H 2430 1904 50  0000 R CNN
F 1 "10k" H 2430 1995 50  0000 R CNN
F 2 "" V 2430 1950 50  0001 C CNN
F 3 "~" H 2500 1950 50  0001 C CNN
	1    2500 1950
	-1   0    0    1   
$EndComp
Wire Wire Line
	2500 1650 2500 1800
$Comp
L MCU_Module:Arduino_Nano_v3.x A1
U 1 1 5F6C991A
P 2100 2700
F 0 "A1" V 2054 1657 50  0000 R CNN
F 1 "Arduino Nano" V 2145 1657 50  0000 R CNN
F 2 "Module:Arduino_Nano" H 2100 2700 50  0001 C CIN
F 3 "http://www.mouser.com/pdfdocs/Gravitech_Arduino_Nano3_0.pdf" H 2100 2700 50  0001 C CNN
	1    2100 2700
	0    1    1    0   
$EndComp
Wire Wire Line
	2100 1650 2500 1650
Wire Wire Line
	1700 1650 1100 1650
Wire Wire Line
	2500 2200 2500 2100
Wire Wire Line
	1100 1650 1100 2050
$Comp
L MCU_Module:Arduino_Nano_v3.x A1
U 1 1 5F6FA60F
P 4850 2700
F 0 "A1" V 4804 1657 50  0000 R CNN
F 1 "Arduino Nano" V 4895 1657 50  0000 R CNN
F 2 "Module:Arduino_Nano" H 4850 2700 50  0001 C CIN
F 3 "http://www.mouser.com/pdfdocs/Gravitech_Arduino_Nano3_0.pdf" H 4850 2700 50  0001 C CNN
	1    4850 2700
	0    1    1    0   
$EndComp
$Comp
L Device:R R1
U 1 1 5F6FF221
P 5250 1450
F 0 "R1" H 5320 1496 50  0000 L CNN
F 1 "10k" H 5320 1405 50  0000 L CNN
F 2 "" V 5180 1450 50  0001 C CNN
F 3 "~" H 5250 1450 50  0001 C CNN
	1    5250 1450
	1    0    0    -1  
$EndComp
$Comp
L Sensor_Magnetic:A1301KUA-T U1
U 1 1 5F7034D7
P 4550 1300
F 0 "U1" V 4183 1300 50  0000 C CNN
F 1 "Hall Sensor" V 4274 1300 50  0000 C CNN
F 2 "" H 4550 950 50  0001 L CIN
F 3 "http://www.allegromicro.com/~/media/Files/Datasheets/A1301-2-Datasheet.ashx" H 4450 1300 50  0001 C CNN
	1    4550 1300
	0    1    1    0   
$EndComp
Wire Wire Line
	5250 1650 4550 1650
Wire Wire Line
	4550 1650 4550 1600
Wire Wire Line
	5850 2900 5900 2900
Wire Wire Line
	5900 2900 5900 1200
Wire Wire Line
	5250 1600 5250 1650
Connection ~ 5250 1650
Wire Wire Line
	5250 1300 5250 1200
Connection ~ 5250 1200
Wire Wire Line
	5250 1200 4950 1200
Wire Wire Line
	4150 1200 3850 1200
Wire Wire Line
	3850 1200 3850 2000
$Comp
L Device:LED D1
U 1 1 5F711062
P 4250 2000
F 0 "D1" H 4243 2217 50  0000 C CNN
F 1 "LED" H 4243 2126 50  0000 C CNN
F 2 "" H 4250 2000 50  0001 C CNN
F 3 "~" H 4250 2000 50  0001 C CNN
	1    4250 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 2000 4650 2000
Wire Wire Line
	5150 2000 5150 2200
Wire Wire Line
	4100 2000 3850 2000
Connection ~ 3850 2000
Wire Wire Line
	3850 2000 3850 2700
$Comp
L Device:R R2
U 1 1 5F712883
P 4800 2000
F 0 "R2" V 4593 2000 50  0000 C CNN
F 1 "220" V 4684 2000 50  0000 C CNN
F 2 "" V 4730 2000 50  0001 C CNN
F 3 "~" H 4800 2000 50  0001 C CNN
	1    4800 2000
	0    1    1    0   
$EndComp
Wire Wire Line
	4950 2000 5150 2000
$Comp
L Device:R R2
U 1 1 5F712E71
P 2050 2050
F 0 "R2" V 1843 2050 50  0000 C CNN
F 1 "220" V 1934 2050 50  0000 C CNN
F 2 "" V 1980 2050 50  0001 C CNN
F 3 "~" H 2050 2050 50  0001 C CNN
	1    2050 2050
	0    1    1    0   
$EndComp
$Comp
L Device:LED D1
U 1 1 5F713FB4
P 1550 2050
F 0 "D1" H 1543 2267 50  0000 C CNN
F 1 "LED" H 1543 2176 50  0000 C CNN
F 2 "" H 1550 2050 50  0001 C CNN
F 3 "~" H 1550 2050 50  0001 C CNN
	1    1550 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1400 2050 1100 2050
Connection ~ 1100 2050
Wire Wire Line
	1100 2050 1100 2700
Wire Wire Line
	1700 2050 1900 2050
Wire Wire Line
	2200 2050 2400 2050
Wire Wire Line
	2400 2050 2400 2200
Text Notes 3950 850  0    94   ~ 0
Example 02: Hall Sensor
Text Notes 1250 850  0    94   ~ 0
Example 01: Switch
Wire Wire Line
	5250 2200 5250 1650
Wire Wire Line
	5250 1200 5900 1200
$EndSCHEMATC
