/*****************************************************************************
*
* Copyright (C) 2013 - 2016 Texas Instruments Incorporated - http://www.ti.com/
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the
*   distribution.
*
* * Neither the name of Texas Instruments Incorporated nor the names of
*   its contributors may be used to endorse or promote products derived
*   from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************
*
* MSP432 blink.c template - P1.0 port toggle
*
* Classic coding
*
****************************************************************************/

#include "msp.h"
#include "i2c.h"


/* DriverLib Defines */
#include "driverlib.h"

/* Standard Defines */
#include <stdint.h>

#include <stdbool.h>
#include <string.h>

/* Slave Address for I2C Slave */
#define SLAVE_ADDRESS       0x62
#define NUM_OF_REC_BYTES    2

/* Variables */
const int TXData[2] = {0x00, 0x04};
//static uint8_t RXData[NUM_OF_REC_BYTES];

//static uint_least8_t RXData[NUM_OF_REC_BYTES];

//static volatile uint32_t xferIndex;
static volatile bool stopSent;

static int RXData[NUM_OF_REC_BYTES];
static volatile uint32_t xferIndex;



/* I2C Master Configuration Parameter */
const eUSCI_I2C_MasterConfig i2cConfig =
{
        EUSCI_B_I2C_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        3000000,                                // SMCLK = 3MHz
        EUSCI_B_I2C_SET_DATA_RATE_400KBPS,      // Desired I2C Clock of 100khz //changed
        0,                                      // No byte counter threshold
        EUSCI_B_I2C_NO_AUTO_STOP                // No Autostop
};

//int main(void)
//{
//    volatile uint32_t ii;
//    int index=0;
//
//    /* Disabling the Watchdog  */
//    MAP_WDT_A_holdTimer();
//
//    /* Select Port 1 for I2C - Set Pin 6, 7 to input Primary Module Function,
//     *   (UCB0SIMO/UCB0SDA, UCB0SOMI/UCB0SCL).
//     */
//    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
//            GPIO_PIN6 + GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
//   // stopSent = false;
//    memset(RXData, 0x00, NUM_OF_REC_BYTES);
//
//    /* Initializing I2C Master to SMCLK at 100khz with no autostop */
//    MAP_I2C_initMaster(EUSCI_B0_BASE, &i2cConfig);
//
//    /* Specify slave address */
//    MAP_I2C_setSlaveAddress(EUSCI_B0_BASE, SLAVE_ADDRESS);
//
//    /* Set Master in transmit mode */
//    MAP_I2C_setMode(EUSCI_B0_BASE, EUSCI_B_I2C_TRANSMIT_MODE);
//
//    /* Enable I2C Module to start operations */
//    MAP_I2C_enableModule(EUSCI_B0_BASE);
//
//
//    while (1)
//    {
//        /* Making sure the last transaction has been completely sent out */
//        while (MAP_I2C_masterIsStopSent(EUSCI_B0_BASE) == EUSCI_B_I2C_SENDING_STOP);
//
//        /* Send start and the first byte of the transmit buffer. We have to send
//         * two bytes to clean out whatever is in the buffer from a previous
//         * send  */
//        MAP_I2C_masterSendMultiByteStart(EUSCI_B0_BASE, TXData[0]);
//       // MAP_I2C_masterSendMultiByteNext(EUSCI_B0_BASE, TXData[0]);
//        MAP_I2C_masterSendMultiByteNext(EUSCI_B0_BASE, TXData[1]);
//
//        MAP_I2C_setMode(EUSCI_B0_BASE, EUSCI_B_I2C_RECEIVE_MODE);
//        MAP_I2C_masterReceiveStart(EUSCI_B0_BASE);
//
//        while(index<2)
//        {
//        	//printf("here");
//        	RXData[index] = MAP_I2C_masterReceiveMultiByteNext(EUSCI_B0_BASE);
//        	index++;
//        }
//
//       // printf("Received data is 0x%2x",RXData);
//
//        MAP_I2C_masterReceiveMultiByteFinish(EUSCI_B0_BASE);
//
//       //stopSent = false;
//    }
//}


int main(void)


{
    volatile uint32_t ii;
    uint8_t bus;

    /* Disabling the Watchdog  */
    MAP_WDT_A_holdTimer();

    /* Select Port 1 for I2C - Set Pin 6, 7 to input Primary Module Function,
     *   (UCB0SIMO/UCB0SDA, UCB0SOMI/UCB0SCL).
     */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
            GPIO_PIN6 + GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
    stopSent = false;
    memset(RXData, 0x00, NUM_OF_REC_BYTES);

    /* Initializing I2C Master to SMCLK at 100khz with no autostop */
    MAP_I2C_initMaster(EUSCI_B0_BASE, &i2cConfig);

    /* Specify slave address */
    MAP_I2C_setSlaveAddress(EUSCI_B0_BASE, SLAVE_ADDRESS);

    /* Set Master in transmit mode */
    MAP_I2C_setMode(EUSCI_B0_BASE, EUSCI_B_I2C_TRANSMIT_MODE);

    /* Enable I2C Module to start operations */
    MAP_I2C_enableModule(EUSCI_B0_BASE);

    /* Enable and clear the interrupt flag */
    MAP_I2C_clearInterruptFlag(EUSCI_B0_BASE,
            EUSCI_B_I2C_TRANSMIT_INTERRUPT0 + EUSCI_B_I2C_RECEIVE_INTERRUPT0);
    //Enable master Receive interrupt
    MAP_I2C_enableInterrupt(EUSCI_B0_BASE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0);
    MAP_Interrupt_enableSleepOnIsrExit();
    MAP_Interrupt_enableInterrupt(INT_EUSCIB0);

    while (1)
    {
        /* Making sure the last transaction has been completely sent out */
        while (MAP_I2C_masterIsStopSent(EUSCI_B0_BASE) == EUSCI_B_I2C_SENDING_STOP);

        /* Send start and the first byte of the transmit buffer. We have to send
         * two bytes to clean out whatever is in the buffer from a previous
         * send  */
        MAP_I2C_masterSendMultiByteStart(EUSCI_B0_BASE, TXData[0]);
        //bus= I2C_isBusBusy(EUSCI_B0_BASE);
        // there MAP_I2C_masterSendMultiByteNext(EUSCI_B0_BASE, TXData[0]);


        /* Enabling transfer interrupt after stop has been sent */
        MAP_I2C_enableInterrupt(EUSCI_B0_BASE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0);

        /* While the stop condition hasn't been sent out... */
        while(!stopSent)
        {
            MAP_PCM_gotoLPM0InterruptSafe();
        }

        stopSent = false;
    }

    return 0;
}

