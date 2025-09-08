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
#include "stdbool.h"
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
uint16_t Stack1[STACK_SIZE], Stack2[STACK_SIZE];
TX_THREAD thread1, thread2;
TX_SEMAPHORE sem1, sem2;

const  uint8_t buff[] = "Button Pressed!\r\n";
//volatile bool flag;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void mythread1(UINT);
void mythread2(UINT);
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
  tx_thread_create(&thread1, "thread1", mythread1, 0x1234, Stack1, STACK_SIZE, 5, 0, TX_NO_TIME_SLICE, TX_AUTO_START);
  tx_thread_create(&thread2, "thread2", mythread2, 0x1234, Stack2, STACK_SIZE, 5, 0, TX_NO_TIME_SLICE, TX_AUTO_START);
  tx_semaphore_create(&sem1, "semaphore1", 5);
  tx_semaphore_create(&sem2, "semaphore2", 5);
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
void mythread1(UINT arg){
	HAL_UART_Init(&huart1);
	while(1){
		tx_semaphore_get(&sem1, TX_WAIT_FOREVER);
		HAL_UART_Transmit(&huart1, buff, strlen(buff), 100);
		tx_thread_sleep(100);
	}
}

void mythread2(UINT arg){
	while(1){
		tx_semaphore_get(&sem2, TX_WAIT_FOREVER);
		for(int i = 0 ; i<6 ; i++){
			HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_1);
			tx_thread_sleep(10);
		}
	}
}
/* USER CODE END 1 */
