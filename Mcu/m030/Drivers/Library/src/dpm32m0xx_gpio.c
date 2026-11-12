/**
 *******************************************************************************
 * @file    dpm32m0xx_gpio.c.
 *
 * @brief   Source file for GPIO firmware driver.
 *          This file provides firmware functions to manage the following
 *          functionalities of the GPIO peripheral:
 *           + Initialization and Configuration
 *           + GPIO Read and Write
 *           + GPIO Alternate functions and analog functions configuration
 *           + Interrupts and flags management
 *
 * @author  DPM
 *
 * @version V1.0.0
 *
 * @date    2025-05-20
 *
 * @verbatim
 ===============================================================================
                       ##### How to use this driver #####
 ===============================================================================
    [..]
      (#) Enable the GPIO AHB clock using the following function
          RCC_AHBPeriphClockCmd(RCC_AHB_PERIPH_GPIO, ENABLE);

      (#) Configure the GPIO pin(s) using GPIO_Init(). Four possible
          configuration are available for each pin:
        (+) Input: Floating, Pull-up, Pull-down.
        (+) Output: Push-Pull (Pull-up, Pull-down or no Pull)
              Open Drain (Pull-up, Pull-down or no Pull). In output mode,
              the speed is configurable: Low, Middle, High, Max.
        (+) Alternate Function: Push-Pull (Pull-up, Pull-down or no Pull) Open
              Drain (Pull-up, Pull-down or no Pull).
        (+) Analog: required mode when a pin is to be used as ADC channel, DAC
              output, PGA or ACMP.

      (#) Peripherals alternate function:
        (+) For ADC, DAC, PGA and ACMP configure the desired pin in analog mode
            using GPIO_InitStruct->GPIO_Mode = GPIO_MODE_ANALOG.
            Connect the pin to the desired peripherals' analog alternate using
            GPIO_AnaAFConfig() function.
        (+) For other peripherals (TIM, USART...), Connect the pin to the desired
            peripherals' Alternate Function (AF) using GPIO_AltFuncConfig()
            function.
        (+) Configure the desired pin in alternate function mode using
            GPIO_InitStruct->GPIO_Mode = GPIO_MODE_ALT_FUNC.
        (+) Select the type, pull-up/pull-down and output speed via
            GPIO_PuPd, GPIO_OType and GPIO_Speed members
        (+) Call GPIO_Init() function.

      (#) To get the level of a pin configured in input mode using
          GPIO_ReadInputBit()

      (#) To set/reset the level of a pin configured in output mode use
          GPIO_SetBit()/GPIO_ResetBit()

      (#) During and just after reset, the alternate functions are not active
          and the GPIO pins are configured in input floating mode (except JTAG
          and reset pins).

      (#) The LSE oscillator pins OSC32_IN and OSC32_OUT can be used as general
          purpose (PC14 and PC15, respectively) when the LSE oscillator is off.
          The LSE has priority over the GPIO function.

      (#) GPIO interrupt configuration:
        (+) Configuration GPIO interrupt using GPIO_IntConfig() functions.
        (+) To activate the GPIO interrupt, using GPIO_IntCmd() functions.
        (+) Check on GPIO interrupt occur flags using the function
            GPIO_GetIntFlagStatus().
        (+) Clear GPIO interrupt flags using the function GPIO_ClearIntFlag().

 * @endverbatim
 *******************************************************************************/

#include "dpm32m0xx_gpio.h"



/**
 *******************************************************************************
 * @brief   Deinitializes the GPIOx peripheral registers to their default reset values,
 *          all pin set to input mode.
 * @param   [in]  GPIOx: Where x can be (A, B, C, D) to select the GPIO peripheral.
 * @retval  None.
 ******************************************************************************/
