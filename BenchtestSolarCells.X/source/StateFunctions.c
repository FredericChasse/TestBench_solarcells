//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook Project Template
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : StateFunctions.c
// Author  : Frederic Chasse
// Date    : 2015-01-03
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This is the C file for the functions of the state machine of
//           the system.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : Function names can and should be renamed by the user to improve the
//           readability of the code.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "..\headers\StateFunctions.h"


//==============================================================================
// Variable declarations
//==============================================================================

extern struct sAllCells sCellValues;
extern const float potRealValues[256];
extern const float potRealValuesInverse[256];
extern UINT8 potValue;
extern UINT32 cellVoltageRaw[16];
extern UINT16 nSamples;
extern BOOL oSmoothData;
const float kFilter = 0.1;
extern sUartFifoBuffer_t matlabData;
extern BOOL oSendData;

// All the buttons used. 3 on the steering wheel, 3 on the board
sButtonStates_t buttons =
{
  .chng   .byte = 0
 ,.buttons.byte = 0

 // switches on board are at 1 when not pressed
 ,.buttons.bits.sw1 = 1
 ,.buttons.bits.sw2 = 1
 ,.buttons.bits.sw3 = 1
};

  
//float sinus[2][15] = { {0 , .4189 , .8378 , 1.2566 , 1.6755 , 2.0944 , 2.5133 , 2.9322 , 3.3510 , 3.7699 , 4.1888 , 4.6077 , 5.0265 , 5.4454 , 5.8643} ,
float sinus[2][15] = { {1 , 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 ,12 ,13, 14, 15} ,
                       {0 , .4067 , .7431 , .9511  , .9945  , .8660  , .5878  , .2079  , -.2079 , -.5878 , -.8660 , -.9945 , -.9511 , -.7431 , -.4067} };
  
sUartLineBuffer_t buffer = 
{ 
   .buffer = {0} 
  ,.length =  0 
};

//==============================================================================
// State Machine private functions prototypes
//==============================================================================


//==============================================================================
// Solar cells functions
//==============================================================================

inline void ComputeCellPower (UINT8 cellIndex)
{
//  sCellValues.cells[cellIndex].cellPowerFloat = sCellValues.cells[cellIndex].cellVoltFloat * sCellValues.cells[cellIndex].cellVoltFloat / potRealValues[potValue];
  sCellValues.cells[cellIndex].cellPowerFloat = sCellValues.cells[cellIndex].cellVoltFloat * sCellValues.cells[cellIndex].cellVoltFloat * potRealValuesInverse[potValue];
}

inline void AddDataToMatlabFifo (UINT8 *buffer, UINT8 size)
{
  FifoWriteBuffer(&matlabData, buffer, size);
  
  if (matlabData.lineBuffer.length >= MATLAB_PACKET_SIZE)
  {
    oSendData = 1;
  }
}


inline void GetAdcValues (void)
{
  memcpy((void *) &cellVoltageRaw[0], (void *) &Adc.Var.adcReadValues[0], 64);  // sizeof(UINT32) * 16 = 64

  sCellValues.cells[ 8].cellVoltRaw[nSamples] = cellVoltageRaw[ 8];
  sCellValues.cells[ 9].cellVoltRaw[nSamples] = cellVoltageRaw[ 9];
  sCellValues.cells[10].cellVoltRaw[nSamples] = cellVoltageRaw[10];
  sCellValues.cells[11].cellVoltRaw[nSamples] = cellVoltageRaw[11];
  nSamples++;
}


inline void ComputeMeanAdcValues (void)
{
  UINT8 i = 0;
  UINT32 meanCellRaw[16] = {0};
  
  for (i = 0; i < N_SAMPLES_PER_ADC_READ; i++)
  {
    meanCellRaw[ 8] += sCellValues.cells[ 8].cellVoltRaw[i];
    meanCellRaw[ 9] += sCellValues.cells[ 9].cellVoltRaw[i];
    meanCellRaw[10] += sCellValues.cells[10].cellVoltRaw[i];
    meanCellRaw[11] += sCellValues.cells[11].cellVoltRaw[i];
  }

  meanCellRaw[ 8] = (float) meanCellRaw[ 8] / (float) N_SAMPLES_PER_ADC_READ + 0.5;
  meanCellRaw[ 9] = (float) meanCellRaw[ 9] / (float) N_SAMPLES_PER_ADC_READ + 0.5;
  meanCellRaw[10] = (float) meanCellRaw[10] / (float) N_SAMPLES_PER_ADC_READ + 0.5;
  meanCellRaw[11] = (float) meanCellRaw[11] / (float) N_SAMPLES_PER_ADC_READ + 0.5;

  if (oSmoothData)  // Smoothing function
  {
    sCellValues.cells[ 8].cellVoltFloat = (kFilter*sCellValues.cells[ 8].cellVoltFloat) + (meanCellRaw[ 8] * VREF / 1024.0f)*(1 - kFilter);
    sCellValues.cells[ 9].cellVoltFloat = (kFilter*sCellValues.cells[ 9].cellVoltFloat) + (meanCellRaw[ 9] * VREF / 1024.0f)*(1 - kFilter);
    sCellValues.cells[10].cellVoltFloat = (kFilter*sCellValues.cells[10].cellVoltFloat) + (meanCellRaw[10] * VREF / 1024.0f)*(1 - kFilter);
    sCellValues.cells[11].cellVoltFloat = (kFilter*sCellValues.cells[11].cellVoltFloat) + (meanCellRaw[11] * VREF / 1024.0f)*(1 - kFilter);
  }
  else
  {
    sCellValues.cells[ 8].cellVoltFloat = meanCellRaw[ 8] * VREF / 1024.0f;
    sCellValues.cells[ 9].cellVoltFloat = meanCellRaw[ 9] * VREF / 1024.0f;
    sCellValues.cells[10].cellVoltFloat = meanCellRaw[10] * VREF / 1024.0f;
    sCellValues.cells[11].cellVoltFloat = meanCellRaw[11] * VREF / 1024.0f;
  }
}


