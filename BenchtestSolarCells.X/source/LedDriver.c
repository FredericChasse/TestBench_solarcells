//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook Project Template
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : SourceTemplate.c
// Author  : Frederic Chasse
// Date    : 2015-01-03
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This is the C file for the functions of the LED driver PCA9685.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : None.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "..\headers\LedDriver.h"


//==============================================================================
// Macro definitions
//==============================================================================


//==============================================================================
// Private functions prototypes
//==============================================================================


//==============================================================================
// Variable definitions
//==============================================================================
I2C_7_BIT_ADDRESS ledDriver = 
{
  .address  = 0b1000000
 ,.rw       = 0b0
};

sMode1Reg_t mode1Reg =
{
   .data    = {0}
  ,.regAdd  =  0
};

sMode2Reg_t mode2Reg =
{
   .data    = {0}
  ,.regAdd  =  1
};

sPreScaleReg_t prescaleReg = 
{
   .value  = 3
  ,.regAdd = 254
};

sLedOn_t allLedOnReg = 
{
   .bytes.word = 0
  ,.regAddLow  = 250
  ,.regAddHigh = 251
};

sLedOff_t allLedOffReg = 
{
   .bytes.word = 2047
  ,.bytes.data.dataHigh.bits.fullOff = 1
  ,.regAddLow  = 252
  ,.regAddHigh = 253
};

//==============================================================================
// Functions
//==============================================================================

/**************************************************************
 * Function name  : ShutdownLedDriver
 * Purpose        : Shutdown all LEDs at once.
 * Arguments      : None.
 * Returns        : None.
 *************************************************************/
inline void ShutdownLedDriver (void)
{
  Port.D.SetBits(BIT_0);
}


/**************************************************************
 * Function name  : TurnOnLedDriver
 * Purpose        : Turn on all LEDs at once.
 * Arguments      : None.
 * Returns        : 0 on success, -1 on failure.
 *************************************************************/
inline void TurnOnLedDriver (void)
{
  Port.D.ClearBits(BIT_0);    // Must be adjusted
}


/**************************************************************
 * Function name  : InitLedDriver
 * Purpose        : Initialize all registers with LEDs at 0.
 * Arguments      : None.
 * Returns        : None.
 *************************************************************/
inline void InitLedDriver (void)
{
  UINT8 dataBuffer[5]; 
  
  // Shutdown all LEDs
  ShutdownLedDriver();
  
  // Set operation mode
  mode1Reg.data.bits.ai = 1;
  mode1Reg.data.bits.sleep = 0;
  
  mode2Reg.data.bits.invrt = 0;
  mode2Reg.data.bits.och = 1;
  mode2Reg.data.bits.outdrv = 1;
  mode2Reg.data.bits.outne = 0b11;
  
  dataBuffer[0] = mode1Reg.regAdd; 
  dataBuffer[1] = mode1Reg.data.word; 
  dataBuffer[2] = mode2Reg.data.word; 
  
  while(I2c.Var.oI2cReadIsRunning[I2C5]);   // Wait for any I2C5 read sequence to end 
  I2c.AddDataToFifoWriteQueue(I2C5, &dataBuffer[0], 3, TRUE);
  
  // Write OFF to all LEDs
  allLedOnReg .bytes.data.dataHigh.bits.fullOn   = 0;
  allLedOffReg.bytes.data.dataHigh.bits.fullOff  = 1;
  
  dataBuffer[0] = allLedOnReg .regAddLow;
  dataBuffer[1] = allLedOnReg .bytes.data.dataLow;
  dataBuffer[2] = allLedOnReg .bytes.data.dataHigh.word;
  dataBuffer[3] = allLedOffReg.bytes.data.dataLow;
  dataBuffer[4] = allLedOffReg.bytes.data.dataHigh.word;
  
  while(I2c.Var.oI2cReadIsRunning[I2C5]);   // Wait for any I2C5 read sequence to end 
  while(I2c.Var.oI2cWriteIsRunning[I2C5]);  // Wait for any I2C5 write sequence to end 
  I2c.AddDataToFifoWriteQueue(I2C5, &dataBuffer[0], 5, TRUE);
  
  while(I2c.Var.oI2cWriteIsRunning[I2C5]);  // Wait for any I2C5 write sequence to end
  TurnOnLedDriver();
}