void GPIO_DeInit(GPIO_Type *GPIOx)
{
  /* Reset GPIOx out type. */
  GPIOx->OUT_TYPE = (uint32_t)0x00000000UL;

  /* Reset GPIOx out speed. */
  GPIOx->OUT_SPEED = (uint32_t)0x00000000UL;

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
  /* Disable GPIOx all interrupt. */
  GPIOx->INT_EN = (uint32_t)0x00000000UL;
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */

#if defined (DPM32M026) || defined (DPM32T55x)
  GPIOx->INT_CFG = (uint32_t)0x00000000UL;
#endif /* DPM32M026 || DPM32T55x */
  
  /* Clear all interrupt flag. */
  GPIOx->INT_SR = (uint32_t)0x0000FFFFUL;

  if(GPIOA == GPIOx)
  {
    /* Reset GPIOA mode, input mode. PA13,PA14 is default SWD functions. */
    GPIOx->MODE = (uint32_t)0x28000000UL;

    /* Reset GPIOA pull, not pull. */
    GPIOx->PUPD = (uint32_t)0x00000000UL;
  }
  else if(GPIOD == GPIOx)
  {
    /* Reset GPIOD mode, input mode. PD9 is default as AF0. */
    GPIOx->MODE = (uint32_t)0x08000000UL;

    /* Reset GPIOD pull, not pull. PD9 is default pull_up */
    GPIOx->PUPD = (uint32_t)0x00040000UL;
  }
  else
  {
    /* Reset GPIOx mode, all pin input mode. */
    GPIOx->MODE = (uint32_t)0x00000000UL;

    /* Reset GPIOx pull, not pull. */
    GPIOx->PUPD = (uint32_t)0x00000000UL;
  }
}

/**
 *******************************************************************************
 * @brief   Initializes the GPIOx peripheral according to the specified parameters
 *          in the GPIO_InitType.
 * @param   [in]  GPIOx: Where x can be (A, B, C, D) to select the GPIO peripheral.
 * @param   [in]  GPIO_InitType: Structure pointer of port configuration.
 * @retval  None.
 ******************************************************************************/
void GPIO_Init(GPIO_Type *GPIOx, GPIO_InitTypeStruct* GPIO_InitType)
{
  uint8_t i = 0u;
  /* Parameters check. */
  PARAM_ASSERT(IS_GPIO_PORT(GPIOx));
  PARAM_ASSERT(IS_GPIO_PIN(GPIO_InitType->GPIO_Pin));
  PARAM_ASSERT(IS_GPIO_MODE(GPIO_InitType->GPIO_Mode));
  PARAM_ASSERT(IS_GPIO_PUPD(GPIO_InitType->GPIO_PuPd));
  PARAM_ASSERT(IS_GPIO_OUTPUT_TYPE(GPIO_InitType->GPIO_OType));
  PARAM_ASSERT(IS_GPIO_SPEED(GPIO_InitType->GPIO_Speed));

  for(i = 0u; i < 16u; i++)
  {
    if(GPIO_InitType->GPIO_Pin & (uint32_t)(1u << i))
    {
      /* Clear the pin output type bit. */
      GPIOx->OUT_TYPE &= ~GPIO_OTYPE_MASK(i);
      /* Set the pin output type bit. */
      GPIOx->OUT_TYPE |= GPIO_OTYPE_SET(i, GPIO_InitType->GPIO_OType);

      /* Clear the pin speed bit. */
      GPIOx->OUT_SPEED &= ~GPIO_SPEED_MASK(i);
      /* Set the pin speed bit. */
      GPIOx->OUT_SPEED |= GPIO_SPEED_SET(i, GPIO_InitType->GPIO_Speed);

      /* Clear the pin mode bit. */
      GPIOx->MODE &= ~GPIO_MODE_MASK(i);
      /* Set the pin mode bit. */
      GPIOx->MODE |= GPIO_MODE_SET(i, GPIO_InitType->GPIO_Mode);

      /* Clear the pin pull bit. */
      GPIOx->PUPD &= ~GPIO_PUPD_MASK(i);
      /* Set the pin pull bit. */
      GPIOx->PUPD |= GPIO_PUPD_SET(i, GPIO_InitType->GPIO_PuPd);
    }
  }
}

/**
 *******************************************************************************
 * @brief   Initialize the GPIO_InitType with default parameters.
 * @param   [out]  GPIO_InitType: Pointer to a GPIO_InitTypeStruct structure
 *                 which will be initialized.
 * @retval  None.
 ******************************************************************************/
void GPIO_StructInit(GPIO_InitTypeStruct* GPIO_InitType)
{
  GPIO_InitType->GPIO_Pin = GPIO_PIN_ALL;
  GPIO_InitType->GPIO_Mode = GPIO_MODE_INPUT;
  GPIO_InitType->GPIO_Speed = GPIO_SPEED_LOW;
  GPIO_InitType->GPIO_OType = GPIO_OTYPE_PP;
  GPIO_InitType->GPIO_PuPd = GPIO_PUPD_NOPULL;
}

