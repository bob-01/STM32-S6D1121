#include "main.h"
#include "stm32f1xx_hal.h"
#include "s6d1121.h"
#include "si5351.h"

#define IND_LED_ON()		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET)
#define IND_LED_OFF()		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET)

I2C_HandleTypeDef hi2c1;
UART_HandleTypeDef huart1;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
void I2C_scanner();

extern unsigned char SmallFont[];
uint8_t HAL_init_error, position_x;
uint8_t str[]="USART Transmit\r\n";

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();

	disp_x_size = 239;
	disp_y_size = 319;
	int buf[318];
	int x, x2;
	int y, y2;
	int r;

	InitLCD(1);
	setFont(SmallFont);

	str[8]=0;
	HAL_UART_Receive_IT(&huart1,(uint8_t*) str,8);

	clrScr();
	setColor(0, 255, 0);
	if(HAL_init_error == HAL_OK) { print("HAL_OK", 1, 180, 0); }
	if(HAL_init_error == HAL_ERROR) { print("HAL_ERROR", 1, 180, 0); }
	if(HAL_init_error == HAL_BUSY) { print("HAL_BUSY", 1, 180, 0); }
	if(HAL_init_error == HAL_TIMEOUT) { print("HAL_TIMEOUT", 1, 180, 0); }
	HAL_Delay(500);

  while (1)
  {
		IND_LED_ON();
		HAL_Delay(500);
		IND_LED_OFF();

		HAL_UART_Transmit(&huart1,str,16,0xFFFF);
		HAL_Delay(100);

        if(huart1.RxXferCount==0)
        {
    		print(str, 1, disp_y_size - cfont.y_size, 0);
			str[8]=0;
			HAL_UART_Receive_IT(&huart1,(uint8_t*) str,8);
        }

		I2C_scanner();

		clrScr();
		setColor(255, 0, 0);
		fillRect(0, 0, 319, 13);
		setColor(64, 64, 64);
		fillRect(0, 226, 319, 239);
		setColor(255, 255, 255);
		setBackColor(255, 0, 0);
		print("* Universal Color TFT Display Library *", CENTER, 1, 0);
		setBackColor(64, 64, 64);
		setColor(255,255,0);
		print("<http://electronics.henningkarlsen.com>", CENTER, 227, 0);
		HAL_Delay(500);

		setColor(0, 0, 255);
		  drawRect(0, 14, 319, 225);

		// Draw crosshairs
		  setColor(0, 0, 255);
		  setBackColor(0, 0, 0);
		  drawLine(159, 15, 159, 224);
		  drawLine(1, 119, 318, 119);
		  for (int i=9; i<310; i+=10)
			drawLine(i, 117, i, 121);
		  for (int i=19; i<220; i+=10)
			drawLine(157, i, 161, i);

		  // Draw sin-, cos- and tan-lines
			setColor(0,255,255);
			print("Sin", 5, 15, 0);
			for (int i=1; i<318; i++)
			{
			  drawPixel(i,119+(sin(((i*1.13)*3.14)/180)*95));
			}

			setColor(255,0,0);
			print("Cos", 5, 27, 0);
			for (int i=1; i<318; i++)
			{
			  drawPixel(i,119+(cos(((i*1.13)*3.14)/180)*95));
			}

			setColor(255,255,0);
			print("Tan", 5, 39, 0);
			for (int i=1; i<318; i++)
			{
			  drawPixel(i,119+(tan(((i*1.13)*3.14)/180)));
			}

		  HAL_Delay(500);

		  setColor(0,0,0);
		  fillRect(1,15,318,224);
		  setColor(0, 0, 255);
		  setBackColor(0, 0, 0);
		  drawLine(159, 15, 159, 224);
		  drawLine(1, 119, 318, 119);

		// Draw a moving sinewave
		  x=1;
		  for (int i=1; i<(318*20); i++)
		  {
			x++;
			if (x==319)
			  x=1;
			if (i>319)
			{
			  if ((x==159)||(buf[x-1]==119))
				setColor(0,0,255);
			  else
				setColor(0,0,0);
			  drawPixel(x,buf[x-1]);
			}
			setColor(0,255,255);
			y=119+(sin(((i*1.1)*3.14)/180)*(90-(i / 100)));
			drawPixel(x,y);
			buf[x-1]=y;
		  }

		  HAL_Delay(500);

		  setColor(0,0,0);
		  fillRect(1,15,318,224);

		// Draw some filled rectangles
		  for (int i=1; i<6; i++)
		  {
			switch (i)
			{
			  case 1:
				setColor(255,0,255);
				break;
			  case 2:
				setColor(255,0,0);
				break;
			  case 3:
				setColor(0,255,0);
				break;
			  case 4:
				setColor(0,0,255);
				break;
			  case 5:
				setColor(255,255,0);
				break;
			}
			fillRect(70+(i*20), 30+(i*20), 130+(i*20), 90+(i*20));
		  }


		  HAL_Delay(500);

		  setColor(0,0,0);
		  fillRect(1,15,318,224);

		// Draw some filled, rounded rectangles
		  for (int i=1; i<6; i++)
		  {
			switch (i)
			{
			  case 1:
				setColor(255,0,255);
				break;
			  case 2:
				setColor(255,0,0);
				break;
			  case 3:
				setColor(0,255,0);
				break;
			  case 4:
				setColor(0,0,255);
				break;
			  case 5:
				setColor(255,255,0);
				break;
			}
			fillRoundRect(190-(i*20), 30+(i*20), 250-(i*20), 90+(i*20));
		  }


		  HAL_Delay(500);
		  setColor(0,0,0);
		  fillRect(1,15,318,224);

		// Draw some filled circles
		  for (int i=1; i<6; i++)
		  {
			switch (i)
			{
			  case 1:
				setColor(255,0,255);
				break;
			  case 2:
				setColor(255,0,0);
				break;
			  case 3:
				setColor(0,255,0);
				break;
			  case 4:
				setColor(0,0,255);
				break;
			  case 5:
				setColor(255,255,0);
				break;
			}
			fillCircle(100+(i*20),60+(i*20), 30);
		  }

		  HAL_Delay(500);

		  setColor(0,0,0);
		  fillRect(1,15,318,224);

		// Draw some lines in a pattern
		  setColor (255,0,0);
		  for (int i=15; i<224; i+=5)
		  {
			drawLine(1, i, (i*1.44)-10, 224);
		  }
		  setColor (255,0,0);
		  for (int i=224; i>15; i-=5)
		  {
			drawLine(318, i, (i*1.44)-11, 15);
		  }
		  setColor (0,255,255);
		  for (int i=224; i>15; i-=5)
		  {
			drawLine(1, i, 331-(i*1.44), 15);
		  }
		  setColor (0,255,255);
		  for (int i=15; i<224; i+=5)
		  {
			drawLine(318, i, 330-(i*1.44), 224);
		  }

		  HAL_Delay(500);

	//		  setColor(0,0,0);
	//		  fillRect(1,15,318,225);

	/*
		// Draw some random circles
		  for (int i=0; i<100; i++)
		  {
			setColor(random(255), random(255), random(255));
			x=32+random(256);
			y=45+random(146);
			r=random(30);
			drawCircle(x, y, r);
		  }

		  HAL_Delay(500);
		  setColor(0,0,0);
		  fillRect(1,15,318,224);

		// Draw some random rectangles
		  for (int i=0; i<100; i++)
		  {
			setColor(random(255), random(255), random(255));
			x=2+random(316);
			y=16+random(207);
			x2=2+random(316);
			y2=16+random(207);
			drawRect(x, y, x2, y2);
		  }

		  HAL_Delay(500);

		  setColor(0,0,0);
		  fillRect(1,15,318,224);

		// Draw some random rounded rectangles
		  for (int i=0; i<100; i++)
		  {
			setColor(random(255), random(255), random(255));
			x=2+random(316);
			y=16+random(207);
			x2=2+random(316);
			y2=16+random(207);
			drawRoundRect(x, y, x2, y2);
		  }
		  HAL_Delay(500);
	*/

	/*
		  setColor(0,0,0);
		  fillRect(1,15,318,224);

		  for (int i=0; i<100; i++)
		  {
			setColor(random(255), random(255), random(255));
			x=2+random(316);
			y=16+random(209);
			x2=2+random(316);
			y2=16+random(209);
			drawLine(x, y, x2, y2);
		  }
	*/
		  HAL_Delay(500);

		  setColor(0,0,0);
		  fillRect(1,15,318,224);

		  for (int i=0; i<10000; i++)
		  {
			setColor(random(255), random(255), random(255));
			drawPixel(2+random(316), 16+random(209));
		  }

		  HAL_Delay(500);

		  fillScr(0, 0, 255);
		  setColor(255, 0, 0);
		  fillRoundRect(80, 70, 239, 169);

		  setColor(255, 255, 255);
		  setBackColor(255, 0, 0);
		  print("That's it!", CENTER, 93, 0);
		  print("Restarting in a", CENTER, 119, 0);
		  print("few seconds...", CENTER, 132, 0);

		  setColor(0, 255, 0);
		  setBackColor(0, 0, 255);
		  print("Runtime: (secs)", CENTER, 210, 0);

		  printNumI(HAL_GetTick()/1000, CENTER, 225,1,' ');

		  HAL_Delay(5000);
  }

  return 0;
}

