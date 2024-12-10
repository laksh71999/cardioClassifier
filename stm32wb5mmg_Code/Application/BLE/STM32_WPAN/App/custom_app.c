/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    App/custom_app.c
 * @author  MCD Application Team
 * @brief   Custom Example Application (Server)
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
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
#include "app_common.h"
#include "dbg_trace.h"
#include "ble.h"
#include "custom_app.h"
#include "custom_stm.h"
#include "stm32_seq.h"
#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
// typedef struct
//{
//   /* AUDIO_STREAM */
//   uint8_t               Audstream_char_Notification_Status;
//   /* USER CODE BEGIN CUSTOM_APP_Context_t */
////////////////////////////////////
//  /* USER CODE END CUSTOM_APP_Context_t */
//
//  uint16_t              ConnectionHandle;
//} Custom_App_Context_t;

/* USER CODE BEGIN PTD */
bool NotificationTransferReq = 0, DtFlowStatus = 0;
uint16_t cntTest1 = 0;
/* USER CODE END PTD */

/* Private defines ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/**
 * START of Section BLE_APP_CONTEXT
 */

// PLACE_IN_SECTION("BLE_APP_CONTEXT") static Custom_App_Context_t Custom_App_Context;

/**
 * END of Section BLE_APP_CONTEXT
 */

uint8_t UpdateCharData[244];
uint8_t NotifyCharData[10][244];

/* USER CODE BEGIN PV */
volatile uint8_t b_head = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* AUDIO_STREAM */
static void Custom_Audstream_char_Update_Char(void);
static void Custom_Audstream_char_Send_Notification(void);

/* USER CODE BEGIN PFP */
void Custom_App_TxPoolAvailableNotification(void)
{
  UTIL_SEQ_SetTask(1 << CFG_TASK_AUDIO_STREAM_ID, CFG_SCH_PRIO_0);
  DtFlowStatus = DTS_APP_FLOW_ON;
  // return;
}

void Resume_Notification(void)
{
  UTIL_SEQ_SetTask(1 << CFG_TASK_AUDIO_STREAM_ID, CFG_SCH_PRIO_0);
  DtFlowStatus = DTS_APP_FLOW_ON;
}
/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void Custom_STM_App_Notification(Custom_STM_App_Notification_evt_t *pNotification)
{
  /* USER CODE BEGIN CUSTOM_STM_App_Notification_1 */

  /* USER CODE END CUSTOM_STM_App_Notification_1 */
  switch (pNotification->Custom_Evt_Opcode)
  {
  /* USER CODE BEGIN CUSTOM_STM_App_Notification_Custom_Evt_Opcode */

  /* USER CODE END CUSTOM_STM_App_Notification_Custom_Evt_Opcode */

  /* AUDIO_STREAM */
  case CUSTOM_STM_AUDSTREAM_CHAR_READ_EVT:
    /* USER CODE BEGIN CUSTOM_STM_AUDSTREAM_CHAR_READ_EVT */

    /* USER CODE END CUSTOM_STM_AUDSTREAM_CHAR_READ_EVT */
    break;

  case CUSTOM_STM_AUDSTREAM_CHAR_NOTIFY_ENABLED_EVT:
    /* USER CODE BEGIN CUSTOM_STM_AUDSTREAM_CHAR_NOTIFY_ENABLED_EVT */
    NotificationTransferReq = DTS_APP_TRANSFER_REQ_ON;
    DtFlowStatus = DTS_APP_FLOW_ON;
    /* USER CODE END CUSTOM_STM_AUDSTREAM_CHAR_NOTIFY_ENABLED_EVT */
    break;

  case CUSTOM_STM_AUDSTREAM_CHAR_NOTIFY_DISABLED_EVT:
    /* USER CODE BEGIN CUSTOM_STM_AUDSTREAM_CHAR_NOTIFY_DISABLED_EVT */
    NotificationTransferReq = DTS_APP_TRANSFER_REQ_OFF;
    DtFlowStatus = DTS_APP_FLOW_OFF;
    /* USER CODE END CUSTOM_STM_AUDSTREAM_CHAR_NOTIFY_DISABLED_EVT */
    break;

  default:
    /* USER CODE BEGIN CUSTOM_STM_App_Notification_default */

    /* USER CODE END CUSTOM_STM_App_Notification_default */
    break;
  }
  /* USER CODE BEGIN CUSTOM_STM_App_Notification_2 */

  /* USER CODE END CUSTOM_STM_App_Notification_2 */
  return;
}

