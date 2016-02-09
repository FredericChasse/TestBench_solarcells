//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook Project Template
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : main.c
// Author  : Frederic Chasse
// Date    : 2015-01-03
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This is the main C file of the template project used by all
//           developpers of Chinook. It uses ChinookLib, which is another
//           repository on Github.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : For ChinookLib to be useable, it must be cloned on your hard
//           drive so the path
//               "..\..\..\ChinookLib\ChinookLib.X\headers\ChinookLib.h"
//           references an existing file.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


#include "..\headers\Setup.h"
#include "..\headers\SkadiFunctions.h"
#include "..\headers\Interrupts.h"
#include "..\headers\StateMachine.h"
#include "..\headers\HardwareProfile.h"


//==============================================================================
// FUNCTION PROTOTYPES
//==============================================================================


//==============================================================================
// VARIABLE DECLARATIONS
//==============================================================================


//==============================================================================
// MAIN CODE
//==============================================================================
void main(void)
{

//==============================================================================
// The next line disables the JTAG for the PIC. If the JTAG is enabled, pins
// RB10-13 can't be used as I/Os. If you want to use the JTAG, comment or
// remove this line.
//==============================================================================
  DDPCONbits.JTAGEN = 0;
//==============================================================================


//==============================================================================
// Configure the device for maximum performance but do not change the PBDIV
// Given the options, this function will change the flash wait states, RAM
// wait state and enable prefetch cache but will not change the PBDIV.
// The PBDIV value is already set via the pragma FPBDIV option in HardwareProfile.h.
//==============================================================================
  SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
//==============================================================================


// State machine init
//=================================================================
  if (Wdt.ReadEvent())    // If a system reset has occured
  {
    Wdt.Disable();        // Disable WDT and
    Wdt.ClearEvent();     // the WDT event bit
    pState = &StateError; // Go to StateError
  }
  else
  {
    pState = &StateInit;  // Else, initialize the system as usual
  }
//=================================================================
  
//  StateInit();
//  Port.E.SetPinsDigitalOut(BIT_4);
//  Port.E.ClearBits(BIT_4);
//  INT16 data;
//  UINT8 i = 0;
//  INT8 err = 0;
//  while(I2c.Var.oI2cWriteIsRunning[I2C5]);  // Wait for any I2C5 write sequence to end 
//  err = SetLedDutyCycle(11, 500);
//  ShutdownLedDriver();
  
//  for (i = 0; i < 16; i++)
//  {
//    if (i != 7)
//    {
//      while(I2c.Var.oI2cWriteIsRunning[I2C5]);  // Wait for any I2C5 write sequence to end 
//      err = SetLedDutyCycle(i, 750);
//      if (err < 0)
//      {
//        LED1_ON;
//      }
//    }
//  }
  
//  float sin[2][15] = { {0 , .4189 , .8378 , 1.2566 , 1.6755 , 2.0944 , 2.5133 , 2.9322 , 3.3510 , 3.7699 , 4.1888 , 4.6077 , 5.0265 , 5.4454 , 5.8643} ,
//                       {0 , .4067 , .7431 , .9511  , .9945  , .8660  , .5878  , .2079  , -.2079 , -.5878 , -.8660 , -.9945 , -.9511 , -.7431 , -.4067} };
//  
//  sUartLineBuffer_t buffer = 
//  { 
//     .buffer = {0} 
//    ,.length =  0 
//  };
//  
//  memcpy(buffer.buffer, sin, 30*4);
//  buffer.length = 30*4;
  
	while(1)  //infinite loop
	{
//    Skadi.GetCmdMsgFifo();
//    if (!SW1)
//    {
//      Uart.PutTxFifoBuffer(U_MATLAB, &buffer);
//      while(!SW1);
//      for (i = 0; i < 15; i++)
//      {
//        sin[0][i] += 2*PI;
//        memcpy(buffer.buffer, sin, 120);
//        buffer.length = 120;
//      }
//    }
//    if ((data = Uart.GetDataByte(UART3)) > 0)
//    {
//      Uart.SendDataByte(UART3, (const UINT8) data);
//    }
//    if ((data = Uart.GetDataByte(UART6)) > 0)
//    {
//      Uart.SendDataByte(UART6, (const UINT8) data);
//    }
//    if (!Port.E.ReadBits(BIT_7))
//    {
//      Uart.SendDataByte(UART3, 'E');
//      Uart.SendDataByte(UART6, 'E');
//    }
//    LED1_TOGGLE;
//    LED2_TOGGLE;
//    Timer.DelayMs(500);
//  // State machine entry & exit point
//  //===========================================================
//		(*pState)();          // jump to next state
//  //===========================================================
//
//  // State scheduler
//  //===========================================================
//    StateScheduler();     // Decides which state will be next
//	//===========================================================
	}
} //END MAIN CODE