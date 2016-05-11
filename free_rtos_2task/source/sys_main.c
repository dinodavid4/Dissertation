//#include "sys_pmu.h"
#include "sys_common.h"
#include "system.h"
#include "stdint.h"
#include "os_projdefs.h"
#include "can.h"
#include "esm.h"
#include "sci.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sys_core.h"
#include "sys_vim.h"

/* Include FreeRTOS scheduler files */
#include "FreeRTOS.h"
#include "os_task.h"
#include "os_semphr.h"
#include "os_portmacro.h"

/* Include HET header file - types, definitions and function declarations for system driver */
#include "het.h"
#include "gio.h"

/* Define Task Handles */
xTaskHandle canTaskTcb;
xTaskHandle absTaskTcb;
xTaskHandle uartAbsTaskTcb;
xTaskHandle uartSteerTaskTcb;
xTaskHandle steerTaskTcb;
xTaskHandle ledTaskTcb;

#define LOW  		0
#define HIGH 		1
#define DATA_LEN	8

#define LED_TASK 0

#define UART_ABS scilinREG //Tx ABS data
#define UART_STEER sciREG //Tx steering data
#define UART_STACK_SIZE	  ( ( unsigned portSHORT ) 256 )

//#define f_HCLK (float) 180.0 // f in [MHz]; HCLK (depends on device setup)


uint8 rx_str_data[5];
uint8 rx_data[DATA_LEN];
char rxMbox1[5];
char rxMbox2[5];
int rxIntMbox1;
//int rxIntMbox2;
int wheelSpin[4] = {0};
int pubSpeed;
static int antiwheelSlip = 0;
static int sensitivity = 0;

SemaphoreHandle_t canIntRxSem;
SemaphoreHandle_t absSem;
SemaphoreHandle_t steerSem;
SemaphoreHandle_t uartAbsTxSem;
SemaphoreHandle_t uartSteerTxSem;
uint32 mailBox;

void delay_ms(unsigned int  delay);
int calculateWheelSlip(int *wheelSpin);
int calculateSteerSensitivity(int pubSpeed);


/* Task1 */
void canTask(void *pvParameters)
{
	static int countbox1 = 0;
	volatile TickType_t CANstartTime = 0;
	volatile TickType_t CANstopTime = 0;
	volatile TickType_t CANexecutionTime [50];
	int i = 0;

	while (1)
	{
		if(xSemaphoreTake(canIntRxSem, 0) == pdTRUE)
		{
			CANstartTime = xTaskGetTickCount ();

			if(mailBox == canMESSAGE_BOX2)
			{

				while(!canIsRxMessageArrived(canREG1, canMESSAGE_BOX2));
				canGetData(canREG1, canMESSAGE_BOX2, rx_data); /* Recieve Completed */
				strncpy(rxMbox1, (char *)rx_data, 4);
				//printf("Concatenated String %s\n", rxMbox1);
				rxIntMbox1 = atoi(rxMbox1);
				//printf("Converted int %d\n", rxIntMbox1);
				wheelSpin[countbox1++] = rxIntMbox1;
				if(countbox1 == 4)
				{
					if(xSemaphoreGive(absSem) == pdTRUE)
					{
						//Can happen if the semaphore is not released from CAN task, but will not
					}
					countbox1 = 0;
				}
			}
			else
			{
				while(!canIsRxMessageArrived(canREG1, canMESSAGE_BOX1));
				canGetData(canREG1, canMESSAGE_BOX1, rx_data); /* Recieve Completed */
				strncpy(rxMbox2, (char *)rx_data, 4);
				pubSpeed = (atoi(rxMbox2));

				if(xSemaphoreGive(steerSem) == pdTRUE)
				{
					//Can happen if the semaphore is not released from CAN task, but will not
				}

			}
			CANstopTime = xTaskGetTickCount ();
			CANexecutionTime[i] = CANstopTime - CANstartTime;
			i++;
			if (i == 50)
			{
				i = 0;
			}


		}
	}
}

