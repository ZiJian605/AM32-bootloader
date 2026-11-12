/**
 *******************************************************************************
 * @file    dpm32m0xx_rcc.c
 *
 * @brief   Source file for RCC firmware driver.
 *          This file provides firmware functions to manage the following
 *          functionalities of the Reset and clock control (RCC) peripheral:
 *           + Internal/external clocks, and MCO configuration
 *           + System, AHB and APB busses clocks configuration
 *           + Peripheral clocks configuration
 *           + Low power mode configuration
 *           + Start up addr configuration
 *
 * @author  DPM
 *
 * @version V1.0.0
 *
 * @date    2023-11-01
 *
 * @verbatim
 ===============================================================================
                       ##### RCC specific features #####
 ===============================================================================
    [..]
      After reset the device is running from Internal High Speed oscillator
      (HSI 48MHz) with Flash 3 wait state, Flash prefetch buffer, all
      peripherals are off except internal SRAM, Flash and JTAG.
      (+) There is no prescaler on High speed (AHB) and Low speed (APB) busses;
          all peripherals mapped on these busses are running at HSI speed.
      (+) The clock for all peripherals is switched off, except the SRAM and
          FLASH.
      (+) All GPIOs are in input floating state, except the JTAG pins and Reset
          pin which are assigned to be used for debug purpose.

    [..]
      Once the device started from reset, the user application has to:
      (+) Configure the clock source to be used to drive the System clock
          (if the application needs higher frequency/performance).
      (+) Configure the System clock frequency and Flash settings.
      (+) Configure the AHB and APB busses prescalers.
      (+) Enable the clock for the peripheral(s) to be used.

    *** System clock configuration ***
    =============================================
      (#) Unlock register write protect using RCC_Unlock().

      (#) Program LSI and LSE status using RCC_OscConfig().

      (#) Program HCLK and PCLK division using RCC_ClockConfig().

    *** MCO configuration ***
    =============================================
      (#) Unlock register write protect using RCC_Unlock().

      (#) Program MCO output source using RCC_MCOConfig().

      (#) Enable output using RCC_MCOEnable().

    *** Low power mode configuration ***
    =============================================
      (#) Unlock register write protect using RCC_Unlock().

      (#) Program peripheral clock status using RCC_DeepSleepModePeriphCmd()
          or RCC_StopModeFunctionCmd().

      (#) System enter low power mode using RCC_EnterLowPowerMode().

 * @endverbatim
 *******************************************************************************/

#include "dpm32m0xx_rcc.h"

/**
 *******************************************************************************
 * @brief   Unlocks the reset and clock configuration register.
 * @retval  None.
 ******************************************************************************/
void RCC_Unlock(void)
{
  RCC->LOCK_CR = 0x900D0000;
}

/**
 *******************************************************************************
 * @brief   Locks the reset and clock configuration register
 * @retval  None.
 ******************************************************************************/
void RCC_Lock(void)
{
  RCC->LOCK_CR = 0x900D0001;
}

/**
 *******************************************************************************
 * @brief   Reset the RCC register to the default reset state.
 * @retval  None.
 ******************************************************************************/
void RCC_DeInit(void)
{
  /* Reset the RCC register to the default reset state. */
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
  RCC->LOCK_CR = 0x900D0000;
  RCC->COMMON_CR = (uint32_t)0x5C000000;
  RCC->CLKEN_CR = (uint32_t)0x00000000;
  RCC->LPM_CR = (uint32_t)0x00000000;
  RCC->DEEP_SLEEP_CR = (uint32_t)0x00000000;
  RCC->STOP_CR = (uint32_t)0x00000006;
  RCC->SYSTEM_CR = (uint32_t)0x00000000;
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */
  
#if defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
  RCC->LOCK_CR = 0x900D0000;
  RCC->COMMON_CR = (uint32_t)0x03000000;
  RCC->CLKEN_CR = (uint32_t)0x00000000;
  RCC->SYSTEM_CR = (uint32_t)0x18000000;
  RCC->LPM_CR = (uint32_t)0x00000000;
  RCC->DEEP_SLEEP_CR = (uint32_t)0x00000000;
  RCC->STOP_CR = (uint32_t)0x00000000;
#endif /* DPM32M036 || DPM32M030 || DPM32M015 */

#if defined (DPM32M026) || defined (DPM32T55x)
  RCC->LOCK_CR = 0x900D0000;
  RCC->COMMON_CR = (uint32_t)0x00000000;
  RCC->CLKEN_CR = (uint32_t)0x00000000;
  RCC->SYSTEM_CR = (uint32_t)0x18000000;
  RCC->STOP_CR = (uint32_t)0x00000000;
#endif  /* DPM32M026 || DPM32T55x */

#if defined (DPM32M08x) || defined (DPM32M05x)
  /* Reset PLL clock configuration. */
  RCC->PLL_CR = (uint32_t)0x00000000;
#endif  /* DPM32M08x || DPM32M05x */

}

/**
 *******************************************************************************
 * @brief   Check RCC clock flag is set or not.
 * @param   [in]  RCC_Clockflag: Specifies the flag to check.
 *                This parameter can be a value of @ref RCC_ClockFlagEnum.
 *                  @arg RCC_CLOCK_FLAG_LSIRDY: Get LSI clock ready flag.
 *                  @arg RCC_CLOCK_FLAG_HSERDY: Get HSE clock ready flag.
 *                  @arg RCC_CLOCK_FLAG_LSERDY: Get LSE clock ready flag.
 *                  @arg RCC_CLOCK_FLAG_PLLRDY: Get PLL clock ready flag.
 * @retval  The new state of __FLAG__ (SET or RESET).
 ******************************************************************************/
FlagState RCC_GetClockFlag(RCC_ClockFlagEnum RCC_Clockflag)
{
  FlagState state = RESET;

#if defined (DPM32M08x) || defined (DPM32M05x)
  /* Get PLL clock ready flag. */
  if(RCC_Clockflag == RCC_CLOCK_FLAG_PLLRDY)
  {
    if(RESET != (RCC->PLL_CR & ((uint32_t)RCC_Clockflag)))
    {
      state = SET;
    }
    else
    {
      state = RESET;
    }
  }
  else
  {
#endif  /* DPM32M08x || DPM32M05x */

    if(RESET != (RCC->COMMON_CR & ((uint32_t)RCC_Clockflag)))
    {
      state = SET;
    }
    else
    {
      state = RESET;
    }

#if defined (DPM32M08x) || defined (DPM32M05x)
  }

#endif  /* DPM32M08x || DPM32M05x */

  /* Return the status of the clock flag status. */
  return state;
}

/**
 *******************************************************************************
 * @brief   Initializes the RCC Oscillators according to the specified parameters
 *          in the RCC_OscInitTypeDef.
 * @param   [in]  RCC_OscInitType: Structure pointer of RCC configuration.
 * @retval  RCC_StatusEnum: Oscillators configuration status.
 ******************************************************************************/
