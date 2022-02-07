#ifndef FDD3000_PARAMETERDEF_h
#pragma once
#define FDD3000_PARAMETERDEF_h

//string for check of DeviceID
#define FDD3000_DEVICETYPE               "C300"     //device type
#ifndef FDD3000_SIGMATEKID
  #define FDD3000_SIGMATEKID               "K0073"   //custom number for sigmatek devices (last 5 chars of product code)
#endif


//*************************************************************************************
//main menu
#define FDD3000_MENU_00_QUICK_SETUP         16#2000      //Menu 00
#define FDD3000_MENU_01_FREQ_REFERENCE      16#2001      //Menu 01
#define FDD3000_MENU_02_FREQ_RAMPS          16#2002      //Menu 02
#define FDD3000_MENU_03_FREQ_MONITORING     16#2003      //Menu 03
#define FDD3000_MENU_04_TOURQUE_CURRENT     16#2004      //Menu 04
#define FDD3000_MENU_05_MOTOR_CONTROL       16#2005      //Menu 05
#define FDD3000_MENU_06_SEQUENCER_CLOCK     16#2006      //Menu 06
#define FDD3000_MENU_07_ANALOG_IO           16#2007      //Menu 07
#define FDD3000_MENU_08_DIGITAL_IO          16#2008      //Menu 08
#define FDD3000_MENU_09_USER_FUNCTION1      16#2009      //Menu 09
#define FDD3000_MENU_10_STATUS_TRIPS        16#200A      //Menu 10
#define FDD3000_MENU_11_MISCELLANEOUS       16#200B      //Menu 11
#define FDD3000_MENU_12_USER_FUNCTION2      16#200C      //Menu 12
#define FDD3000_MENU_14_USER_PID            16#200E      //Menu 14
#define FDD3000_MENU_15_SI_MODULE           16#200F      //Menu 15
#define FDD3000_MENU_18_APP_MENU1           16#2012      //Menu 18
#define FDD3000_MENU_20_APP_MENU3           16#2014      //Menu 20
#define FDD3000_MENU_21_MOTOR2_Paras        16#2015      //Menu 21
#define FDD3000_MENU_22_MENU_SETUP          16#2016      //Menu 22


//*************************************************************************************
//Parameter SI-Comm module menu
#define FDD3000_SI_MENU_01                  16#2101      //SI-Menu 1.01
#define FDD3000_SI_MENU_02                  16#2102      //SI-Menu 1.02
#define FDD3000_SI_MENU_03                  16#2103      //SI-Menu 1.03
#define FDD3000_SI_MENU_04                  16#2104      //SI-Menu 1.04
#define FDD3000_SI_MENU_05                  16#2105      //SI-Menu 1.05
#define FDD3000_SI_MENU_06                  16#2106      //SI-Menu 1.06
#define FDD3000_SI_MENU_07                  16#2107      //SI-Menu 1.07
#define FDD3000_SI_MENU_09                  16#2109      //SI-Menu 1.09


//*************************************************************************************
//Parameter main menu 01
#define FDD3000_01_PARA_MAX_REF_CLAMP            6 // 01.006 Maximum Reference clamp
#define FDD3000_01_PARA_MIN_REF_CLAMP            7 // 01.007 Minimum Reference clamp


//*************************************************************************************
//Parameter main menu 02
#define FDD3000_02_PARA_ACCRATE                 11 // 02.011 Acceleration Rate 1
#define FDD3000_02_PARA_DEC_RATE                21 // 02.021 Deceleration Rate 1


//*************************************************************************************
//Parameter main menu 04
#define FDD3000_04_PARA_CURRLOOP_KPGAIN          13 // 04.013 
#define FDD3000_04_PARA_CURRLOOP_KIGAIN          14 // 04.013 


//*************************************************************************************
//Parameter main menu 05
#define FDD3000_05_PARA_MOTOR_FREQ               6 // 05.006 Motor Rated Frequency
#define FDD3000_05_PARA_MOTOR_CURR               7 // 05.007 Motor Rated Current
#define FDD3000_05_PARA_MOTOR_SPEED              8 // 05.008 Motor Rated Speed
#define FDD3000_05_PARA_MOTOR_VOLTAGE            9 // 05.009 Motor Rated Voltage
#define FDD3000_05_PARA_MOTOR_POWERFACTOR       10 // 05.010 Motor Rated Power Factor
#define FDD3000_05_PARA_AUTOTUNE                12 // 05.015 Low Frequency Voltage Boost
#define FDD3000_05_PARA_LOWFREQVOLBOOST         15 // 05.015 Low Frequency Voltage Boost
               
#define FDD3000_05_PARA_STATOR_RESISTANCE       17 // 05.017 OpenLoop / RFC-A Stator Resistance 
#define FDD3000_05_PARA_TRANSIENT_INDUCTANCE    24 // 05.024 OpenLoop / RFC-A Transient Inductance 
#define FDD3000_05_PARA_STATOR_INDUCTANCE       25 // 05.025 OpenLoop / RFC-A Stator Inductance 
               
