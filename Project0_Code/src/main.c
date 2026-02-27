/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wwrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/*
FreeRTOS is a market leading RTOS from Real Time Engineers Ltd. that supports
31 architectures and receives 77500 downloads a year. It is professionally
developed, strictly quality controlled, robust, supported, and free to use in
commercial products without any requirement to expose your proprietary source
code.

This simple FreeRTOS demo does not make use of any IO ports, so will execute on
any Cortex-M3 of Cortex-M4 hardware.  Look for TODO markers in the code for
locations that may require tailoring to, for example, include a manufacturer
specific header file.

This is a starter project, so only a subset of the RTOS features are
demonstrated.  Ample source comments are provided, along with web links to
relevant pages on the http://www.FreeRTOS.org site.

Here is a description of the project's functionality:

The main() Function:
main() creates the tasks and software timers described in this section, before
starting the scheduler.

The Queue Send Task:
The queue send task is implemented by the prvQueueSendTask() function.
The task uses the FreeRTOS vTaskDelayUntil() and xQueueSend() API functions to
periodically send the number 100 on a queue.  The period is set to 200ms.  See
the comments in the function for more details.
http://www.freertos.org/vtaskdelayuntil.html
http://www.freertos.org/a00117.html

The Queue Receive Task:
The queue receive task is implemented by the prvQueueReceiveTask() function.
The task uses the FreeRTOS xQueueReceive() API function to receive values from
a queue.  The values received are those sent by the queue send task.  The queue
receive task increments the ulCountOfItemsReceivedOnQueue variable each time it
receives the value 100.  Therefore, as values are sent to the queue every 200ms,
the value of ulCountOfItemsReceivedOnQueue will increase by 5 every second.
http://www.freertos.org/a00118.html

An example software timer:
A software timer is created with an auto reloading period of 1000ms.  The
timer's callback function increments the ulCountOfTimerCallbackExecutions
variable each time it is called.  Therefore the value of
ulCountOfTimerCallbackExecutions will count seconds.
http://www.freertos.org/RTOS-software-timer.html

The FreeRTOS RTOS tick hook (or callback) function:
The tick hook function executes in the context of the FreeRTOS tick interrupt.
The function 'gives' a semaphore every 500th time it executes.  The semaphore
is used to synchronise with the event semaphore task, which is described next.

The event semaphore task:
The event semaphore task uses the FreeRTOS xSemaphoreTake() API function to
wait for the semaphore that is given by the RTOS tick hook function.  The task
increments the ulCountOfReceivedSemaphores variable each time the semaphore is
received.  As the semaphore is given every 500ms (assuming a tick frequency of
1KHz), the value of ulCountOfReceivedSemaphores will increase by 2 each second.

The idle hook (or callback) function:
The idle hook function queries the amount of free FreeRTOS heap space available.
See vApplicationIdleHook().

The malloc failed and stack overflow hook (or callback) functions:
These two hook functions are provided as examples, but do not contain any
functionality.
*/

/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include "stm32f4_discovery.h"
/* Kernel includes. */
#include "stm32f4xx.h"
#include "../FreeRTOS_Source/include/FreeRTOS.h"
#include "../FreeRTOS_Source/include/queue.h"
#include "../FreeRTOS_Source/include/semphr.h"
#include "../FreeRTOS_Source/include/task.h"
#include "../FreeRTOS_Source/include/timers.h"
#include "../src/middleware.h"

#include <stdbool.h>


/*-----------------------------------------------------------*/
#define mainQUEUE_LENGTH 100

#define amber  	0
#define green  	1
#define red  	2
#define blue  	3

#define amber_led	LED3
#define green_led	LED4
#define red_led		LED5
#define blue_led	LED6

#define RED_BIT (1 << 0)
#define GREEN_BIT (1 << 1) //useless I think
#define AMBER_BIT (1 << 2)

#define BUFFER_SIZE 100



typedef enum {
	RED_STATE,
	GREEN_STATE,
	AMBER_STATE
} TrafficState;

#define SPAWN_BIT (1UL << 0)
#define ADVANCE_LIGHT_BIT (1UL << 1)