RCC_StatusEnum RCC_OscConfig(RCC_OscInitTypeStruct * RCC_OscInitType)
{
  uint32_t timeCount = 0u;
  uint32_t readyFlag = 0u;

#if defined (DPM32M08x) || defined (DPM32M05x)
  uint32_t tmpReg = 0u;
#endif

  /* Check Null pointer */
  if(RCC_OscInitType == NULL)
  {
    return RCC_ERROR;
  }

#if defined (DPM32M08x)
  /*-------------------------------- HSE Configuration -----------------------*/
  if((RCC_OscInitType->RCC_OscillatorType & RCC_OSCILLATOR_TYPE_HSE) == RCC_OSCILLATOR_TYPE_HSE)
  {
    if(RCC_HSE_OFF != RCC_OscInitType->RCC_HSEState)
    {
      /* Enable the HSE. */
      RCC->COMMON_CR |= RCC_COMMON_CR_HSE_EN_Msk;
      /* Wait till HSE is ready */
      timeCount = 0u;

      do
      {
        timeCount++;
        readyFlag = RCC_GetClockFlag(RCC_CLOCK_FLAG_HSERDY);
      }
      while ((readyFlag == 0x00u) && (timeCount != HSE_TIMEOUT_VALUE));

      /* If fail.*/
      if (RCC_GetClockFlag(RCC_CLOCK_FLAG_HSERDY) == RESET)
      {
        return RCC_TIMEOUT;
      }
    }
    else
    {
      /* Disable the HSE. */
      RCC->COMMON_CR &= ~RCC_COMMON_CR_HSE_EN_Msk;
      /* Wait till HSE is disabled */
      timeCount = 0u;
  
      do
      {
        timeCount++;
        readyFlag = RCC_GetClockFlag(RCC_CLOCK_FLAG_HSERDY);
      }
      while ((readyFlag == 0x01u) && (timeCount != HSE_TIMEOUT_VALUE));

      /* If fail.*/
      if (RCC_GetClockFlag(RCC_CLOCK_FLAG_HSERDY) != RESET)
      {
        return RCC_TIMEOUT;
      }
    }
  }
#endif /* DPM32M08x */

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
  /*-------------------------------- LSE Configuration -----------------------*/
  if((RCC_OscInitType->RCC_OscillatorType & RCC_OSCILLATOR_TYPE_LSE) == RCC_OSCILLATOR_TYPE_LSE)
  {
    if(RCC_LSE_OFF != RCC_OscInitType->RCC_LSEState)
    {
      /* Enable the LSE. */
      RCC->COMMON_CR |= RCC_COMMON_CR_LSE_EN_Msk;
      /* Wait till LSE is ready */
      timeCount = 0;

      do
      {
        timeCount++;
        readyFlag = RCC_GetClockFlag(RCC_CLOCK_FLAG_LSERDY);
      }
      while((readyFlag == 0x00u) && (timeCount != LSE_TIMEOUT_VALUE));

      /* If fail.*/
      if(RCC_GetClockFlag(RCC_CLOCK_FLAG_LSERDY) == RESET)
      {
        return RCC_TIMEOUT;
      }
    }
    else
    {
      /* Disable the LSE. */
      RCC->COMMON_CR &= ~RCC_COMMON_CR_LSE_EN_Msk;
      /* Wait till LSE is disabled */
      timeCount = 0;

      do
      {
        timeCount++;
        readyFlag = RCC_GetClockFlag(RCC_CLOCK_FLAG_LSERDY);
      }
      while((readyFlag == 0x01u) && (timeCount != LSE_TIMEOUT_VALUE));

      /* If fail.*/
      if(RCC_GetClockFlag(RCC_CLOCK_FLAG_LSERDY) != RESET)
      {
        return RCC_TIMEOUT;
      }
    }
  }
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */

#if defined (DPM32M08x) || defined (DPM32M05x)
  /*-------------------------------- PLL Configuration -----------------------*/
  if(RCC_PLL_NONE != RCC_OscInitType->RCC_PLL.PLLState)
  {
    if(RCC_PLL_OFF != RCC_OscInitType->RCC_PLL.PLLState)
    {
      /* Read the value of the COMMON_CR register. */
      tmpReg = RCC->COMMON_CR;

      /* Clear HSCLK_SEL bit. */
      tmpReg &= ~RCC_COMMON_CR_HSCK_SEL_Msk;

      /* Set HSCLK_SEL bit according to source value. */
      tmpReg |= (uint32_t)(RCC_OscInitType->RCC_PLL.PLLSource << RCC_COMMON_CR_HSCK_SEL_Pos);

      /* Store the new value. */
      RCC->COMMON_CR = tmpReg;

      /* Configures PLL clock multiplication. */
      /* Read the value of the PLL_CR register. */
      tmpReg = RCC->PLL_CR;

      /* Clear PLL_MUL[3:0] bits. */
      tmpReg &= ~RCC_PLL_CR_MULTPS_Msk;

      /* Set PLL_MUL[3:0] bits according to source value. */
      tmpReg |=  (uint32_t)(RCC_OscInitType->RCC_PLL.PLLMUL << RCC_PLL_CR_MULTPS_Pos);

      /* Enable PLL clock. */
      tmpReg |= RCC_PLL_CR_EN_Msk;

      /* Store the new value. */
      RCC->PLL_CR = tmpReg;

      /* Wait till PLL is ready */
      timeCount = 0;

      do
      {
        timeCount++;
        readyFlag = RCC_GetClockFlag(RCC_CLOCK_FLAG_PLLRDY);
      }
      while((readyFlag == 0x00u) && (timeCount != PLL_TIMEOUT_VALUE));

      /* If fail.*/
      if(RCC_GetClockFlag(RCC_CLOCK_FLAG_PLLRDY) == RESET)
      {
        return RCC_TIMEOUT;
      }
    }
    else
    {
      /* Disable the PLL. */
      RCC->PLL_CR &= ~RCC_PLL_CR_EN_Msk;
      /* Wait till PLL is disabled */
      timeCount = 0;

      do
      {
        timeCount++;
        readyFlag = RCC_GetClockFlag(RCC_CLOCK_FLAG_PLLRDY);
      }
      while((readyFlag == 0x01u) && (timeCount != PLL_TIMEOUT_VALUE));

      /* If fail.*/
      if(RCC_GetClockFlag(RCC_CLOCK_FLAG_PLLRDY) != RESET)
      {
        return RCC_TIMEOUT;
      }
    }
  }

#endif  /* DPM32M08x || DPM32M05x */

  /*-------------------------------- LSI Configuration -----------------------*/
  if((RCC_OscInitType->RCC_OscillatorType & RCC_OSCILLATOR_TYPE_LSI) == RCC_OSCILLATOR_TYPE_LSI)
  {
    if(RCC_LSI_OFF != RCC_OscInitType->RCC_LSIState)
    {
      /* Enable the LSI. */
      RCC->COMMON_CR |= RCC_COMMON_CR_LSI_EN_Msk;
      /* Wait till LSI is ready */
      timeCount = 0;

      do
      {
        timeCount++;
        readyFlag = RCC_GetClockFlag(RCC_CLOCK_FLAG_LSIRDY);
      }
      while((readyFlag == 0x00u) && (timeCount != LSI_TIMEOUT_VALUE));

      /* If fail.*/
      if(RCC_GetClockFlag(RCC_CLOCK_FLAG_LSIRDY) == RESET)
      {
        return RCC_TIMEOUT;
      }
    }
    else
    {
      /* Disable the LSI. */
      RCC->COMMON_CR &= ~RCC_COMMON_CR_LSI_EN_Msk;
      /* Wait till LSI is disabled */
      timeCount = 0;

      do
      {
        timeCount++;
        readyFlag = RCC_GetClockFlag(RCC_CLOCK_FLAG_LSIRDY);
      }
      while((readyFlag == 0x01u) && (timeCount != LSI_TIMEOUT_VALUE));

      /* If fail.*/
      if(RCC_GetClockFlag(RCC_CLOCK_FLAG_LSIRDY) != RESET)
      {
        return RCC_TIMEOUT;
      }
    }
  }

  return RCC_OK;
}