/* Task1 */
void absTask(void *pvParameters)
{
	volatile TickType_t ABSstartTime = 0;
	volatile TickType_t ABSstopTime = 0;
	volatile TickType_t ABSexecutionTime [50];
	int i = 0;

	while(1)
	{
		if(xSemaphoreTake(absSem, 0) == pdTRUE)
		{
			ABSstartTime = xTaskGetTickCount ();
			//vTaskDelay(25);
			antiwheelSlip = calculateWheelSlip(wheelSpin);
			if(xSemaphoreGive(uartAbsTxSem) == pdTRUE)
			{
				//gioSetPort(hetPORT1, gioGetPort(hetPORT1) ^ 0x00FFF001);
				//Can happen if the semaphore is not released from CAN task, but will not
			}
			ABSstopTime = xTaskGetTickCount ();
			ABSexecutionTime[i] = ABSstopTime - ABSstartTime;
			i++;
			if (i == 50)
			{
				i = 0;
			}

		}
	}
}

/* Task1 */
void steerTask(void *pvParameters)
{
	volatile TickType_t SteerstartTime = 0;
	volatile TickType_t SteerstopTime = 0;
	volatile TickType_t SteerexecutionTime [50];
	int i = 0;

	while(1)
	{
		if(xSemaphoreTake(steerSem, 0) == pdTRUE)
		{
			SteerstartTime = xTaskGetTickCount ();
			//vTaskDelay(25);
			sensitivity = calculateSteerSensitivity(pubSpeed);
			if(xSemaphoreGive(uartSteerTxSem) == pdTRUE)
			{
				//gioSetPort(hetPORT1, gioGetPort(hetPORT1) ^ 0x00FFF001);
				//Can happen if the semaphore is not released from CAN task, but will not
			}
			SteerstopTime = xTaskGetTickCount ();
			SteerexecutionTime[i] = SteerstopTime - SteerstartTime;
			i++;
			if (i == 50)
			{
				i = 0;
			}

		}
	}
}

/* Task1 */
void uartSteerTask(void *pvParameters)
{
	volatile TickType_t UARTSteerstartTime = 0;
	volatile TickType_t UARTSteerstopTime = 0;
	volatile TickType_t UARTSteerexecutionTime [50];
	int i = 0;

	uint8 uart_steerdata[5] = {0};
    while(1)
    {
		if(xSemaphoreTake(uartSteerTxSem, 0) == pdTRUE)
		{
			UARTSteerstartTime = xTaskGetTickCount ();
			snprintf((char *)uart_steerdata, 5,"%d", sensitivity);
			sciSend(UART_STEER, 1, uart_steerdata);
			gioSetPort(hetPORT1, gioGetPort(hetPORT1) ^ 0xA2000000);
			UARTSteerstopTime = xTaskGetTickCount ();
			UARTSteerexecutionTime[i] = UARTSteerstopTime - UARTSteerstartTime;
			i++;
			if (i == 50)
			{
				i = 0;
			}

		}

    }
}

/* Task1 */
void uartAbsTask(void *pvParameters)
{
	uint8 uart_absdata[5] = {0};
	volatile TickType_t UARTABSstartTime = 0;
	volatile TickType_t UARTABSstopTime = 0;
	volatile TickType_t UARTABSexecutionTime [50];
	int i = 0;

    while(1)
    {
		if(xSemaphoreTake(uartAbsTxSem, 0) == pdTRUE)
		{
			UARTABSstartTime = xTaskGetTickCount ();
			snprintf((char *)uart_absdata, 5,"%d", antiwheelSlip);
			sciSend(UART_ABS, 4, uart_absdata);
			gioSetPort(hetPORT1, gioGetPort(hetPORT1) ^ 0x00FFF001);
			UARTABSstopTime = xTaskGetTickCount ();
			UARTABSexecutionTime[i] = UARTABSstopTime - UARTABSstartTime;
			i++;
			if (i == 50)
			{
				i = 0;
			}

		}


    }
}

void ledTask(void *pvParameters)
{
	int i = 0;
	volatile TickType_t startTime = 0;
	volatile TickType_t lastTime = 0;

    while(1)
    {
    		startTime = xTaskGetTickCount ();
			gioSetPort(hetPORT1, gioGetPort(hetPORT1) ^ 0x00FFF001);
			for(i = 0; i < 10000; i++);
			lastTime = xTaskGetTickCount ();
    }
}

