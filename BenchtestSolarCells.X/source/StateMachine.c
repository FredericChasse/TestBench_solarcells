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

extern UINT8 matlabPacketSize;

BOOL  oSendData       = 0
     ,oNewSample      = 0
     ,oErrorFlag      = 0
     ,oMatlabReady    = 0
     ,oCaracMode      = 1   // Caracterization by default
     ,oCaracDone      = 1
     ,oPsoMode        = 1
     ,oPsoDone        = 1
     ,oMultiUnitMode  = 0
     ,oMultiUnitDone  = 0
     ;

UINT16 dutyCycle = 500;

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

UINT8 potIndexValue[16] = {0};

BOOL oSmoothData = 1;

UINT16 nSamples = 0;

extern sUartLineBuffer_t buffer;

extern UINT32 iteration;

extern sPsoValues_t psoValues;
extern sMultiUnitValues_t multiUnitValues;

extern float sinus[2][15];

float testFloat = 0;
UINT32 testUint32 = 0;
INT32 testInt32 = 0;
UINT8 testUint8 = 0;

BOOL oFirstTimeCallFromMatlab = 1;

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
      pState = &StateAcq;
    }
    else if (INIT_2_SEND_DATA)
    {
      pState = &StateSendData;
    }
    else if (INIT_2_ERROR)
    {
      pState = &StateError;
    }
    else if (INIT_2_COMP)
    {
      pState = &StateCompute;
    }
    else
    {
      pState = &StateError;   // Go to Error state by default
    }
  }

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Current state == StateAcq
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  else if (pState == &StateAcq)
  {
    if (ACQ_2_COMP)
    {
      pState = &StateCompute;
    }
    else if (ACQ_2_SEND_DATA)
    {
      pState = &StateSendData;
    }
    else if (ACQ_2_ACQ)
    {
      pState = &StateAcq;
    }
    else if (ACQ_2_ERROR)
    {
      pState = &StateError;
    }
    else
    {
      pState = &StateError;   // Go to Error state by default
    }
  }

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Current state == StateSendData
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  else if (pState == &StateSendData)
  {
    if (SEND_DATA_2_ACQ)
    {
      pState = &StateAcq;
    }
    else if (SEND_DATA_2_SEND_DATA)
    {
      pState = &StateSendData;
    }
    else if (SEND_DATA_2_ERROR)
    {
      pState = &StateError;
    }
    else if (SEND_DATA_2_COMP)
    {
      pState = &StateCompute;
    }
    else
    {
      pState = &StateError;   // Go to Error state by default
    }
  }

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Current state == StateError
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  else if (pState == &StateError)
  {
    if (ERROR_2_ACQ)
    {
      pState = &StateAcq;
    }
    else if (ERROR_2_SEND_DATA)
    {
      pState = &StateSendData;
    }
    else if (ERROR_2_COMP)
    {
      pState = &StateCompute;
    }
    else if (ERROR_2_ERROR)
    {
      pState = &StateError;
    }
    else
    {
      pState = &StateError;   // Go to Error state by default
    }
  }

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Current state == StateCompute
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  else if (pState == &StateCompute)
  {
    if (COMP_2_SEND_DATA)
    {
      pState = &StateSendData;
    }
    else if (COMP_2_ACQ)
    {
      pState = &StateAcq;
    }
    else if (COMP_2_COMP)
    {
      pState = &StateCompute;
    }
    else if (COMP_2_ERROR)
    {
      pState = &StateError;
    }
    else
    {
      pState = &StateError;   // Go to Error state by default
    }
  }

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Current state undetermined
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  else
  {
    pState = &StateError;   // Go to Error state by default
  }

}


