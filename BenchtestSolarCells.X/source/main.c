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
  
  StateInit();
//  Port.E.SetPinsDigitalOut(BIT_4);
//  Port.E.ClearBits(BIT_4);
  LED1_OFF;
  LED2_OFF;
  INT16 data;
  SetLedDutyCycle(0, 500);
  while(I2c.Var.oI2cWriteIsRunning[I2C5]);  // Wait for any I2C5 write sequence to end 
  
	while(1)  //infinite loop
	{
    if ((data = Uart.GetDataByte(UART3)) > 0)
    {
      Uart.SendDataByte(UART3, (const UINT8) data);
    }
    if ((data = Uart.GetDataByte(UART6)) > 0)
    {
      Uart.SendDataByte(UART6, (const UINT8) data);
    }
    if (!Port.E.ReadBits(BIT_7))
    {
      Uart.SendDataByte(UART3, 'E');
      Uart.SendDataByte(UART6, 'E');
    }
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