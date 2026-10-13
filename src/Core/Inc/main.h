/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MOT1ST_Pin GPIO_PIN_0
#define MOT1ST_GPIO_Port GPIOC
#define MOT2ST_Pin GPIO_PIN_1
#define MOT2ST_GPIO_Port GPIOC
#define MOT3ST_Pin GPIO_PIN_2
#define MOT3ST_GPIO_Port GPIOC
#define MOT1DR_Pin GPIO_PIN_0
#define MOT1DR_GPIO_Port GPIOA
#define MOT2DR_Pin GPIO_PIN_1
#define MOT2DR_GPIO_Port GPIOA
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define MOT3DR_Pin GPIO_PIN_4
#define MOT3DR_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define PIN_BUTTON_RIGHT_Pin GPIO_PIN_1
#define PIN_BUTTON_RIGHT_GPIO_Port GPIOB
#define PIN_BUTTON_RIGHT_EXTI_IRQn EXTI1_IRQn
#define PIN_BUTTON_SEL_Pin GPIO_PIN_2
#define PIN_BUTTON_SEL_GPIO_Port GPIOB
#define PIN_BUTTON_SEL_EXTI_IRQn EXTI2_IRQn
#define PIN_HOME_X_Pin GPIO_PIN_10
#define PIN_HOME_X_GPIO_Port GPIOB
#define PIN_HOME_X_EXTI_IRQn EXTI15_10_IRQn
#define PIN_BUTTON_LEFT_Pin GPIO_PIN_13
#define PIN_BUTTON_LEFT_GPIO_Port GPIOB
#define PIN_BUTTON_LEFT_EXTI_IRQn EXTI15_10_IRQn
#define PIN_BUTTON_UP_Pin GPIO_PIN_14
#define PIN_BUTTON_UP_GPIO_Port GPIOB
#define PIN_BUTTON_UP_EXTI_IRQn EXTI15_10_IRQn
#define PIN_BUTTON_DOWN_Pin GPIO_PIN_15
#define PIN_BUTTON_DOWN_GPIO_Port GPIOB
#define PIN_BUTTON_DOWN_EXTI_IRQn EXTI15_10_IRQn
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define MOT1EN_Pin GPIO_PIN_10
#define MOT1EN_GPIO_Port GPIOC
#define MOT2EN_Pin GPIO_PIN_11
#define MOT2EN_GPIO_Port GPIOC
#define MOT3EN_Pin GPIO_PIN_12
#define MOT3EN_GPIO_Port GPIOC
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define PIN_HOME_Y_Pin GPIO_PIN_4
#define PIN_HOME_Y_GPIO_Port GPIOB
#define PIN_HOME_Y_EXTI_IRQn EXTI4_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