/**
 *******************************************************************************
 * @brief   Initialize the RCC_OscInitType with default parameters.
 * @param   [out]  RCC_OscInitType: Pointer to a RCC_OscInitTypeStruct structure
 *                 which will be initialized.
 * @retval  None.
 ******************************************************************************/
void RCC_OscStructInit(RCC_OscInitTypeStruct * RCC_OscInitType)
{
  RCC_OscInitType->RCC_OscillatorType = RCC_OSCILLATOR_TYPE_NONE;

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
  RCC_OscInitType->RCC_LSEState = RCC_LSE_OFF;
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */

#if defined (DPM32M08x)
  RCC_OscInitType->RCC_HSEState = RCC_HSE_OFF;
#endif /* DPM32M08x */

  RCC_OscInitType->RCC_LSIState = RCC_LSI_OFF;

#if defined (DPM32M08x) || defined (DPM32M05x)
  RCC_OscInitType->RCC_PLL.PLLSource = RCC_PLL_SOURCE_HSI_DIV8;
  RCC_OscInitType->RCC_PLL.PLLMUL = RCC_PLL_MUL8;
  RCC_OscInitType->RCC_PLL.PLLState = RCC_PLL_NONE;
#endif  /* DPM32M08x || DPM32M05x */
}

/**
 *******************************************************************************
 * @brief   Initializes the CPU, AHB and APB buses clocks according to the specified
 *          parameters in the RCC_ClkInitType.
 * @param   [in]  RCC_ClkInitType: Structure pointer of RCC configuration.
 * @retval  RCC_StatusEnum: Clock configuration status.
 ******************************************************************************/
RCC_StatusEnum RCC_ClockConfig(RCC_ClkInitTypeStruct  *RCC_ClkInitType)
{
  uint32_t tmpReg = 0UL;

  /* Check Null pointer */
  if(RCC_ClkInitType == NULL)
  {
    return RCC_ERROR;
  }

#if defined (DPM32M08x)
  /*-------------------------------- HSCLK Configuration -----------------------*/
  if((RCC_ClkInitType->RCC_ClockType & RCC_CLOCK_TYPE_HSCLK) == RCC_CLOCK_TYPE_HSCLK)
  {
    /* HSE is selected as HSCLK Source */
    if(RCC_ClkInitType->HSCLKSource == RCC_HSCLK_SOURCE_HSE)
    {
      /* Check the HSE ready flag */
      if (RCC_GetClockFlag(RCC_CLOCK_FLAG_HSERDY) == RESET)
      {
        return RCC_ERROR;
      }
    }
    /* Read the value of the COMMON_CR register. */
    tmpReg = RCC->COMMON_CR;

    /* Clear HSCLK_SEL bit. */
    tmpReg &= ~RCC_COMMON_CR_HSCK_SEL_Msk;

    /* Set HSCLK_SEL bit according to source value. */
    tmpReg |=  (uint32_t)(RCC_ClkInitType->HSCLKSource << RCC_COMMON_CR_HSCK_SEL_Pos);

    /* Store the new value. */
    RCC->COMMON_CR = tmpReg;
  }
#endif  /* DPM32M08x */

#if defined (DPM32M08x) || defined (DPM32M05x)

  /*-------------------------------- SYSCLK Configuration -----------------------*/
  if((RCC_ClkInitType->RCC_ClockType & RCC_CLOCK_TYPE_SYSCLK) == RCC_CLOCK_TYPE_SYSCLK)
  {
    /* Read the value of the COMMON_CR register. */
    tmpReg = RCC->COMMON_CR;

    /* Clear SYSCLK_SEL bit. */
    tmpReg &= ~RCC_COMMON_CR_SYSCK_SEL_Msk;
  
    /* PLL is selected as SYSCLK Source */
    if(RCC_ClkInitType->SYSCLKSource == RCC_SYSCLK_SOURCE_PLL)
    {
      /* Check the PLL ready flag */
      if(RCC_GetClockFlag(RCC_CLOCK_FLAG_PLLRDY) == RESET)
      {
        return RCC_ERROR;
      }

      /* Set SYSCLK_SEL bit according to source value. */
      tmpReg |= RCC_COMMON_CR_SYSCK_SEL_Msk;
    }

    /* Store the new value. */
    RCC->COMMON_CR = tmpReg;
  }

#endif  /* DPM32M08x || DPM32M05x */

  /*-------------------------------- HCLK Configuration -----------------------*/
  if((RCC_ClkInitType->RCC_ClockType & RCC_CLOCK_TYPE_HCLK) == RCC_CLOCK_TYPE_HCLK)
  {
    /* Read the value of the COMMON_CR register. */
    tmpReg = RCC->COMMON_CR;

    /* Clear HCLK_DIV[2:0] bits. */
    tmpReg &= ~RCC_COMMON_CR_HCKD_Msk;

    /* Set HCLK_DIV[2:0] bits according to source value. */
    tmpReg |=  (uint32_t)(RCC_ClkInitType->HCLKDivider << RCC_COMMON_CR_HCKD_Pos);

    /* Store the new value. */
    RCC->COMMON_CR = tmpReg;
  }

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
  /*-------------------------------- PCLK Configuration -----------------------*/
  if((RCC_ClkInitType->RCC_ClockType & RCC_CLOCK_TYPE_PCLK) == RCC_CLOCK_TYPE_PCLK)
  {
    /* Read the value of the COMMON_CR register. */
    tmpReg = RCC->COMMON_CR;

    /* Clear PCLK_DIV[2:0] bits. */
    tmpReg &= ~RCC_COMMON_CR_PCKD_Msk;

    /* Set PCLK_DIV[2:0] bits according to source value. */
    tmpReg |=  (uint32_t)(RCC_ClkInitType->PCLKDivider << RCC_COMMON_CR_PCKD_Pos);

    /* Store the new value. */
    RCC->COMMON_CR = tmpReg;
  }
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)

  /*-------------------------------- LSCLK Configuration -----------------------*/
  if((RCC_ClkInitType->RCC_ClockType & RCC_CLOCK_TYPE_LSCLK) == RCC_CLOCK_TYPE_LSCLK)
  {
    /* LSE is selected as LSCLK Source */
    if(RCC_ClkInitType->LSCLKSource == RCC_LSCLK_SOURCE_LSE)
    {
      /* Check the LSE ready flag */
      if(RCC_GetClockFlag(RCC_CLOCK_FLAG_LSERDY) == RESET)
      {
        return RCC_ERROR;
      }
    }
    else  /* LSI is selected as LSCLK Source */
    {
      /* Check the LSI ready flag */
      if(RCC_GetClockFlag(RCC_CLOCK_FLAG_LSIRDY) == RESET)
      {
        return RCC_ERROR;
      }
    }

    /* Read the value of the COMMON_CR register. */
    tmpReg = RCC->COMMON_CR;

    /* Clear HSCLK_SEL bit. */
    tmpReg &= ~RCC_COMMON_CR_LSCK_SEL_Msk;

    /* Set HSCLK_SEL bit according to source value. */
    tmpReg |=  (uint32_t)(RCC_ClkInitType->LSCLKSource << RCC_COMMON_CR_LSCK_SEL_Pos);

    /* Store the new value. */
    RCC->COMMON_CR = tmpReg;
  }