//===============================================================
// Name     : StateInit
// Purpose  : Initialization of the system.
//===============================================================
void StateInit(void)
{
  UINT8 i = 0;

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
  
//  InitRandomValue();
  
//  // Init digital potentiometers AD8403
//  InitPot(0);
//  InitPot(1);
  InitPot(2);
  InitPot(3);
  for (i = 0; i < 16; i++)
  {
    potIndexValue[i] = 0;
  }
  
  SetPotAllUnits(2, potIndexValue[0]);
  SetPotAllUnits(3, potIndexValue[0]);
  
  // Init LED driver PCA9685
  InitLedDriver();
  
  InitRandomValue();
  
//  UINT16 dutyCycle = 400;
  
  SetLedDutyCycle( 0, dutyCycle);
  SetLedDutyCycle( 1, dutyCycle);
  SetLedDutyCycle( 2, dutyCycle);
  SetLedDutyCycle( 3, dutyCycle);
  
//  SetLedDutyCycle( 4, dutyCycle);
//  SetLedDutyCycle( 5, dutyCycle);
//  SetLedDutyCycle( 6, dutyCycle);
//  SetLedDutyCycle( 7, dutyCycle);
  
//  SetLedDutyCycle( 8, dutyCycle);
//  SetLedDutyCycle( 9, dutyCycle);
//  SetLedDutyCycle(10, dutyCycle);
//  SetLedDutyCycle(11, dutyCycle);
  
  SetLedDutyCycle(12, dutyCycle);
  SetLedDutyCycle(13, dutyCycle);
  SetLedDutyCycle(14, dutyCycle);
  SetLedDutyCycle(15, dutyCycle);

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
  UINT32 i = 0, j = 0;
  
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
      
      nSamples++;
      oNewSample = 1;   // Go to stateCompute
    }
  }
  
  //==================================================================
  // Check for skadi messages, U_MATLAB msgs or button changes
  //==================================================================
  Skadi.GetCmdMsgFifo();
  
  AssessButtons();

  if (Uart.Var.oIsRxDataAvailable[UART6])                 // Check if RX interrupt occured
  {
    err = Uart.GetRxFifoBuffer(UART6, &buffer, FALSE);    // put received data in uart6Data
    if (err >= 0)                                         // If no error occured
    {
      if (oFirstTimeCallFromMatlab)
      {
        oFirstTimeCallFromMatlab = 0;
        InitRandomValue();
      }
      if      (buffer.buffer[0] == 'c')       // Caracterization mode
      {
        oMatlabReady    = 1;
        
        oSmoothData     = 1;
        
        oMultiUnitMode  = 0;
        oPsoMode        = 0;
        oCaracMode      = 1;
        
        oCaracDone      = 0;
        oMultiUnitDone  = 1;
        oPsoDone        = 1;
        
        nSamples        = 0;
        
        SetPotInitialCondition();
        
        oAdcReady       = 0;
      }
      else if ( (buffer.buffer[0] == 'p') || (buffer.buffer[0] == 'P') )      // PSO mode
      {
        oMatlabReady    = 1;
        
        oSmoothData     = 0;
        
        oMultiUnitMode  = 0;
        oCaracMode      = 0;
        oPsoMode        = 1;
        
        oCaracDone      = 1;
        oMultiUnitDone  = 1;
        oPsoDone        = 0;
        
        nSamples        = 0;
        
//        InitRandomValue();
        
        SetPotInitialCondition();
        
        oAdcReady       = 0;
        
        if (buffer.buffer[0] == 'P')
        {
          psoValues.oDoPerturb = 1;
        }
        else
        {
          psoValues.oDoPerturb = 0;
        }
      }
      else if ( (buffer.buffer[0] == 'm') || (buffer.buffer[0] == 'M') )       // Multi-Unit mode
      {
        LED1_OFF;
        LED2_OFF;
        
        oMatlabReady    = 1;
        
        oSmoothData     = 0;
        
        oPsoMode        = 0;
        oMultiUnitMode  = 1;
        oCaracMode      = 0;
        
        oCaracDone      = 1;
        oMultiUnitDone  = 0;
        oPsoDone        = 1;
        
        nSamples        = 0;
        
        SetPotInitialCondition();
        
        oAdcReady       = 0;
        
        if (buffer.buffer[0] == 'M')
        {
          multiUnitValues.oDoPerturb = 1;
        }
        else
        {
          multiUnitValues.oDoPerturb = 0;
        }
      }
      else if (buffer.buffer[0] == 's')       // Stop current mode. Reset and wait for new command
      {
        oMatlabReady    = 0;
        oNewSample      = 0;
        oSendData       = 0;
        oAdcReady       = 0;
        
        nSamples        = 0;
        iteration       = 0;
        
        oCaracDone      = 1;
        oMultiUnitDone  = 1;
        oPsoDone        = 1;
        
        matlabData.bufEmpty           = 1;
        matlabData.bufFull            = 0;
        matlabData.lineBuffer.length  = 0;
        matlabData.inIdx              = 0;
        matlabData.outIdx             = 0;
        
        for (i = 0; i < 16; i++)
        {
          sCellValues.cells[i].cellPowerFloat = 0;
          sCellValues.cells[i].cellVoltFloat  = 0;
          sCellValues.cells[i].nSamples       = 0;
          
          for (j = 0; j < N_SAMPLES_PER_ADC_READ; j++)
          {
            sCellValues.cells[i].cellVoltRaw[j] = 0;
          }
        }
        
        SetPotInitialCondition();
        
        oPsoMode        = 0;
        oMultiUnitMode  = 0;
        oCaracMode      = 1;
        
//        UINT16 dutyCycle = 150;
        
        SetLedDutyCycle( 0, dutyCycle);
        SetLedDutyCycle( 1, dutyCycle);
        SetLedDutyCycle( 2, dutyCycle);
        SetLedDutyCycle( 3, dutyCycle);
        SetLedDutyCycle(12, dutyCycle);
        SetLedDutyCycle(13, dutyCycle);
        SetLedDutyCycle(14, dutyCycle);
        SetLedDutyCycle(15, dutyCycle);
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
  
  for (i = 0; i < matlabPacketSize; i++)
  {
    FifoRead(&matlabData, &localBuffer.buffer[i]);
    localBuffer.length++;
  }
  
  Uart.PutTxFifoBuffer(U_MATLAB, &localBuffer);
}


//===============================================================
// Name     : StateCompute
// Purpose  : Compute algorithm.
//===============================================================
void StateCompute(void)
{  
  oNewSample = 0;
  UINT8 i;
  
  if (nSamples >= N_SAMPLES_PER_ADC_READ)
  {
    nSamples -= N_SAMPLES_PER_ADC_READ;

    ComputeMeanAdcValues();
            
    if (oCaracMode)
    {
//      ComputeCellPower( 0, potIndexValue[0]);
//      ComputeCellPower( 1, potIndexValue[0]);
//      ComputeCellPower( 2, potIndexValue[0]);
//      ComputeCellPower( 3, potIndexValue[0]);

//      ComputeCellPower( 4, potIndexValue[0]);
//      ComputeCellPower( 5, potIndexValue[0]);
//      ComputeCellPower( 6, potIndexValue[0]);
//      ComputeCellPower( 7, potIndexValue[0]);

      ComputeCellPower( 8, potIndexValue[0]);
      ComputeCellPower( 9, potIndexValue[0]);
      ComputeCellPower(10, potIndexValue[0]);
      ComputeCellPower(11, potIndexValue[0]);

      ComputeCellPower(12, potIndexValue[0]);
      ComputeCellPower(13, potIndexValue[0]);
      ComputeCellPower(14, potIndexValue[0]);
      ComputeCellPower(15, potIndexValue[0]);
      
      Caracterization();
    }
    else if (oPsoMode)
    {
      for (i = 0; i < psoValues.nParticles; i++)
      {
        ComputeCellPower(psoValues.particleIndex[i], potIndexValue[psoValues.particleIndex[i]]);
      }
//      ComputeCellPower(psoValues.particleIndex[0], potIndexValue[psoValues.particleIndex[0]]);
//      ComputeCellPower(psoValues.particleIndex[1], potIndexValue[psoValues.particleIndex[1]]);
////      ComputeCellPower(10, potIndexValue[10]);
//      ComputeCellPower(psoValues.particleIndex[2], potIndexValue[psoValues.particleIndex[2]]);
      
//      UINT32 coreTickRate = Timer.Tic(1500, SCALE_US);
      ParticleSwarmOptimization();
//      testInt32 = Timer.Toc(1500, coreTickRate);
//      testFloat = 0;
    }
    else if (oMultiUnitMode)
    {
      ComputeCellPower(multiUnitValues.unitIndex[0], potIndexValue[multiUnitValues.unitIndex[0]]);
      ComputeCellPower(multiUnitValues.unitIndex[1], potIndexValue[multiUnitValues.unitIndex[1]]);
      MultiUnit();
    }
    else
    {
      oErrorFlag = 1;
    }
  }
}


//===============================================================
// Name     : StateError
// Purpose  : Error state of the system. Used to assess and
//            correct errors in the system.
//===============================================================
void StateError(void)
{
  INTDisableInterrupts();   // Disable all interrupts of the system.
  
  LED1_ON;
  LED2_OFF;
  
  while(1)
  {
    LED1_TOGGLE;
    LED2_TOGGLE;
    
    Timer.DelayMs(500);
  }
}