void main(void)
{
	//volatile unsigned long cycles_PMU_start = 0, cycles_PMU_end = 0, cycles_PMU_measure = 0, cycles_PMU_comp = 0,
	//cycles_PMU_code = 0;
	//volatile float time_PMU_code = 0.0;


	//int i,j;
    /* Set high end timer GIO port hetPort pin direction to all output */
    gioSetDirection(hetPORT1, 0xFFFFFFFF);

	/* UART init */
	sciInit();

	/* Configuring CAN1: MB1, Msg ID-0x82 to recieve from ABS Gateway; MB2, Msg ID-0x81 to recieve from Suspension/OBD Gateway */
	canInit();

	//printf("Time %f us\n", time_PMU_code);
	//taskDISABLE_INTERRUPTS();
	//_disable_interrupt_();
	vimDisableInterrupt(16);


	canIntRxSem = xSemaphoreCreateBinary();
	if(NULL == canIntRxSem)
	{
		/* Failed to create Semaphore */
		while(1);
	}
	uartAbsTxSem = xSemaphoreCreateBinary();
	if(NULL == uartAbsTxSem)
	{
		/* Failed to create Semaphore */
		while(1);
	}
	uartSteerTxSem = xSemaphoreCreateBinary();
	if(NULL == uartSteerTxSem)
	{
		/* Failed to create Semaphore */
		while(1);
	}

	absSem = xSemaphoreCreateBinary();
	if(NULL == absSem)
	{
		/* Failed to create Semaphore */
		while(1);
	}

	steerSem = xSemaphoreCreateBinary();
	if(NULL == steerSem)
	{
		/* Failed to create Semaphore */
		while(1);
	}

    if (xTaskCreate(canTask,"CAN Task", configMINIMAL_STACK_SIZE, NULL, 1, &canTaskTcb) != pdTRUE)
    {
        /* Task could not be created */
        while(1);
    }
    if (xTaskCreate(absTask,"ABS Task", UART_STACK_SIZE, NULL, 1, &absTaskTcb) != pdTRUE)
    {
        /* Task could not be created */
        while(1);
    }
    if (xTaskCreate(steerTask,"STEER Task", configMINIMAL_STACK_SIZE, NULL, 1, &steerTaskTcb) != pdTRUE)
    {
        /* Task could not be created */
        while(1);
    }

    if (xTaskCreate(uartAbsTask,"UART ABS Task", UART_STACK_SIZE, NULL, 1, &uartAbsTaskTcb) != pdTRUE)
    {
        /* Task could not be created */
        while(1);
    }

    if (xTaskCreate(uartSteerTask,"UART Steer Task", UART_STACK_SIZE, NULL, 1, &uartSteerTaskTcb) != pdTRUE)
    {
        /* Task could not be created */
        while(1);
    }

#if LED_TASK
    if (xTaskCreate(ledTask,"LED Task", configMINIMAL_STACK_SIZE, NULL, 1, &ledTaskTcb) != pdTRUE)
    {
        /* Task could not be created */
        while(1);
    }
#endif

    //taskENABLE_INTERRUPTS();
	//_enable_interrupt_();
    vimEnableInterrupt(16, SYS_IRQ);

#if 0
	_pmuInit_();
	_pmuEnableCountersGlobal_();
	_pmuSetCountEvent_(pmuCOUNTER0, PMU_CYCLE_COUNT); // PMU_INST_ARCH_EXECUTED

	_pmuResetCounters_();
	_pmuStartCounters_(pmuCOUNTER0);
	cycles_PMU_start = _pmuGetEventCount_(pmuCOUNTER0);

	/* Place the task here to measure the time */

	//for(i = 0 ; i < 100000; i++);
	for(i = 0 ; i < 10000; i++)
	{
		for(j = 0 ; j < 10000; j++);
	}
/*
	for(i = 0 ; i < 10000; i++)
	{
		for(j = 0 ; j < 10000; i++);
	}
*/
	_pmuStopCounters_(pmuCOUNTER0);
	cycles_PMU_end = _pmuGetEventCount_(pmuCOUNTER0);
	cycles_PMU_measure = cycles_PMU_end - cycles_PMU_start;

	/* Measure the time compensation */
	_pmuResetCounters_();
	_pmuStartCounters_(pmuCOUNTER0);
	cycles_PMU_start = _pmuGetEventCount_(pmuCOUNTER0);

	_pmuStopCounters_(pmuCOUNTER0);
	cycles_PMU_end = _pmuGetEventCount_(pmuCOUNTER0);
	cycles_PMU_comp = cycles_PMU_end - cycles_PMU_start;

	/* Calculate Time */
	cycles_PMU_code = cycles_PMU_measure - cycles_PMU_comp;
	time_PMU_code = cycles_PMU_code / (f_HCLK); // time_code [us], f_HCLK [MHz]
	//time_PMU_code = cycles_PMU_code / (f_HCLK * loop_Count_max); //
#endif
    /* Start Scheduler */
    vTaskStartScheduler();

    /* Run forever */
    while(1);
/* USER CODE END */
}