#endif /* DPM32M08x || DPM32M05x || DPM32M03x */

  return RCC_OK;
}

/**
 *******************************************************************************
 * @brief   Initialize the RCC_ClkInitType with default parameters.
 * @param   [out]  RCC_ClkInitType: Pointer to a RCC_ClkInitTypeStruct structure
 *                 which will be initialized.
 * @retval  None.
 ******************************************************************************/
void RCC_ClockStructInit(RCC_ClkInitTypeStruct * RCC_ClkInitType)
{
  RCC_ClkInitType->RCC_ClockType = RCC_CLOCK_TYPE_NONE;

#if defined (DPM32M08x)
  RCC_ClkInitType->HSCLKSource = RCC_HSCLK_SOURCE_HSI;
#endif /* DPM32M08x */

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
  RCC_ClkInitType->LSCLKSource = RCC_LSCLK_SOURCE_LSI;
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */

  RCC_ClkInitType->HCLKDivider = RCC_SYSCLK_DIV1;

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
  || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
  RCC_ClkInitType->PCLKDivider = RCC_HCLK_DIV1;
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */

#if defined (DPM32M08x) || defined (DPM32M05x)
  RCC_ClkInitType->SYSCLKSource = RCC_SYSCLK_SOURCE_HSI;
#endif  /* DPM32M08x || DPM32M05x */
}

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
/**
 *******************************************************************************
 * @brief   Set the HSI start up time.
 * @param   [in]  RCC_StartTime: Specifies the HSI start up time.
 *                This parameter can be a value of @ref RCC_InCLKStartupTimeEnum.
 *                  @arg RCC_IN_CLK_STARTUP_TIME_4: Wait for 4 cycles.
 *                  @arg RCC_IN_CLK_STARTUP_TIME_16: Wait for 16 cycles.
 *                  @arg RCC_IN_CLK_STARTUP_TIME_64: Wait for 64 cycles.
 *                  @arg RCC_IN_CLK_STARTUP_TIME_256: Wait for 256 cycles.
 * @retval  None.
 ******************************************************************************/
void RCC_SetHSIStartUpTime(RCC_InCLKStartupTimeEnum RCC_StartTime)
{
  uint32_t tmpReg = 0UL;

  /* Read the value of the COMMON_CR register. */
  tmpReg = RCC->COMMON_CR;

  /* Clear ST_TIM_HSI[1:0] bits. */
  tmpReg &= ~RCC_COMMON_CR_HSI_ST_Msk;

  /* Set ST_TIM_HSI[1:0] bits according to startTime value. */
  tmpReg |= (uint32_t)(RCC_StartTime << RCC_COMMON_CR_HSI_ST_Pos);

  /* Store the new value. */
  RCC->COMMON_CR = tmpReg;
}
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */

#if defined (DPM32M08x)
/**
 *******************************************************************************
 * @brief   Set the HSE start up time.
 * @param   [in]  RCC_StartTime: Specifies the HSE start up time.
 *                This parameter can be a value of @ref RCC_ExCLKStartupTimeEnum.
 *                  @arg RCC_EX_CLK_STARTUP_TIME_1024: Wait for 1024 cycles.
 *                  @arg RCC_EX_CLK_STARTUP_TIME_4096: Wait for 4096 cycles.
 *                  @arg RCC_EX_CLK_STARTUP_TIME_16384: Wait for 16384 cycles.
 *                  @arg RCC_EX_CLK_STARTUP_TIME_0: Wait for 0 cycles.
 * @retval  None.
 ******************************************************************************/
void RCC_SetHSEStartUpTime(RCC_ExCLKStartupTimeEnum RCC_StartTime)
{
  uint32_t tmpReg = 0UL;

  /* Read the value of the COMMON_CR register. */
  tmpReg = RCC->COMMON_CR;

  /* Clear ST_TIM_HSE[1:0] bits. */
  tmpReg &= ~RCC_COMMON_CR_HSE_ST_Msk;

  /* Set ST_TIM_HSE[1:0] bits according to startTime value. */
  tmpReg |= (uint32_t)(RCC_StartTime << RCC_COMMON_CR_HSE_ST_Pos);

  /* Store the new value. */
  RCC->COMMON_CR = tmpReg;
}
#endif /* DPM32M08x */

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
/**
 *******************************************************************************
 * @brief   Set the LSI start up time.
 * @param   [in]  RCC_StartTime: Specifies the LSI start up time.
 *                This parameter can be a value of @ref RCC_InCLKStartupTimeEnum.
 *                  @arg RCC_IN_CLK_STARTUP_TIME_4: Wait for 4 cycles.
 *                  @arg RCC_IN_CLK_STARTUP_TIME_16: Wait for 16 cycles.
 *                  @arg RCC_IN_CLK_STARTUP_TIME_64: Wait for 64 cycles.
 *                  @arg RCC_IN_CLK_STARTUP_TIME_256: Wait for 256 cycles.
 * @retval  None.
 ******************************************************************************/
void RCC_SetLSIStartUpTime(RCC_InCLKStartupTimeEnum RCC_StartTime)
{
  uint32_t tmpReg = 0UL;

  /* Read the value of the COMMON_CR register. */
  tmpReg = RCC->COMMON_CR;

  /* Clear ST_TIM_LSI[1:0] bits. */
  tmpReg &= ~RCC_COMMON_CR_LSI_ST_Msk;

  /* Set ST_TIM_LSI[1:0] bits according to startTime value. */
  tmpReg |= (uint32_t)(RCC_StartTime << RCC_COMMON_CR_LSI_ST_Pos);

  /* Store the new value. */
  RCC->COMMON_CR = tmpReg;
}
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)

/**
 *******************************************************************************
 * @brief   Set the LSE start up time.
 * @param   [in]  RCC_StartTime: Specifies the LSE start up time.
 *                This parameter can be a value of @ref RCC_ExCLKStartupTimeEnum.
 *                  @arg RCC_EX_CLK_STARTUP_TIME_1024: Wait for 1024 cycles.
 *                  @arg RCC_EX_CLK_STARTUP_TIME_4096: Wait for 4096 cycles.
 *                  @arg RCC_EX_CLK_STARTUP_TIME_16384: Wait for 16384 cycles.
 *                  @arg RCC_EX_CLK_STARTUP_TIME_0: Wait for 0 cycles.
 * @retval  None.
 ******************************************************************************/
