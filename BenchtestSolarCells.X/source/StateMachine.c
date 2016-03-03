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


extern volatile BOOL   oAdcReady
                      ,oTimer3Ready
                      ;

BOOL  oSendData       = 0
     ,oMatlabReady    = 0
     ,oCaracMode      = 0
     ,oPsoMode        = 0
     ,oMultiUnitMode  = 0
     ;

UINT32 cellVoltageRaw [16] = {0};

struct sAllCells sCellValues = {0};

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

const float kFilter = 0.1;
BOOL oSmoothData = 1;

UINT16 nSamples = 0;

extern sUartLineBuffer_t buffer;

extern float sinus[2][15];
//float sinus[2][15] = { {0 , .4189 , .8378 , 1.2566 , 1.6755 , 2.0944 , 2.5133 , 2.9322 , 3.3510 , 3.7699 , 4.1888 , 4.6077 , 5.0265 , 5.4454 , 5.8643} ,
//                       {0 , .4067 , .7431 , .9511  , .9945  , .8660  , .5878  , .2079  , -.2079 , -.5878 , -.8660 , -.9945 , -.9511 , -.7431 , -.4067} };
//float sinx[32] = {0, 0.19509, 0.38268, 0.55557, 0.70711, 0.83147, 0.92388, 0.98079, 1, 0.98079, 0.92388
//                 ,0.83147, 0.70711, 0.55557, 0.38268, 0.19509, 0, -0.19509, -0.38268, -0.55557, -0.70711, -0.83147
//                 ,-0.92388, -0.98079, -1, -0.98079, -0.92388, -0.83147, -0.70711, -0.55557, -0.38268, -0.19509
//                 };

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
  INIT_TIMER;
  INIT_ADC;
  INIT_UART;
  INIT_SKADI;
  INIT_I2C;
  INIT_SPI;
  INIT_WDT;
  
  START_INTERRUPTS;
  
//  // Init digital potentiometers AD8403
//  InitPot(0);
//  InitPot(1);
  InitPot(2);
//  InitPot(3);
  potValue = 0;
  
  SetPotAllUnits(2, potValue);
  
  // Init LED driver PCA9685
  InitLedDriver();
  
  SetLedDutyCycle(12, 200);
  SetLedDutyCycle(13, 200);
  SetLedDutyCycle(14, 200);
  SetLedDutyCycle(15, 200);

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
  INT32 err = 0;
  UINT8 matlabBuffer[100];
  float fPotValue;
  
  //==================================================================
  // ADC READ
  // Check cells voltage
  //==================================================================
  if (oMatlabReady)
  {
    if (oAdcReady)
    {
      oAdcReady = 0;
      
      GetAdcValues();

      if (nSamples >= N_SAMPLES_PER_ADC_READ)
      {
        nSamples -= N_SAMPLES_PER_ADC_READ;
        
        ComputeMeanAdcValues();
        
        fPotValue = potValue;
        memcpy(&matlabBuffer[ 0], &fPotValue, 4);
        memcpy(&matlabBuffer[ 4], &sCellValues.cells[ 8].cellVoltFloat, 4);
        memcpy(&matlabBuffer[ 8], &sCellValues.cells[ 9].cellVoltFloat, 4);
        memcpy(&matlabBuffer[12], &sCellValues.cells[10].cellVoltFloat, 4);
        memcpy(&matlabBuffer[16], &sCellValues.cells[11].cellVoltFloat, 4);
        AddDataToMatlabFifo(matlabBuffer, 20);
        
        if (potValue < 255)
        {
          potValue++;
          SetPotAllUnits(2, potValue);
        }
      }
    }
  }
  
  //==================================================================
  // Check for skadi messages, U_MATLAB msgs or button changes
  //==================================================================
  Skadi.GetCmdMsgFifo();
  
  AssessButtons();

  if (Uart.Var.oIsRxDataAvailable[UART6])                 // Check if RX interrupt occured
  {
    err = Uart.GetRxFifoBuffer(UART6, &buffer, FALSE); // put received data in uart6Data
    if (err >= 0)                                         // If no error occured
    {
      if (buffer.buffer[0] == 'c')
      {
        oMatlabReady = 1;
        oCaracMode   = 1;
      }
      else if (buffer.buffer[0] == 'p')
      {
        oMatlabReady   = 1;
        oMultiUnitMode = 1;
      }
      else if (buffer.buffer[0] == 'm')
      {
        oMatlabReady = 1;
        oPsoMode     = 1;
      }
      else if (buffer.buffer[0] == 's')
      {
        oMatlabReady = 0;
      }
    }
  }
}


//===============================================================
// Name     : StateSendData
// Purpose  : Send data on UART.
//===============================================================
void StateSendData(void)
{
  oSendData = 0;
  sUartLineBuffer_t localBuffer = 
  { 
     .buffer = {0} 
    ,.length =  0 
  };
  
  UINT32 i = 0;
  
  for (i = 0; i < MATLAB_PACKET_SIZE; i++)
  {
    FifoRead(&matlabData, &localBuffer.buffer[i]);
    localBuffer.length++;
  }
  
  Uart.PutTxFifoBuffer(U_MATLAB, &localBuffer);
  
  if (potValue == 255)
  {
    LED1_ON;
    LED2_ON;
    while(1);
  }
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
}


//===============================================================
// Name     : StateError
// Purpose  : Error state of the system. Used to assess and
//            correct errors in the system.
//===============================================================
void StateError(void)
{
  LED1_ON;
  LED2_OFF;
  
  while(1)
  {
    LED1_TOGGLE;
    LED2_TOGGLE;
    
    Timer.DelayMs(500);
  }
}
