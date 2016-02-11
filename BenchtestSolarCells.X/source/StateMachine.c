//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook Project Template
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : StateMachine.c
// Author  : Frederic Chasse
// Date    : 2015-02-25
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This is the C file for the state machine of the system.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : State names can and should be renamed by the user to improve the
//           readability of the code.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "..\headers\StateMachine.h"


extern volatile BOOL oAdcReady;

BOOL oSendData = 0;

UINT32 cellVoltage[16] = {0};

sUartFifoBuffer_t matlabData = 
{
   .bufEmpty   = 1
  ,.bufFull    = 0
  ,.inIdx      = 0
  ,.outIdx     = 0
  ,.lineBuffer = {0}
  ,.maxBufSize = UART_LINE_BUFFER_LENGTH
};

UINT8 potValue = 0;

//==============================================================================
//	STATES OF STATE MACHINE
//==============================================================================

//===============================================================
// Name     : StateScheduler
// Purpose  : Decides which state is next depending on current
//            state and flags. Used as a function
//===============================================================
void StateScheduler(void)
{
  // Clear WDT
  //============================
  Wdt.Clear();
  //============================

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Current state == StateInit
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  if (pState == &StateInit)
  {
    if (INIT_2_ACQ)
    {
      pState = &StateAcq;       // First state
    }
    else if (INIT_2_SEND_DATA)
    {
      pState = &StateSendData;       // Second state
    }
    else if (INIT_2_ERROR)
    {
      pState = &StateError;   // Error state
    }
    else
    {
      pState = &StateError;   // Go to Error state by default
    }
  }
  /*
   * DEVELOPPER CODE HERE
   */

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Current state == StateAcq
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  else if (pState == &StateAcq)
  {
    if (ACQ_2_ACQ)
    {
      pState = &StateAcq;       // First state
    }
    else if (ACQ_2_SEND_DATA)
    {
      pState = &StateSendData;       // Second state
    }
    else if (ACQ_2_ERROR)
    {
      pState = &StateError;   // Error state
    }
    else
    {
      pState = &StateError;   // Go to Error state by default
    }
  }
  /*
   * DEVELOPPER CODE HERE
   */

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Current state == State2
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  else if (pState == &StateSendData)
  {
    if (SEND_DATA_2_ACQ)
    {
      pState = &StateAcq;       // First state
    }
    else if (SEND_DATA_2_SEND_DATA)
    {
      pState = &StateSendData;       // Second state
    }
    else if (SEND_DATA_2_ERROR)
    {
      pState = &StateError;   // Error state
    }
    else
    {
      pState = &StateError;   // Go to Error state by default
    }
  }
  /*
   * DEVELOPPER CODE HERE
   */

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Current state == StateError
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  else if (pState == &StateError)
  {
    if (ERROR_2_ACQ)
    {
      pState = &StateAcq;       // First state
    }
    else if (ERROR_2_SEND_DATA)
    {
      pState = &StateSendData;       // Second state
    }
    else if (ERROR_2_ERROR)
    {
      pState = &StateError;   // Error state
    }
    else
    {
      pState = &StateError;   // Go to Error state by default
    }
  }
  /*
   * DEVELOPPER CODE HERE
   */

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Current state undetermined
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  else
  {
    pState = &StateError;   // Go to Error state by default
  }
  /*
   * DEVELOPPER CODE HERE
   */

}


//===============================================================
// Name     : StateInit
// Purpose  : Initialization of the system.
//===============================================================
void StateInit(void)
{

  INTDisableInterrupts();   // Disable all interrupts of the system.

  INIT_PORTS;
//  INIT_TIMER;
//  INIT_ADC;
  INIT_UART;
  INIT_SKADI;
  INIT_I2C;
  INIT_SPI;
//  INIT_WDT;
  
  START_INTERRUPTS;
  
//  // Init digital potentiometers AD8403
//  InitPot(0);
//  InitPot(1);
//  InitPot(2);
  InitPot(3);
  potValue = 0;
  SetPot(3, 0, potValue);
  SetPot(3, 1, potValue);
  SetPot(3, 2, potValue);
  SetPot(3, 3, potValue);
//  
  // Init LED driver PCA9685
  InitLedDriver();
//  ShutdownLedDriver();
  SetLedDutyCycle(12, 200);

}


//===============================================================
// Name     : StateAcq
// Purpose  : Check for received data
//===============================================================
void StateAcq(void)
{
  //==================================================================
  // VARIABLE DECLARATIONS
  //==================================================================

  //==================================================================
  // ADC READ
  // Check cells voltage
  //==================================================================
  if (oAdcReady)
  {
    oAdcReady = 0;
    memcpy(cellVoltage, (void *) &Adc.Var.adcReadValues[0], sizeof(UINT32) * 16);
    FifoWrite(&matlabData, &cellVoltage[12]);
    if (matlabData.lineBuffer.length >= MATLAB_PACKET_SIZE)
    {
      oSendData = 1;
    }
    
    if (potValue == 255)
    {
      LED1_ON;
      LED2_ON;
      while(1);
    }
    potValue++;
    SetPot(3, 0, potValue);
  }
  
  //==================================================================
  // Check for skadi messages or button changes
  //==================================================================
  Skadi.GetCmdMsgFifo();
  
  AssessButtons();

  //==================================================================
  // SECOND PART OF STATE
  // Developper should add a small description of expected behavior
  //==================================================================
//  sUartLineBuffer_t  matlabData = {
//                                    .buffer = {0}
//                                   ,.length =  0
//                                  }
//                                  ;
//  
//  INT32 err = 0;
//  
//  if (Uart.Var.oIsRxDataAvailable[UART6])                 // Check if RX interrupt occured
//  {
//    err = Uart.GetRxFifoBuffer(UART6, &uart6Data, FALSE); // put received data in uart6Data
//    if (err >= 0)                                         // If no error occured
//    {
//      /* Do something */
//      Uart.PutTxFifoBuffer(UART6, &uart6Data);            // Put data received in TX FIFO buffer
//    }
//  }

}


//===============================================================
// Name     : StateSendData
// Purpose  : Send data on UART.
//===============================================================
void StateSendData(void)
{
  oSendData = 0;
  Uart.PutTxFifoBuffer(U_MATLAB, &matlabData);
}


//===============================================================
// Name     : StateCompute
// Purpose  : Compute algorithm.
//===============================================================
void StateCompute(void)
{
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // VARIABLE DECLARATIONS
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  
  
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // FIRST PART OF STATE
  // Developper should add a small description of expected behavior
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  
  
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // SECOND PART OF STATE
  // Developper should add a small description of expected behavior
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  

}


//===============================================================
// Name     : StateError
// Purpose  : Error state of the system. Used to assess and
//            correct errors in the system.
//===============================================================
void StateError(void)
{
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // VARIABLE DECLARATIONS
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  /*
   * DEVELOPPER CODE HERE
   */

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // FIRST PART OF STATE
  // Developper should add a small description of expected behavior
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  /*
   * DEVELOPPER CODE HERE
   */

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // SECOND PART OF STATE
  // Developper should add a small description of expected behavior
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  /*
   * DEVELOPPER CODE HERE
   */

}