void RCC_SetLSEStartUpTime(RCC_ExCLKStartupTimeEnum RCC_StartTime)
{
  uint32_t tmpReg = 0UL;

  /* Read the value of the COMMON_CR register. */
  tmpReg = RCC->COMMON_CR;

  /* Clear ST_TIM_HSI[1:0] bits. */
  tmpReg &= ~RCC_COMMON_CR_LSE_ST_Msk;

  /* Set ST_TIM_HSI[1:0] bits according to startTime value. */
  tmpReg |= (uint32_t)(RCC_StartTime << RCC_COMMON_CR_LSE_ST_Pos);

  /* Store the new value. */
  RCC->COMMON_CR = tmpReg;
}

#endif /* DPM32M08x || DPM32M05x || DPM32M03x */

/**
 *******************************************************************************
 * @brief   Configures the MCO clock source.
 * @param   [in]  RCC_Source: Specifies the clock source used as MCO clock source.
 *                This parameter can be a value of @ref RCC_MCOSourceEnum.
 *                  @arg RCC_MCO_SOURCE_HSI_DIV6: HSI/6 as MCO source.
 *                  @arg RCC_MCO_SOURCE_LSI: LSI as MCO source.
 *                  @arg RCC_MCO_SOURCE_LSE: LSE as MCO source.
 *                  @arg RCC_MCO_SOURCE_PLL_DIV6: PLL/6 as MCO source.
 *                  @arg RCC_MCO_SOURCE_PLL_DIV12: PLL/12 as MCO source.
 * @retval  None.
 ******************************************************************************/
void RCC_MCOConfig(RCC_MCOSourceEnum RCC_Source)
{
  uint32_t tmpReg = 0UL;

  /* Read the value of the COMMON_CR register. */
  tmpReg = RCC->COMMON_CR;

  /* Clear MCO_SEL[2:0] bits. */
  tmpReg &= ~RCC_COMMON_CR_MCO_SEL_Msk;

  /* Set MCO_SEL[2:0] bits according to source value. */
  tmpReg |=  (uint32_t)(RCC_Source << RCC_COMMON_CR_MCO_SEL_Pos);

  /* Store the new value. */
  RCC->COMMON_CR = tmpReg;
}

/**
 *******************************************************************************
 * @brief   Disable the MCO clock output.
 * @retval  None.
 ******************************************************************************/
void RCC_MCODisable(void)
{
  /* Disable the MCO clock output. */
  RCC->COMMON_CR &= ~RCC_COMMON_CR_MCO_EN_Msk;
}

/**
 *******************************************************************************
 * @brief   Enable the MCO clock output.
 * @retval  None.
 ******************************************************************************/
void RCC_MCOEnable(void)
{
  /* Enable the MCO clock output. */
  RCC->COMMON_CR |= RCC_COMMON_CR_MCO_EN_Msk;
}

/**
 *******************************************************************************
 * @brief   Enables or disables the AHB peripheral clock.
 * @param   [in]  RCC_AHBClk: Specifies the AHB peripheral to gates its clock.
 *                This parameter can be a value of @ref RCC_AHBPeriphClockEnum.
 *                  @arg RCC_AHB_PERIPH_GPIO: GPIO clock.
 *                  @arg RCC_AHB_PERIPH_DSP: DSP clock.
 *                  @arg RCC_AHB_PERIPH_MATH_ACC: MATH_ACC clock.
 * @param   [in]  NewState: New state of the specifies peripheral clock.
 *                This parameter can be: ENABLE or DISABLE.
 * @retval  None.
 ******************************************************************************/
void RCC_AHBPeriphClockCmd(RCC_AHBPeriphClockEnum RCC_AHBClk, FunctionalState NewState)
{
  if(DISABLE != NewState)
  {
    /* Enable the specifies AHB peripheral clock. */
    RCC->CLKEN_CR |= RCC_AHBClk;
  }
  else
  {
    /* Disable the specifies AHB peripheral clock. */
    RCC->CLKEN_CR &= ~RCC_AHBClk;
  }
}

/**
 *******************************************************************************
 * @brief   Enables or disables the APB peripheral clock.
 * @param   [in]  RCC_APBClk: Specifies the APB peripheral to gates its clock.
 *                This parameter can be a value of @ref RCC_APBPeriphClockEnum.
 *                  @arg RCC_APB_PERIPH_UART0: UART0 clock.
 *                  @arg RCC_APB_PERIPH_UART1: UART1 clock.
 *                  @arg RCC_APB_PERIPH_UART2: UART2 clock.
 *                  @arg RCC_APB_PERIPH_SPI0: SPI0 clock.
 *                  @arg RCC_APB_PERIPH_SPI1: SPI1 clock.
 *                  @arg RCC_APB_PERIPH_TIM0: TIM0 clock.
 *                  @arg RCC_APB_PERIPH_TIM1: TIM1 clock.
 *                  @arg RCC_APB_PERIPH_TIM2: TIM2 clock.
 *                  @arg RCC_APB_PERIPH_TIM3: TIM3 clock.
 *                  @arg RCC_APB_PERIPH_CCT0: CCT0 clock.
 *                  @arg RCC_APB_PERIPH_CCT1: CCT1 clock.
 *                  @arg RCC_APB_PERIPH_EPWM: EPWM clock.
 *                  @arg RCC_APB_PERIPH_PVD: PVD clock.
 *                  @arg RCC_APB_PERIPH_ADC: ADC0 clock.
 *                  @arg RCC_APB_PERIPH_DAC: DAC clock.
 *                  @arg RCC_APB_PERIPH_I2C: I2C clock.
 *                  @arg RCC_APB_PERIPH_POSIF: POSIF clock.
 *                  @arg RCC_APB_PERIPH_CAN: CAN clock.
 *                  @arg RCC_APB_PERIPH_ACMP: ACMP clock.
 *                  @arg RCC_APB_PERIPH_TIM4: TIM4 clock.
 *                  @arg RCC_APB_PERIPH_TIM5: TIM5 clock.
 *                  @arg RCC_APB_PERIPH_DMA: DMA clock.
 *                  @arg RCC_APB_PERIPH_CRC: CRC clock.
 *                  @arg RCC_APB_PERIPH_TSI: TSI clock.
 * @param   [in]  NewState: New state of the specifies peripheral clock.
 *                This parameter can be: ENABLE or DISABLE.
 * @retval  None.
 ******************************************************************************/
void RCC_APBPeriphClockCmd(RCC_APBPeriphClockEnum RCC_APBClk, FunctionalState NewState)
{
  if(DISABLE != NewState)
  {
    /* Enable the specifies APB peripheral clock. */
    RCC->CLKEN_CR |= RCC_APBClk;
  }
  else
  {
    /* Disable the specifies APB peripheral clock. */
    RCC->CLKEN_CR &= ~RCC_APBClk;
  }
}

/**
 *******************************************************************************
 * @brief   Calculates the value of a number raised to a power.
            RCC_POWCal() can be used to raise a number to a fractional power.
 * @param   [in]  base: Power base number.
 * @param   [in]  index: Power index number.
 * @retval  uint32_t: Fractional power.
 ******************************************************************************/
