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

BOOL oSendData = 0;

BOOL oMatlabReady = 0;

UINT32 cellVoltageRaw [16] = {0};
float  cellVoltageReal[16] = {0};

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

float potValues[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

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
  
  SetPot(2, 0, potValue);
  SetPot(2, 1, potValue);
  SetPot(2, 2, potValue);
  SetPot(2, 3, potValue);
//  SetPot(3, 0, potValue);
//  SetPot(3, 1, potValue);
//  SetPot(3, 2, potValue);
//  SetPot(3, 3, potValue);
//  ShutdownPot(3);
  
  // Init LED driver PCA9685
  InitLedDriver();
//  ShutdownLedDriver();
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
  static UINT8 i = 0;
  static UINT8 k = 0;
  UINT8 j = 0;
  INT32 err = 0;
  UINT8 floatToByte[4] = {0};
  float fPotValue = 0;
  //==================================================================
  // VARIABLE DECLARATIONS
  //==================================================================
  
//  if (oTimer3Ready)
//  {
//    oTimer3Ready = 0;
//    
//    if (oMatlabReady)
//    {
////      memcpy(buffer.buffer, sinus, 120);
////      buffer.length = 120;
////
////      Uart.PutTxFifoBuffer(U_MATLAB, &buffer);
////      for (i = 0; i < 15; i++)
////      {
////        sinus[0][i] += 2*PI;
////      }
//    
//      memcpy(floatToByte, &sinus[k][i], 4);
//      FifoWrite(&matlabData, &floatToByte[0]);
//      FifoWrite(&matlabData, &floatToByte[1]);
//      FifoWrite(&matlabData, &floatToByte[2]);
//      FifoWrite(&matlabData, &floatToByte[3]);
//      if (i < 14)
//      {
//        i++;
//      }
//      else
//      {
//        if (k < 1)
//        {
//          k++;
//          i = 0;
//        }
//        else
//        {
//          for (j = 0; j < 15; j++)
//          {
//            sinus[0][j] += 15;
////            sinus[0][j] += 2*PI;
//          }
//          i = 0;
//          k = 0;
//        }
//      }
//
//      if (matlabData.lineBuffer.length >= MATLAB_PACKET_SIZE)
//      {
////        buffer.length = 0;
//        oSendData = 1;
////        for (j = 0; j < MATLAB_PACKET_SIZE; j++)
////        {
////          FifoRead(&matlabData, &buffer.buffer[j]);
////          buffer.length++;
////        }
////
////        Uart.PutTxFifoBuffer(U_MATLAB, &buffer);
//        i = 0;
//        k = 0;
//      }
//    }
//  }

  //==================================================================
  // ADC READ
  // Check cells voltage
  //==================================================================
  if (oMatlabReady)
  {
    if (oAdcReady)
    {
      oAdcReady = 0;
//      memcpy((void *) &cellVoltageRaw[0], (void *) &Adc.Var.adcReadValues[0], sizeof(UINT32) * 16);
      memcpy((void *) &cellVoltageRaw[0], (void *) &Adc.Var.adcReadValues[0], 64);  // sizeof(UINT32) * 16 = 64
  //    cellVoltageRaw[12] = Adc.Var.adcReadValues[12];

      cellVoltageReal[ 8] = (cellVoltageRaw[ 8] + 1) * VREF / 1024.0f;
      cellVoltageReal[ 9] = (cellVoltageRaw[ 9] + 1) * VREF / 1024.0f;
      cellVoltageReal[10] = (cellVoltageRaw[10] + 1) * VREF / 1024.0f;
      cellVoltageReal[11] = (cellVoltageRaw[11] + 1) * VREF / 1024.0f;
//      cellVoltageReal[12] = (cellVoltageRaw[12] + 1) * VREF / 1024.0f;
  //    for (i = 0; i < 16; i++)
  //    {
  //      cellVoltageReal[i] = (cellVoltageRaw[i] + 1) * VREF / 1024.0f;
  //    }

      
      fPotValue = potValue;
      memcpy(floatToByte, &fPotValue, 4);
      
      FifoWrite(&matlabData, &floatToByte[0]);
      FifoWrite(&matlabData, &floatToByte[1]);
      FifoWrite(&matlabData, &floatToByte[2]);
      FifoWrite(&matlabData, &floatToByte[3]);
      
//      memcpy(floatToByte, &cellVoltageReal[12], 4);
      memcpy(floatToByte, &cellVoltageReal[8], 4);
      FifoWrite(&matlabData, &floatToByte[0]);
      FifoWrite(&matlabData, &floatToByte[1]);
      FifoWrite(&matlabData, &floatToByte[2]);
      FifoWrite(&matlabData, &floatToByte[3]);
      
      memcpy(floatToByte, &cellVoltageReal[9], 4);
      FifoWrite(&matlabData, &floatToByte[0]);
      FifoWrite(&matlabData, &floatToByte[1]);
      FifoWrite(&matlabData, &floatToByte[2]);
      FifoWrite(&matlabData, &floatToByte[3]);
      
      memcpy(floatToByte, &cellVoltageReal[10], 4);
      FifoWrite(&matlabData, &floatToByte[0]);
      FifoWrite(&matlabData, &floatToByte[1]);
      FifoWrite(&matlabData, &floatToByte[2]);
      FifoWrite(&matlabData, &floatToByte[3]);
      
      memcpy(floatToByte, &cellVoltageReal[11], 4);
      FifoWrite(&matlabData, &floatToByte[0]);
      FifoWrite(&matlabData, &floatToByte[1]);
      FifoWrite(&matlabData, &floatToByte[2]);
      FifoWrite(&matlabData, &floatToByte[3]);
      

      if (matlabData.lineBuffer.length >= MATLAB_PACKET_SIZE)
      {
        oSendData = 1;
      }

      if (potValue < 255)
      {
        potValue++;
//        SetPot(3, 0, potValue);
        SetPot(2, 0, potValue);
        SetPot(2, 1, potValue);
        SetPot(2, 2, potValue);
        SetPot(2, 3, potValue);
      }
    }
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
  
  if (Uart.Var.oIsRxDataAvailable[UART6])                 // Check if RX interrupt occured
  {
    err = Uart.GetRxFifoBuffer(UART6, &buffer, FALSE); // put received data in uart6Data
    if (err >= 0)                                         // If no error occured
    {
      if (buffer.buffer[0] == 'g')
      {
        oMatlabReady = 1;
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
