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
// #include "semphr.h" // Plus nécessaire pour notifications
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern UART_HandleTypeDef huart2;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define STACK_SIZE 256
#define TASK1_PRIORITY 1
#define TASK2_PRIORITY 2
#define TASK1_DELAY 1
#define TASK2_DELAY 2
/* USER CODE END PD */

/* Private variables -----------------------------------------------------------*/
/* USER CODE BEGIN Variables */
// QueueHandle_t myQueue; // Plus utilisé, car on utilise uniquement les notifications
/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId LEDTASKHandle;
osThreadId TaskGIVEHandle;
osThreadId TaskTAKEHandle;
osThreadId myTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
// void task_bug(void * pvParameters); // Commenté car pas nécessaire
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void LEDtask(void const * argument);
void StartTaskGive(void const * argument);
void StartTaskTake(void const * argument);
void StartTask(void const * argument);

void MX_FREERTOS_Init(void);

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  // BaseType_t ret; // Plus utilisé
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_QUEUES */
  // myQueue = xQueueCreate(10, sizeof(uint32_t));
  // if (myQueue == NULL) {
  //     printf("Erreur : Queue non créée !\r\n");
  // }
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  osThreadDef(LEDTASK, LEDtask, osPriorityNormal, 0, 128);
  LEDTASKHandle = osThreadCreate(osThread(LEDTASK), NULL);

  osThreadDef(TaskGIVE, StartTaskGive, osPriorityBelowNormal, 0, 128);
  TaskGIVEHandle = osThreadCreate(osThread(TaskGIVE), NULL);

  osThreadDef(TaskTAKE, StartTaskTake, osPriorityHigh, 0, 128);
  TaskTAKEHandle = osThreadCreate(osThread(TaskTAKE), NULL);

  osThreadDef(myTask, StartTask, osPriorityIdle, 0, 128);
  myTaskHandle = osThreadCreate(osThread(myTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  // ret = xTaskCreate(task_bug, "Tache 1", STACK_SIZE, (void *) TASK1_DELAY, TASK1_PRIORITY, NULL);
  // configASSERT(pdPASS == ret);

  // ret = xTaskCreate(task_bug, "Tache 2", STACK_SIZE, (void *) TASK2_DELAY, TASK2_PRIORITY, NULL);
  // configASSERT(pdPASS == ret);
  /* USER CODE END RTOS_THREADS */
}

/* USER CODE BEGIN Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_LEDtask */
void LEDtask(void const * argument)
{
  /* USER CODE BEGIN LEDtask */
  for(;;)
  {
    HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_1);
    printf("LED toggled!\r\n");
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
  /* USER CODE END LEDtask */
}

/* USER CODE BEGIN Header_StartTaskGive */
void StartTaskGive(void const * argument)
{
  /* USER CODE BEGIN StartTaskGive */
  uint32_t delayMs = 100;

  for(;;)
  {
    printf("taskGive : notification envoyée !\r\n");

    xTaskNotifyGive(TaskTAKEHandle);

    vTaskDelay(delayMs / portTICK_PERIOD_MS);

    if (delayMs < 2000)
    {
      delayMs += 100;
    }
  }
  /* USER CODE END StartTaskGive */
}

/* USER CODE BEGIN Header_StartTaskTake */
void StartTaskTake(void const * argument)
{
  /* USER CODE BEGIN StartTaskTake */
  for(;;)
  {
    if (ulTaskNotifyTake(pdTRUE, portMAX_DELAY) > 0)
    {
      printf("taskTake : notification reçue !\r\n");
    }
  }
  /* USER CODE END StartTaskTake */
}

/* USER CODE BEGIN Header_StartTask */
void StartTask(void const * argument)
{
  /* USER CODE BEGIN StartTask */
  for(;;)
  {
    printf("Sécurité : attente notification 1s...\r\n");

    if (ulTaskNotifyTake(pdTRUE, 1000 / portTICK_PERIOD_MS) > 0)
    {
      printf("Sécurité : notification reçue !\r\n");
    }
    else
    {
      printf("ERREUR : timeout sans notification -> RESET !\r\n");
      HAL_Delay(100);
      NVIC_SystemReset();
    }
  }
  /* USER CODE END StartTask */
}

/*
// USER CODE BEGIN Header_task_bug
void task_bug(void * pvParameters)
{
  TickType_t delay = (TickType_t) pvParameters;

  for(;;)
  {
    printf("Je suis %s, je dors %lu ticks\r\n", pcTaskGetName(NULL), delay);
    vTaskDelay(delay);
  }
}
// USER CODE END Header_task_bug
*/

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/* USER CODE END Application */