static uint32_t RCC_POWCal(uint8_t base, uint32_t index)
{
  uint32_t i = 0;
  uint32_t mul = base;

  if(index != 0)
  {
    for(i = 1; i < index; i++)
    {
      mul *= base;
    }
  }
  else
  {
    mul = 1;
  }

  return mul;
}

/**
 *******************************************************************************
 * @brief   Get the frequency of the SYSCLK.
 * @retval  uint32_t: SYSCLK frequency.
 ******************************************************************************/
uint32_t RCC_GetSYSCLKFrequency(void)
{
#if defined (DPM32M08x) || defined (DPM32M05x)
  uint32_t freq = 0;
  uint32_t tmpReg = 0UL;

  /* Get the SYSCLK source. */
  tmpReg = ((RCC->COMMON_CR & RCC_COMMON_CR_SYSCK_SEL_Msk) >> RCC_COMMON_CR_SYSCK_SEL_Pos);

  if(tmpReg != 0)
  {
    /* SYSCLK source is PLL. */
    freq = RCC_GetPLLFrequency();
  }
  else
  {
    /* SYSCLK source is HSCLK. */
#if defined (DPM32M08x)
    freq = RCC_GetHSCLKFrequency();
#else
    freq = HSI_VALUE;
#endif
  }

  /* Return SYSCLK frequency. */
  return freq;
#endif  /* DPM32M08x || DPM32M05x */

#if defined (DPM32M03x) || defined (DPM32M036) || defined (DPM32M030) \
 || defined (DPM32M015) || defined (DPM32M026) || defined (DPM32T55x)
  return HSI_VALUE;
#endif  /* DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 || DPM32M026 || DPM32T55x */
}

/**
 *******************************************************************************
 * @brief   Get the frequency of the HCLK.
 * @retval  uint32_t: HCLK frequency.
 ******************************************************************************/
uint32_t RCC_GetHCLKFrequency(void)
{
  uint32_t tmpReg = 0UL;
  uint32_t freq = 0;

  /* Get HCLK frequency division. */
  tmpReg = ((RCC->COMMON_CR & RCC_COMMON_CR_HCKD_Msk) >> RCC_COMMON_CR_HCKD_Pos);

  freq = RCC_GetSYSCLKFrequency() / RCC_POWCal(2, tmpReg);

  /* Return HCLK frequency. */
  return freq;
}

/**
 *******************************************************************************
 * @brief   Get the frequency of the PCLK.
 * @retval  uint32_t: PCLK frequency.
 ******************************************************************************/
uint32_t RCC_GetPCLKFrequency(void)
{
  uint32_t freq = 0;

#if defined(DPM32M026) || defined (DPM32T55x)
  freq = RCC_GetHCLKFrequency();
#elif defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
   || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
  uint32_t tmpReg = 0UL;

  /* Get PCLK frequency division. */
  tmpReg = ((RCC->COMMON_CR & RCC_COMMON_CR_PCKD_Msk) >> RCC_COMMON_CR_PCKD_Pos);

  freq = RCC_GetHCLKFrequency() / RCC_POWCal(2, tmpReg);
#endif /* DPM32M026 || DPM32T55x */
 
/* Return PCLK frequency. */
  return freq;
}

#if defined (DPM32M08x)
/**
 *******************************************************************************
 * @brief   Get the frequency of the HSCK.
 * @retval  uint32_t: HSCK frequency.
 ******************************************************************************/
uint32_t RCC_GetHSCLKFrequency(void)
{
  uint32_t tmpReg = 0UL;
  uint32_t HSCKFreq = 0;
  /* Get the HSCLK source. */
  tmpReg = RCC->COMMON_CR & RCC_COMMON_CR_HSCK_SEL_Msk;

  if(tmpReg != 0)
  {
    /* HSCLK source is HSE. */
    HSCKFreq =  HSE_VALUE;
  }
  else
  {
    /* HSCLK source is HSI. */
    HSCKFreq =  HSI_VALUE;
  }
  /* Return the frequency of the HSCLK */
  return HSCKFreq;
}
#endif  /* DPM32M08x */

#if defined (DPM32M08x) || defined (DPM32M05x)
/**
 *******************************************************************************
 * @brief   Get the frequency of the PLL.
 * @retval  uint32_t: PLL frequency.
 ******************************************************************************/
uint32_t RCC_GetPLLFrequency(void)
{
  uint32_t freq = 0;
  uint32_t tmpReg = 0UL;
  uint32_t PLLFreq = 0u;

  /* Get the HSCLK source. */
  tmpReg = RCC->COMMON_CR & RCC_COMMON_CR_HSCK_SEL_Msk;

#if defined (DPM32M08x) 
  if(tmpReg != 0)
  {
    /* HSCLK source is HSE. */
    freq = HSE_VALUE;
  }
  else
#endif  /* DPM32M08x */
  {
    /* HSCLK source is HSI. */
    freq = HSI_VALUE / PLL_HSI_DIV;
  }

  /* Get the PLL multiplication factor. */
  tmpReg = ((RCC->PLL_CR & RCC_PLL_CR_MULTPS_Msk) >> RCC_PLL_CR_MULTPS_Pos);

  PLLFreq = freq * (tmpReg + 1);

  /* Return PLL clock. */
  return PLLFreq;
}

#endif  /* DPM32M08x || DPM32M05x */

/**
 *******************************************************************************
 * @brief   Get the system reset source.
 * @retval  RCC_RSTSourceEnum: The system reset source.
 *          This parameter can be a value of @ref RCC_RSTSourceEnum.
 *            @arg RCC_RST_SOURCE_POR_PDR: Power on or power off reset.
 *            @arg RCC_RST_SOURCE_BUTTON: button reset.
 *            @arg RCC_RST_SOURCE_SOFTWARE: software reset.
 *            @arg RCC_RST_SOURCE_LOCKUP: LOCKUP reset.
 *            @arg RCC_RST_SOURCE_DSP: DSP reset.
 *            @arg RCC_RST_SOURCE_WDG: watch dog reset.
 *            @arg RCC_RST_SOURCE_PVD: low voltage detect reset.
 *            @arg RCC_RST_SOURCE_DSTB_DET: BOOT FAIL reset.
 ******************************************************************************/
RCC_RSTSourceEnum RCC_GetSysResetSource(void)
{
  /* Return the system reset source. */
  return (RCC_RSTSourceEnum)(RCC->RESET_SR & 0xFFFFUL);
}

/**
 *******************************************************************************
 * @brief   Clear all system reset source flag.
 * @retval  None.
 ******************************************************************************/
