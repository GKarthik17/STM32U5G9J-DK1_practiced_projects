/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
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
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include "stdint.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define STACK_SIZE 1024
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
UART_HandleTypeDef huart1;

TX_SEMAPHORE semaphore1;
TX_MUTEX mutex1;
TX_QUEUE queue1;

TX_THREAD thread1, thread2;
uint16_t Stack2[STACK_SIZE], Stack1[STACK_SIZE];
ULONG Queue[10];
uint8_t count = 0;
uint8_t buff1[] = "Hello";
uint8_t buff2[] = " & count - ";
uint8_t buff3[] = "\r\n";
uint8_t buff4[20];
uint8_t buff5[3];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void *MyThread1(UINT arg);
void *MyThread2(UINT arg);

/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  /* USER CODE BEGIN App_ThreadX_MEM_POOL */

  /* USER CODE END App_ThreadX_MEM_POOL */
  /* USER CODE BEGIN App_ThreadX_Init */
  tx_thread_create(&thread1, "Mythread1", MyThread1, 0x1234, Stack1, STACK_SIZE, 4, 0, TX_NO_TIME_SLICE, TX_AUTO_START);
  tx_thread_create(&thread2, "Mythread2", MyThread2, 0x1234, Stack2, STACK_SIZE, 4, 0, TX_NO_TIME_SLICE, TX_AUTO_START);
  tx_semaphore_create(&semaphore1, "Semaphore1", 0);
  tx_mutex_create(&mutex1, "Mutex1", TX_NO_INHERIT);
  tx_queue_create(&queue1, "Queue1", 4, Queue, sizeof(Queue));
  /* USER CODE END App_ThreadX_Init */

  return ret;
}

  /**
  * @brief  Function that implements the kernel's initialization.
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN Before_Kernel_Start */

  /* USER CODE END Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN Kernel_Start_Error */

  /* USER CODE END Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */
void *MyThread1(UINT arg)
{
	  while(1){
		  tx_semaphore_get(&semaphore1, TX_WAIT_FOREVER);
		  tx_queue_send(&queue1, buff1, 10);
		  tx_mutex_get(&mutex1, TX_NO_WAIT);
		  count++;
		  tx_mutex_put(&mutex1);
	  }
}

void *MyThread2(UINT arg)
{

	HAL_UART_Init(&huart1);
	while(1){
		tx_queue_receive(&queue1, buff4, TX_WAIT_FOREVER);
		HAL_UART_Transmit(&huart1, buff4, strlen(buff4), 100);
		tx_thread_sleep(10);
		HAL_UART_Transmit(&huart1, buff2, strlen(buff2), 100);
		tx_thread_sleep(10);
		itoa(count, buff5, 10);
		HAL_UART_Transmit(&huart1, buff5, 1, 100);
		tx_thread_sleep(10);
		HAL_UART_Transmit(&huart1, buff3, strlen(buff3), 100);
		tx_thread_sleep(100);
	}
}
/* USER CODE END 1 */