/**
 *******************************************************************************
 * @brief   Write disable some GPIO registers.
 * @param   [in]  GPIOx: Where x can be (A, B, C, D) to select the GPIO peripheral.
 * @retval  None.
 * @note    GPIOx_MODER, GPIOx_OTYPER, GPIOx_PUPDR, GPIOx_AFRL and GPIOx_AFRH
 *          registers write disable.
 ******************************************************************************/
void GPIO_Lock(GPIO_Type *GPIOx)
{
  GPIOx->LOCK = 0x900D0001UL;
}

/**
 *******************************************************************************
 * @brief   Write enables some GPIO registers.
 * @param   [in]  GPIOx: Where x can be (A, B, C, D) to select the GPIO peripheral.
 * @retval  None.
 * @note    GPIOx_MODER, GPIOx_OTYPER, GPIOx_PUPDR, GPIOx_AFRL and GPIOx_AFRH
 *          registers write enable.
 ******************************************************************************/
void GPIO_Unlock(GPIO_Type *GPIOx)
{
  GPIOx->LOCK = 0x900D0000UL;
}

/**
 *******************************************************************************
 * @brief   Set the specified GPIO pin status.
 * @param   [in]  GPIOx: Where x can be (A, B, C, D) to select the GPIO peripheral.
 * @param   [in]  GPIO_Pin: Specifies the port bits to be written.
 * @retval  None.
 ******************************************************************************/
void GPIO_SetBit(GPIO_Type *GPIOx, uint16_t GPIO_Pin)
{
  /* Set bit. */
  GPIOx->BIT_SET_RST = GPIO_Pin;
}

/**
 *******************************************************************************
 * @brief   Reset the specified GPIO pin status.
 * @param   [in]  GPIOx: Where x can be (A, B, C, D) to select the GPIO peripheral.
 * @param   [in]  GPIO_Pin: Specifies the port bits to be written.
 * @retval  None.
 ******************************************************************************/
void GPIO_ResetBit(GPIO_Type *GPIOx, uint16_t GPIO_Pin)
{
  /* Reset bit. */
  GPIOx->BIT_RST = GPIO_Pin;
}

/**
 *******************************************************************************
 * @brief   Toggle the specified GPIO pin status.
 * @param   [in]  GPIOx: Where x can be (A, B, C, D) to select the GPIO peripheral.
 * @param   [in]  GPIO_Pin: Specifies the port bits to be written.
 * @retval  None.
 ******************************************************************************/
void GPIO_ToggleBit(GPIO_Type *GPIOx, uint16_t GPIO_Pin)
{
  if((GPIOx->DATA_OUT & GPIO_Pin) != 0x00u)
  {
    /* Reset bit */
    GPIOx->BIT_RST = (uint32_t)GPIO_Pin;
  }
  else
  {
    /* Set bit */
    GPIOx->BIT_SET_RST = (uint32_t)GPIO_Pin;
  }
}

/**
 *******************************************************************************
 * @brief   Write data to the specified GPIO pin.
 * @param   [in]  GPIOx: Where x can be (A, B, C, D) to select the GPIO peripheral.
 * @param   [in]  GPIO_Pin: Specifies the port bits to be written.
 * @param   [in]  PinValue: Specifies the value to be written to the selected bit.
 *                The new state of GPIO_Pin (BIT_SET or BIT_RESET).
 * @retval None.
 ******************************************************************************/
void GPIO_WriteBit(GPIO_Type *GPIOx, uint16_t GPIO_Pin, GPIO_BitState PinValue)
{
  if(BIT_RESET != PinValue)
  {
    /* Set bit. */
    GPIOx->BIT_SET_RST = (uint32_t)GPIO_Pin;
  }
  else
  {
    /* Reset bit */
    GPIOx->BIT_RST = (uint32_t)GPIO_Pin;
  }
}

/**
 *******************************************************************************
 * @brief   Set the specified GPIO port status.
 * @param   [in]  GPIOx: Where x can be (A, B, C, D) to select the GPIO peripheral.
 * @retval  None.
 ******************************************************************************/
void GPIO_SetPort(GPIO_Type* GPIOx)
{
  GPIOx->BIT_SET_RST = (uint32_t)0xFFFFUL;
}

/**
 *******************************************************************************
 * @brief   Reset the specified GPIO port status.
 * @param   [in]  GPIOx: Where x can be (A, B, C, D) to select the GPIO peripheral.
 * @retval  None.
 ******************************************************************************/
void GPIO_ResetPort(GPIO_Type* GPIOx)
{
  GPIOx->BIT_RST = (uint32_t)0xFFFFUL;
}

