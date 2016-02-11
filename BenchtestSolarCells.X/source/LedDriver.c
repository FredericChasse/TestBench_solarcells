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
   .value  = 5
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

sLedOn_t  ledOnReg [16] = {0};
sLedOff_t ledOffReg[16] = {0};

sLedOn_t ledOnReg0 = 
{
   .bytes.word = 0
  ,.regAddLow  = 6
  ,.regAddHigh = 7
};

sLedOff_t ledOffReg0 = 
{
   .bytes.word = 2047
  ,.bytes.data.dataHigh.bits.fullOff = 0
  ,.regAddLow  = 8
  ,.regAddHigh = 9
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
  UINT8 dataBuffer[6], i; 
  
  // Shutdown all LEDs
  ShutdownLedDriver();
  
  // Set registry addresses for LEDs
  for (i = 0; i < 16; i++)
  {
    ledOnReg[i]. regAddLow  = 6 + i*4;
    ledOnReg[i]. regAddHigh = ledOnReg[i].regAddLow + 1;
    ledOffReg[i].regAddLow  = ledOnReg[i].regAddLow + 2;
    ledOffReg[i].regAddHigh = ledOnReg[i].regAddLow + 3;
  }
  
  // Set operation mode
  mode1Reg.data.bits.ai = 1;
  mode1Reg.data.bits.sleep = 0;
  
  mode2Reg.data.bits.invrt = 0;
//  mode2Reg.data.bits.invrt = 1;
  mode2Reg.data.bits.och = 0;
  mode2Reg.data.bits.outdrv = 1;
//  mode2Reg.data.bits.outdrv = 0;
  mode2Reg.data.bits.outne = 0b11;
  
  ledDriver.rw = I2C_WRITE;
  dataBuffer[0] = ledDriver.byte;  // Slave address
  dataBuffer[1] = mode1Reg.regAdd; 
  dataBuffer[2] = mode1Reg.data.word; 
  dataBuffer[3] = mode2Reg.data.word; 
  
  while(I2c.Var.oI2cReadIsRunning[I2C5]);   // Wait for any I2C5 read sequence to end 
  I2c.AddDataToFifoWriteQueue(I2C5, &dataBuffer[0], 4, TRUE);
  while(I2c.Var.oI2cWriteIsRunning[I2C5]);  // Wait for any I2C5 write sequence to end 
  
  // Write OFF to all LEDs
  allLedOnReg .bytes.data.dataHigh.bits.fullOn   = 0;
  allLedOffReg.bytes.data.dataHigh.bits.fullOff  = 1;
  
  dataBuffer[1] = allLedOnReg .regAddLow;
  dataBuffer[2] = allLedOnReg .bytes.data.dataLow;
  dataBuffer[3] = allLedOnReg .bytes.data.dataHigh.word;
  dataBuffer[4] = allLedOffReg.bytes.data.dataLow;
  dataBuffer[5] = allLedOffReg.bytes.data.dataHigh.word;
  
  while(I2c.Var.oI2cReadIsRunning[I2C5]);   // Wait for any I2C5 read sequence to end 
  while(I2c.Var.oI2cWriteIsRunning[I2C5]);  // Wait for any I2C5 write sequence to end 
  I2c.AddDataToFifoWriteQueue(I2C5, &dataBuffer[0], 6, TRUE);
  
  
  // PWM frequency
  mode1Reg.data.bits.sleep = 1;
  dataBuffer[1] = mode1Reg.regAdd;
  dataBuffer[2] = mode1Reg.data.word;
  
  while(I2c.Var.oI2cReadIsRunning[I2C5]);   // Wait for any I2C5 read sequence to end 
  while(I2c.Var.oI2cWriteIsRunning[I2C5]);  // Wait for any I2C5 write sequence to end 
  I2c.AddDataToFifoWriteQueue(I2C5, &dataBuffer[0], 3, TRUE);
  
  prescaleReg.value = 5;                // PWM frequency of 1017.25 Hz
//  prescaleReg.value = 60;                // PWM frequency of 100 Hz
//  prescaleReg.value = 30;                // PWM frequency of 200 Hz
  dataBuffer[1] = prescaleReg.regAdd;
  dataBuffer[2] = prescaleReg.value;    
  
  while(I2c.Var.oI2cReadIsRunning[I2C5]);   // Wait for any I2C5 read sequence to end 
  while(I2c.Var.oI2cWriteIsRunning[I2C5]);  // Wait for any I2C5 write sequence to end 
  I2c.AddDataToFifoWriteQueue(I2C5, &dataBuffer[0], 3, TRUE);
  
  mode1Reg.data.bits.sleep = 0;
  dataBuffer[1] = mode1Reg.regAdd;
  dataBuffer[2] = mode1Reg.data.word;
  
  while(I2c.Var.oI2cReadIsRunning[I2C5]);   // Wait for any I2C5 read sequence to end 
  while(I2c.Var.oI2cWriteIsRunning[I2C5]);  // Wait for any I2C5 write sequence to end 
  I2c.AddDataToFifoWriteQueue(I2C5, &dataBuffer[0], 3, TRUE);
  
  while(I2c.Var.oI2cWriteIsRunning[I2C5]);  // Wait for any I2C5 write sequence to end
  TurnOnLedDriver();
}


/**************************************************************
 * Function name  : SetLedPwm
 * Purpose        : Set the duty cycle of one LED.
 * Arguments      : UINT8 numLed : number of the LED to set. 0 - 15
 *                  UINT16 dutyCycle : duty cycle in percentage. Precision 
 *                  of 0.1%. Ex: 50.5% = 505. Max = 1000 (100.0%).
 * Returns        : -1 on failure, 0 on success.
 *************************************************************/
inline INT8 SetLedDutyCycle (UINT8 numLed, UINT16 dutyCycle)
{
  UINT8 dataBuffer[6]; 
  UINT16 pwmValue = 0;
  
  if (numLed >= 16)
  {
    return -1;    // Max 15
  }
  if (dutyCycle >= 1000)
  {
    return -1;    // Max 100.0%
  }
  
  pwmValue = ((float) dutyCycle / 10.0f * 4096.0f / 100.0f - 1.0) + 0.5;
  
  ledDriver.rw = I2C_WRITE;
  dataBuffer[0] = ledDriver.byte;  // Slave address
  
  ledOnReg[numLed].bytes.word = 0;
  
  ledOffReg[numLed].bytes.data.dataHigh.bits.msb     = pwmValue >> 8;
  ledOffReg[numLed].bytes.data.dataLow               = pwmValue;
//  ledOffReg[numLed].bytes.data.dataHigh.bits.msb     = 0x7;
//  ledOffReg[numLed].bytes.data.dataLow               = 0xFF;
  ledOffReg[numLed].bytes.data.dataHigh.bits.fullOff = 0;
  
  // Data to send  
  dataBuffer[1] = ledOnReg[numLed]. regAddLow;
  dataBuffer[2] = ledOnReg[numLed]. bytes.data.dataLow;
  dataBuffer[3] = ledOnReg[numLed]. bytes.data.dataHigh.word;
  dataBuffer[4] = ledOffReg[numLed].bytes.data.dataLow;
  dataBuffer[5] = ledOffReg[numLed].bytes.data.dataHigh.word;
  
  while(I2c.Var.oI2cReadIsRunning[I2C5]);   // Wait for any I2C5 read sequence to end 
  while(I2c.Var.oI2cWriteIsRunning[I2C5]);  // Wait for any I2C5 write sequence to end 
  I2c.AddDataToFifoWriteQueue(I2C5, &dataBuffer[0], 6, TRUE);
  
  return 0;
}