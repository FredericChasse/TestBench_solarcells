//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook Project Template
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : Interrupts.c
// Author  : Frederic Chasse
// Date    : 2015-01-03
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file declares all interrupt subroutines used.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : All priorities must be set and verified by the developper. All
//           priorities and subpriorities used are there as example. Also, note
//           that interrupt can be defined as iplnauto, iplnsrs and iplnsoft,
//           where n can be from 0 to 7. 7 is the highest priority, while 1 is
//           the lowest. A priority of 0 will disable the interrupt.
//           Subpriorities can go from (lowest to highest) 0 to 3.
//           SRS mode uses the shadow registers, soft uses the software
//           registers and auto is an automatic assignment.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "..\headers\Interrupts.h"
#include "..\headers\Potentiometer.h"

volatile BOOL  oAdcReady    = 0
              ,oTimer3Ready = 0
              ;

/*******************************************************************************
 ***********************                               *************************
 ********************           TIMER INTERRUPTS          **********************
 ***********************                               *************************
 ******************************************************************************/

//=============================================
// Configure the Timer 1 interrupt handler
//=============================================
void __ISR(_TIMER_1_VECTOR, T1_INTERRUPT_PRIORITY) Timer1InterruptHandler(void)
{
  // Increment the number of overflows from this timer. Used primarily by Input Capture
  Timer.Var.nOverflows[0]++;
  
  mT1ClearIntFlag();
}

//=============================================
// Configure the Timer 2 interrupt handler
//=============================================
void __ISR(_TIMER_2_VECTOR, T2_INTERRUPT_PRIORITY) Timer2InterruptHandler(void)
{
  // Increment the number of overflows from this timer. Used primarily by Input Capture
  Timer.Var.nOverflows[1]++;

  mT2ClearIntFlag();
}

//=============================================
// Configure the Timer 3 interrupt handler
//=============================================
void __ISR(_TIMER_3_VECTOR, T3_INTERRUPT_PRIORITY) Timer3InterruptHandler(void)
{
  oTimer3Ready = 1;

  // Increment the number of overflows from this timer. Used primarily by Input Capture
  Timer.Var.nOverflows[2]++;

  mT3ClearIntFlag();
}

//=============================================
// Configure the Timer 4 interrupt handler
//=============================================
void __ISR(_TIMER_4_VECTOR, T4_INTERRUPT_PRIORITY) Timer4InterruptHandler(void)
{
  // Increment the number of overflows from this timer. Used primarily by Input Capture
  Timer.Var.nOverflows[3]++;

  mT4ClearIntFlag();
}

//=============================================
// Configure the Timer 5 interrupt handler
//=============================================
void __ISR(_TIMER_5_VECTOR, T5_INTERRUPT_PRIORITY) Timer5InterruptHandler(void)
{
  // Increment the number of overflows from this timer. Used primarily by Input Capture
  Timer.Var.nOverflows[4]++;

  mT5ClearIntFlag();
}


/*******************************************************************************
 ***********************                               *************************
 ********************           UART INTERRUPTS           **********************
 ***********************                               *************************
 *******************************************************************************/


//=============================================
// Configure the UART 3 interrupt handler
//=============================================