/* can interrupt notification */
void canMessageNotification(canBASE_t *node, uint32 messageBox)
{

	//static int sensitivity = 0;
	//printf("Intrpt\n");
	if(node == canREG1)
	{
		if(messageBox == canMESSAGE_BOX1 || messageBox == canMESSAGE_BOX2)
		{
			//vimDisableInterrupt(16);
			mailBox = messageBox;
			if(xSemaphoreGiveFromISR(canIntRxSem, NULL) == pdTRUE)
			{

			}
		}
	}
}

#if 0
			while(!canIsRxMessageArrived(canREG1, canMESSAGE_BOX1));
			canGetData(canREG1, canMESSAGE_BOX1, rx_data); /* Recieve Completed */
			strncpy(rxMbox1, (char *)rx_data, 4);
			//printf("Concatenated String %s\n", rxMbox1);
			rxIntMbox1 = atoi(rxMbox1);
			//printf("Converted int %d\n", rxIntMbox1);
			wheelSpin[countbox1++] = rxIntMbox1;
			if(countbox1 == 4)
			{
				countbox1 = 0;
				antiwheelSlip = calculateWheelSlip(wheelSpin);
				//printf("Brake Integer %d\n", brake);
				snprintf((char *)rx_str_data, 5,"%d", antiwheelSlip);
				//printf("Concatenated Brake String %s\n", rx_str_data);
				sciSend(UART_ABS, 4, rx_str_data);
				gioSetPort(hetPORT1, gioGetPort(hetPORT1) ^ 0x00FFF001);/* Toggle HET pin 0.                                    */
				//vTaskDelay(50);
			}
			vimEnableInterrupt(16, SYS_IRQ);
		}

		if(messageBox == canMESSAGE_BOX2)
		{
			vimDisableInterrupt(16);
			while(!canIsRxMessageArrived(canREG1, canMESSAGE_BOX2));
			canGetData(canREG1, canMESSAGE_BOX2, rx_data); /* Recieve Completed */
			strncpy(rxMbox2, (char *)rx_data, 4);
			rxIntMbox2 = (atoi(rxMbox2));
			pubSpeed = rxIntMbox2;
			//if(countbox2 == 4)
			{
				//countbox2 = 0;
				sensitivity = calculateSteerSensitivity(pubSpeed);
				//printf("Brake Integer %d\n", brake);
				snprintf((char *)rx_str_data, 5,"%d", sensitivity);
				//printf("Concatenated Brake String %s\n", rx_str_data);
				sciSend(UART_STEER, 1, rx_str_data);
				gioSetPort(hetPORT1, gioGetPort(hetPORT1) ^ 0xA2000000);/* Toggle HET pin 0.                                    */
				//vTaskDelay(50);
			}
			vimEnableInterrupt(16, SYS_IRQ);
		}
#endif

int calculateSteerSensitivity(int pubSpeed)
{
	float sensitivity;

	if(pubSpeed < 20)
		sensitivity = 2;//high sensitivity
	else
		sensitivity = 1;//low sensitivity

	return sensitivity;
}

int calculateWheelSlip(int *wheelSpin)
{
	int slip;
	float wheelradius[4] = {0.330600, 0.330600, 0.327600, 0.327600};
	int i;
	for(i = 0; i < 4; i++)
	{
		slip += wheelSpin[i] * wheelradius[i];
	}
	slip = slip/4;
	return slip;
}