//==============================================================================
// Buttons functions
//==============================================================================
void AssessButtons (void)
{
  UINT16 i = 0;
  
  // <editor-fold defaultstate="collapsed" desc="Check changes on board">
  // <editor-fold defaultstate="collapsed" desc="Change on SW1 on board">
  if (buttons.buttons.bits.sw1 != SW1)
  {
    buttons.buttons.bits.sw1    = SW1;
    buttons.chng.bits.sw1       =   1;
  }
  // </editor-fold>

  // <editor-fold defaultstate="collapsed" desc="Change on SW2 on board">
  if (buttons.buttons.bits.sw2 != SW2)
  {
    buttons.buttons.bits.sw2    = SW2;
    buttons.chng.bits.sw2       =   1;
  }
  // </editor-fold>

  // <editor-fold defaultstate="collapsed" desc="Change on SW3 on board">
  if (buttons.buttons.bits.sw3 != SW3)
  {
    buttons.buttons.bits.sw3    = SW3;
    buttons.chng.bits.sw3       =   1;
  }
  // </editor-fold>
  // </editor-fold>

  // <editor-fold defaultstate="collapsed" desc="Assess changes">
  if (buttons.chng.byte)  // If any change has occured on any button
  {
    // <editor-fold defaultstate="collapsed" desc="SW1 on board">
    if (buttons.chng.bits.sw1)
    {
      buttons.chng.bits.sw1 = 0;

      if (!buttons.buttons.bits.sw1)     // If SW1 is pressed
      {
        memcpy(buffer.buffer, sinus, 120);
        buffer.length = 120;
        
        Uart.PutTxFifoBuffer(U_MATLAB, &buffer);
        while(!SW1);
        for (i = 0; i < 15; i++)
        {
          sinus[0][i] += 2*PI;
        }
      }
      else                                // If SW1 is not pressed
      {
        
      }
    }
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="SW2 on board">
    if (buttons.chng.bits.sw2)
    {
      buttons.chng.bits.sw2 = 0;

      if (!buttons.buttons.bits.sw2)     // If SW2 is pressed
      {
        
      }
      else                              // If SW2 is not pressed
      {

      }
    }
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="SW3 on board">
    if (buttons.chng.bits.sw3)
    {
      buttons.chng.bits.sw3 = 0;

      if (!buttons.buttons.bits.sw3)     // If SW3 is pressed
      {
        
      }
      else                          // If SW3 is not pressed
      {
        
      }
    }
    // </editor-fold>
  }
  // </editor-fold>
}


//==============================================================================
// MATLAB FIFOs functions
//==============================================================================

inline INT8 FifoWrite(sUartFifoBuffer_t *fifo, UINT8 *data)
{
  if (fifo->bufFull)
  {
    return -1;
  }
  fifo->bufEmpty = 0;
  fifo->lineBuffer.buffer[fifo->inIdx] = *data;
  fifo->inIdx = (fifo->inIdx + 1) % fifo->maxBufSize;
  if (fifo->inIdx == fifo->outIdx)
  {
    fifo->bufFull = 1;
  }
  fifo->lineBuffer.length++;
  return 0;
}

inline INT8 FifoWriteBuffer(sUartFifoBuffer_t *fifo, UINT8 *data, UINT8 length)
{
  UINT8 i;
  
  if (fifo->bufFull)
  {
    return -1;
  }
  
  if ((fifo->maxBufSize - fifo->lineBuffer.length) < length)
  {
    return -1;
  }
  
  fifo->bufEmpty = 0;
  for (i = 0; i < length; i++)
  {
    fifo->lineBuffer.buffer[fifo->inIdx] = *data;
    fifo->inIdx = (fifo->inIdx + 1) % fifo->maxBufSize;
    fifo->lineBuffer.length++;
    data++;
  }
  
  if (fifo->inIdx == fifo->outIdx)
  {
    fifo->bufFull = 1;
  }
  return 0;
}


inline INT8 FifoRead (sUartFifoBuffer_t *fifo, UINT8 *data)
{
  if (fifo->bufEmpty)
  {
    return -1;
  }
  fifo->bufFull = 0;
  *data = fifo->lineBuffer.buffer[fifo->outIdx];
  fifo->outIdx = (fifo->outIdx + 1) % fifo->maxBufSize;
  if (fifo->outIdx == fifo->inIdx)
  {
    fifo->bufEmpty = 1;
  }
  fifo->lineBuffer.length--;
  return 0;
}