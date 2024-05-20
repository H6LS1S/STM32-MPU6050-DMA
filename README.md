# STM32-MPU6050-DMA
Simple and small library for working with MPU6050 via I2C + DMA

![image](https://github.com/H6LS1S/STM32-MPU6050-DMA/assets/13855054/96eab005-40a4-431e-b226-cb1a3fa6e18e)
![image](https://github.com/H6LS1S/STM32-MPU6050-DMA/assets/13855054/24454e40-7228-4257-a247-3ff4df1d83f7)
![image](https://github.com/H6LS1S/STM32-MPU6050-DMA/assets/13855054/c924fd06-f03d-42eb-b3a7-b6f506ab3009)


Example
```c
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
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  MPU6050_Init('G', &hi2c1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
	{

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}
```