void RCC_ClearSysResetSourceFlag(void)
{
  /* Clear the system reset source flag. */
  RCC->RESET_SR = RCC_RESET_SR_CLR_Msk;
}
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
/**
 *******************************************************************************
 * @brief   Enables or disables the peripheral clock in sleep mode.
 * @param   [in]  RCC_DeepSleepClk: Specifies the peripheral to config its clock in sleep mode.
 *                This parameter can be a value of @ref RCC_DeepSleepPeriphClockEnum.
 *                  @arg RCC_DEEP_SLEEP_PERIPH_UART0: UART0 clock.
 *                  @arg RCC_DEEP_SLEEP_PERIPH_UART1: UART1 clock.
 *                  @arg RCC_DEEP_SLEEP_PERIPH_UART2: UART2 clock.
 *                  @arg RCC_DEEP_SLEEP_PERIPH_SPI0: SPI0 clock.
 *                  @arg RCC_DEEP_SLEEP_PERIPH_SPI1: SPI1 clock.
 *                  @arg RCC_DEEP_SLEEP_PERIPH_TIM0: TIM0 clock.
 *                  @arg RCC_DEEP_SLEEP_PERIPH_TIM1: TIM1 clock.
 *                  @arg RCC_DEEP_SLEEP_PERIPH_TIM2: TIM2 clock.
 *                  @arg RCC_DEEP_SLEEP_PERIPH_TIM3: TIM3 clock.
 *                  @arg RCC_DEEP_SLEEP_PERIPH_CCT0: CCT0 clock.
 *                  @arg RCC_DEEP_SLEEP_PERIPH_CCT1: CCT1 clock.
 *                  @arg RCC_DEEP_SLEEP_PERIPH_EPWM: EPWM clock.
 *                  @arg RCC_DEEP_SLEEP_PERIPH_PVD: PVD clock.
 *                  @arg RCC_DEEP_SLEEP_PERIPH_ADC0: ADC0 clock.
 *                  @arg RCC_DEEP_SLEEP_PERIPH_DAC: DAC clock.
 *                  @arg RCC_DEEP_SLEEP_PERIPH_I2C: I2C clock.
 *                  @arg RCC_DEEP_SLEEP_PERIPH_POSIF: POSIF clock.
 *                  @arg RCC_DEEP_SLEEP_PERIPH_CAN: CAN clock.
 *                  @arg RCC_DEEP_SLEEP_PERIPH_ACMP: ACMP clock.
 *                  @arg RCC_DEEP_SLEEP_PERIPH_TIM4: TIM4 clock.
 *                  @arg RCC_DEEP_SLEEP_PERIPH_TIM5: TIM5 clock.
 *                  @arg RCC_DEEP_SLEEP_PERIPH_DMA: DMA clock.
 *                  @arg RCC_DEEP_SLEEP_PERIPH_CRC: CRC clock.
 *                  @arg RCC_DEEP_SLEEP_PERIPH_GPIO: GPIO clock.
 *                  @arg RCC_DEEP_SLEEP_PERIPH_DSP: DSP clock.
 *                  @arg RCC_DEEP_SLEEP_PERIPH_MATH_ACC: MATH_ACC clock.
 * @param   [in]  NewState: New state of the specifies peripheral clock.
 *                This parameter can be: ENABLE or DISABLE.
 * @retval  None.
 ******************************************************************************/
void RCC_DeepSleepModePeriphCmd(RCC_DeepSleepPeriphClockEnum RCC_DeepSleepClk, FunctionalState NewState)
{
  if(DISABLE != NewState)
  {
    /* Enable the specifies peripheral clock in deep sleep mode. */
    RCC->DEEP_SLEEP_CR |= RCC_DeepSleepClk;
  }
  else
  {
    /* Disable the specifies peripheral clock in deep sleep mode. */
    RCC->DEEP_SLEEP_CR &= ~RCC_DeepSleepClk;
  }
}
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */
/**
 *******************************************************************************
 * @brief   Enables or disables the specifies functions in stop mode.
 * @param   [in]  RCC_StopFunc: Specifies the functions to config its in stop mode.
 *                This parameter can be a value of @ref RCC_StopFuncEnum.
 *                  @arg RCC_STOP_FUNC_LSCLK: LSCLK functions.
 * @param   [in]  NewState: New state of the specifies peripheral functions.
 *                This parameter can be: ENABLE or DISABLE.
 * @retval  None.
 ******************************************************************************/
void RCC_StopModeFunctionCmd(RCC_StopFuncEnum RCC_StopFunc, FunctionalState NewState)
{
  if(DISABLE != NewState)
  {
    /* Enable the specifies functions in stop mode. */
    RCC->STOP_CR |= RCC_StopFunc;
  }
  else
  {
    /* Disable the specifies functions in stop mode. */
    RCC->STOP_CR &= ~RCC_StopFunc;
  }
}
/**
 *******************************************************************************
 * @brief   System enter low power mode.
 * @param   [in]  RCC_LowPowerType: specifies the low power mode type.
 *                This parameter can be a value of @ref RCC_LowPowerModeEnum.
 *                  @arg RCC_LOW_POWER_MODE_SLEEP: Sleep mode.
 *                  @arg RCC_LOW_POWER_MODE_DEEP_SLEEP: Deep sleep mode.
 *                  @arg RCC_LOW_POWER_MODE_STOP: Stop mode.
 * @param   [in]  RCC_LowPowerEntry: specifies the low power mode in entered with WFI
 *                or WFE instruction.
 *                This parameter can be a value of @ref RCC_LowPowerEntryEnum.
 *                  @arg RCC_LOW_POWER_ENTRY_WFI: Enter low power mode with WFI instruction.
 *                  @arg RCC_LOW_POWER_ENTRY_WFE: Enter low power mode with WFE instruction.
 * @retval  None.
 ******************************************************************************/
void RCC_EnterLowPowerMode(RCC_LowPowerModeEnum RCC_LowPowerType, RCC_LowPowerEntryEnum RCC_LowPowerEntry)
{
  
  
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
  if(RCC_LowPowerType == RCC_LOW_POWER_MODE_SLEEP)
  {
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
  }
  else if(RCC_LowPowerType == RCC_LOW_POWER_MODE_DEEP_SLEEP)
  {
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    /* Clear LD_MODE[1:0] bits to set sleep mode. */
    RCC->LPM_CR &= ~RCC_LPM_CR_MODE_Msk;
  }
  else if(RCC_LowPowerType == RCC_LOW_POWER_MODE_STOP)
  {
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    /* Clear LD_MODE[1:0] bits to set sleep mode. */
    RCC->LPM_CR &= ~RCC_LPM_CR_MODE_Msk;
    /* Set LD_MODE[1:0] bits to set stop mode. */
    RCC->LPM_CR |= (uint32_t)(1u << RCC_LPM_CR_MODE_Pos);
  }
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */
  
#if defined (DPM32M026) || defined (DPM32T55x)
  if(RCC_LowPowerType == RCC_LOW_POWER_MODE_SLEEP)
  {
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
  }
  else if(RCC_LowPowerType == RCC_LOW_POWER_MODE_STOP)
  {
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
  }
#endif /* DPM32M026 || DPM32T55x */

  /* Select SLEEP mode entry. */
  if(RCC_LowPowerEntry == RCC_LOW_POWER_ENTRY_WFI)
  {
    /* Request Wait For Interrupt. */
    __WFI();
  }
  else
  {
    /* Request Wait For Event. */
    __SEV();
    __WFE();
    __WFE();
  }
}

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
/**
 *******************************************************************************
 * @brief   System start up address remap.
 * @param   [in]  RCC_StartAddr: Specifies the system start up address remap.
 *                This parameter can be a value of @ref RCC_StartupAddrRemapEnum.
 *                  @arg RCC_STARTUP_ADDR_REMAP_FLASH: System start up from flash.
 *                  @arg RCC_STARTUP_ADDR_REMAP_SRAM: System start up from flash.
 *                  @arg RCC_STARTUP_ADDR_REMAP_BOOTROM0: Enter stop mode with WFI instruction.
 *                  @arg RCC_STARTUP_ADDR_REMAP_BOOTROM1: Enter stop mode with WFE instruction.
 * @retval  None.
 ******************************************************************************/