/**
 *******************************************************************************
 * @brief   Toggle the specified GPIO port status.
 * @param   [in]  GPIOx: Where x can be (A, B, C, D) to select the GPIO peripheral.
 * @retval  None.
 ******************************************************************************/
void GPIO_TogglePort(GPIO_Type* GPIOx)
{
  uint16_t i = 0u;

  for(i = 0u; i < 16u; i++)
  {
    if((GPIOx->DATA_OUT & (1u << i)) != 0x00u)
    {
      /* Reset bit. */
      GPIOx->BIT_RST = (uint32_t)(1u << i);
    }
    else
    {
      /* Set bit. */
      GPIOx->BIT_SET_RST = (uint32_t)(1u << i);
    }
  }
}

/**
 *******************************************************************************
 * @brief   Write data to the specified GPIO port.
 * @param   [in]  GPIOx: Where x can be (A, B, C, D) to select the GPIO peripheral.
 * @param   [in]  GPIO_Value: Specifies the port value to be written.
 * @retval  None.
 ******************************************************************************/
void GPIO_WritePort(GPIO_Type *GPIOx, uint16_t GPIO_Value)
{
  GPIOx->DATA_OUT = (uint32_t)GPIO_Value;
}

/**
 *******************************************************************************
 * @brief   Read the specified GPIO input pin status.
 * @param   [in]  GPIOx: Where x can be (A, B, C, D) to select the GPIO peripheral.
 * @param   [in]  GPIO_Pin: Specifies the port bits to be get.
 * @retval  uint8_t: The input port bit state(BIT_SET or BIT_RESET).
 ******************************************************************************/
uint8_t GPIO_ReadInputBit(GPIO_Type *GPIOx, uint16_t GPIO_Pin)
{
  GPIO_BitState state = BIT_RESET;

  if((uint32_t)BIT_RESET != ((GPIOx->DATA_IN) & (GPIO_Pin)))
  {
    state =  BIT_SET;
  }
  else
  {
    state =  BIT_RESET;
  }

  return state;
}

/**
 *******************************************************************************
 * @brief   Read the specified GPIO input port status.
 * @param   [in]  GPIOx: Where x can be (A, B, C, D) to select the GPIO peripheral.
 * @retval  uint16_t: The input port value.
 ******************************************************************************/
uint16_t GPIO_ReadInputPort(GPIO_Type *GPIOx)
{
  return (uint16_t)GPIOx->DATA_IN;
}

/**
 *******************************************************************************
 * @brief   Read the specified GPIO output pin status.
 * @param   [in]  GPIOx: Where x can be (A, B, C, D) to select the GPIO peripheral.
 * @param   [in]  GPIO_Pin: Specifies the port bits to be get.
 * @retval  uint8_t: The output port bit state(BIT_SET or BIT_RESET).
 ******************************************************************************/
uint8_t GPIO_ReadOutputBit(GPIO_Type *GPIOx, uint16_t GPIO_Pin)
{
  GPIO_BitState state = BIT_RESET;

  if((uint32_t)BIT_RESET != ((GPIOx->DATA_OUT) & (GPIO_Pin)))
  {
    state = BIT_SET;
  }
  else
  {
    state = BIT_RESET;
  }

  return state;
}

/**
 *******************************************************************************
 * @brief   Read the specified GPIO output port status.
 * @param   [in]  GPIOx: Where x can be (A, B, C, D) to select the GPIO peripheral.
 * @retval  uint16_t: The output port value.
 ******************************************************************************/
uint16_t GPIO_ReadOutputPort(GPIO_Type *GPIOx)
{
  return (uint16_t)GPIOx->DATA_OUT;
}