static TimerHandle_t xLightTimer;
static TimerHandle_t xSpawnTimer;

static TickType_t initialSpawnDuration;
static TickType_t initialRedDuration;




/*
 * TODO: Implement this function for any hardware specific clock configuration
 * that was not already performed before main() was called.
 */
static void prvSetupHardware( void );

QueueHandle_t xTrafficLoadQueue;
/*
 * The queue send and receive tasks as described in the comments at the top of
 * this file.
 */
/*
static void Manager_Task( void *pvParameters );
static void Blue_LED_Controller_Task( void *pvParameters );
static void Green_LED_Controller_Task( void *pvParameters );
static void Red_LED_Controller_Task( void *pvParameters );
static void Amber_LED_Controller_Task( void *pvParameters );
*/
static void Traffic_Load_Task(void *pvParameters);
static void Traffic_Controller_Task(void *pvParameters);
static void vSpawn_Callback(TimerHandle_t xSpawnTimer);
static void vTraffic_Light_Callback(TimerHandle_t xLightTimer);
TickType_t GetLightDuration(TrafficState lightState, uint16_t trafficLevel);
void AdvanceTrafficLight(TrafficState *state);
void RenderRoad(uint8_t leds[], uint8_t left[8], uint8_t right[8]);
void UpdateRoadAndIntersection(uint8_t inter[3], uint8_t left[8], uint8_t right[8], bool spawnCar, TrafficState state);
void ShiftLEDArray(uint8_t leds[], int count);
void RenderIntersection(uint8_t leds[], uint8_t intersection[3]);
void RenderTrafficLight(uint8_t leds[], TrafficState state);
void ConvertLEDToBytes(uint8_t leds[]);

TaskHandle_t xTraffic_Controller_Task;
TaskHandle_t xTraffic_Load_Task;

typedef enum {
	TRAFFIC_LOW = 0,
	TRAFFIC_MID,
	TRAFFIC_HIGH
} TrafficLevelBand;



/*-----------------------------------------------------------*/

int main(void)
{


	/* Configure the system ready to run the demo.  The clock configuration
	can be done here if it was not done before main() was called. */
	prvSetupHardware();



	xTrafficLoadQueue = xQueueCreate(10, sizeof(uint16_t)); //Sends Traffic Load from ADC
																   //to Traffic Controller Task


	initialSpawnDuration = pdMS_TO_TICKS(500); //Set on RED to start. Just like
											   //Traffic Controller Task
	initialRedDuration = pdMS_TO_TICKS(1000);

	xLightTimer = xTimerCreate(
			"Light Timer",
			pdMS_TO_TICKS(initialRedDuration),  //Set on Red to start
			pdTRUE,
			NULL,
			vTraffic_Light_Callback
			);

	xSpawnTimer = xTimerCreate(
			"Spawn Timer",
			pdMS_TO_TICKS(initialSpawnDuration),
			pdTRUE,
			NULL,
			vSpawn_Callback
			);




	xTaskCreate(Traffic_Load_Task, "TrafficLoad", 512, NULL, 1, &xTraffic_Load_Task);



	xTaskCreate(Traffic_Controller_Task, "Traffic", 1024, NULL, 2, &xTraffic_Controller_Task);


	/* Start the tasks and timer running. */
	xTimerStart(xLightTimer,0);
	xTimerStart(xSpawnTimer, 0);
	vTaskStartScheduler();

	return 0;
}



/*-------------------------------------------------------*/