void RCC_SysStartupAddrRemap(RCC_StartupAddrRemapEnum RCC_StartAddr)
{
  uint32_t tmpReg = 0UL;

  /* Read the value of the SYS_CTRL register. */
  tmpReg = RCC->SYSTEM_CR;

  /* Clear REMAP[1:0] bits. */
  tmpReg &= ~RCC_SYSTEM_CR_REMAP_Msk;

  /* Set REMAP[1:0] bits according to remap value. */
  tmpReg |= (uint32_t)(RCC_StartAddr);

  /* Store the new value. */
  RCC->SYSTEM_CR = tmpReg;
}

#endif /* DPM32M08x || DPM32M05x || DPM32M03x */

#if defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015) || \
    defined (DPM32M026) || defined (DPM32T55x)
/**
 *******************************************************************************
 * @brief   System start up address remap.
 * @param   [in]  RCC_VectorAddr: Specifies the system interrupt vector table address.
 * @retval  None.
 ******************************************************************************/
void RCC_SetVectorTableAddr(uint32_t RCC_VectorAddr)
{
  uint32_t tmpReg = 0UL;

  /* Read the value of the SYS_CTRL register. */
  tmpReg = RCC->SYSTEM_CR;

  /* Clear VTOR[23:0] bits. */
  tmpReg &= ~RCC_SYSTEM_CR_VTOR_Msk;

  /* Set VTOR[23:0] bits according to remap value. */
  tmpReg |= (uint32_t)(RCC_VectorAddr << RCC_SYSTEM_CR_VTOR_Pos);

  /* Store the new value. */
  RCC->SYSTEM_CR = tmpReg;
}

#endif /* DPM32M036 || DPM32M030 || DPM32M015 || DPM32M026 || DPM32T55x */

/**
 *******************************************************************************
 * @brief   Enables or disables the lock up reset functions.
 * @param   [in]  NewState: New state of the functions.
 *                This parameter can be: ENABLE or DISABLE.
 * @retval  None.
 ******************************************************************************/
void RCC_LockupResetCmd(FunctionalState NewState)
{
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)

  if(DISABLE != NewState)
  {
    /* Enable the specifies functions in stop mode. */
    RCC->SYSTEM_CR |= RCC_SYSTEM_CR_LKUPRST_EN_Msk;
  }
  else
  {
    /* Disable the specifies functions in stop mode. */
    RCC->SYSTEM_CR &= ~RCC_SYSTEM_CR_LKUPRST_EN_Msk;
  }
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */

#if defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015) || \
    defined (DPM32M026) || defined (DPM32T55x)

  if(DISABLE != NewState)
  {
    /* Enable the specifies functions in stop mode. */
    RCC->RESET_SR |= RCC_RESET_SR_LKUPRST_EN_Msk;
  }
  else
  {
    /* Disable the specifies functions in stop mode. */
    RCC->RESET_SR &= ~RCC_RESET_SR_LKUPRST_EN_Msk;
  }
#endif /* DPM32M036 || DPM32M030 || DPM32M015 || DPM32M026 || DPM32T55x */
}

/**
 *******************************************************************************
 * @brief   Enables or disables the LSI.
 * @param   [in]  NewState: New state of the functions.
 *                This parameter can be: ENABLE or DISABLE.
 * @retval  None.
 ******************************************************************************/
RCC_StatusEnum RCC_LSICmd(FunctionalState NewState)
{
  uint32_t timeCount = 0u;
  uint32_t readyFlag = 0u;
  
  if(DISABLE != NewState)
  {
    /* Enable the LSI. */
    RCC->COMMON_CR |= RCC_COMMON_CR_LSI_EN_Msk;
    /* Wait till LSI is ready */
    timeCount = 0;

    do
    {
      timeCount++;
      readyFlag = RCC_GetClockFlag(RCC_CLOCK_FLAG_LSIRDY);
    }
    while((readyFlag == 0x00u) && (timeCount != LSI_TIMEOUT_VALUE));

    /* If fail.*/
    if(RCC_GetClockFlag(RCC_CLOCK_FLAG_LSIRDY) == RESET)
    {
      return RCC_TIMEOUT;
    }
  }
  else
  {
    /* Disable the LSI. */
    RCC->COMMON_CR &= ~RCC_COMMON_CR_LSI_EN_Msk;
    /* Wait till LSI is disabled */
    timeCount = 0;

    do
    {
      timeCount++;
      readyFlag = RCC_GetClockFlag(RCC_CLOCK_FLAG_LSIRDY);
    }
    while((readyFlag == 0x01u) && (timeCount != LSI_TIMEOUT_VALUE));

    /* If fail.*/
    if(RCC_GetClockFlag(RCC_CLOCK_FLAG_LSIRDY) != RESET)
    {
      return RCC_TIMEOUT;
    }
  }
  return RCC_OK;
}
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
/**
 *******************************************************************************
 * @brief   Enables or disables the LSE.
 * @param   [in]  NewState: New state of the functions.
 *                This parameter can be: ENABLE or DISABLE.
 * @retval  None.
 ******************************************************************************/
RCC_StatusEnum RCC_LSECmd(FunctionalState NewState)
{
  uint32_t timeCount = 0u;
  uint32_t readyFlag = 0u;

  if(DISABLE != NewState)
  {
    /* Enable the LSE. */
    RCC->COMMON_CR |= RCC_COMMON_CR_LSE_EN_Msk;
    /* Wait till LSE is ready */
    timeCount = 0;

    do
    {
      timeCount++;
      readyFlag = RCC_GetClockFlag(RCC_CLOCK_FLAG_LSERDY);
    }
    while((readyFlag == 0x00u) && (timeCount != LSE_TIMEOUT_VALUE));

    /* If fail.*/
    if(RCC_GetClockFlag(RCC_CLOCK_FLAG_LSERDY) == RESET)
    {
      return RCC_TIMEOUT;
    }
  }
  else
  {
    /* Disable the LSE. */
    RCC->COMMON_CR &= ~RCC_COMMON_CR_LSE_EN_Msk;
    /* Wait till LSE is disabled */
    timeCount = 0;

    do
    {
      timeCount++;
      readyFlag = RCC_GetClockFlag(RCC_CLOCK_FLAG_LSERDY);
    }
    while((readyFlag == 0x01u) && (timeCount != LSE_TIMEOUT_VALUE));

    /* If fail.*/
    if(RCC_GetClockFlag(RCC_CLOCK_FLAG_LSERDY) != RESET)
    {
      return RCC_TIMEOUT;
    }
  }

  return RCC_OK;
}
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */
