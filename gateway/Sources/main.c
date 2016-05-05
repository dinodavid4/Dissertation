/*
 * 
 * Gateway Module
 * UART Communication between TORCS(PC) and Gateway
 * CAN communication between Gateway and Electronic Control Unit(ECU/Hercules) 
 *
 */

#include "CLK.h"
#include "UART.h"
#include <stdio.h>
#include "typedefs.h"
#include "msCANdrv.h"
#include "derivative.h" /* include peripheral declarations */
#include "MSCAN_Module.h"

void Led_init();
void Led_toggle();
void Initialize_UART();
uint8 Initialize_CAN();
void delay_ms(UINT16 delay);
void Uart2_Receive(UINT8 rxBuffer);
void Enable_Interrupt(UINT8 vector_number);
void Disable_Interrupt(UINT8 vector_number);

int main(void)
{
	uint8 indx = 0;

	Clk_Init();
	Led_init();
	Initialize_UART();
	Initialize_CAN();

	/* Start up: Toggle LED's */
	for(indx = 0; indx < 16; indx++) {
		Led_toggle();
		delay_ms(100);
	}
	
	Enable_Interrupt(INT_UART2); //Enable UART interrupt
	//EnableInterrupts; //Enable CAN interrupt

	/* CAN Transmission is carried out in Interrupt Handler; */
	
	while(1);
	
#if 0	
	while (1) 
	{
	    Check_CAN_MB_Status(0, 0, buffer_status);
	    while (buffer_status[0] != NEWDATA) //Wait for the Receive ISR to finish and change the buffer status, NEWDATA indicates that the buffer has receive a new data
		{	
			Check_CAN_MB_Status(0, 0, buffer_status);
		}
		Read_CAN_MB_Data(0, 0, rx_data);

		indx = 1;
		
		//printf("Rx <-----");
		//for(indx = 0; indx < 9; indx++)
		//{
			Uart_SendChar(rx_data[indx]);
//			Led_toggle();
//			delay_ms(4);
			Enable_Interrupt(INT_UART2);
		//}
	}
#endif
	
	return 0;
}

/* Uart2 Recieve interrupt handler */
void Uart2_Receive(UINT8 rxBuffer)
{
	uint8 indx = 0;
	static uint8 can_data_len = 0;
	static uint8 data[9] = {8, 0, 0, 0, 0, 0, 0, 0, 0};
	can_data_len++;
	data[can_data_len] = rxBuffer;
	
	if(can_data_len == 4)
	{
		Disable_Interrupt(INT_UART2);
		can_data_len = 0;
		Load_CAN_MB (0, 1, data);//Load the data to the buffer of Channel 0 buffer 1
		Transmit_CAN_MB(0, 1);//Transmit the frame of Channel 0 buffer 1;
		//if(retval)
			//printf("Tx error\n");

		for(indx = 1; indx < 9; indx++)
		{
			data[indx] = 0;
		}
		Led_toggle();
		//delay_ms(10);
		Enable_Interrupt(INT_UART2);
	}
}

/* Initializing UART and  interrupt handler*/ 
void Initialize_UART()
{
	UART_Init();
	Uart_SetCallback(Uart2_Receive); 
}

/* Initializing CAN and interrupt handler*/
uint8 Initialize_CAN()
{
	uint8 CAN_status[3];
	uint8 err_status;
	
	MSCAN_ModuleEn();
	GPIOB_PDDR |= (1<<16); //(Workaround for new kinteis revision(schematic D))to enable STB mode in MC33901 (CAN controller chip)
	GPIOB_PCOR = (1<<16);

	Init_CAN(0, FAST); //Initialize msCAN channel 0 and switch without completing current transmission(FAST) 
	 
	Config_CAN_MB (0, 1, TXDF, 1); //Channel: 0, Buffer/Message object: 1, ID: 0x81, TX ----> ECU
	Config_CAN_MB (0, 2, TXDF, 2); //Channel: 0, Buffer/Message object: 2, ID: 0x82, TX ----> ECU
	//Config_CAN_MB (0, 0, RXDF, 0); //Channel: 0, Buffer/Message object: 0, ID: 0x80, RX <---- ECU
	
    while ((CAN_status[0] & SYNCH) == 0) //Wait for msCAN channel 0 synchronized to CAN Bus
    {	
        err_status = Check_CAN_Status(0, CAN_status);
    }
    if(err_status == ERR_OK)
    	return ERR_OK;
    else 
    	return err_status;
}

/* Enable UART interrupt */
void Enable_Interrupt(UINT8 vector_number)
{
	vector_number= vector_number - 16;
	/* Set the ICPR and ISER registers accordingly */
	NVIC_ICPR |= 1 << (vector_number%32);
	NVIC_ISER |= 1 << (vector_number%32);
}

/* Disable UART interrupt */
void Disable_Interrupt(UINT8 vector_number)
{
	vector_number= vector_number-16;
	
	/* Clear the ICPR and ISER registers accordingly */
	NVIC_ICPR &= 1 << ~(vector_number%32);
	NVIC_ISER &= 1 << ~(vector_number%32);

}

void delay_ms(UINT16 delay)
{
	UINT32 i=0;
	while (i<delay*1333)
	{ i++; }	
}

void Led_init()
{
	GPIOA_PDDR|=1 <<16; /* Set pin in port PTC0 as output*/
	GPIOA_PDDR|=1 <<17;
	GPIOA_PDDR|=1 <<18;
	GPIOA_PDDR|=1 <<19;
}

void Led_toggle()
{
	static int i = 16;
	GPIOA_PDOR ^= 1<<i++;
	if(i == 20)
		i = 16;
}