void __ISR(_UART_3_VECTOR, U3_INTERRUPT_PRIORITY) Uart3InterruptHandler(void)
{
  UINT8  i
        ,iMax   // Read/write max 8 bytes/interrupt
        ,data   // used in UartFifoWrite/Read functions
        ;

	// TX interrupt handling
  //===========================================================

  if ( INTGetEnable ( INT_SOURCE_UART_TX(UART3) ) )               // If TX interrupts enabled
  {
    if ( INTGetFlag ( INT_SOURCE_UART_TX(UART3) ) )               // If TX interrupt occured
    {
      if ( UARTTransmitterIsReady(UART3) && !Uart.Var.uartTxFifo[UART3].bufEmpty )  // If TX buffer is ready to receive data and the user's TX buffer is not empty
      {
        if (Uart.Var.uartTxFifo[UART3].lineBuffer.length < 8)     // Write max 8 bytes/interrupt
        {
          iMax = Uart.Var.uartTxFifo[UART3].lineBuffer.length;
        }
        else
        {
          iMax = 8;
        }

        for (i = 0; i < iMax; i++)
        {
          UartFifoRead((void *) &Uart.Var.uartTxFifo[UART3], &data);  // Copy from user
          U3TXREG = data;                                         // Put data in PIC32's TX buffer
        }
      }

      if (Uart.Var.uartTxFifo[UART3].bufEmpty)                    // If User's TX buffer is empty
      {
        Uart.DisableTxInterrupts(UART3);                          // Disable TX interrupts
      }

      INTClearFlag(INT_SOURCE_UART_TX(UART3));                    // Clear the TX interrupt Flag
    }
  }
  //===========================================================
  

	// RX interrupt handling
  //===========================================================
  if ( INTGetEnable ( INT_SOURCE_UART_RX(UART3) ) )               // If RX interrupts enabled
  {
    if ( INTGetFlag ( INT_SOURCE_UART_RX(UART3) ) )               // If RX interrupt occured
    {
      i = 0;
      iMax = 8;                                                   // Read max 8 bytes/interrupt
      while (   UARTReceivedDataIsAvailable(UART3)                // While RX data available
            && !Uart.Var.uartRxFifo[UART3].bufFull                // and user's RX buffer not full
            && (i < iMax)                                         // and under 8 bytes read
            )
      { // while ^
        data = UARTGetDataByte(UART3);                            // Get data for PIC32's RX FIFO buffer and copy it to user (next line)
        if ( UartFifoWrite((void *) &Uart.Var.uartRxFifo[UART3], &data) < 0 ) // If copy to user did not work
        {
          break;                                                  // Exit while loop
        }
        i++;
      } // end while

      if (!Uart.Var.uartRxFifo[UART3].bufEmpty)                   // If there is data in the user's RX buffer
      {
        Uart.Var.oIsRxDataAvailable[UART3] = 1;                   // Set according flag
      }

      INTClearFlag (INT_SOURCE_UART_RX(UART3) );                  // Clear the RX interrupt Flag

    }
	}
  //===========================================================
}
//=============================================


//=============================================
// Configure the UART 6 interrupt handler
//=============================================

void __ISR(_UART_6_VECTOR, U6_INTERRUPT_PRIORITY) Uart6InterruptHandler(void)
{
  UINT8  i
        ,iMax   // Read/write max 8 bytes/interrupt
        ,data   // used in UartFifoWrite/Read functions
        ;

	// TX interrupt handling
  //===========================================================

  if ( INTGetEnable ( INT_SOURCE_UART_TX(UART6) ) )               // If TX interrupts enabled
  {
    if ( INTGetFlag ( INT_SOURCE_UART_TX(UART6) ) )               // If TX interrupt occured
    {
      if ( UARTTransmitterIsReady(UART6) && !Uart.Var.uartTxFifo[UART6].bufEmpty )  // If TX buffer is ready to receive data and the user's TX buffer is not empty
      {
        if (Uart.Var.uartTxFifo[UART6].lineBuffer.length < 8)     // Write max 8 bytes/interrupt
        {
          iMax = Uart.Var.uartTxFifo[UART6].lineBuffer.length;
        }
        else
        {
          iMax = 8;
        }

        for (i = 0; i < iMax; i++)
        {
          UartFifoRead((void *) &Uart.Var.uartTxFifo[UART6], &data);  // Copy from user
          U6TXREG = data;                                         // Put data in PIC32's TX buffer
        }
      }

      if (Uart.Var.uartTxFifo[UART6].bufEmpty)                    // If User's TX buffer is empty
      {
        Uart.DisableTxInterrupts(UART6);                          // Disable TX interrupts
      }

      INTClearFlag(INT_SOURCE_UART_TX(UART6));                    // Clear the TX interrupt Flag
    }
  }
  //===========================================================
  

	// RX interrupt handling
  //===========================================================
  if ( INTGetEnable ( INT_SOURCE_UART_RX(UART6) ) )               // If RX interrupts enabled
  {
    if ( INTGetFlag ( INT_SOURCE_UART_RX(UART6) ) )               // If RX interrupt occured
    {
      i = 0;
      iMax = 8;                                                   // Read max 8 bytes/interrupt
      while (   UARTReceivedDataIsAvailable(UART6)                // While RX data available
            && !Uart.Var.uartRxFifo[UART6].bufFull                // and user's RX buffer not full
            && (i < iMax)                                         // and under 8 bytes read
            )
      { // while ^
        data = UARTGetDataByte(UART6);                            // Get data for PIC32's RX FIFO buffer and copy it to user (next line)
        if ( UartFifoWrite((void *) &Uart.Var.uartRxFifo[UART6], &data) < 0 ) // If copy to user did not work
        {
          break;                                                  // Exit while loop
        }
        i++;
      } // end while

      if (!Uart.Var.uartRxFifo[UART6].bufEmpty)                   // If there is data in the user's RX buffer
      {
        Uart.Var.oIsRxDataAvailable[UART6] = 1;                   // Set according flag
      }

      INTClearFlag (INT_SOURCE_UART_RX(UART6) );                  // Clear the RX interrupt Flag

    }
	}
  //===========================================================
}
//=============================================