#define FDD3000_05_PARA_MAX_DEADTIME_COMP       59 // 05.059 OpenLoop Maximum Deadtime compensation
#define FDD3000_05_PARA_CURRENT_MAX_DEADTIME    60 // 05.060 OpenLoop Current At Maximum Deadtime Compensation 
               
#define FDD3000_05_PARA_SAT_BREAKPOINT1         29 // 05.029 RFC-A motor saturation breakpoints 1 Pr
#define FDD3000_05_PARA_SAT_BREAKPOINT3         30 // 05.030 RFC-A motor saturation breakpoints 1 Pr
#define FDD3000_05_PARA_SAT_BREAKPOINT2         62 // 05.062 RFC-A motor saturation breakpoints 1 Pr
#define FDD3000_05_PARA_SAT_BREAKPOINT4         63 // 05.063 RFC-A motor saturation breakpoints 1 Pr

//*************************************************************************************
//Parameter main menu 06
#define FDD3000_06_CONTROLWORD_ENABLE           43 // 06.043  Control Word Enable

//*************************************************************************************
//Parameter main menu 10
#define FDD3000_10_PARA_TRIP_0                  20 // 10.020  Trip 0
#define FDD3000_10_PARA_AUTO_RESET              36 // 10.036  Auto-reset Hold Drive Healthy
#define FDD3000_10_PARA_RESET_DRIVE             38 // 10.038 Reset Drive (with 100 = RESET)
#define FDD3000_10_PARA_RESET_DRIVE_DATA       100 // 100 = RESET if written to Para 10.038


//*************************************************************************************
//Parameter main menu 11
#define FDD3000_11_PARA_FW_VERSION              29 // 11.029  Firmware Version Drive ww.xx.yy.zz

#define FDD3000_11_PARA_SETDRIVE_OP_MODE        31 // 11.031  User Drive Mode (Openloop / RFC-A)

#define FDD3000_11_PARA_SERIALNR_LS             52 // 11.052 Serial Number LS
#define FDD3000_11_PARA_SERIALNR_MS             53 // 11.053 Serial Number MS

#define FDD3000_11_PARA_PRODUCT_IDENTIFIER      64 // 11.064 AAAA Product Identifier Characters ()
#define FDD3000_11_PARA_FRAME_SIZE_VOLTAGE      65 // 11.065 nnn Frame size and voltage code ()
#define FDD3000_11_PARA_DRIVE_CURRENT           68 // 11.068 ppppp Drive current rating ()
#define FDD3000_11_PARA_ADD_IDENTIFIER1         91 // 11.091 CCCC Additional Identifier Characters 1 ()
#define FDD3000_11_PARA_ADD_IDENTIFIER2         92 // 11.092 DDEE Additional Identifier Characters 2 ()
#define FDD3000_11_PARA_ADD_IDENTIFIER3         93 // 11.093 FFFB Additional Identifier Characters 3 ()

#define FDD3000_11_PARA_DRIVE_OP_MODE           84 // 11.084  Drive Mode (Openloop / RFC-A)


//*************************************************************************************
//Parameter main menu 15
#define FDD3000_15_PARA_FW_VERSION               2 // 15.002  Firmware Version Comm Module ww.xx.yy.zz
#define FDD3000_15_PARA_HW_VERSION               3 // 15.003  Hardware Version Comm Module ww.xx


//*************************************************************************************
//Parameter SI-Comm menu 1.01
#define FDD3000_101_PARA_TIMEOUT_DELAY          10 // 01.01.010  Timeout Delay
#define FDD3000_101_PARA_TIMEOUT_ACTION         11 // 01.01.011  Timeout Action
#define FDD3000_101_PARA_CIAPROFILE_ENABLE      20 // 01.01.020  CiA402 profiles Enable

       
//defines min-max values
//******************************************************************************
#define FDD3000_PARA_MAXIMUM_SPEED_FREQ              55000
#define FDD3000_PARA_MAXIMUM_ACC_DEC_RATE           320000
#define FDD3000_PARA_MAXIMUM_SPEED_RPM              330000
#define FDD3000_PARA_MAXIMUM_VOLTAGE                   765
#define FDD3000_PARA_MAXIMUM_POWERFACTOR               100
#define FDD3000_PARA_MAXIMUM_FREQVOLTAGEBOOST          250
#define FDD3000_PARA_MAXIMUM_STATORRESISTANCE       999999
#define FDD3000_PARA_MAXIMUM_TRANSIENTINDUCTANCE    500000
#define FDD3000_PARA_MAXIMUM_STATORINDUCTANCE       500000
#define FDD3000_PARA_MAXIMUM_KPGAIN                 400000
#define FDD3000_PARA_MAXIMUM_KIGAIN                 600000
#define FDD3000_PARA_MAXIMUM_SATURATIONBREAKP         1000


#endif