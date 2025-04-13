/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "stdio.h"
#include "semphr.h"  // <- nécessaire pour xSemaphoreCreateBinary, xSemaphoreTake, etc.

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern UART_HandleTypeDef huart2;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
SemaphoreHandle_t xSemaphore;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId LEDTASKHandle;
osThreadId TaskGIVEHandle;
osThreadId TaskTAKEHandle;
osThreadId myTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void LEDtask(void const * argument);
void StartTaskGive(void const * argument);
void SartTaskTakes(void const * argument);
void StartTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
	  // Créer le sémaphore binaire
	  xSemaphore = xSemaphoreCreateBinary();

	  if (xSemaphore != NULL)
	  {
	    // Création des deux nouvelles tâches
		  osThreadDef(TaskTAKE, SartTaskTakes, osPriorityHigh, 0, 128);
		  TaskTAKEHandle = osThreadCreate(osThread(TaskTAKE), NULL);

		  osThreadDef(TaskGIVE, StartTaskGive, osPriorityBelowNormal, 0, 128);
		  TaskGIVEHandle = osThreadCreate(osThread(TaskGIVE), NULL);
	  }

  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of LEDTASK */
  osThreadDef(LEDTASK, LEDtask, osPriorityNormal, 0, 128);
  LEDTASKHandle = osThreadCreate(osThread(LEDTASK), NULL);

  /* definition and creation of TaskGIVE */
  osThreadDef(TaskGIVE, StartTaskGive, osPriorityIdle, 0, 128);
  TaskGIVEHandle = osThreadCreate(osThread(TaskGIVE), NULL);

  /* definition and creation of TaskTAKE */
  osThreadDef(TaskTAKE, SartTaskTakes, osPriorityIdle, 0, 128);
  TaskTAKEHandle = osThreadCreate(osThread(TaskTAKE), NULL);

  /* definition and creation of myTask */
  osThreadDef(myTask, StartTask, osPriorityIdle, 0, 128);
  myTaskHandle = osThreadCreate(osThread(myTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_LEDtask */
/**
* @brief Function implementing the LEDTASK thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LEDtask */
void LEDtask(void const * argument)
{
  /* USER CODE BEGIN LEDtask */
  /* Infinite loop */
  for(;;)
  {
	    HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_1);  // Clignoter PI1
	    printf("LED toggled!\r\n");             // Message série (si printf bien redirigé)
	    vTaskDelay(100 / portTICK_PERIOD_MS);   // Attente de 100 ms
  }
  /* USER CODE END LEDtask */
}

/* USER CODE BEGIN Header_StartTaskGive */
/**
* @brief Function implementing the TaskGIVE thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskGive */
void StartTaskGive(void const * argument)
{
  /* USER CODE BEGIN StartTaskGive */
	uint32_t delayMs = 100;  // départ à 100 ms
  /* Infinite loop */
  for(;;)
  {
	    printf("taskGive : je vais donner le sémaphore après %lu ms\r\n", delayMs);
	    vTaskDelay(delayMs / portTICK_PERIOD_MS);

	    xSemaphoreGive(xSemaphore);
	    printf("taskGive : sémaphore donné\r\n");

	    if (delayMs < 2000) {
	      delayMs += 100;
	    }
  }
  /* USER CODE END StartTaskGive */
}

/* USER CODE BEGIN Header_SartTaskTakes */
/**
* @brief Function implementing the TaskTAKE thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_SartTaskTakes */
void SartTaskTakes(void const * argument)
{
  /* USER CODE BEGIN SartTaskTakes */
  /* Infinite loop */
  for(;;)
  {
	    printf("taskTake : en attente du sémaphore\r\n");
	    if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE)
	    {
	      printf("taskTake : sémaphore pris avec succès\r\n");
	    }
  }
  /* USER CODE END SartTaskTakes */
}

/* USER CODE BEGIN Header_StartTask */
/**
* @brief Function implementing the myTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask */
void StartTask(void const * argument)
{
  /* USER CODE BEGIN StartTask */
  /* Infinite loop */
  for(;;)
  {
	    printf("taskTake : en attente du sémaphore (timeout 1s)...\r\n");

	    // Timeout : 1000 ms
	    if (xSemaphoreTake(xSemaphore, 1000 / portTICK_PERIOD_MS) == pdTRUE)
	    {
	      printf("taskTake : sémaphore pris avec succès\r\n");
	    }
	    else
	    {
	      printf("ERREUR : sémaphore non reçu, reset du système !\r\n");
	      HAL_Delay(100); // petit délai pour envoyer le message
	      NVIC_SystemReset(); // 🔁 reset logiciel STM32
	    }
  }
  /* USER CODE END StartTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