/*******************************************************************************
 ***********************                               *************************
 ********************           ADC INTERRUPT             **********************
 ***********************                               *************************
 *******************************************************************************/

//=============================================
// Configure the ADC interrupt handler
//=============================================
void __ISR(_ADC_VECTOR, ADC_INTERRUPT_PRIO) AdcInterruptHandler(void)
{
  Adc.Read();               // Read the enabled channels and puts them in Adc.Var.adcReadValues[]
  oAdcReady = 1;
  INTClearFlag(INT_AD1);    // Clear the ADC conversion done interrupt Flag
}
//=============================================


/*******************************************************************************
 ***********************                               *************************
 ********************           I2C INTERRUPTS            **********************
 ***********************                               *************************
 *******************************************************************************/

//=============================================
// Configure the I2C5 interrupt handler
//=============================================
void __ISR(_I2C_5_VECTOR, I2C5_INT_PRIORITY) I2c5InterruptHandler(void)
{
//  if (INTGetFlag(INT_I2C5S))  // Slave interrupt
//  {
//    INTClearFlag(INT_I2C5S);
//  }
  
//  if (INTGetFlag(INT_I2C5B))  //Bus Collision interrupt
//  {
//    INTClearFlag(INT_I2C5B);
//  }
  
  sI2cCmdBuffer_t masterData;   // Used for keeping track of the current state
  
  if (INTGetFlag(INT_I2C5M))  // Master interrupt
  {
    INTClearFlag(INT_I2C5M);

    if (I2c.Var.oReadDataInNextInterrupt[I2C5])     // If a read was started last interrupt
    {
      masterData.data  = I2C5RCV;                   // Read from I2C buffer
      masterData.state = I2C_MASTER_RECEIVE_DATA;   // Dummy write
      I2cFifoWrite((void *) &I2c.Var.i2cUserFifo[I2C5], &masterData);   // Copy to user
      I2c.Var.oRxDataAvailable[I2C5] = 1;
      I2c.Var.oReadDataInNextInterrupt[I2C5] = 0;
    }
    
    if (I2c.Var.oI2cWriteIsRunning[I2C5])   // Writing procedure
    {
      I2cFifoRead((void *) &I2c.Var.i2cWriteQueue[I2C5], &masterData);  // Get current state
      switch (masterData.state)
      {
      //======================================================  
        case I2C_MASTER_RECEIVE_DATA : 
          I2C5CONbits.RCEN = 1;                         // Receive byte sequence
          I2c.Var.oReadDataInNextInterrupt[I2C5] = 1;   // Flag for the next interrupt to read the RX buffer
          break;
      //====================================================== 

      //======================================================  
        case I2C_MASTER_START_CONDITION : 
          I2C5CONbits.SEN = 1;                          // Start condition sequence
          break;
      //====================================================== 

      //======================================================  
        case I2C_MASTER_STOP_CONDITION : 
          I2C5CONbits.PEN = 1;                          // Stop condition sequence
          
          if (I2c.Var.oPoolSlaveAcknowledge[I2C5])      // If the user wanted to poll
          {
            if (!I2c.Var.oSecondStopAfterPooling[I2C5]) // If this is the first stop after writing all the data
            {                                           // Add the next three states required for polling
              masterData.state = I2C_MASTER_START_CONDITION;
              I2cFifoWrite((void *) &I2c.Var.i2cWriteQueue[I2C5], &masterData);
              
              masterData.state = I2C_MASTER_TRANSMIT_DATA;
              I2cFifoWrite((void *) &I2c.Var.i2cWriteQueue[I2C5], &masterData);
              
              masterData.state = I2C_MASTER_STOP_CONDITION;
              I2cFifoWrite((void *) &I2c.Var.i2cWriteQueue[I2C5], &masterData);
              
              I2c.Var.oSecondStopAfterPooling[I2C5] = 1;
            }
            else                                        // If the first stop has passed
            {
              if (!I2CByteWasAcknowledged(I2C5))        // If slave sent NACK
              {                                         // Redo the 3 states for the polling
                masterData.state = I2C_MASTER_START_CONDITION;
                I2cFifoWrite((void *) &I2c.Var.i2cWriteQueue[I2C5], &masterData);

                masterData.state = I2C_MASTER_TRANSMIT_DATA;
                I2cFifoWrite((void *) &I2c.Var.i2cWriteQueue[I2C5], &masterData);

                masterData.state = I2C_MASTER_STOP_CONDITION;
                I2cFifoWrite((void *) &I2c.Var.i2cWriteQueue[I2C5], &masterData);
              }
              else                                      // If slave sent ACK
              {                                         // End the communication with the slave
                masterData.state = I2C_MASTER_DONE;
                I2cFifoWrite((void *) &I2c.Var.i2cWriteQueue[I2C5], &masterData);
                I2c.Var.oSecondStopAfterPooling[I2C5] = 0;
              }
            }
          }
          break;
      //====================================================== 

      //======================================================    
        case I2C_MASTER_TRANSMIT_DATA : 
          I2C5TRN = masterData.data;      // Put data in TX buffer
          break;
      //====================================================== 

      //======================================================  
        case I2C_MASTER_DONE : 
          if (I2c.Var.i2cWriteQueue[I2C5].bufEmpty)     // If there is nothing more to send
          {
            I2c.Var.oI2cWriteIsRunning[I2C5] = 0;       // Turn off writing process
          }
          else                                          // If there is still data in the WriteQueue
          {
            INTSetFlag(INT_I2C5M);                      // Start another writing process
          }
          break;
      //====================================================== 

      //======================================================  
        case I2C_MASTER_REPEAT_START : 
          I2C5CONbits.RSEN = 1;   //repeated start condition sequence
          break;
      //====================================================== 

      //======================================================  
        case I2C_MASTER_SLAVE_SENT_STOP :   // Not used
//          LED_ERROR_ON;
          break;
      //====================================================== 

      //======================================================  
        case I2C_MASTER_SEND_ACK : 
          I2C5CONbits.ACKDT = 0;  //ACK
          I2C5CONbits.ACKEN = 1;  //Send ACK sequence
          break;
      //====================================================== 

      //======================================================  
        case I2C_MASTER_SEND_NACK : 
          I2C5CONbits.ACKDT = 1;  //NACK
          I2C5CONbits.ACKEN = 1;  //Send NACK sequence
          break;
      //====================================================== 

      //======================================================  
        case I2C_CMD_ERROR :      // Not used
//          LED_ERROR_ON;
          break;
      //====================================================== 

      //======================================================  
        default : 
          break;
      //======================================================  
      } // end switch (master.state)
    } // end if (writing sequence)
    
    

    if (I2c.Var.oI2cReadIsRunning[I2C5])    // Reading procedure
    {
      I2cFifoRead((void *) &I2c.Var.i2cReadQueue[I2C5], &masterData);   // Get current state
      switch (masterData.state)
      {
      //======================================================  
        case I2C_MASTER_RECEIVE_DATA : 
          I2C5CONbits.RCEN = 1;             // Receive byte sequence
          I2c.Var.oReadDataInNextInterrupt[I2C5] = 1;
          break;
      //====================================================== 

      //======================================================  
        case I2C_MASTER_START_CONDITION : 
          I2C5CONbits.SEN = 1;              // Start condition sequence
          break;
      //====================================================== 

      //======================================================  
        case I2C_MASTER_REPEAT_START : 
          I2C5CONbits.RSEN = 1;             // Repeated start condition sequence
          break;
      //====================================================== 

      //======================================================  
        case I2C_MASTER_STOP_CONDITION : 
          I2C5CONbits.PEN = 1;              // Stop condition
          break;
      //====================================================== 

      //======================================================    
        case I2C_MASTER_TRANSMIT_DATA : 
          I2C5TRN = masterData.data;        // Put data in TX buffer
          break;
      //====================================================== 

      //======================================================  
        case I2C_MASTER_DONE : 
          if (I2c.Var.i2cReadQueue[I2C5].bufEmpty)    // If there is nothing more to send
          {
            I2c.Var.oI2cReadIsRunning[I2C5] = 0;      // Turn off reading process
          }
          else                                        // If there is still data in the ReadQueue
          {
            INTSetFlag(INT_I2C5M);                    // Start another reading process
          }
          break;
      //====================================================== 

      //======================================================  
        case I2C_MASTER_SLAVE_SENT_STOP :   // Not used
//          LED_ERROR_ON;
          break;
      //====================================================== 

      //======================================================  
        case I2C_MASTER_SEND_ACK : 
          I2C5CONbits.ACKDT = 0;  // ACK
          I2C5CONbits.ACKEN = 1;  // Send ACK sequence
          break;
      //====================================================== 

      //======================================================  
        case I2C_MASTER_SEND_NACK : 
          I2C5CONbits.ACKDT = 1;  // NACK
          I2C5CONbits.ACKEN = 1;  // Send NACK sequence
          break;
      //====================================================== 

      //======================================================  
        case I2C_CMD_ERROR :      // Not used
//          LED_ERROR_ON;
          break;
      //======================================================

      //======================================================  
        default : 
          break;
      //======================================================  
      } // end switch (master.state)
    } // end if (read sequence)
  }  // end if (master interrupt)
}   // end interrupt
//=============================================


