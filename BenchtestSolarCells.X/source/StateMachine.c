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

struct sAllCells sCellVoltage = {0};

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

const float kFilter = 0.1;
BOOL oSmoothData = 1;

UINT8 nSamples = 0;

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
  static UINT8 i = 0;
  static UINT8 k = 0;
  UINT8 j = 0;
  INT32 err = 0;
  UINT8 floatToByte[4] = {0};
  float fPotValue = 0;
  UINT32 meanCellRaw[16] = {0};
  
  //==================================================================
  // ADC READ
  // Check cells voltage
  //==================================================================
  if (oMatlabReady)
  {
    if (oAdcReady)
    {
      oAdcReady = 0;
      memcpy((void *) &cellVoltageRaw[0], (void *) &Adc.Var.adcReadValues[0], 64);  // sizeof(UINT32) * 16 = 64

      sCellVoltage.cells[ 8].cellRaw[nSamples] = cellVoltageRaw[ 8];
      sCellVoltage.cells[ 9].cellRaw[nSamples] = cellVoltageRaw[ 9];
      sCellVoltage.cells[10].cellRaw[nSamples] = cellVoltageRaw[10];
      sCellVoltage.cells[11].cellRaw[nSamples] = cellVoltageRaw[11];
      nSamples++;

      
      if (nSamples == N_SAMPLES)
      {
        nSamples = 0;
        
        for (i = 0; i < N_SAMPLES; i++)
        {
          meanCellRaw[ 8] += sCellVoltage.cells[ 8].cellRaw[i];
          meanCellRaw[ 9] += sCellVoltage.cells[ 9].cellRaw[i];
          meanCellRaw[10] += sCellVoltage.cells[10].cellRaw[i];
          meanCellRaw[11] += sCellVoltage.cells[11].cellRaw[i];
        }
        
        meanCellRaw[ 8] = (float) meanCellRaw[ 8] / (float) N_SAMPLES + 0.5;
        meanCellRaw[ 9] = (float) meanCellRaw[ 9] / (float) N_SAMPLES + 0.5;
        meanCellRaw[10] = (float) meanCellRaw[10] / (float) N_SAMPLES + 0.5;
        meanCellRaw[11] = (float) meanCellRaw[11] / (float) N_SAMPLES + 0.5;
        
        if (oSmoothData)  // Smoothing function
        {
          sCellVoltage.cells[ 8].cellFloat = (kFilter*sCellVoltage.cells[ 8].cellFloat) + (meanCellRaw[ 8] * VREF / 1024.0f)*(1 - kFilter);
          sCellVoltage.cells[ 9].cellFloat = (kFilter*sCellVoltage.cells[ 9].cellFloat) + (meanCellRaw[ 9] * VREF / 1024.0f)*(1 - kFilter);
          sCellVoltage.cells[10].cellFloat = (kFilter*sCellVoltage.cells[10].cellFloat) + (meanCellRaw[10] * VREF / 1024.0f)*(1 - kFilter);
          sCellVoltage.cells[11].cellFloat = (kFilter*sCellVoltage.cells[11].cellFloat) + (meanCellRaw[11] * VREF / 1024.0f)*(1 - kFilter);
        }
        else
        {
          sCellVoltage.cells[ 8].cellFloat = meanCellRaw[ 8] * VREF / 1024.0f;
          sCellVoltage.cells[ 9].cellFloat = meanCellRaw[ 9] * VREF / 1024.0f;
          sCellVoltage.cells[10].cellFloat = meanCellRaw[10] * VREF / 1024.0f;
          sCellVoltage.cells[11].cellFloat = meanCellRaw[11] * VREF / 1024.0f;
        }
        
        fPotValue = potValue;
        memcpy(floatToByte, &fPotValue, 4);

        FifoWrite(&matlabData, &floatToByte[0]);
        FifoWrite(&matlabData, &floatToByte[1]);
        FifoWrite(&matlabData, &floatToByte[2]);
        FifoWrite(&matlabData, &floatToByte[3]);

        memcpy(floatToByte, &sCellVoltage.cells[8].cellFloat, 4);
        FifoWrite(&matlabData, &floatToByte[0]);
        FifoWrite(&matlabData, &floatToByte[1]);
        FifoWrite(&matlabData, &floatToByte[2]);
        FifoWrite(&matlabData, &floatToByte[3]);

        memcpy(floatToByte, &sCellVoltage.cells[9].cellFloat, 4);
        FifoWrite(&matlabData, &floatToByte[0]);
        FifoWrite(&matlabData, &floatToByte[1]);
        FifoWrite(&matlabData, &floatToByte[2]);
        FifoWrite(&matlabData, &floatToByte[3]);

        memcpy(floatToByte, &sCellVoltage.cells[10].cellFloat, 4);
        FifoWrite(&matlabData, &floatToByte[0]);
        FifoWrite(&matlabData, &floatToByte[1]);
        FifoWrite(&matlabData, &floatToByte[2]);
        FifoWrite(&matlabData, &floatToByte[3]);

        memcpy(floatToByte, &sCellVoltage.cells[11].cellFloat, 4);
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
          SetPotAllUnits(2, potValue);
        }
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
      if (buffer.buffer[0] == 'c')
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