static void Traffic_Load_Task(void *pvParameters)
{
	static TrafficLevelBand currentBand = -1;
	while(1)
	{
		TickType_t newPeriod;
		TrafficLevelBand newBand;
		if (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
		{
			printf("Load Task Alive");
			uint16_t trafficLevel = ADC_GetConversionValue(ADC1);
			xQueueSend(xTrafficLoadQueue, &trafficLevel, 0);


				// Send traffic load to Traffic_Controller_Task for changing the traffic light duration.


				// Change the period of the Spawn Callback with Traffic Level

				if (trafficLevel < 1365)
				{
					newPeriod = pdMS_TO_TICKS(500);
					newBand = TRAFFIC_LOW;


				}
				else if (trafficLevel < 2730)
				{
					newPeriod = pdMS_TO_TICKS(800);
					newBand = TRAFFIC_MID;


				}
				else
				{
					newPeriod = pdMS_TO_TICKS(1500);
					newBand = TRAFFIC_HIGH;


				}

				if (newBand != currentBand)
				{
					currentBand = newBand;
					xTimerChangePeriod(xSpawnTimer, newPeriod, 0);
				}




			}
			vTaskDelay(pdMS_TO_TICKS(10));




	}
}

/*------------------------------------------------------*/

static void Traffic_Controller_Task(void *pvParameters)
{
	(void) pvParameters;
	TrafficState state = RED_STATE;
	uint8_t leds[22] = {0};
	uint8_t roadLeft[8] = {0};
	uint8_t roadRight[8] = {0};
	uint8_t intersection[3] = {0};
	bool spawnCar;
	uint16_t trafficLevel;


	while(1)
	{
		if (xQueueReceive(xTrafficLoadQueue, &trafficLevel, pdMS_TO_TICKS(1000)) == pdTRUE){
			printf("Received: %u\n", trafficLevel);//MAX DELAY 1000
		}
 // May need to dereference



			//See if it is time to spawn a car or change the light
			uint32_t bits = 0;

			xTaskNotifyWait(0, 0xFFFFFFFF, &bits, portMAX_DELAY);

			if (bits & (1 << SPAWN_BIT))
			{
				spawnCar = true;
			}
			if (bits & (1 << ADVANCE_LIGHT_BIT))
			{

				AdvanceTrafficLight(&state);
				TickType_t nextPeriod = GetLightDuration(state, trafficLevel);
				xTimerChangePeriod(xLightTimer, nextPeriod, 0);
			}

			//Updates state of the road

			UpdateRoadAndIntersection(intersection, roadLeft, roadRight, spawnCar, state);

			spawnCar = false;

			//Make spawnCar false again after updating the roadAndIntersection


			//Sets the state of the road to the LED display

			RenderRoad(leds, roadLeft, roadRight);
			RenderIntersection(leds, intersection);
			RenderTrafficLight(leds, state);

			ConvertLEDToBytes(leds);




			vTaskDelay(pdMS_TO_TICKS(50));
	}

}

/*-----------------------------------------------------------*/

static void vSpawn_Callback(TimerHandle_t xSpawnTimer)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	xTaskNotifyFromISR(xTraffic_Controller_Task, (1 << SPAWN_BIT), eSetBits, &xHigherPriorityTaskWoken);

	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/*-----------------------------------------------------------*/

static void vTraffic_Light_Callback(TimerHandle_t xLightTimer)
{

	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xTaskNotifyFromISR(xTraffic_Controller_Task, (1 << ADVANCE_LIGHT_BIT), eSetBits, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

}


/*-----------------------------------------------------------*/

void RenderTrafficLight(uint8_t leds[], TrafficState state)
{
	leds[10] = (state == RED_STATE);
	leds[9] = (state == AMBER_STATE);
	leds[8] = (state == GREEN_STATE);

	//correct led numbers
}

void RenderIntersection(uint8_t leds[], uint8_t intersection[3])
{
	leds[11] = intersection[0];
	leds[12] = intersection[1];
	leds[13] = intersection[2];

	//correct led numbers

}

void RenderRoad(uint8_t leds[], uint8_t left[8], uint8_t right[8])
{

	leds[3] = right[0];
	leds[2] = right[1];
	leds[1] = right[2];
	leds[0] = right[3];

	leds[4] = right[6];
	leds[5] = right[5];
	leds[6] = right[4];

	//correct for right side of road

	//led [7] is q0 on second shift register.

	leds[7] = 0;


	//LED [7] is always 0 because it is the output of the chain.


	leds[14] = left[3];
	leds[15] = left[2];
	leds[16] = left[1];
	leds[17] = left[0];

	leds[18] = left[5];
	leds[19] = left[4];
	leds[20] = left[6];
	leds[21] = left[7];

	//moved to next shift register;





}

/*-----------------------------------------------------------*/

void ConvertLEDToBytes(uint8_t leds[])
{
	uint8_t sr1 = 0;
	uint8_t sr2 = 0;
	uint8_t sr3 = 0;

	for (int i = 0; i < 7; i++)
	{
		sr1 |= (leds[i] & 1) << i;

	}
	for (int i = 0; i < 8; i++)
	{
		sr2 |= (leds[7+i] & 1) << i;

	}
	for (int i = 0; i < 7; i++)
	{

		sr3 |= (leds[15+i] & 1) << i;

	}



	//MSB first
	shiftByte(sr3);
	shiftByte(sr2);
	shiftByte(sr1);
}

/*-----------------------------------------------------------*/

void UpdateRoadAndIntersection(uint8_t inter[3], uint8_t left[8], uint8_t right[8], bool spawnCar, TrafficState state)
{
	//Update intersection && Road.

	//Doesn't do anything to the lights.
	//The lights are rendered from the traffic light state in the Traffic Controller Task

	//update right side of road first
	right[7] = 0;
	for (int i = 7; i > 0; i--)
	{
		right[i] = right[i-1];

	}
	right[0] = 0;

	//update intersection and determine if entry is allowed
	//in the intersection

	if (inter[2] == 1)
	{
		right[0] = 1;
	}
	inter[2] = inter[1];
	inter[1] = inter[0];
	inter[0] = 0;

	bool entryAllowed =
			(state == GREEN_STATE) &&
			(inter[0]==0);


	// Move into the intersection and update left side of road.
	if (entryAllowed && left[7] == 1)
	{
		inter[0] = 1;
		left[7] = 0;
	}
	else if (!entryAllowed && left[7] == 1)
	{
		//left[7] stays where it is.
	}
	else
	{
		//Left[7] == 0. Left side can go on normally
	}

	for (int i = 7; i > 0; i--)
	{
		if (left[i] == 0)	//Only update the left side of the road spaces if there is no car in front
		{
			left[i] = left[i-1];
		}
	}
	if (spawnCar == true && left[0]!=1)
	{
		left[0] = 1;
	}




}

TickType_t GetLightDuration(TrafficState lightState, uint16_t trafficLevel)
{
	TickType_t newPeriod;

	if (lightState == GREEN_STATE)
	{
		//Yellow Light next. Constant 500ms
		newPeriod = pdMS_TO_TICKS(500);
	}
	else if (lightState == AMBER_STATE) // red light next
	{

		if (trafficLevel < 1365)
			{
				//High Traffic. red gets shorter
				newPeriod = pdMS_TO_TICKS(1000);

			}
			else if (trafficLevel < 2730)
			{
				// Low Traffic. Red gets longer
				newPeriod = pdMS_TO_TICKS(3000);

			}
			else
			{
				// middle red is kinda short
				newPeriod = pdMS_TO_TICKS(1500);

			}
	}
	else // light is red. Green goes next
	{
		if (trafficLevel < 1365)
			{
				//High Traffic. Green gets longer
				newPeriod = pdMS_TO_TICKS(3000);

			}
			else if (trafficLevel < 2730)
			{
				// Low Traffic. Green is short
				newPeriod = pdMS_TO_TICKS(1500);

			}
			else
			{
				// middle green is still short
				newPeriod = pdMS_TO_TICKS(1500);

			}

	}
	return newPeriod;
}

/*-----------------------------------------------------------*/

void AdvanceTrafficLight(TrafficState *state)
{
	switch (*state)
	{
	case RED_STATE:
		*state = GREEN_STATE;
		break;

	case GREEN_STATE:
		*state = AMBER_STATE;
		break;

	case AMBER_STATE:
		*state = RED_STATE;
		break;

	}
}


/*------EndOfMyCode------------------------------------------*/


/*-----------------------------------------------------------*/
/*
static void Manager_Task( void *pvParameters )
{
	uint16_t tx_data = amber;


	while(1)
	{

		if(tx_data == amber)
			STM_EVAL_LEDOn(amber_led);
		if(tx_data == green)
			STM_EVAL_LEDOn(green_led);
		if(tx_data == red)
			STM_EVAL_LEDOn(red_led);
		if(tx_data == blue)
			STM_EVAL_LEDOn(blue_led);

		if( xQueueSend(xQueue_handle,&tx_data,1000))
		{
			printf("Manager: %u ON!\n", tx_data);
			if(++tx_data == 4)
				tx_data = 0;
			vTaskDelay(1000);
		}
		else
		{
			printf("Manager Failed!\n");
		}
	}
}



static void Blue_LED_Controller_Task( void *pvParameters )
{
	uint16_t rx_data;
	while(1)
	{
		if(xQueueReceive(xQueue_handle, &rx_data, 500))
		{
			if(rx_data == blue)
			{
				vTaskDelay(250);
				STM_EVAL_LEDOff(blue_led);
				printf("Blue Off.\n");
			}
			else
			{
				if( xQueueSend(xQueue_handle,&rx_data,1000))
					{
						printf("BlueTask GRP (%u).\n", rx_data); // Got wwrong Package
						vTaskDelay(500);
					}
			}
		}
	}
}



static void Green_LED_Controller_Task( void *pvParameters )
{
	uint16_t rx_data;
	while(1)
	{
		if(xQueueReceive(xQueue_handle, &rx_data, 500))
		{
			if(rx_data == green)
			{
				vTaskDelay(250);
				STM_EVAL_LEDOff(green_led);
				printf("Green Off.\n");
			}
			else
			{
				if( xQueueSend(xQueue_handle,&rx_data,1000))
					{
						printf("GreenTask GRP (%u).\n", rx_data); // Got wrong Package
						vTaskDelay(500);
					}
			}
		}
	}
}



static void Red_LED_Controller_Task( void *pvParameters )
{
	uint16_t rx_data;
	while(1)
	{
		if(xQueueReceive(xQueue_handle, &rx_data, 500))
		{
			if(rx_data == red)
			{
				vTaskDelay(250);
				STM_EVAL_LEDOff(red_led);
				printf("Red off.\n");
			}
			else
			{
				if( xQueueSend(xQueue_handle,&rx_data,1000))
					{
						printf("RedTask GRP (%u).\n", rx_data); // Got wrong Package
						vTaskDelay(500);
					}
			}
		}
	}
}




static void Amber_LED_Controller_Task( void *pvParameters )
{
	uint16_t rx_data;
	while(1)
	{
		if(xQueueReceive(xQueue_handle, &rx_data, 500))
		{
			if(rx_data == amber)
			{
				vTaskDelay(250);
				STM_EVAL_LEDOff(amber_led);
				printf("Amber Off.\n");
			}
			else
			{
				if( xQueueSend(xQueue_handle,&rx_data,1000))
					{
						printf("AmberTask GRP (%u).\n", rx_data); // Got wrong Package
						vTaskDelay(500);
					}
			}
		}
	}
}

*/
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	/* The malloc failed hook is enabled by setting
	configUSE_MALLOC_FAILED_HOOK to 1 in FreeRTOSConfig.h.

	Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues, software 
	timers, and semaphores.  The size of the FreeRTOS heap is set by the
	configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configconfigCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected.  pxCurrentTCB can be
	inspected in the debugger if the task name passed into this function is
	corrupt. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
volatile size_t xFreeStackSpace;

	/* The idle task hook is enabled by setting configUSE_IDLE_HOOK to 1 in
	FreeRTOSConfig.h.

	This function is called on each cycle of the idle task.  In this case it
	does nothing useful, other than report the amount of FreeRTOS heap that
	remains unallocated. */
	xFreeStackSpace = xPortGetFreeHeapSize();

	if( xFreeStackSpace > 100 )
	{
		/* By now, the kernel has allocated everything it is going to, so
		if there is a lot of heap remaining unallocated then
		the value of configTOTAL_HEAP_SIZE in FreeRTOSConfig.h can be
		reduced accordingly. */
	}
}
/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
	/* Ensure all priority bits are assigned as preemption priority bits.
	http://www.freertos.org/RTOS-Cortex-M3-M4.html */
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_4);

	enableClocks();
	ADC_GPIO_Initialization();
	ShiftReg_GPIO_Init();
	ADC_Initialization();

	/* TODO: Setup the clocks, etc. here, if they were not configured before
	main() was called. */
}