/*******************************************************************************
 ***********************                               *************************
 ********************           SPI INTERRUPTS            **********************
 ***********************                               *************************
 *******************************************************************************/

//=============================================
// Configure the SPI 3 interrupt handler
//=============================================

void __ISR(_SPI_3_VECTOR, S3_INTERRUPT_PRIORITY) Spi3InterruptHandler(void)
{
  UINT8  i
        ,iMax   // Read/write max 8 bytes/interrupt
        ,data   // used in SpiFifoWrite/Read functions
        ;

	// TX interrupt handling
  //===========================================================

  if ( INTGetEnable ( INT_SOURCE_SPI_TX(SPI3) ) )               // If TX interrupts enabled
  {
    if ( INTGetFlag ( INT_SOURCE_SPI_TX(SPI3) ) )               // If TX interrupt occured
    {
      if ( !SpiChnIsBusy(SPI_CHANNEL3) && !Spi.Var.spiTxFifo[SPI3].bufEmpty )  // If TX buffer is ready to receive data and the user's TX buffer is not empty
      {
        if (Spi.Var.spiTxFifo[SPI3].lineBuffer.length < Spi.Var.spiTxFifo[SPI3].maxWordsPerInt)     // Write max 8 bytes/interrupt
        {
          iMax = Spi.Var.spiTxFifo[SPI3].lineBuffer.length;
        }
        else
        {
          iMax = Spi.Var.spiTxFifo[SPI3].maxWordsPerInt;
        }

        CS0 = 0;
        for (i = 0; i < iMax; i++)
        {
          SpiFifoRead((void *) &Spi.Var.spiTxFifo[SPI3], &data);  // Copy from user
          SPI3BUF = data;                                         // Put data in PIC32's TX buffer
        }
      }

      if (Spi.Var.spiTxFifo[SPI3].bufEmpty)                    // If User's TX buffer is empty
      {
        Spi.DisableTxInterrupts(SPI3);                          // Disable TX interrupts
        CS0 = 1;
      }

      INTClearFlag(INT_SOURCE_SPI_TX(SPI3));                    // Clear the TX interrupt Flag
    }
  }
  //===========================================================
  

	// RX interrupt handling
  //===========================================================
  if ( INTGetEnable ( INT_SOURCE_SPI_RX(SPI3) ) )               // If RX interrupts enabled
  {
    if ( INTGetFlag ( INT_SOURCE_SPI_RX(SPI3) ) )               // If RX interrupt occured
    {
      i = 0;
      iMax = Spi.Var.spiRxFifo[SPI3].maxWordsPerInt;            // Read max 8 bytes/interrupt
      while (   SpiChnDataRdy(SPI_CHANNEL3)                // While RX data available
            && !Spi.Var.spiRxFifo[UART3].bufFull                // and user's RX buffer not full
            && (i < iMax)                                         // and under 8 bytes read
            )
      { // while ^
        data = SpiChnGetC(SPI_CHANNEL3);                            // Get data for PIC32's RX FIFO buffer and copy it to user (next line)
        if ( SpiFifoWrite((void *) &Spi.Var.spiRxFifo[SPI3], &data) < 0 ) // If copy to user did not work
        {
          break;                                                  // Exit while loop
        }
        i++;
      } // end while

      if (!Spi.Var.spiRxFifo[SPI3].bufEmpty)                   // If there is data in the user's RX buffer
      {
        Spi.Var.oIsRxDataAvailable[SPI3] = 1;                   // Set according flag
      }

      INTClearFlag (INT_SOURCE_SPI_RX(SPI3) );                  // Clear the RX interrupt Flag

    }
	}
  //===========================================================
}
//=============================================