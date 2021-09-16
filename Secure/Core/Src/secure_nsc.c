/**
  ******************************************************************************
  * @file    Secure/Src/secure_nsc.c
  * @author  MCD Application Team
  * @brief   This file contains the non-secure callable APIs (secure world)
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE BEGIN Non_Secure_CallLib */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "secure_nsc.h"

#include <stdio.h>

#include <stdint.h>
#include <string.h>

#include <stdlib.h>


/** @addtogroup STM32L5xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Global variables ----------------------------------------------------------*/
void *pSecureFaultCallback = NULL;   /* Pointer to secure fault callback in Non-secure */
void *pSecureErrorCallback = NULL;   /* Pointer to secure error callback in Non-secure */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Secure registration of non-secure callback.
  * @param  CallbackId  callback identifier
  * @param  func        pointer to non-secure function
  * @retval None
  */
CMSE_NS_ENTRY void SECURE_RegisterCallback(SECURE_CallbackIDTypeDef CallbackId, void *func)
{
  if(func != NULL)
  {
    switch(CallbackId)
    {
      case SECURE_FAULT_CB_ID:           /* SecureFault Interrupt occurred */
        pSecureFaultCallback = func;
        break;
      case GTZC_ERROR_CB_ID:             /* GTZC Interrupt occurred */
        pSecureErrorCallback = func;
        break;
      default:
        /* unknown */
        break;
    }
  }
}

/**
  * @brief  Secure LED toggle
  * @retval None
 */
CMSE_NS_ENTRY
void
SECURE_CryptoSuite (void)
{
  HAL_GPIO_TogglePin (LED_GREEN_GPIO_Port, LED_GREEN_Pin);

  printf("\r\n");
  printf("--- SECURE BEGIN ---\r\n");
  printf("Press button to continue...\r\n");
  // wait for user to press a button BUT_1
  while (1) {
    if (HAL_GPIO_ReadPin (BUT_1_GPIO_Port, BUT_1_Pin) == 1) break;
  }

  //lstcrypto_example_nucleo_l552ze_q_suite (); // run lstcrypto suite
  embedded_example_nucleo_l552ze_q_suite ();

  printf ("--- SECURE END ---\r\n");
  printf("Press button to continue...\r\n");
  // wait for user to press a button BUT_1
  while (1) {
    if (HAL_GPIO_ReadPin (BUT_1_GPIO_Port, BUT_1_Pin) == 1) break;
  }
  return;
}

/**
  * @}
  */

/**
  * @}
  */
/* USER CODE END Non_Secure_CallLib */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
