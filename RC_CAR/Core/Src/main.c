/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//#define IN1_Pin GPIO_PIN_13
//#define IN1_GPIO_Port GPIOB
//
//#define IN2_Pin GPIO_PIN_14
//#define IN2_GPIO_Port GPIOB
//
//#define IN3_Pin GPIO_PIN_15
//#define IN3_GPIO_Port GPIOB
//
//#define IN4_Pin GPIO_PIN_1
//#define IN4_GPIO_Port GPIOB

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t rxData;      // 수신 문자 1바이트 저장

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */


// === UART 수신 콜백 ===
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    // UART1 (휴대폰)에서 수신된 경우
    if(huart->Instance == USART1)       //USART1 과 연결된 블루투스 의 들어올시 동작
    {
        // 받은 데이터를 UART2 로 전달
        HAL_UART_Transmit(&huart2, &rxData, 1, 100);    //받은 데이터 huart2에서 출력Tx (rxData데이터를)

        //자동차 제어
        if(rxData == 'S')       //자동차 정지
        {
              HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);       //IN1, 1
              HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);       //IN2, 1
              HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);       //IN3, 1
              HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);        //IN4, 1
        }
        else if(rxData == 'F')  //자동차 전진
        {
               HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);       //IN1, 1
               HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);     //IN2, 0
               HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);       //IN3, 1
               HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);      //IN4, 0
        }
        else if(rxData == 'B')  //자동차 후진
        {
               HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);   //IN1, 0
               HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);     //IN2, 1
               HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);   //IN3, 0
               HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);      //IN4, 1
        }
        else if(rxData == 'L')  //자동차 좌회전
        {
               HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13,GPIO_PIN_RESET );    //IN1, 0
               HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,GPIO_PIN_SET);       //IN2, 1
               HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15,GPIO_PIN_SET);       //IN3, 1
               HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);     //IN4, 0

        }
        else if(rxData == 'R')  // 자동차 우회전
        {
               HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);       //IN1, 1
               HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);     //IN2, 0
               HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);     //IN3, 0
               HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);        //IN4, 1
        }

        // 다음 데이터 수신 준비 (DMA)
        HAL_UART_Receive_DMA(&huart1, &rxData, 1);
    }

    // UART2에서 수신된 경우 (필요한 경우만 사용)(컴퓨터 수신)
    else if(huart->Instance == USART2)
    {
        // 예: echo back to PC
       // 받은 데이터를 UART2 로 전달
        HAL_UART_Transmit(&huart2, &rxData, 1, 100);
        // 받은 데이터를 UART1 로 전달
        HAL_UART_Transmit(&huart1, &rxData, 1, 100);
        HAL_UART_Receive_IT(&huart2, &rxData, 1); // 계속 수신



        //자동차 제어
        if(rxData == 'S')       //자동차 정지
        {
               HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);       //IN1, 1
               HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);       //IN2, 1
               HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);       //IN3, 1
               HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);        //IN4, 1
        }
        else if(rxData == 'F')  //자동차 전진
        {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);       //IN1, 1
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);     //IN2, 0
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);       //IN3, 1
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);      //IN4, 0
        }
        else if(rxData == 'B')  //자동차 후진
        {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);   //IN1, 0
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);     //IN2, 1
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);   //IN3, 0
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);      //IN4, 1
        }
        else if(rxData == 'L')  //자동차 좌회전
        {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13,GPIO_PIN_RESET );    //IN1, 0
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,GPIO_PIN_SET);       //IN2, 1
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15,GPIO_PIN_SET);       //IN3, 1
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);     //IN4, 0

        }
        else if(rxData == 'R')  // 자동차 우회전
        {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);       //IN1, 1
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);     //IN2, 0
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);     //IN3, 0
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);        //IN4, 1
        }
    }
}



/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  // PWM 시작 (속도 제어)


    HAL_UART_Receive_DMA(&huart1, &rxData, 1);  // 휴대폰에서 DMA 방식으로 수신
    HAL_UART_Receive_IT(&huart2, &rxData, 1);   // PC에서 인터럽트 방식 수신
//  char msg[] = "AT+NAMEswp\r\n";
//  HAL_UART_Transmit(&huart1, msg, sizeof(msg)  , HAL_MAX_DELAY);


//  // === 전원 인가 시 전진 테스트 ===
//  MotorA_Control(1000, 1);   // 왼쪽 모터 전진
//  MotorB_Control(1000, 1);   // 오른쪽 모터 전진
//  HAL_Delay(3000);            // 3초간 전진
//  MotorA_Control(0, -1);      // 정지
//  MotorB_Control(0, -1);
//  HAL_Delay(1000);            // 잠시 대기



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    //IN1, IN2
        //LOW  LOW         전원연결x
        //LOW  HIGH        역방향
        //HIGH LOW         정방향
        //HIGH HIGH        브레이크


        //휴대폰 블루투스 연결 LEDON OFF제어
        if(rxData == 'S')       //자동차 정지
        {
           HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);       //IN1, 1
           HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);       //IN2, 1
           HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);       //IN3, 1
           HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);        //IN4, 1
        }
        else if(rxData == 'F')  //자동차 전진
        {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);       //IN1, 1
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);     //IN2, 0
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);       //IN3, 1
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);      //IN4, 0
        }
        else if(rxData == 'B')  //자동차 후진
        {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);   //IN1, 0
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);     //IN2, 1
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);   //IN3, 0
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);      //IN4, 1
        }
        else if(rxData == 'L')  //자동차 좌회전
        {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13,GPIO_PIN_RESET );    //IN1, 0
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,GPIO_PIN_SET);       //IN2, 1
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15,GPIO_PIN_SET);       //IN3, 1
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);     //IN4, 0

        }
        else if(rxData == 'R')  // 자동차 우회전
        {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);       //IN1, 1
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);     //IN2, 0
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);     //IN3, 0
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);        //IN4, 1
        }
        HAL_UART_Receive_IT(&huart1, &rxData, 1);




    // === 전원 인가 시 전진 테스트 ===
    //전,진코드


//
//#define IN1_Pin GPIO_PIN_13   //IN1
//#define IN1_GPIO_Port GPIOB
//
//#define IN2_Pin GPIO_PIN_14   //IN2
//#define IN2_GPIO_Port GPIOB
//
//#define IN3_Pin GPIO_PIN_15   //IN3
//#define IN3_GPIO_Port GPIOB
//
//#define IN4_Pin GPIO_PIN_1   //IN4
//#define IN4_GPIO_Port GPIOB


////    // 수신된 명령에 따라 동작
//    switch (rxData)
//    {
//    case 'w': // 전진
//        MotorA_Control(1000, 1);
//        MotorB_Control(1000, 1);
//        break;
//    case 's': // 후진
//        MotorA_Control(1000, 0);
//        MotorB_Control(1000, 0);
//        break;
//    case 'a': // 좌회전
//        MotorA_Control(700, 0);
//        MotorB_Control(700, 1);
//        break;
//    case 'd': // 우회전
//        MotorA_Control(700, 1);
//        MotorB_Control(700, 0);
//        break;
//    case 'c': // 정지
//    default:
//        MotorA_Control(0, -1);
//        MotorB_Control(0, -1);
//        break;
//
//
//        }


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
