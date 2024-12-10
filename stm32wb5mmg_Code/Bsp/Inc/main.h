/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32wbxx_hal.h"
#include "app_conf.h"
#include "app_entry.h"
#include "app_common.h"
#include "stm32_seq.h"
#include "OpusCodecConfiguration.h"

#include <stdbool.h>

  /* Private includes
   * ----------------------------------------------------------*/
  /* USER CODE BEGIN Includes */
#define DTS_APP_FLOW_OFF 0
#define DTS_APP_FLOW_ON 1

#define DTS_APP_TRANSFER_REQ_OFF 0
#define DTS_APP_TRANSFER_REQ_ON 1

// #define AUDIO_BUFFER_SIZE 488
  /* USER CODE END Includes */

  /* Exported types
   * ------------------------------------------------------------*/
  /* USER CODE BEGIN ET */
// typedef enum
// {
//   DTS_APP_FLOW_OFF,
//   DTS_APP_FLOW_ON
// } DTS_App_Flow_Status_t;

// typedef enum
// {
//   DTS_APP_TRANSFER_REQ_OFF,
//   DTS_APP_TRANSFER_REQ_ON
// } DTS_App_Transfer_Req_Status_t;

typedef struct
{
  uint16_t ConnectionHandle;
  /* AUDIO_STREAM */
  uint8_t Stream_char_Notification_Status;
  /* USER CODE BEGIN CUSTOM_APP_Context_t */
  // DTS_App_Transfer_Req_Status_t NotificationTransferReq;
//  DTS_App_Transfer_Req_Status_t ButtonTransferReq;
  // DTS_App_Flow_Status_t DtFlowStatus;
  /* USER CODE END CUSTOM_APP_Context_t */
} Custom_App_Context_t;


  /* USER CODE END ET */

  /* Exported constants
   * --------------------------------------------------------*/
  /* USER CODE BEGIN EC */
PLACE_IN_SECTION("BLE_APP_CONTEXT") static Custom_App_Context_t Custom_App_Context;
  /* USER CODE END EC */

  /* Exported macro
   * ------------------------------------------------------------*/
  /* USER CODE BEGIN EM */

  /* USER CODE END EM */
  // uint8_t state  = 0;
  
  enum
  {
    BUFFER_NONE,
    BUFFER_HALF,
    BUFFER_FULL
  };
  /* Exported functions prototypes
   * ---------------------------------------------*/
  void Error_Handler(void);
  int bsp_main(void);
  void halfIntpt(void);
  void fullIntpt(void);
  /* USER CODE BEGIN EFP */

  /* USER CODE END EFP */

  /* Private defines
   * -----------------------------------------------------------*/

  /* USER CODE BEGIN Private defines */

  /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