/**
 *******************************************************************************
 * @brief   Set the specified GPIO pin alternate function.
 * @param   [in]  GPIOx: Where x can be (A, B, C, D) to select the GPIO peripheral.
 * @param   [in]  GPIO_Pin: Specifies the port bits to be set.
 * @param   [in]  GPIO_AltFunc: Selects the pin to used as Alternate function.
 *                This parameter can be a value of @ref GPIO_AltFuncEnum.
 *                DPM32M08x|DPM32M05x|DPM32M03x|
 *                DPM32M036|DPM32M030|DPM32M015:
 *                  @arg GPIO_ALT_FUNC_0: RESET,JTAG,UART0,UART1,SPI0,SPI1,EPWM,CCT0,
 *                       CCT1,POSIF,MCO,ADC.
 *                  @arg GPIO_ALT_FUNC_1: UART0,UART1,UART2,SPI1,I2C,CCT0,CCT1,POSIF.
 *                  @arg GPIO_ALT_FUNC_2: UART0,EPWM,CCT0,POSIF.
 *                  @arg GPIO_ALT_FUNC_3: CAN,CCT1,POSIF.
 *                  @arg GPIO_ALT_FUNC_4: UART2,I2C,CAN.
 *                  @arg GPIO_ALT_FUNC_5: SPI1,I2C.
 *                  @arg GPIO_ALT_FUNC_6: .
 *                  @arg GPIO_ALT_FUNC_7: .
 *                DPM32M026|DPM32T55x:
 *                  @arg GPIO_ALT_FUNC_EPWM_CH0_P: Analog alternate select epwm ch0 postive channel.
 *                  @arg GPIO_ALT_FUNC_EPWM_CH0_N: Analog alternate select epwm ch0 negtive channel.
 *                  @arg GPIO_ALT_FUNC_EPWM_CH1_P: Analog alternate select epwm ch1 postive channel.
 *                  @arg GPIO_ALT_FUNC_EPWM_CH1_N: Analog alternate select epwm ch1 negtive channel.
 *                  @arg GPIO_ALT_FUNC_EPWM_CH2_P: Analog alternate select epwm ch2 postive channel.
 *                  @arg GPIO_ALT_FUNC_EPWM_CH2_N: Analog alternate select epwm ch2 negtive channel.
 *                  @arg GPIO_ALT_FUNC_TIM0_PWM: Analog alternate select tim0 pwm out.
 *                  @arg GPIO_ALT_FUNC_TIM1_PWM: Analog alternate select tim1 pwm out.
 *                  @arg GPIO_ALT_FUNC_SPI_SCK: Analog alternate select spi clock.
 *                  @arg GPIO_ALT_FUNC_SPI_MOSI: Analog alternate select spi mosi.
 *                  @arg GPIO_ALT_FUNC_SPI_MISO: Analog alternate select spi miso.
 *                  @arg GPIO_ALT_FUNC_CCT0_CH0: Analog alternate select cct0 channel 0.
 *                  @arg GPIO_ALT_FUNC_CCT0_CH1: Analog alternate select cct0 channel 1.
 *                  @arg GPIO_ALT_FUNC_CCT0_CH2: Analog alternate select cct0 channel 2.
 *                  @arg GPIO_ALT_FUNC_SPI_NSS: Analog alternate select spi nss.
 *                  @arg GPIO_ALT_FUNC_SWCLK: Analog alternate select swclk.
 *                  @arg GPIO_ALT_FUNC_ADC_TRIGGER: Analog alternate select adc trigger.
 *                  @arg GPIO_ALT_FUNC_UART0_TX: Analog alternate select uart0 tx.
 *                  @arg GPIO_ALT_FUNC_SWDIO: Analog alternate select swdio.
 *                  @arg GPIO_ALT_FUNC_SYS_DBG: Analog alternate select system debug.
 *                  @arg GPIO_ALT_FUNC_BUTTON_RSTN: Analog alternate select button reset, PD9 exclusive.
 * @retval  None.
 ******************************************************************************/
