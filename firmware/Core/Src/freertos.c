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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "stdio.h"
#include "semphr.h"
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

/* Private variables ----------------------------------------------------------*/
/* USER CODE BEGIN Variables */
TaskHandle_t defaultTaskHandle;
TaskHandle_t LEDTASKHandle;
TaskHandle_t TaskGIVEHandle;
TaskHandle_t TaskTAKEHandle;
TaskHandle_t myTaskHandle;
SemaphoreHandle_t xMutex;
/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void task_bug(void * pvParameters);
// void StartDefaultTask(void * argument);
// void LEDtask(void * argument);
// void StartTaskGive(void * argument);
// void StartTaskTake(void * argument);
// void StartTask(void * argument);
/* USER CODE END FunctionPrototypes */

/* USER CODE BEGIN Application */
void MX_FREERTOS_Init(void)
{
    BaseType_t ret;

    /* USER CODE BEGIN Init */
    /* USER CODE END Init */

    /* USER CODE BEGIN RTOS_MUTEX */
    xMutex = xSemaphoreCreateMutex();
    configASSERT(xMutex != NULL);
    /* USER CODE END RTOS_MUTEX */

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_QUEUES */
    /* USER CODE END RTOS_QUEUES */

    /* Create the thread(s) */

    /*
    ret = xTaskCreate(StartDefaultTask, "DefaultTask", 128, NULL, 2, &defaultTaskHandle);
    configASSERT(ret == pdPASS);

    ret = xTaskCreate(LEDtask, "LEDTask", 128, NULL, 2, &LEDTASKHandle);
    configASSERT(ret == pdPASS);

    ret = xTaskCreate(StartTaskGive, "TaskGIVE", 128, NULL, 1, &TaskGIVEHandle);
    configASSERT(ret == pdPASS);

    ret = xTaskCreate(StartTaskTake, "TaskTAKE", 128, NULL, 3, &TaskTAKEHandle);
    configASSERT(ret == pdPASS);

    ret = xTaskCreate(StartTask, "SafetyTask", 128, NULL, 1, &myTaskHandle);
    configASSERT(ret == pdPASS);
    */

    /* USER CODE BEGIN RTOS_THREADS */
    ret = xTaskCreate(task_bug, "Tache1", STACK_SIZE, (void *) TASK1_DELAY, TASK1_PRIORITY, NULL);
    configASSERT(ret == pdPASS);

    ret = xTaskCreate(task_bug, "Tache2", STACK_SIZE, (void *) TASK2_DELAY, TASK2_PRIORITY, NULL);
    configASSERT(ret == pdPASS);
    /* USER CODE END RTOS_THREADS */
}
/* USER CODE END Application */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
/*
void StartDefaultTask(void * argument)
{
    for(;;)
    {
        vTaskDelay(1);
    }
}
*/

/* USER CODE BEGIN Header_LEDtask */
/**
  * @brief  Function implementing the LEDTASK thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_LEDtask */
/*
void LEDtask(void * argument)
{
    for(;;)
    {
        HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_1);
        printf("LED toggled!\r\n");
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
*/

/* USER CODE BEGIN Header_StartTaskGive */
/**
  * @brief  Function implementing the TaskGIVE thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartTaskGive */
/*
void StartTaskGive(void * argument)
{
    uint32_t delayMs = 100;

    for(;;)
    {
        printf("taskGive : notification envoyée !\r\n");
        xTaskNotifyGive(TaskTAKEHandle);
        vTaskDelay(pdMS_TO_TICKS(delayMs));

        if (delayMs < 2000)
        {
            delayMs += 100;
        }
    }
}
*/

/* USER CODE BEGIN Header_StartTaskTake */
/**
  * @brief  Function implementing the TaskTAKE thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartTaskTake */
/*
void StartTaskTake(void * argument)
{
    for(;;)
    {
        if (ulTaskNotifyTake(pdTRUE, portMAX_DELAY) > 0)
        {
            printf("taskTake : notification reçue !\r\n");
        }
    }
}
*/

/* USER CODE BEGIN Header_StartTask */
/**
  * @brief  Function implementing the SafetyTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartTask */
/*
void StartTask(void * argument)
{
    for(;;)
    {
        printf("Sécurité : attente notification 1s...\r\n");

        if (ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(1000)) > 0)
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
}
*/

/* USER CODE BEGIN Header_task_bug */
/**
  * @brief  Function implementing the task_bug thread.
  * @param  argument: delay in ticks
  * @retval None
  */
/* USER CODE END Header_task_bug */
void task_bug(void * pvParameters)
{
    /* USER CODE BEGIN task_bug */
	TickType_t delay = (TickType_t) pvParameters;

	    for(;;)
	    {
	        xSemaphoreTake(xMutex, portMAX_DELAY);
	        printf("Je suis %s, je dors %lu ticks\r\n", pcTaskGetName(NULL), delay);
	        xSemaphoreGive(xMutex);
	        vTaskDelay(delay);
	    }
    /* USER CODE END task_bug */
}

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t IdleTaskTCB;
static StackType_t IdleTaskStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &IdleTaskTCB;
    *ppxIdleTaskStackBuffer = IdleTaskStack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/* USER CODE END Application */