void SystemClock_Config(void)
{
	 RCC_OscInitTypeDef RCC_OscInitStruct;
	  RCC_ClkInitTypeDef RCC_ClkInitStruct;

	    /**Initializes the CPU, AHB and APB busses clocks
	    */
	  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	  {
	    _Error_Handler(__FILE__, __LINE__);
	  }

	    /**Initializes the CPU, AHB and APB busses clocks
	    */
	  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	  {
	    _Error_Handler(__FILE__, __LINE__);
	  }

	    /**Configure the Systick interrupt time
	    */
	  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	    /**Configure the Systick
	    */
	  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	  /* SysTick_IRQn interrupt configuration */
	  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* I2C1 init function */
static void MX_I2C1_Init(void)
{
  __HAL_RCC_I2C1_CLK_ENABLE();
  HAL_Delay(100);
  __HAL_RCC_I2C1_FORCE_RESET();
  HAL_Delay(100);
  __HAL_RCC_I2C1_RELEASE_RESET();
  HAL_Delay(100);

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
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
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

void I2C_scanner()
{
/**
 * This tests the standard 7-bit addresses
 * Devices with higher bit address might not be seen properly.
 *
 */
	uint8_t aTxBuffer[2],aRxBuffer;

	clrScr();
	setColor(64, 255, 64);
	setBackColor(0,0,0);
	print("I2C Scanner", CENTER, 1, 0);

	uint8_t error, address;
	int nDevices;

	setColor(64, 64, 64);
	print("Scanning...", 1, 1 + cfont.y_size, 0);

	nDevices = 0;
	position_x = 80;

	for(address = 15; address < 127; address++ )
	{
	// The i2c_scanner uses the return value of
	// the Write.endTransmisstion to see if
	// a device did acknowledge to the address.

	    error = HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t) address << 1 , 10, 100);

		setColor(255, 255, 64);

		if(error == HAL_OK)
		{
			printNumI(address , 1, 30, 1,' ');
			print("I2C device found at address:", 1, position_x, 0);
			printNumI(address , cfont.x_size*29 + 5, position_x, 1,' ');
			position_x += cfont.y_size;
			nDevices++;
			continue;
		}

		if(error == HAL_ERROR)
		{
			printNumI(address , 1, 30, 1,' ');
			print("HAL_ERROR", 30, 30, 0);
			continue;
		}
		if(error == HAL_BUSY)
		{
			printNumI(address , 1, 30, 1,' ');
			print("HAL_BUSY", 30, 30 + cfont.y_size*1, 0);
			continue;
		}
		if(error == HAL_TIMEOUT)
		{
			printNumI(address , 1, 30, 1,' ');
			print("HAL_TIMEOUT", 30, 30 + cfont.y_size*2, 0);
			continue;
		}
		HAL_Delay(500);
	}

	if (nDevices == 0)
	{
		setColor(255, 64, 64);
		print("No I2C devices found", CENTER, 120, 0);
	}
	else
	{
		setColor(255, 0, 0);
		print("Address found", CENTER, 120, 0);
	}
	HAL_Delay(2000);
}