void GPIO_AltFuncConfig(GPIO_Type* GPIOx, uint16_t GPIO_Pin, uint32_t GPIO_AltFunc)
{
  uint16_t i = 0u;

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
  for(i = 0u; i < 8u; i++)
  {
    if(GPIO_Pin & (uint16_t)(1u << i))
    {
      /* Clear the alternate function bits. */
      GPIOx->AF_LOW &= ~GPIO_AF_MASK(i);
      /* Set the alternate function bits. */
      GPIOx->AF_LOW |= GPIO_AF_SET(i, GPIO_AltFunc);
    }
  }

  for(i = 8u; i < 16u; i++)
  {
    if(GPIO_Pin & (uint16_t)(1u << i))
    {
      /* Clear the alternate function bits. */
      GPIOx->AF_HIGH &= ~GPIO_AF_MASK(i - 8u);
      /* Set the alternate function bits. */
      GPIOx->AF_HIGH |= GPIO_AF_SET(i - 8u, GPIO_AltFunc);
    }
  }
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */

#if defined (DPM32M026) || defined (DPM32T55x)
  for(i = 0u; i < 4u; i++)
  {
    if(GPIO_Pin & (uint16_t)(1u << i))
    {
      /* Clear the alternate function bits. */
      GPIOx->ALT_FUNC0 &= ~GPIO_AF_MASK(i);
      /* Set the alternate function bits. */
      GPIOx->ALT_FUNC0 |= GPIO_AF_SET(i, GPIO_AltFunc);
    }
  }
  
  for(i = 4u; i < 8u; i++)
  {
    if(GPIO_Pin & (uint16_t)(1u << i))
    {
      /* Clear the alternate function bits. */
      GPIOx->ALT_FUNC1 &= ~GPIO_AF_MASK(i - 4u);
      /* Set the alternate function bits. */
      GPIOx->ALT_FUNC1 |= GPIO_AF_SET(i - 4u, GPIO_AltFunc);
    }
  }
  
  for(i = 8u; i < 12u; i++)
  {
    if(GPIO_Pin & (uint16_t)(1u << i))
    {
      /* Clear the alternate function bits. */
      GPIOx->ALT_FUNC2 &= ~GPIO_AF_MASK(i - 8u);
      /* Set the alternate function bits. */
      GPIOx->ALT_FUNC2 |= GPIO_AF_SET(i - 8u, GPIO_AltFunc);
    }
  }
  
  for(i = 12u; i < 16u; i++)
  {
    if(GPIO_Pin & (uint16_t)(1u << i))
    {
      /* Clear the alternate function bits. */
      GPIOx->ALT_FUNC3 &= ~GPIO_AF_MASK(i - 12u);
      /* Set the alternate function bits. */
      GPIOx->ALT_FUNC3 |= GPIO_AF_SET(i - 12u, GPIO_AltFunc);
    }
  }
#endif /* DPM32M026 || DPM32T55x */
}

#if defined (DPM32M026) || defined (DPM32T55x)
/**
 *******************************************************************************
 * @brief   Read the specified GPIO output pin status.
 * @param   [in]  GPIOx: Where x can be (A, B, C, D) to select the GPIO peripheral.
 * @param   [in]  GPIO_AltSysDbg: Specifies the port system debug functions.
 *                This parameter can be a value of @ref GPIO_AltSysDbgEnum.
 *                  @arg GPIO_ALT_SYS_DBG_MCO: Select MCO functions.
 *                  @arg GPIO_ALT_SYS_DBG_EPMW_ADC_TRIG: Select EPMW ADC trigger functions.
 *                  @arg GPIO_ALT_SYS_DBG_ACMP0_OUT: Select ACMP0 output functions.
 *                  @arg GPIO_ALT_SYS_DBG_ACMP1_OUT: Select ACMP1 output functions.
 * @retval  uint8_t: The output port bit state(BIT_SET or BIT_RESET).
 ******************************************************************************/
void GPIO_SysDbgConfig(GPIO_Type* GPIOx, GPIO_AltSysDbgEnum GPIO_AltSysDbg)
{
  /* Set the SYS_DBG_SEL[1:0] bits. */
  GPIOx->ALT_FUNC4 = GPIO_AltSysDbg;
}
#endif /* DPM32M026 || DPM32T55x */


/**
 *******************************************************************************
 * @brief   Set the specified GPIO pin analog alternate function.
 * @param   [in]  GPIOx: Where x can be (A, B, C, D) to select the GPIO peripheral.
 * @param   [in]  GPIO_Pin: Specifies the port bits to be set.
 * @param   [in]  GPIO_AnaAF: Selects the pin to used as analog Alternate function.
 *                This parameter can be a value of @ref GPIO_AnaAltFuncEnum.
 *                DPM32M08x|DPM32M05x|DPM32M03x|
 *                DPM32M036|DPM32M030|DPM32M015:
 *                  @arg GPIO_ANA_AF_0: Analog alternate 0 functions.
 *                  @arg GPIO_ANA_AF_1: analog alternate 1 functions.
 *                DPM32M026|DPM32T55x:
 *                  @arg GPIO_ANA_AF_ADC: Analog alternate select adc.
 *                  @arg GPIO_ANA_AF_TCH: Analog alternate select tch.
 *                  @arg GPIO_ANA_AF_PGA: Analog alternate select pga.
 *                  @arg GPIO_ANA_AF_ACMP: Analog alternate select acmp.
 * @retval  None.
 * @note    Analog alternate functions form.
 *                DPM32M08x|DPM32M05x|DPM32M03x|
 *                DPM32M036|DPM32M030|DPM32M015:
 * ----------------------------------------------------------------------------------
 *               | PA0       | PA1      | PA2      | PA3      | PA4      | PA5
 * GPIO_ANA_AF_0 | ADC_IN0   | ADC_IN1  | ADC_IN2  | ADC_IN3  | ADC_IN4  | ADC_IN5
 * GPIO_ANA_AF_1 | PGA0_IN0  | PGA0_IN1 | PGA1_IN0 | PGA1_IN1 | ACMP_IN0 | ACMP_IN1
 * ----------------------------------------------------------------------------------
 *               | PA6       | PA7      | PB0      | PB1      | PB10     | PB11
 * GPIO_ANA_AF_0 | ADC_IN6   | ADC_IN7  | ADC_IN8  | ADC_IN9  | PGA2_IN0 | PGA2_IN1
 * GPIO_ANA_AF_1 | ACMP_IN2  |          |          |          |          |
 * ----------------------------------------------------------------------------------
 *               | PC0       | PC1      | PC2      | PC3      | PD11     | PD12
 * GPIO_ANA_AF_0 | ADC_IN10  | ADC_IN11 | ADC_IN12 | ADC_IN13 | DAC_OUT  | PGA3_IN0
 * GPIO_ANA_AF_1 |           |          |          |          | ACMP_IN3 | ACMP_IN4
 * ----------------------------------------------------------------------------------
 *               | PD13
 * GPIO_ANA_AF_0 | PGA3_IN1
 * GPIO_ANA_AF_1 | ACMP_IN5
 ******************************************************************************/
