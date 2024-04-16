/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdbool.h>
#include <string.h>
#include <stdio.h>

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
IWDG_HandleTypeDef hiwdg;

RTC_HandleTypeDef hrtc;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_RTC_Init(void);
static void MX_IWDG_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

int received_number = 0;
bool led_states[] = { false, false, false, false, false, false};
bool pressed = false;
bool enter_pressed = false;
bool user_button_pressed = false;
int alarms_time[3][5];
int alarm_number = 0;
int number = 0;
int current_minute = 0;
int current_secound = 0;

void led_set_1(int led, bool turn_on)
{
 GPIO_PinState state = (turn_on) ? GPIO_PIN_SET : GPIO_PIN_RESET;
 switch (led) {
 case 1:
 HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, state);
 break;
 case 2:
 HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, state);
 break;
 case 3:
 HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, state);
 break;
 case 4:
 HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, state);
 break;
 case 5:
 HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, state);
 break;
 default:
 break;
 }
}

int __io_putchar(int ch)
{
 if (ch == '\n') {
 uint8_t ch2 = '\r';
 HAL_UART_Transmit(&huart2, &ch2, 1, HAL_MAX_DELAY);
 }
 HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
 return 1;
}

bool is_user_button_pressed(int button) {
  switch (button) {
  case 0:
    if (HAL_GPIO_ReadPin(USER_BUTTON3_GPIO_Port, USER_BUTTON3_Pin) == GPIO_PIN_RESET) {
      return true;
    } else {
      return false;
    }
  case 1:
    if (HAL_GPIO_ReadPin(USER_BUTTON2_GPIO_Port, USER_BUTTON2_Pin) == GPIO_PIN_RESET) {
      return true;
    } else {
      return false;
    }

  default:
    return false;
  }
}

bool is_button_pressed(void) {
 if (HAL_GPIO_ReadPin(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin) == GPIO_PIN_RESET) {
 return true;
 } else {
 return false;
 }
}



#define LINE_MAX_LENGTH 80
static char line_buffer[LINE_MAX_LENGTH + 1];
static uint32_t line_length;
void line_append(uint8_t value)
{
 if (value == '\r' || value == '\n') {
 // odebraliśmy znak końca linii
 if (line_length > 0) {
 // jeśli bufor nie jest pusty to dodajemy
 // 0 na końcu linii
 line_buffer[line_length] = '\0';
 enter_pressed = true;
 // przetwarzamy dane

 printf("%s", line_buffer);
 received_number = atoi(line_buffer);

 // zaczynamy zbieranie danych od nowa
 line_length = 0;
 }
 }
 else {
 if (line_length >= LINE_MAX_LENGTH) {
 // za dużo danych, usuwamy wszystko co
 // odebraliśmy
 line_length = 0;
 }
 // dopisujemy wartość do bufora
 line_buffer[line_length++] = value;
 }
}

int hours = 0;
int minutes = 0;

void print_alarms(){
	  printf("\n");
	  int i;
	  for (i = 0; i < 5; i++) {
		  printf("%01d Alarmu %02d:%02d ->stan:%01d\n", i+1, alarms_time[0][i], alarms_time[1][i], alarms_time[2][i]);
	  }
	  printf("\n");
}



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
  MX_USART2_UART_Init();
  MX_RTC_Init();
  MX_IWDG_Init();
  /* USER CODE BEGIN 2 */
  int i;
  for (i = 0; i < 10; i++){
	  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	  HAL_Delay(100);
  }
  uint32_t last_ms = HAL_GetTick();

//  uint8_t value = 5;
//  line_append(value);
//  line_buffer = 5;
//  line_append(value);



	  const char message[] = "Set alarms time!\r\n";
	  HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);

	  	  	  // zerowanie czasu
	  		   RTC_TimeTypeDef new_time = {0};
	  		   new_time.Hours = 0;
	  		   new_time.Minutes = 0;
	  		   new_time.Seconds = 40;
	  		   HAL_RTC_SetTime(&hrtc, &new_time, RTC_FORMAT_BIN);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  //Watchdog
	  HAL_IWDG_Refresh(&hiwdg);
	  uint32_t now = HAL_GetTick();
	  if(now - last_ms >= 500){
		  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
		  last_ms = HAL_GetTick();
	  }