/*******************************************************************************
 * eUSCIB0 ISR. The repeated start and transmit/receive operations happen
 * within this ISR.
 *******************************************************************************/
void EUSCIB0_IRQHandler(void)
{
    uint_fast16_t status;
    volatile uint32_t i;
    P1DIR |= BIT0;
   // P2DIR |= BIT2;
    uint8_t bus,data;
    status = MAP_I2C_getEnabledInterruptStatus(EUSCI_B0_BASE);
    MAP_I2C_clearInterruptFlag(EUSCI_B0_BASE, status);

    /* If we reached the transmit interrupt, it means we are at index 1 of
     * the transmit buffer. When doing a repeated start, before we reach the
     * last byte we will need to change the mode to receive mode, set the start
     * condition send bit, and then load the final byte into the TXBUF.
     */
    if (status & EUSCI_B_I2C_TRANSMIT_INTERRUPT0)
    {
        MAP_I2C_masterSendMultiByteNext(EUSCI_B0_BASE, TXData[1]);
      //  bus= I2C_isBusBusy(EUSCI_B0_BASE);
        MAP_I2C_disableInterrupt(EUSCI_B0_BASE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0);


        MAP_I2C_masterSendMultiByteStop(EUSCI_B0_BASE);


        MAP_I2C_setMode(EUSCI_B0_BASE, EUSCI_B_I2C_RECEIVE_MODE);
        xferIndex = 0;
        MAP_I2C_masterReceiveStart(EUSCI_B0_BASE);
        MAP_I2C_enableInterrupt(EUSCI_B0_BASE, EUSCI_B_I2C_RECEIVE_INTERRUPT0);

    }

    /* Receives bytes into the receive buffer. If we have received all bytes,
     * send a STOP condition */
    if (status & EUSCI_B_I2C_RECEIVE_INTERRUPT0)
    {
        if(xferIndex == NUM_OF_REC_BYTES - 2)
        {

            MAP_I2C_masterReceiveMultiByteStop(EUSCI_B0_BASE);
          //  data=UCB0RXBUF;
            RXData[xferIndex++] = MAP_I2C_masterReceiveMultiByteNext(EUSCI_B0_BASE);


            if(RXData[xferIndex]>0 && RXData[xferIndex]<100)
            {
            	P1OUT ^= BIT0;                           /* Toggle P1.0 */
            for(i=10000; i>0; i--);
            P1OUT ^= BIT0;
            }
        /* if(RXData[xferIndex]<0){
            	P2OUT ^= BIT2;
            	for(i=10000; i>0; i--);
            	            P2OUT ^= BIT2;

            }*/
        }
        else if(xferIndex == NUM_OF_REC_BYTES - 1)
        {
            RXData[xferIndex++] = MAP_I2C_masterReceiveMultiByteNext(EUSCI_B0_BASE);
         //   data=UCB0RXBUF;
            MAP_I2C_disableInterrupt(EUSCI_B0_BASE, EUSCI_B_I2C_RECEIVE_INTERRUPT0);
            MAP_I2C_setMode(EUSCI_B0_BASE, EUSCI_B_I2C_TRANSMIT_MODE);
            xferIndex = 0;
            stopSent = true;
            MAP_Interrupt_disableSleepOnIsrExit();
//            P1OUT ^= BIT0;                           /* Toggle P1.0 */
//            for(i=10000; i>0; i--);
//                        P1OUT ^= BIT0;
            if(RXData[xferIndex]>0 && RXData[xferIndex]<100)
                        {
                        	P1OUT ^= BIT0;                           /* Toggle P1.0 */
                        for(i=10000; i>0; i--);
                        P1OUT ^= BIT0;
                        }
                       /* if(RXData[xferIndex]<0){
                        	P2OUT ^= BIT2;
                        	for(i=10000; i>0; i--);
                        	            P2OUT ^= BIT2;

                        }*/
        }
        else
        {
            RXData[xferIndex++] = MAP_I2C_masterReceiveMultiByteNext(EUSCI_B0_BASE);
           // data=UCB0RXBUF;
//            P1OUT ^= BIT0;                           /* Toggle P1.0 */
//            for(i=10000; i>0; i--);
//                        P1OUT ^= BIT0;
            if(RXData[xferIndex]>0 && RXData[xferIndex]<100)
                        {
                        	P1OUT ^= BIT0;                           /* Toggle P1.0 */
                        for(i=10000; i>0; i--);
                        P1OUT ^= BIT0;
                        }
                        /*if(RXData[xferIndex]<0){
                        	P2OUT ^= BIT2;
                        	for(i=10000; i>0; i--);
                        	            P2OUT ^= BIT2;

                        }*/
        }

    }

  // sprintf("Received data is 0x%2x",RXData);
}