void GPIO_AnaAFConfig(GPIO_Type* GPIOx, uint16_t GPIO_Pin, GPIO_AnaAltFuncEnum GPIO_AnaAF)
{
  uint16_t i = 0u;

  /* Configuration ADC functions. */
  for(i = 0u; i <= 15u; i++)
  {
    if((GPIO_Pin & (uint16_t)(1u << i)))
    {
      /* Clear the analog alternate function bits. */
      GPIOx->ANA_AF &= ~GPIO_ANA_AF_MASK(i);

      /* Set the analog alternate function bits. */
      GPIOx->ANA_AF |= GPIO_ANA_AF_SET(i, GPIO_AnaAF);
    }
  }
}

/**
 *******************************************************************************
 * @brief   Configuration the GPIO interrupt type.
 * @param   [in]  GPIOx: Where x can be (A, B, C, D) to select the GPIO peripheral.
 * @param   [in]  GPIO_Pin: Specifies the port bits to be interrupt.
 * @param   [in]  GPIO_IntType: GPIO interrupt type.
 *                This parameter can be a value of @ref GPIO_IntTypeEnum.
 *                DPM32M08x|DPM32M05x|DPM32M03x|
 *                DPM32M036|DPM32M030|DPM32M015:
 *                  @arg GPIO_INT_RISING: rising trigger interrupt.
 *                  @arg GPIO_INT_FALLING: falling trigger interrupt.
 *                  @arg GPIO_INT_HIGH: high level trigger interrupt.
 *                  @arg GPIO_INT_LOW: low level trigger interrupt.
 *                  @arg GPIO_INT_RISING_FALLING: rising or falling trigger interrupt.
 *                DPM32M026|DPM32T55x:
 *                  @arg GPIO_INT_DISABLE: disable interrupt.
 *                  @arg GPIO_INT_RISING: rising trigger interrupt.
 *                  @arg GPIO_INT_FALLING: falling trigger interrupt.
 *                  @arg GPIO_INT_RISING_FALLING: rising or falling trigger interrupt.
 * @retval  None.
 ******************************************************************************/
void GPIO_IntConfig(GPIO_Type* GPIOx, uint16_t GPIO_Pin, GPIO_IntTypeEnum GPIO_IntType)
{
  uint16_t i = 0u;

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
  for(i = 0u; i < 8u; i++)
  {
    if(GPIO_Pin & (1u << i))
    {
      /* Clear the interrput type bits. */
      GPIOx->INT_TYPE_LOW &= ~GPIO_INT_TYPE_MASK(i);
      /* Set the interrput type bits. */
      GPIOx->INT_TYPE_LOW |= GPIO_INT_TYPE_SET(i, GPIO_IntType);
    }
  }

  for(i = 8u; i < 16u; i++)
  {
    if(GPIO_Pin & (1u << i))
    {
      /* Clear the interrput bits. */
      GPIOx->INT_TYPE_HIGH &= ~GPIO_INT_TYPE_MASK(i - 8u);
      /* Set the interrput type bits. */
      GPIOx->INT_TYPE_HIGH |= GPIO_INT_TYPE_SET(i - 8u, GPIO_IntType);
    }
  }
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */

#if defined (DPM32M026) || defined (DPM32T55x)
  for(i = 0u; i < 16u; i++)
  {
    if(GPIO_Pin & (1u << i))
    {
      /* Clear the interrput type bits. */
      GPIOx->INT_CFG &= ~GPIO_INT_TYPE_MASK(i);
      /* Set the interrput type bits. */
      GPIOx->INT_CFG |= GPIO_INT_TYPE_SET(i, GPIO_IntType);
    }
  }
#endif /* DPM32M026 || DPM32T55x */
}

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
/**
 *******************************************************************************
 * @brief   Enable the specified GPIO pin interrupt.
 * @param   [in]  GPIOx: Where x can be (A, B, C, D) to select the GPIO peripheral.
 * @param   [in]  GPIO_Pin: Specifies the port bits to be interrupt.
 * @retval  None.
 ******************************************************************************/