//	  if (is_button_pressed()) {
//		  while(1){}
//	  }


	  // ustalanie godziny i minut alarmu
	  if (is_user_button_pressed(0)) {
		  if(user_button_pressed == false){
			  user_button_pressed = true;
		  	led_set_1(2, true);
		  	hours ++;
		  	if (hours >= 24){
			  	hours = 0;
		  	}
		  	printf("Czas alarmu: %02d:%02d\n", hours, minutes);
		  }
	  }
	  else if (is_user_button_pressed(1)) {
		  if(user_button_pressed == false){
			  user_button_pressed = true;
			  led_set_1(1, true);
			  minutes++;
			  if (minutes >= 60){
	 		 	minutes = 0;
			  }
			  printf("Czas alarmu: %02d:%02d\n", hours, minutes);
		  }
	  }
	  else if (is_button_pressed()) {
		  if(user_button_pressed == false){
			  user_button_pressed = true;
			  printf("\n");
			  led_set_1(2, true);

			  if (alarm_number > 5){
				  alarm_number = 1;
			  }
			  alarms_time[0][alarm_number] = hours;
			  alarms_time[1][alarm_number] = minutes;

			  print_alarms();
			  alarm_number++;
		  }
	  }
	  else{
		  user_button_pressed = false;
		  led_set_1(1, false);
		  led_set_1(2, false);
	  }






	  // aktualny czas + wyswietlanie
	  RTC_TimeTypeDef time;
	  RTC_DateTypeDef date;
	  //wyświetlanie czasu
	   HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	   HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
//	   if(current_secound != time.Seconds){
//		   current_secound = time.Seconds;
//		   printf("Aktualny czas: %02d:%02d:%02d\n", time.Hours, time.Minutes, time.Seconds);
//	   }



	   if(current_minute != time.Minutes){
		   current_minute = time.Minutes;
		   print_alarms();
		   //porównanie alarmu z czasem
//		   printf("-> checking alarms \n");
//		   printf("-> alarms_time[0][i] Hours: %d - %d \n", alarms_time[0][i],time.Hours);
//		   printf("-> alarms_time[1][i] Minutes: %d - %d \n", alarms_time[1][i],time.Minutes);
		   int i;
		   for (i = 0; i < 5; i++) {

			  if(alarms_time[0][i] == time.Hours && alarms_time[1][i] == time.Minutes){
				  printf("\n");

				  if(alarms_time[2][i] == 1)
				  {
					  led_set_1(3, true);
					  printf("-> Alarm: %d, dioda: on\n", i+1);
				  }
				  else{
					  led_set_1(3, false);
					  printf("!!! Alarm: %d, dioda: off\n", i+1);
				  }
			  }
		   }
		   printf("\n");
	   }








	  uint8_t value;
	  if (HAL_UART_Receive(&huart2, &value, 1, 0) == HAL_OK)
	  {
	      line_append(value);
	  }

	  if(enter_pressed){
		  enter_pressed = false;
//	      printf("\r dioda LED ");
//	      if(led_states[received_number]){
//	    	  printf("off ");
//	      }else{
//	    	  printf("on ");
//	      }
//	      printf("%d\n",received_number);
		  received_number = received_number - 1;

	      if(alarms_time[2][received_number] == 0){
	    	  alarms_time[2][received_number] = 1;
	      }else{
	    	  alarms_time[2][received_number] = 0;
	      }

	      print_alarms();

	      led_states[received_number] = !led_states[received_number];
	      led_set_1(received_number, led_states[received_number]);
	  }


//	  // jeśli naciśnięto przycisk
//	   if (is_button_pressed()) {
//		   number++;
//		   led_set_1(number, true);
//
////		   // ważne: nieużywane pola muszą być wyzerowane
////		   RTC_TimeTypeDef new_time = {0};
////		   // czekamy na zwolnienie przycisku
////		   while (is_button_pressed()) {}
////		   // ustawiamy godzinę 07:45:00
////		   new_time.Hours = 7;
////		   new_time.Minutes = 45;
////		   new_time.Seconds = 0;
////		   HAL_RTC_SetTime(&hrtc, &new_time, RTC_FORMAT_BIN);
//	   }else{
//		   led_set_1(number, false);
//		   if(number >= 5){
//			   number = 0;
//		   }
//	   }


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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief IWDG Initialization Function
  * @param None
  * @retval None
  */
static void MX_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_32;
  hiwdg.Init.Window = 4095;
  hiwdg.Init.Reload = 4095;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED3_Pin|LED4_Pin|LED5_Pin|LED2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_BUTTON_Pin */
  GPIO_InitStruct.Pin = USER_BUTTON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_BUTTON_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED3_Pin LED4_Pin LED5_Pin LED2_Pin */
  GPIO_InitStruct.Pin = LED3_Pin|LED4_Pin|LED5_Pin|LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : USER_BUTTON2_Pin USER_BUTTON3_Pin */
  GPIO_InitStruct.Pin = USER_BUTTON2_Pin|USER_BUTTON3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : LED1_Pin */
  GPIO_InitStruct.Pin = LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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

#ifdef  USE_FULL_ASSERT
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