void Custom_APP_Notification(Custom_App_ConnHandle_Not_evt_t *pNotification)
{
  /* USER CODE BEGIN CUSTOM_APP_Notification_1 */

  /* USER CODE END CUSTOM_APP_Notification_1 */

  switch (pNotification->Custom_Evt_Opcode)
  {
  /* USER CODE BEGIN CUSTOM_APP_Notification_Custom_Evt_Opcode */

  /* USER CODE END P2PS_CUSTOM_Notification_Custom_Evt_Opcode */
  case CUSTOM_CONN_HANDLE_EVT:
    /* USER CODE BEGIN CUSTOM_CONN_HANDLE_EVT */

    /* USER CODE END CUSTOM_CONN_HANDLE_EVT */
    break;

  case CUSTOM_DISCON_HANDLE_EVT:
    /* USER CODE BEGIN CUSTOM_DISCON_HANDLE_EVT */

    /* USER CODE END CUSTOM_DISCON_HANDLE_EVT */
    break;

  default:
    /* USER CODE BEGIN CUSTOM_APP_Notification_default */

    /* USER CODE END CUSTOM_APP_Notification_default */
    break;
  }

  /* USER CODE BEGIN CUSTOM_APP_Notification_2 */

  /* USER CODE END CUSTOM_APP_Notification_2 */

  return;
}

void Custom_APP_Init(void)
{
  /* USER CODE BEGIN CUSTOM_APP_Init */
  UTIL_SEQ_RegTask(1 << CFG_TASK_AUDIO_STREAM_ID, UTIL_SEQ_RFU, Custom_Audstream_char_Send_Notification);
  // UTIL_SEQ_SetTask(1 << CFG_TASK_AUDIO_STREAM_ID, CFG_SCH_PRIO_0);

  opusEncodeCreate();
  opusBitrateSet();
  /* USER CODE END CUSTOM_APP_Init */
  return;
}

/* USER CODE BEGIN FD */

/* USER CODE END FD */

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/

/* AUDIO_STREAM */
void Custom_Audstream_char_Update_Char(void) /* Property Read */
{
  //  uint8_t updateflag = 0;

  /* USER CODE BEGIN Audstream_char_UC_1*/

  /* USER CODE END Audstream_char_UC_1*/

  //  if (updateflag != 0)
  //  {
  Custom_STM_App_Update_Char(CUSTOM_STM_AUDSTREAM_CHAR, (uint8_t *)UpdateCharData);
  //  }

  /* USER CODE BEGIN Audstream_char_UC_Last*/

  /* USER CODE END Audstream_char_UC_Last*/
  return;
}

void Custom_Audstream_char_Send_Notification(void) /* Property Notification */
{
  //  uint8_t updateflag = 0;

  /* USER CODE BEGIN Audstream_char_NS_1*/

  /* USER CODE END Audstream_char_NS_1*/

  //  if (updateflag != 0)
  //  {
  //    Custom_STM_App_Update_Char(CUSTOM_STM_AUDSTREAM_CHAR, (uint8_t *)NotifyCharData);
  //  }

  /* USER CODE BEGIN Audstream_char_NS_Last*/
  tBleStatus status = BLE_STATUS_INVALID_PARAMS;
  if (NotificationTransferReq != DTS_APP_TRANSFER_REQ_OFF)
  {
    status = Custom_STM_App_Update_Char(CUSTOM_STM_AUDSTREAM_CHAR, (uint8_t *)NotifyCharData[b_head]);
    // if (status == BLE_STATUS_BUSY)
    // {
    // }
    // else
    // {
      if (status == BLE_STATUS_INSUFFICIENT_RESOURCES)
      {
        DtFlowStatus = DTS_APP_FLOW_OFF;
        b_head = b_head--;
      }
      else
      {
        b_head++;
        if (b_head >= 10)
          b_head = 0;
      }
    // }
  }
  else
  {
    APP_DBG_MSG("-- CUSTOM APPLICATION : CAN'T INFORM CLIENT -  NOTIFICATION DISABLED\n ");
  }
  /* USER CODE END Audstream_char_NS_Last*/

  return;
}

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS*/

/* USER CODE END FD_LOCAL_FUNCTIONS*/