void GPIO_IntEnable(GPIO_Type* GPIOx, uint16_t GPIO_Pin)
{
  uint16_t i = 0u;

  for(i = 0u; i < 16u; i++)
  {
    if((GPIO_Pin & (uint16_t)(1u << i)))
    {
      /* Clear the interrupt enable bits. */
      GPIOx->INT_EN &= ~GPIO_INT_EN_MASK(i);
      /* Set the interrupt enable bits. */
      GPIOx->INT_EN |= GPIO_INT_EN_SET(i, ENABLE);
    }
  }
}

/**
 *******************************************************************************
 * @brief   Disable the specified GPIO pin interrupt.
 * @param   [in]  GPIOx: Where x can be (A, B, C, D) to select the GPIO peripheral.
 * @param   [in]  GPIO_Pin: Specifies the port bits to be interrupt.
 * @retval  None.
 ******************************************************************************/
void GPIO_IntDisable(GPIO_Type* GPIOx, uint16_t GPIO_Pin)
{
  uint16_t i = 0u;

  for(i = 0u; i < 16u; i++)
  {
    if((GPIO_Pin & (uint16_t)(1u << i)))
    {
      /* Clear the interrupt enable bits. */
      GPIOx->INT_EN &= ~GPIO_INT_EN_MASK(i);
    }
  }
}

/**
 *******************************************************************************
 * @brief   Enable or disable the specifies GPIO pin interrupt.
 * @param   [in]  GPIOx: Where x can be (A, B, C, D) to select the GPIO peripheral.
 * @param   [in]  GPIO_Pin: Specifies the port bits to be interrupt.
 * @param   [in]  NewState: This parameter can be: ENABLE or DISABLE.
 * @retval  None.
 ******************************************************************************/
void GPIO_IntCmd(GPIO_Type* GPIOx, uint16_t GPIO_Pin, FunctionalState NewState)
{
  uint16_t i = 0u;

  for(i = 0u; i < 16u; i++)
  {
    if((GPIO_Pin & (uint16_t)(1u << i)))
    {
      /* Clear the interrupt enable bits. */
      GPIOx->INT_EN &= ~GPIO_INT_EN_MASK(i);
      /* Set the interrupt enable bits. */
      GPIOx->INT_EN |= GPIO_INT_EN_SET(i, NewState);
    }
  }
}
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */

/**
 *******************************************************************************
 * @brief   Get the specified GPIO pin interrupt flag status.
 * @param   [in]  GPIOx: Where x can be (A, B, C, D) to select the GPIO peripheral.
 * @param   [in]  GPIO_Pin: Specifies the port bits to be interrupt.
 * @retval  FlagState: Interrupt flag status(SET or RESET).
 ******************************************************************************/
FlagState GPIO_GetIntFlagStatus(GPIO_Type* GPIOx, uint16_t GPIO_Pin)
{
  if((GPIOx->INT_SR & GPIO_Pin) != RESET)
  {
    return SET;
  }
  else
  {
    return RESET;
  }
}

/**
 *******************************************************************************
 * @brief   Clear the specified GPIO pin interrupt flag.
 * @param   [in]  GPIOx: Where x can be (A, B, C, D) to select the GPIO peripheral.
 * @param   [in]  GPIO_Pin: Specifies the port bits to be interrupt.
 * @retval  None.
 ******************************************************************************/
void GPIO_ClearIntFlag(GPIO_Type* GPIOx, uint16_t GPIO_Pin)
{
  /* Write 1 to interrupt status register, the corresponding GPIO hardware interrupt
     and interrupt status bits are cleared. */
  GPIOx->INT_SR = GPIO_Pin;
}
