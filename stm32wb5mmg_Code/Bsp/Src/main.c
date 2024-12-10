/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "ipcc.h"
#include "rf.h"
#include "rtc.h"
#include "sai.h"
#include "tim.h"
#include "gpio.h"

// #include <stdbool.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t state = BUFFER_NONE, volume = 1, initCmplt = 0, halfReady = 0, fullReady = 0, row = 0, cntFull_Cmplt = 0, cntHalf_Cmplt = 0;

bool halfFill = false, fullFill = false, volUpFlag = false, volDownFlag = false, ble_recFlag, ble_volUpFlag, ble_volDownFlag;

// int32_t PCM_IN_Buff[AUDIO_BUFFER_SIZE], PCM_CPY_Buff[20][AUDIO_BUFFER_SIZE/2];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int bsp_main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  /* Config code for STM32_WPAN (HSE Tuning must be done before system clock configuration) */
  MX_APPE_Config();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* IPCC initialisation */
  MX_IPCC_Init();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SAI1_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_RF_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END 2 */

  /* Init code for STM32_WPAN */
  MX_APPE_Init();

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  initCmplt = 1;
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure LSE Drive Capability
   */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_MEDIUMHIGH);

  /** Configure the main internal regulator output voltage
   */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV8;
  RCC_OscInitStruct.PLL.PLLN = 32;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK4 | RCC_CLOCKTYPE_HCLK2 | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK2Divider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.AHBCLK4Divider = RCC_SYSCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
  // HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_SYSCLK, RCC_MCODIV_2); //to monitor sysclock in PA8 Pin
  // HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_PLLCLK, RCC_MCODIV_2); //to monitor pllclock in PA8 Pin
}

/**
 * @brief Peripherals Common Clock Configuration
 * @retval None
 */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
   */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SMPS | RCC_PERIPHCLK_RFWAKEUP | RCC_PERIPHCLK_SAI1;
  // PeriphClkInitStruct.PLLSAI1.PLLN = 86;
  // PeriphClkInitStruct.PLLSAI1.PLLP = RCC_PLLP_DIV7;
  // PeriphClkInitStruct.PLLSAI1.PLLQ = RCC_PLLQ_DIV2;
  // PeriphClkInitStruct.PLLSAI1.PLLR = RCC_PLLR_DIV2;

  /*Sai clock for 16000samplerate*/
  /*ref BVOPUS_PER line 870 in stm32wb5mm_dk_audio.c*/
  PeriphClkInitStruct.PLLSAI1.PLLN = 82;
  PeriphClkInitStruct.PLLSAI1.PLLP = RCC_PLLP_DIV8;

  PeriphClkInitStruct.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_SAI1CLK;
  PeriphClkInitStruct.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLLSAI1;
  PeriphClkInitStruct.RFWakeUpClockSelection = RCC_RFWKPCLKSOURCE_LSE;
  PeriphClkInitStruct.SmpsClockSelection = RCC_SMPSCLKSOURCE_HSI;
  PeriphClkInitStruct.SmpsDivSelection = RCC_SMPSCLKDIV_RANGE1;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN Smps */

  /* USER CODE END Smps */
}

/* USER CODE BEGIN 4 */
void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hsai_BlockA1)
{
  if (halfFill == true)
  {
    // while(1){};
    // cntHalf++;
  }
  else
  {
    cntHalf_Cmplt++;
    halfFill = true;
  }
  fullReady = 0;
  halfReady = 1;
  // cntHalf++;

  // /* Trying Moving Buffer for collecting PCM Samples */
  // memcpy(PCM_CPY_Buff[row++], &PCM_IN_Buff[0], AUDIO_BUFFER_SIZE/2);
  // if(row>=20)
  //   row = 0;
}

void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hsai_BlockA1)
{
  if (fullFill == true)
  {
    // while(1){};
    // cntFull++;
  }
  else
  {
    cntFull_Cmplt++;
    fullFill = true;
  }
  fullReady = 1;
  halfReady = 0;
  // cntFull++;

  /* Trying Moving Buffer for collecting PCM Samples */
  // memcpy(PCM_CPY_Buff[row++], &PCM_IN_Buff[AUDIO_BUFFER_SIZE/2], AUDIO_BUFFER_SIZE/2);
  // if(row>=20)
  //   row = 0;
  // fullReady = 1;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if ((GPIO_Pin == GPIO_PIN_4) && (1 == initCmplt))
  { // Record
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_10);
    // SEGGER_RTT_printf(0, "RecordInterrupt: %d\n", ble_recFlag);
    if (0 == ble_recFlag)
    {
      ble_recFlag = 1;
    }
    else
    {
      ble_recFlag = 0;
    }
  }
  if ((GPIO_Pin == GPIO_PIN_1) && (false == volUpFlag) && (1 == initCmplt))
  { // Volume Up
    if (1 == volume)
      volume += 3;
    else
      volume += 5;
    if (volume > 14)
      volume = 14;
    volUpFlag = true;
    // SEGGER_RTT_printf(0, "Volume = %d\n", volume);

    if (0 == ble_volUpFlag)
    {
      ble_volUpFlag = 1;
    }
    else
    {
      ble_volUpFlag = 0;
    }
  }
  if ((GPIO_Pin == GPIO_PIN_3) && (false == volDownFlag) && (1 == initCmplt))
  { // Volume Down
    if (1 == volume)
      volume = 1;
    else
    {
      if (3 == volume)
        volume -= 2;
      else if (4 == volume)
        volume -= 3;
      else
        volume -= 5;
    }
    volDownFlag = true;
    // SEGGER_RTT_printf(0, "Volume = %d\n", volume);

    if (0 == ble_volDownFlag)
    {
      ble_volDownFlag = 1;
    }
    else
    {
      ble_volDownFlag = 0;
    }
  }
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
