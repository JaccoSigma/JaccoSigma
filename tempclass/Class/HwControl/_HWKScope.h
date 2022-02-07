#ifndef _HWKSCOPE_H
#define _HWKSCOPE_H

// NewInst Command Range 0x8420-0x843F
#define HWKSCOPE_CMD_CONFIG_ANALOG_DATA_BUFFER    0x8420
#define HWKSCOPE_CMD_CONFIG_ANALOG_DATA_TRIGGER   0x8421
#define HWKSCOPE_CMD_CONFIG_ANALOG_DATA_TIME      0x8422
#define HWKSCOPE_CMD_STARTSTOP_ANALOG_DATA_BUFFER 0x8423
#define HWKSCOPE_CMD_GET_ANALOG_DATA_STATE        0x8424
#define HWKSCOPE_CMD_GET_ANALOG_DATA_BUFFER       0x8425  // This Command is equal to #define SDIAS_CLT_GET_ANALOG_DATA_BUFFER    0x8370
#define HWKSCOPE_CMD_RESETCONFIGURATION           0x8426


//+++++++++++++++++++++++++++++++++++++
// #define HWKSCOPE_CMD_CONFIG_ANALOG_DATA_BUFFER     0x8420
// Configures a single channel of the scope

      //aPara[0] = version of command
      //Parameter of Version 1:
      //aPara[1] = Channel Number 1 = First channel ...
      //aPara[2] = Channel Type,  (1 = Drive Parameter)
      //aPara[3] = Channel ID     (For _DriveScope: Drive Parameter ID that is measured with this channel.)
      //aPara[4] = Axis Place     If "Axis Place" >= 0, "Axis Place" and "Axis No" must equal the clients "Place" and "AxisNo" of the "_DriveAxis" class
      //aPara[5] = Axis No        If "Axis Place" < 0, "Axis No" is a sequential number per axis (= Dias Drive Tool selection)
      
      // result of version 1
      //result.aData[0x0]       = version
      
#define HWKSCOPE_CHNLTYPE_DRIVEPARA 1

//+++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++
// #define HWKSCOPE_CMD_CONFIG_ANALOG_DATA_TRIGGER    0x8421
// Configures the trigger of the scope.

      //aPara[0] = version of command
      //Parameter of Version 1:
      //aPara[1] = TriggerChannel Number: 0 = Deativated, 1 = first channel, 2 = second channel, 3 = third channel
      //aPara[2] = Trigger type: 1 Rising edge, 2 falling edge, 3 rising or falling edge (3 not supoorted by _DriveScope)
      //aPara[3] = Trigger level
      //aPara[4] = Number of values to be recorded after the trigger
      
      // result of version 1
      //result.aData[0x0]       = version   

  Type      
	    e_HWKSCOPE_Edge :
	    (
	      Rising:=1,
	      Falling:=2,
	      Both:=3
	    )$USINT;      

  END_TYPE
//+++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++
// #define HWKSCOPE_CMD_CONFIG_ANALOG_DATA_TIME       0x8422
// Configuration of the scope time settings

      //aPara[0] = version of command
      //Parameter of Version 1:
      //aPara[1] = Sample rate [0.1*us]
      //aPara[2] = No of samples
      
      //Parameter of Version 2:
      //aPara[1] = Sample rate [0.1*us]
      //aPara[2] = No of samples      
      //aPara[3] = Prescaler
      
      
      // result of version 1
      //result.aData[0x0]       = version
      //result.aData[0x1]$DINT  = Sample rate [0.1*us] confirmation
      //result.aData[0x5]$DINT  = No of samples confirmation                               

      // result of version 2
      //result.aData[0x0]       = version
      //result.aData[0x1]$DINT  = Sample rate [0.1*us] confirmation
      //result.aData[0x5]$DINT  = No of samples confirmation                               

//+++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++
// #define HWKSCOPE_CMD_STARTSTOP_ANALOG_DATA_BUFFER  0x8423
// Start/Stops the scope measurment

      //aPara[0] = version of command
      //Parameter of Version 1:
      //aPara[1] = : 0 = Stop recording, 1 Start recording
      
      // result of version 1
      //result.aData[0x0]       = version
      //result.aData[0x1]       = :  0 = Confirmation Stop recording, 1 Confirmation Start recording
      //result.aData[0x2]       = Reason why start/stop was not accepted:
                               // 0 .. start/stop was accepted
                               // 1 .. Recording already running
                               // 2 .. Invalid configuration
                               // 3 .. Drive offline
                               // 4 .. Busy cannot start now, try again later

//+++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++
// #define HWKSCOPE_CMD_GET_ANALOG_DATA_STATE         0x8424
// Gets the state of the scope HWK

      //aPara[0] = version of command
      //Parameter of Version 1:
      
      //retcode = error => command not supported
      //retcode = ready => result is filled with data
      // result of version 1
      //result.aData[0x0]       = version
      //result.aData[0x1]$SINT  = state:      
                              // -1 .. No communication with drive
                              //  0 .. No data available (Stop, Default state)
                              //  1 .. Start of recording failed 
                              //  2 .. Recording is beeing configured
                              //  3 .. Recording was startet (not yet triggered)
                              //  4 .. Recording in progress
                              //  5 .. Data is being uploaded (portions can be retrieved)
                              //  6 .. Data is completely uploaded 
                              //  7 .. Recording is beeing stopped
                              
  Type      
	    e_HWKSCOPE_State :
	    (
	      _Offline:=-1,
	      _NoData:=0,
	      _StartFailed:=1,
        _Config:=2,
        _RecordingStarted:=3,
        _RecordingTriggered:=4,
        _DataUpload:=5,
        _DataValid:=6,
        _Stopping:=7
	    )$DINT;

  END_TYPE                              
      
//+++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++
// #define HWKSCOPE_CMD_GET_ANALOG_DATA_BUFFER       0x8425
// #define SDIAS_CLT_GET_ANALOG_DATA_BUFFER          0x8370
// Gets the measurment data from the scope HWK

      //aPara[0] = version of command
      //Parameter of Version 1:
      //aPara[1] = Channel Number, 1 = first channel, 2 = second channel ... (mandatory)
      //aPara[2] = pointer to buffer where the data should be copied (can be NIL)
      //aPara[3] = size of buffer (value is ignored if aPara[2]=NIL)
      
      //retcode = error => command not supported
      //retcode = ready => result is filled with data
      // result of version 1
      //result.aData[0x0]       = version
      //result.aData[0x1]       = size of one value in byte
      //result.aData[0x2]$UDINT = max avaliable values per cycle, 0 for asynchronous measurements
      //result.aData[0x6]$UDINT = number of values copied to buffer
      //result.aData[0xA]       = datacount avaliable => 0= no counter, 1 = counter avaliable
      //result.aData[0xB]$UDINT = new data counter ( always 0 if not avaliable)
//+++++++++++++++++++++++++++++++++++++


//+++++++++++++++++++++++++++++++++++++
// #define HWKSCOPE_CMD_RESETCONFIGURATION           0x8426
// Resets the configuration of the scope.

      //aPara[0] = version of command
      //Parameter of Version 1:      
      
      //retcode = error => command not supported or reset of configuration is not possible
      //retcode = ready => reset of configuration was done
      // result of version 1
      //result.aData[0x0]       = version
      
#endif
