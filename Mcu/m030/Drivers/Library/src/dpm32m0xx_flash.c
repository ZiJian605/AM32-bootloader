/**
 *******************************************************************************
 * @file    dpm32m0xx_flash.c
 *
 * @brief   Source file for FLASH firmware driver.
 *          This file provides firmware functions to manage the following
 *          functionalities of the FLASH peripheral:
 *            + FLASH Interface configuration
 *            + FLASH Memory Programming
 *            + Option Bytes Programming
 *            + Restore factory function
 *
 * @author  DPM
 *
 * @version V1.0.0
 *
 * @date    2023-11-01
 *
 * @verbatim
 ===============================================================================
                       ##### How to use this driver #####
 ===============================================================================
    [..]
      This driver provides functions to configure and program the FLASH memory
      of all DPM32M0xx devices. These functions are split in 4 groups:

      (#) FLASH Interface configuration functions: this group includes the
          management of the following features:
        (+) Set the read wait cycles
        (+) Enable/Disable the prefetch buffer

      (#) FLASH Memory Programming functions: this group includes all needed
          functions to erase and program the main memory:
        (+) Lock and Unlock the FLASH interface
        (+) Erase function: Erase page, erase chip
        (+) Program functions: byte, half word, word

      (#) Option Bytes Programming functions: this group includes all needed
          functions to manage the Option Bytes:
        (+) Set/Reset the write protection
        (+) Set/Reset the disable debug function
        (+) Set/Reset the disable ISP function
        (+) Set/Reset the disable recover function

      (#) Restore factory functions: this group includes all needed functions to:
        (+) Flash restore factory mode

 * @endverbatim
 *******************************************************************************/

#include "dpm32m0xx_flash.h"

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
/**
 *******************************************************************************
 * @brief   Set the flash read wait cycle.
 * @param  FLASH_ReadWait: Wait cycle.
 *         This parameter can be a value of @ref FLASH_ReadWaitCycleEnum.
 *         DPM32M08x|DPM32M05x|DPM32M03x:
 *           @arg FLASH_READ_WAIT_HCLK_CYCLE1: When 24Mhz < HCLK <= 48Mhz,
 *                read flash need wait 1 HCLK cycle.
 *           @arg FLASH_READ_WAIT_HCLK_CYCLE2: When 48Mhz < HCLK <= 72Mhz,
 *                read flash need wait 2 HCLK cycle.
 *           @arg FLASH_READ_WAIT_HCLK_CYCLE3: When 72Mhz < HCLK <= 96Mhz,
 *                read flash need wait 3 HCLK cycle.
 *         DPM32M036|DPM32M030|DPM32M015:
 *           @arg FLASH_READ_WAIT_HCLK_CYCLE0: When 0Mhz < HCLK <= 24Mhz,
 *                read flash need wait 0 HCLK cycle.
 *           @arg FLASH_READ_WAIT_HCLK_CYCLE1: When 24Mhz < HCLK <= 48Mhz,
 *                read flash need wait 1 HCLK cycle.
 * @retval  None.
 ******************************************************************************/
void FLASH_SetReadWaitCycle(FLASH_ReadWaitCycleEnum FLASH_ReadWait)
{
  uint32_t tmpReg = 0UL;

  /* Read the value of the flash control register. */
  tmpReg = FLASH->CR;

  /* Clear CR_KEY[15:0] bits. */
  tmpReg &= ~FLASH_CR_KEY_Msk;

  /* Set CR_KEY[15:0] bits according to FLASH_CR_KEY. */
  tmpReg |= (uint32_t)(FLASH_CR_KEY);

  /* Clear WS[1:0] bits. */
  tmpReg &= ~FLASH_CR_WS_Msk;

  /* Set WS[1:0] bits with chip erase. */
  tmpReg |=  (uint32_t)(FLASH_ReadWait << FLASH_CR_WS_Pos);

  /* Store the new value. */
  FLASH->CR = tmpReg;
}
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) || defined (DPM32M036)
/**
 *******************************************************************************
 * @brief   Enables or disables the prefetch buffer.
 * @param   [in]  NewState: This parameter can be: ENABLE or DISABLE.
 * @retval  None.
 ******************************************************************************/
void FLASH_PrefetchBufferCmd(FunctionalState NewState)
{
  uint32_t tmpReg = 0UL;

  /* Read the value of the flash control register. */
  tmpReg = FLASH->CR;

  /* Clear CR_KEY[15:0] bits. */
  tmpReg &= ~FLASH_CR_KEY_Msk;

  /* Set CR_KEY[15:0] bits according to FLASH_CR_KEY. */
  tmpReg |= (uint32_t)(FLASH_CR_KEY);

  if(NewState != DISABLE)
  {
    tmpReg |= FLASH_CR_PF_EN_Msk;
  }
  else
  {
    tmpReg &= ~FLASH_CR_PF_EN_Msk;
  }

  /* Store the new value. */
  FLASH->CR = tmpReg;
}

/**
 *******************************************************************************
 * @brief   Get the status of the FLASH prefetch buffer.
 * @retval  FlagState: The state of FLASH prefetch buffer (SET or RESET).
 ******************************************************************************/
FlagState FLASH_GetPrefetchBufferStatus(void)
{
  FlagState state = RESET;

  /* Get the status of the Interrupt */
  if(RESET != (FLASH->CR & ((uint32_t)FLASH_CR_PF_EN_Msk)))
  {
    state = SET;
  }
  else
  {
    state = RESET;
  }

  /* Return the status of the FLASH Prefetch Buffer. */
  return state;
}
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 */

/**
 *******************************************************************************
 * @brief   Unlocks the flash configuration register.
 * @retval  None.
 * @note    FLASH_CR, FLASH_ADDR and FLASH_DATA write enable.
 ******************************************************************************/
void FLASH_Unlock(void)
{
  FLASH->CR_LOCK = 0x900D0000;
}

/**
 *******************************************************************************
 * @brief   Locks the flash configuration register.
 * @retval  None.
 * @note    FLASH_CR, FLASH_ADDR and FLASH_DATA write disable.
 ******************************************************************************/
void FLASH_Lock(void)
{
  FLASH->CR_LOCK = 0x900D0001;
}

/**
 *******************************************************************************
 * @brief   Returns the FLASH Status.
 * @retval  FLASH_StatusEnum: Flash status.
 *          This parameter can be a value of @ref FLASH_StatusEnum.
 *            @arg FLASH_OK: Flash OK.
 *            @arg FLASH_BUSY: Flash busy.
 *            @arg FLASH_ERROR_CR: Write configure register error.
 *            @arg FLASH_ERROR_WP: Write protected error.
 *            @arg FLASH_ERROR_ADDR: Program or erase address out of range error.
 *            @arg FLASH_TIMEOUT: Write flash timeout.
 ******************************************************************************/
FLASH_StatusEnum FLASH_GetStatus(void)
{
  FLASH_StatusEnum status = FLASH_OK;

  if((FLASH->SR & FLASH_FLAG_BUSY) == FLASH_FLAG_BUSY)
  {
    /* Flash busy. */
    status = FLASH_BUSY;
  }
  else
  {
    if(((FLASH->SR & (uint32_t) FLASH_FLAG_ERROR_CR_BSY) != (uint32_t)0x00) \
        || ((FLASH->SR & (uint32_t) FLASH_FLAG_ERROR_CR_LK) != (uint32_t)0x00))
    {
      /* Flash write configure register error. */
      status = FLASH_ERROR_CR;
    }
#if defined (DPM32M026) || defined (DPM32T55x)
    else if(((FLASH->SR & (uint32_t) FLASH_FLAG_ERROR_CPE_WP) != (uint32_t)0x00) \
            || ((FLASH->SR & (uint32_t) FLASH_FLAG_ERROR_MN_WP) != (uint32_t)0x00) \
            || ((FLASH->SR & (uint32_t) FLASH_FLAG_ERROR_OP_WP) != (uint32_t)0x00) \
            || ((FLASH->SR & (uint32_t) FLASH_FLAG_ERROR_EEPROM_WP) != (uint32_t)0x00))
#else /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */
    else if(((FLASH->SR & (uint32_t) FLASH_FLAG_ERROR_CPE_WP) != (uint32_t)0x00) \
            || ((FLASH->SR & (uint32_t) FLASH_FLAG_ERROR_MN_WP) != (uint32_t)0x00) \
            || ((FLASH->SR & (uint32_t) FLASH_FLAG_ERROR_OP_WP) != (uint32_t)0x00))
#endif /* DPM32M026 || DPM32T55x */
    {
      /* Flash write protected error. */
      status = FLASH_ERROR_WP;
    }
    else if(((FLASH->SR & (uint32_t) FLASH_FLAG_ERROR_ADDR) != (uint32_t)0x00))
    {
      /* Program or erase address out of range error. */
      status = FLASH_ERROR_ADDR;
    }
    else
    {
      status = FLASH_OK;
    }
  }

  /* Return the FLASH Status */
  return status;
}


/**
 *******************************************************************************
 * @brief   Wait for a FLASH operation to complete or TIMEOUT to occur.
 * @param   [in]  FLASH_Timeout: Wait timeout.
 * @retval  FLASH_StatusEnum: Flash status.
 *          This parameter can be a value of @ref FLASH_StatusEnum.
 *            @arg FLASH_OK: Flash OK.
 *            @arg FLASH_BUSY: Flash busy.
 *            @arg FLASH_ERROR_CR: Write configure register error.
 *            @arg FLASH_ERROR_WP: Write protected error.
 *            @arg FLASH_ERROR_ADDR: Program or erase address out of range error.
 *            @arg FLASH_TIMEOUT: Write flash timeout.
 ******************************************************************************/
FLASH_StatusEnum FLASH_WaitForLastOperation(uint32_t FLASH_Timeout)
{
  FLASH_StatusEnum status = FLASH_OK;

  /* Check for the FLASH Status */
  status = FLASH_GetStatus();

  /* Wait for a FLASH operation to complete or a TIMEOUT to occur */
  while((status == FLASH_BUSY) && (FLASH_Timeout != 0x00))
  {
    status = FLASH_GetStatus();
    FLASH_Timeout--;
  }

  if(FLASH_Timeout == 0x00)
  {
    status = FLASH_TIMEOUT;
  }

  /* Return the operation status */
  return status;
}

/**
 *******************************************************************************
 * @brief   Flash page erase,512 bytes.
 * @param   [in]  FLASH_EraseAddr: Erase address.
 * @retval  FLASH_StatusEnum: Flash status.
 *          This parameter can be a value of @ref FLASH_StatusEnum.
 *            @arg FLASH_OK: Flash OK.
 *            @arg FLASH_BUSY: Flash busy.
 *            @arg FLASH_ERROR_CR: Write configure register error.
 *            @arg FLASH_ERROR_WP: Write protected error.
 *            @arg FLASH_ERROR_ADDR: Program or erase address out of range error.
 *            @arg FLASH_TIMEOUT: Write flash timeout.
 ******************************************************************************/
FLASH_StatusEnum FLASH_ErasePage(uint32_t FLASH_EraseAddr)
{
  FLASH_StatusEnum status = FLASH_OK;
  uint32_t tmpReg = 0UL;

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

  if(status == FLASH_OK)
  {
    /* If the previous operation is completed, proceed to erase the page */
    FLASH->ADDR = FLASH_EraseAddr;

    /* Read the value of the flash control register. */
    tmpReg = FLASH->CR;

    /* Clear CR_KEY[15:0] bits. */
    tmpReg &= ~FLASH_CR_KEY_Msk;

    /* Set CR_KEY[15:0] bits according to FLASH_CR_KEY. */
    tmpReg |= (uint32_t)(FLASH_CR_KEY);

    /* Clear OP_MODE[2:0] bits. */
    tmpReg &= ~FLASH_CR_OP_MODE_Msk;

    /* Set OP_MODE[2:0] bits with chip erase. */
    tmpReg |=  (uint32_t)(FLASH_OPERATION_PAGE_ERASE << FLASH_CR_OP_MODE_Pos);

    /* Clear OP_START bit. */
    tmpReg &= ~FLASH_CR_OP_START_Msk;

    /* Set OP_START bit with chip erase. */
    tmpReg |=  (uint32_t)(0x01UL << FLASH_CR_OP_START_Pos);

    /* Store the new value. */
    FLASH->CR = tmpReg;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);
  }

  /* Return the Erase Status */
  return status;
}

/**
 *******************************************************************************
 * @brief   Flash chip erase.
 * @retval  FLASH_StatusEnum: Flash status.
 *          This parameter can be a value of @ref FLASH_StatusEnum.
 *            @arg FLASH_OK: Flash OK.
 *            @arg FLASH_BUSY: Flash busy.
 *            @arg FLASH_ERROR_CR: Write configure register error.
 *            @arg FLASH_ERROR_WP: Write protected error.
 *            @arg FLASH_ERROR_ADDR: Program or erase address out of range error.
 *            @arg FLASH_TIMEOUT: Write flash timeout.
 ******************************************************************************/
FLASH_StatusEnum FLASH_EraseChip(void)
{
  FLASH_StatusEnum status = FLASH_OK;
  uint32_t tmpReg = 0UL;

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

  if(status == FLASH_OK)
  {
    /* Read the value of the flash control register. */
    tmpReg = FLASH->CR;

    /* Clear CR_KEY[15:0] bits. */
    tmpReg &= ~FLASH_CR_KEY_Msk;

    /* Set CR_KEY[15:0] bits according to FLASH_CR_KEY. */
    tmpReg |= (uint32_t)(FLASH_CR_KEY);

    /* Clear OP_MODE[2:0] bits. */
    tmpReg &= ~FLASH_CR_OP_MODE_Msk;

    /* Set OP_MODE[2:0] bits with chip erase. */
    tmpReg |=  (uint32_t)(FLASH_OPERATION_CHIP_ERASE << FLASH_CR_OP_MODE_Pos);

    /* Clear OP_START bit. */
    tmpReg &= ~FLASH_CR_OP_START_Msk;

    /* Set OP_START bit with chip erase. */
    tmpReg |= (uint32_t)FLASH_CR_OP_START_Msk;

    /* Store the new value. */
    FLASH->CR = tmpReg;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);
  }

  /* Return the Erase Status */
  return status;
}

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)

/**
 *******************************************************************************
 * @brief   Programs a word at specified address.
 * @param   [in]  FLASH_Addr: Specifies the address to be programmed.
 * @param   [in]  FLASH_Data: Specifies the data to be programmed.
 * @retval  FLASH_StatusEnum: Flash status.
 *          This parameter can be a value of @ref FLASH_StatusEnum.
 *            @arg FLASH_OK: Flash OK.
 *            @arg FLASH_BUSY: Flash busy.
 *            @arg FLASH_ERROR_CR: Write configure register error.
 *            @arg FLASH_ERROR_WP: Write protected error.
 *            @arg FLASH_ERROR_ADDR: Program or erase address out of range error.
 *            @arg FLASH_TIMEOUT: Write flash timeout.
 ******************************************************************************/
FLASH_StatusEnum FLASH_ProgramWord(uint32_t FLASH_Addr, uint32_t FLASH_Data)
{
  FLASH_StatusEnum status = FLASH_OK;
  
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

  if(status == FLASH_OK)
  {
    
    uint32_t tmpReg = 0UL;
    
    /* Programs address. */
    FLASH->ADDR = FLASH_Addr;

    /* Programs data. */
    FLASH->DATA = FLASH_Data;

    /* Read the value of the flash control register. */
    tmpReg = FLASH->CR;

    /* Clear CR_KEY[15:0] bits. */
    tmpReg &= ~FLASH_CR_KEY_Msk;

    /* Set CR_KEY[15:0] bits according to FLASH_CR_KEY. */
    tmpReg |= (uint32_t)(FLASH_CR_KEY);

    /* Clear OP_MODE[2:0] bits. */
    tmpReg &= ~FLASH_CR_OP_MODE_Msk;

    /* Set OP_MODE[2:0] bits with chip erase. */
    tmpReg |=  (uint32_t)(FLASH_OPERATION_PROGRAM << FLASH_CR_OP_MODE_Pos);

    /* Set OP_START bit with chip erase. */
    tmpReg |=  FLASH_CR_OP_START_Msk;

    /* Store the new value. */
    FLASH->CR = tmpReg;
    
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);
  }

  /* Return the programs Status */
  return status;
}
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */

#if defined (DPM32M026) || defined (DPM32T55x)
/**
 *******************************************************************************
 * @brief   Programs a word at specified address.
 * @param   [in]  FLASH_PageStartAddr: Specifies the page start addr.
 * @param   [in]  FLASH_Data: Specifies the data to be programmed.
 * @retval  FLASH_StatusEnum: Flash status.
 *          This parameter can be a value of @ref FLASH_StatusEnum.
 *            @arg FLASH_OK: Flash OK.
 *            @arg FLASH_BUSY: Flash busy.
 *            @arg FLASH_ERROR_CR: Write configure register error.
 *            @arg FLASH_ERROR_WP: Write protected error.
 *            @arg FLASH_ERROR_ADDR: Program or erase address out of range error.
 *            @arg FLASH_TIMEOUT: Write flash timeout.
 *            @arg FLASH_ERROR_DATA: Write data error.
 ******************************************************************************/
FLASH_StatusEnum FLASH_ProgramPage(uint32_t FLASH_PageStartAddr, uint32_t* FLASH_Data)
{
  FLASH_StatusEnum status = FLASH_OK;
  uint32_t index = 0UL;
  
  /* Check write data. */
  if(FLASH_Data == NULL)
    return FLASH_ERROR_DATA;

  /* Check write address. */
  if(FLASH_PageStartAddr % DPM_PAGE_SIZE != 0)
    return FLASH_ERROR_ADDR;
  
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

  if(status == FLASH_OK)
  {
    /* Clear all status. */
    FLASH->SR = FLASH_SR_BUSY_Msk | FLASH_SR_CR_BSY_ERR_Msk | FLASH_SR_CR_LK_ERR_Msk
    | FLASH_SR_CPE_WP_ERR_Msk | FLASH_SR_MN_WP_ERR_Msk | FLASH_SR_OP_WP_ERR_Msk 
    | FLASH_SR_ADDR_ERR_Msk | FLASH_SR_ALLOW_DATA_WR_Msk | FLASH_SR_EEPROM_WP_ERR_Msk;
    
    /* Programs address. */
    FLASH->ADDR = FLASH_PageStartAddr;

    /* Enter programe mode. */
    FLASH->CR = FLASH_CR_PAGE_PROGRAME_MODE;

    while(FLASH_GetFlagStatus(FLASH_FLAG_ALLOW_DATA_WR) == RESET)
    {
      status = FLASH_GetStatus();
      if((status != FLASH_BUSY) && (status != FLASH_OK))
      {
        return status;
      } 
    }
        
    /* Write data. */
    for(index = 0; index < DPM_PAGE_SIZE / 4u; index++)
    {
      FLASH->DATA = FLASH_Data[index];
    }
    
    /* Data write done. */
    FLASH->CR = FLASH_CR_PAGE_PROGRAME_DATA_DONE;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);
  }

  /* Return the programs Status */
  return status;
}
#endif /* DPM32M026 || DPM32T55x */


/**
 *******************************************************************************
 * @brief   Read a word from specified address.
 * @param   [in]  FLASH_Addr: Specifies the address to be read.
 *                The address must be between 0x18000000UL and 0x1801FFFFUL.
 * @retval  uint32_t: The value of the specified address data.
 ******************************************************************************/
uint32_t FLASH_ReadWord(uint32_t FLASH_Addr)
{
  /* Retrun the value of the specified address data. */
  return *(uint32_t*)FLASH_Addr;
}

/**
 *******************************************************************************
 * @brief   Get the Flash flag is set or not.
 * @param   [in]  FLASH_FlagType: Specifies the Flash flag.
 *                This parameter can be a value of @ref FLASH_FlagTypeEnum.
 *                  @arg FLASH_FLAG_BUSY: Flash busy flag.
 *                  @arg FLASH_FLAG_ERROR_CR_BSY: Write configure register
 *                       when flash busy error flag.
 *                  @arg FLASH_FLAG_ERROR_CR_LOCK: Write configure register
 *                       when locked error flag.
 *                  @arg FLASH_FLAG_ERROR_CPE_WP: Chip erase when any main page
 *                       write protected error flag.
 *                  @arg FLASH_FLAG_ERROR_MN_WP: Program or erase when main page
 *                       write protected error flag.
 *                  @arg FLASH_FLAG_ERROR_OP_WP: Program or erase when option
 *                       bytes page write protected error flag.
 *                  @arg FLASH_FLAG_ERROR_ADDR: Program or erase address
 *                       out of range error flag.
 *                  DPM32M026|DPM32T55x:
 *                  @arg FLASH_FLAG_ALLOW_DATA_WR: Indicate whether data is 
 *                       allowed to be written to the FLASH-DATA register
 *                  @arg FLASH_FLAG_ERROR_EEPROM_WP: EEPROM program or erase error.
 * @retval  FlagState: The state of UART_IntFlag (SET or RESET).
 ******************************************************************************/
FlagState FLASH_GetFlagStatus(FLASH_FlagTypeEnum FLASH_FlagType)
{
  FlagState state = RESET;

  /* Get the status of the Interrupt */
  if(RESET != (FLASH->SR & ((uint32_t)FLASH_FlagType)))
  {
    state = SET;
  }
  else
  {
    state = RESET;
  }

  /* Return the status of the Flash flag. */
  return state;
}

/**
 *******************************************************************************
 * @brief   Clears the Flash flags.
 * @param   [in]  FLASH_FlagType: Specifies the Flash flag.
 *                This parameter can be a value of @ref FLASH_FlagTypeEnum.
 *                  @arg FLASH_FLAG_BUSY: Flash busy flag.
 *                  @arg FLASH_FLAG_ERROR_CR_BSY: Write configure register
 *                       when flash busy error flag.
 *                  @arg FLASH_FLAG_ERROR_CR_LOCK: Write configure register
 *                       when locked error flag.
 *                  @arg FLASH_FLAG_ERROR_CPE_WP: Chip erase when any main page
 *                       write protected error flag.
 *                  @arg FLASH_FLAG_ERROR_MN_WP: Program or erase when main page
 *                       write protected error flag.
 *                  @arg FLASH_FLAG_ERROR_OP_WP: Program or erase when option
 *                       bytes page write protected error flag.
 *                  @arg FLASH_FLAG_ERROR_ADDR: Program or erase address
 *                       out of range error flag.
 *                  DPM32M026|DPM32T55x:
 *                  @arg FLASH_FLAG_ALLOW_DATA_WR: Indicate whether data is 
 *                       allowed to be written to the FLASH-DATA register
 *                  @arg FLASH_FLAG_ERROR_EEPROM_WP: EEPROM program or erase error.
 * @retval  None.
 ******************************************************************************/
void FLASH_ClearFlag(FLASH_FlagTypeEnum FLASH_FlagType)
{
  /* Clear interrupt flags.*/
  if(FLASH_FlagType != FLASH_FLAG_BUSY)
  {
    FLASH->SR = ((uint32_t)FLASH_FlagType);
  }
}


#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
/**
 *******************************************************************************
 * @brief   Set user Bootloader Entry Address.
 * @param   [in]  FLASH_Addr: Specifies the user Bootloader Entry Address.
 *                The address must be between 0x18000000UL and 0x1801FFFFUL.
 * @retval  FLASH_StatusEnum: Flash status.
 *          This parameter can be a value of @ref FLASH_StatusEnum.
 *            @arg FLASH_OK: Flash OK.
 *            @arg FLASH_BUSY: Flash busy.
 *            @arg FLASH_ERROR_CR: Write configure register error.
 *            @arg FLASH_ERROR_WP: Write protected error.
 *            @arg FLASH_ERROR_ADDR: Program or erase address out of range error.
 *            @arg FLASH_TIMEOUT: Write flash timeout.
 ******************************************************************************/
FLASH_StatusEnum FLASH_OB0_SetBootLoaderAddr(uint32_t FLASH_Addr)
{
  FLASH_StatusEnum status = FLASH_OK;

  status = FLASH_ProgramWord(BOOTLOADER_ENTRY_ADDR, FLASH_Addr);

  /* Return the SetBootLoader entry Status. */
  return status;
}
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */

/**
 *******************************************************************************
 * @brief   Erase option bytes page0 configuratons.
 * @retval  FLASH_StatusEnum: Flash status.
 *          This parameter can be a value of @ref FLASH_StatusEnum.
 *            @arg FLASH_OK: Flash OK.
 *            @arg FLASH_BUSY: Flash busy.
 *            @arg FLASH_ERROR_CR: Write configure register error.
 *            @arg FLASH_ERROR_WP: Write protected error.
 *            @arg FLASH_ERROR_ADDR: Program or erase address out of range error.
 *            @arg FLASH_TIMEOUT: Write flash timeout.
 * 
 ******************************************************************************/
FLASH_StatusEnum FLASH_OB0_Erase(void)
{
  FLASH_StatusEnum status = FLASH_OK;

  status = FLASH_ErasePage(FLASH_OB0_PROGRAM_ADDR);

  /* Return the erase Status. */
  return status;
}

/**
 *******************************************************************************
 * @brief   Enable or disable the specifies option bytes page0 functions.
 *          System reset configuration takes effect.
 * @param   [in]  OB0_Func: Specifies the option bytes page0 functions.
 *                This parameter can be a value of @ref FLASH_OB0FunctionEnum.
 *                  @arg FLASH_OB0_FUNCTION_DISDEBUG: Disable debug functions.
 *                  @arg FLASH_OB0_FUNCTION_READPRO: Flash read protect.
 *                  @arg FLASH_OB0_FUNCTION_DISISP: Disable ISP download functions.
 *                  @arg FLASH_OB0_FUNCTION_DISRECOVE: Disbale recover functions.
 *                  @arg FLASH_OB0_FUNCTION_OB0WP: Option bytes page0 write protect functions.
 * @param   [in]  NewState: New state of the option bytes functions.
 *                This parameter can be: ENABLE or DISABLE.
 * @retval  FLASH_StatusEnum: Flash status.
 *                This parameter can be a value of @ref FLASH_StatusEnum.
 *                  @arg FLASH_OK: Flash OK.
 *                  @arg FLASH_BUSY: Flash busy.
 *                  @arg FLASH_ERROR_CR: Write configure register error.
 *                  @arg FLASH_ERROR_WP: Write protected error.
 *                  @arg FLASH_ERROR_ADDR: Program or erase address out of range error.
 *                  @arg FLASH_TIMEOUT: Write flash timeout.
 ******************************************************************************/
FLASH_StatusEnum FLASH_OB0_FunctionConfig(FLASH_OB0FunctionEnum OB0_Func, FunctionalState NewState)
{
  FLASH_StatusEnum status = FLASH_OK;

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) || \
    defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
  uint32_t tmpReg = 0UL;

  if(DISABLE != NewState)
  {
    /* Enable the specifies option bytes functions. */
    tmpReg = *(uint32_t*)FLASH_OB0_FUNCTION_ADDR;
    tmpReg |= OB0_Func;
    tmpReg &= ~(uint32_t)(OB0_Func << 16u);

    /* Store the new value. */
    status = FLASH_ProgramWord(FLASH_OB0_FUNCTION_ADDR, tmpReg);
  }
  else
  {
    /* Disable the specifies option bytes functions. */
    tmpReg = *(uint32_t*)FLASH_OB0_FUNCTION_ADDR;
    tmpReg &= ~(uint32_t)OB0_Func;
    tmpReg |= (uint32_t)(OB0_Func << 16u);

    /* Store the new value. */
    status = FLASH_ProgramWord(FLASH_OB0_FUNCTION_ADDR, tmpReg);
  }
  
#elif defined (DPM32M026) || defined (DPM32T55x)

  uint32_t tmpBuf[DPM_PAGE_SIZE / 4u] = { 0u };
  uint32_t i = 0u;
  uint32_t index = (FLASH_OB0_FUNCTION_ADDR - FLASH_OB0_START_ADDR) / 4u;
  
  for(i = 0; i < DPM_PAGE_SIZE / 4u; i++)
  {
    tmpBuf[i] = *(uint32_t*)(FLASH_OB0_START_ADDR + i * 4u);
  }
  
  if(DISABLE != NewState)
  {
    /* Enable the specifies option bytes functions. */
    tmpBuf[index] |= OB0_Func;
    tmpBuf[index] &= ~(uint32_t)(OB0_Func << 16u);

    /* Store the new value. */
    status = FLASH_ProgramPage(FLASH_OB0_START_ADDR, tmpBuf);
  }
  else
  {
    /* Disable the specifies option bytes functions. */
    tmpBuf[index] &= ~(uint32_t)OB0_Func;
    tmpBuf[index] |= (uint32_t)(OB0_Func << 16u);

    /* Store the new value. */
    status = FLASH_ProgramPage(FLASH_OB0_START_ADDR, tmpBuf);
  }
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */

  /* Return the config Status. */
  return status;
}

/**
 *******************************************************************************
 * @brief   Enable or disable the specifies page write protect functions.
 *          System reset configuration takes effect.
 * @param   [in]  FlashPage: Specifies page to operate.
 * @param   [in]  NewState: New state of the page write protect.
 *                This parameter can be: ENABLE or DISABLE.
 * @retval  FLASH_StatusEnum: Flash status.
 *                This parameter can be a value of @ref FLASH_StatusEnum.
 *                  @arg FLASH_OK: Flash OK.
 *                  @arg FLASH_BUSY: Flash busy.
 *                  @arg FLASH_ERROR_CR: Write configure register error.
 *                  @arg FLASH_ERROR_WP: Write protected error.
 *                  @arg FLASH_ERROR_ADDR: Program or erase address out of range error.
 *                  @arg FLASH_TIMEOUT: Write flash timeout.
 ******************************************************************************/
FLASH_StatusEnum FLASH_OB0_WriteProConfig(uint8_t FlashPage, FunctionalState NewState)
{
  FLASH_StatusEnum status = FLASH_OK;
  
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) || \
    defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
  uint32_t tmpReg = 0UL;

  uint32_t writeAddr = FLASH_OB0_WP_PAGE_START_ADDR + FlashPage / 32 * 4;

  if(DISABLE != NewState)
  {
    /* Enable the specifies option bytes functions. */
    tmpReg = *(uint32_t*)(writeAddr);
    tmpReg |= (uint32_t)(1 << FlashPage % 32);

    /* Store the new value. */
    status = FLASH_ProgramWord(writeAddr, tmpReg);
  }
  else
  {
    /* Disable the specifies option bytes functions. */
    tmpReg = *(uint32_t*)(writeAddr);
    tmpReg &= ~(uint32_t)(1 << FlashPage % 32);

    /* Store the new value. */
    status = FLASH_ProgramWord(writeAddr, tmpReg);
  }
#elif defined (DPM32M026) || defined (DPM32T55x)

  uint32_t tmpBuf[DPM_PAGE_SIZE / 4u] = { 0u };
  uint32_t i = 0u;

  uint32_t writeAddrIndex = FlashPage / 32u * 4u;
  
  for(i = 0; i < DPM_PAGE_SIZE / 4u; i++)
  {
    tmpBuf[i] = *(uint32_t*)(FLASH_OB0_START_ADDR + i * 4u);
  }
  
  if(DISABLE != NewState)
  {
    /* Enable the specifies option bytes functions. */
    tmpBuf[writeAddrIndex] |= (uint32_t)(1 << FlashPage % 32);

    /* Store the new value. */
    status = FLASH_ProgramPage(FLASH_OB0_START_ADDR, tmpBuf);
  }
  else
  {
    /* Disable the specifies option bytes functions. */
    tmpBuf[writeAddrIndex] &= ~(uint32_t)(1 << FlashPage % 32);

    /* Store the new value. */
    status = FLASH_ProgramPage(FLASH_OB0_START_ADDR, tmpBuf);
  }

#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */
  
  /* Return the config Status. */
  return status;
}

/**
 *******************************************************************************
 * @brief   Enable or disable the specifies page write protect functions.
 *          System reset configuration takes effect.
 * @param   [in]  FlashPageProBuf: Flash write protect buffer.
 * @param   [in]  ProBufLen: Write protect buffer length.
 * @retval  FLASH_StatusEnum: Flash status.
 *                This parameter can be a value of @ref FLASH_StatusEnum.
 *                  @arg FLASH_OK: Flash OK.
 *                  @arg FLASH_BUSY: Flash busy.
 *                  @arg FLASH_ERROR_CR: Write configure register error.
 *                  @arg FLASH_ERROR_WP: Write protected error.
 *                  @arg FLASH_ERROR_ADDR: Program or erase address out of range error.
 *                  @arg FLASH_TIMEOUT: Write flash timeout.
 ******************************************************************************/
FLASH_StatusEnum FLASH_OB0_WriteProBufConfig(uint32_t* FlashPageProBuf, uint8_t ProBufLen)
{
  FLASH_StatusEnum status = FLASH_OK;

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) || \
    defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
  
  status = FLASH_ProgramBufNotCheck(FLASH_OB0_WP_PAGE_START_ADDR, FlashPageProBuf, ProBufLen);

#elif defined (DPM32M026) || defined (DPM32T55x)
  
  uint32_t tmpBuf[DPM_PAGE_SIZE / 4u] = { 0u };
  uint32_t i = 0u;

  uint32_t writeAddrIndex = (FLASH_OB0_WP_PAGE_START_ADDR - FLASH_OB0_START_ADDR) / 4u;
  
  for(i = 0; i < DPM_PAGE_SIZE / 4u; i++)
  {
    tmpBuf[i] = *(uint32_t*)(FLASH_OB0_START_ADDR + i * 4u);
  }
  
  for(i = 0; i < ProBufLen; i++)
  {
    tmpBuf[writeAddrIndex++] = FlashPageProBuf[i];
  }

  /* Store the new value. */
  status = FLASH_ProgramPage(FLASH_OB0_START_ADDR, tmpBuf);
  
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */
  /* Return the config Status. */
  return status;
}

/**
 *******************************************************************************
 * @brief   Enable or disable the option bytes page0 functions.
 * @param   [in]  NewState: New state of the option bytes page0 functions.
 *                This parameter can be: ENABLE or DISABLE.
 * @retval  FLASH_StatusEnum: Flash status.
 *                This parameter can be a value of @ref FLASH_StatusEnum.
 *                  @arg FLASH_OK: Flash OK.
 *                  @arg FLASH_BUSY: Flash busy.
 *                  @arg FLASH_ERROR_CR: Write configure register error.
 *                  @arg FLASH_ERROR_WP: Write protected error.
 *                  @arg FLASH_ERROR_ADDR: Program or erase address out of range error.
 *                  @arg FLASH_TIMEOUT: Write flash timeout.
 ******************************************************************************/
FLASH_StatusEnum FLASH_OB0_Cmd(FunctionalState NewState)
{
  FLASH_StatusEnum status = FLASH_OK;

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) || \
    defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
  
  if(DISABLE != NewState)
  {
    /* Enable the specifies option bytes functions. */
    status = FLASH_ProgramWord(FLASH_OB0_PROGRAM_ADDR, 0x00000000);
  }
  else
  {
    /* Disable the specifies option bytes functions. */
    status = FLASH_ProgramWord(FLASH_OB0_PROGRAM_ADDR, 0xFFFFFFFF);
  }
  
#elif defined (DPM32M026) || defined (DPM32T55x)
  
  uint32_t tmpBuf[DPM_PAGE_SIZE / 4u] = { 0u };
  uint32_t i = 0u;

  
  for(i = 0; i < DPM_PAGE_SIZE / 4u; i++)
  {
    tmpBuf[i] = *(uint32_t*)(FLASH_OB0_START_ADDR + i * 4u);
  }
  
  if(DISABLE != NewState)
  {
    /* Enable the specifies option bytes functions. */
     tmpBuf[DPM_PAGE_SIZE / 4u - 1] = 0x00000000u;
  }
  else
  {
    /* Disable the specifies option bytes functions. */
     tmpBuf[DPM_PAGE_SIZE / 4u - 1] = 0xFFFFFFFFu;
  }
  
  /* Store the new value. */
  status = FLASH_ProgramPage(FLASH_OB0_START_ADDR, tmpBuf);
  
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */
  /* Return the config Status. */
  return status;
}

/**
 *******************************************************************************
 * @brief   Erase option bytes page1 configuratons.
 * @retval  FLASH_StatusEnum: Flash status.
 *          This parameter can be a value of @ref FLASH_StatusEnum.
 *            @arg FLASH_OK: Flash OK.
 *            @arg FLASH_BUSY: Flash busy.
 *            @arg FLASH_ERROR_CR: Write configure register error.
 *            @arg FLASH_ERROR_WP: Write protected error.
 *            @arg FLASH_ERROR_ADDR: Program or erase address out of range error.
 *            @arg FLASH_TIMEOUT: Write flash timeout.
 ******************************************************************************/
FLASH_StatusEnum FLASH_OB1_Erase(void)
{
  FLASH_StatusEnum status = FLASH_OK;

  status = FLASH_ErasePage(FLASH_OB1_USR_DATA_START_ADDR);

  /* Return the erase Status. */
  return status;
}

/**
 *******************************************************************************
 * @brief   Programs a word at specified address.
 * @param   [in]  FLASH_Addr: Specifies the address to be programmed.
 *                The address must be between 0x10001A00 and 0x10001BF8.
 * @param   [in]  FLASH_Data: Specifies the data to be programmed.
 * @retval  FLASH_StatusEnum: Flash status.
 *                This parameter can be a value of @ref FLASH_StatusEnum.
 *                  @arg FLASH_OK: Flash OK.
 *                  @arg FLASH_BUSY: Flash busy.
 *                  @arg FLASH_ERROR_CR: Write configure register error.
 *                  @arg FLASH_ERROR_WP: Write protected error.
 *                  @arg FLASH_ERROR_ADDR: Program or erase address out of range error.
 *                  @arg FLASH_TIMEOUT: Write flash timeout.
 ******************************************************************************/
FLASH_StatusEnum FLASH_OB1_ProgramUserData(uint32_t FLASH_Addr, uint32_t FLASH_Data)
{
  FLASH_StatusEnum status = FLASH_OK;

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) || \
    defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
  
  status = FLASH_ProgramWord(FLASH_Addr, FLASH_Data);

#elif defined (DPM32M026) || defined (DPM32T55x)
  
  uint32_t tmpBuf[DPM_PAGE_SIZE / 4u] = { 0u };
  uint32_t i = 0u;
  
  for(i = 0; i < DPM_PAGE_SIZE / 4u; i++)
  {
    tmpBuf[i] = *(uint32_t*)(FLASH_OB1_USR_DATA_START_ADDR + i * 4u);
  }
  
  tmpBuf[(FLASH_Addr - FLASH_OB1_USR_DATA_START_ADDR) / 4u] = FLASH_Data;
  
  /* Store the new value. */
  status = FLASH_ProgramPage(FLASH_OB1_USR_DATA_START_ADDR, tmpBuf);
  
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */
  
  /* Return the program Status. */
  return status;
}

/**
 *******************************************************************************
 * @brief   Enable or disable the option bytes page1 functions.
 * @param   [in]  NewState: New state of the option bytes page1 functions.
 *                This parameter can be: ENABLE or DISABLE.
 * @retval  FLASH_StatusEnum: Flash status.
 *                This parameter can be a value of @ref FLASH_StatusEnum.
 *                  @arg FLASH_OK: Flash OK.
 *                  @arg FLASH_BUSY: Flash busy.
 *                  @arg FLASH_ERROR_CR: Write configure register error.
 *                  @arg FLASH_ERROR_WP: Write protected error.
 *                  @arg FLASH_ERROR_ADDR: Program or erase address out of range error.
 *                  @arg FLASH_TIMEOUT: Write flash timeout.
 ******************************************************************************/
FLASH_StatusEnum FLASH_OB1_WritePro(FunctionalState NewState)
{
  FLASH_StatusEnum status = FLASH_OK;

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) || \
    defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
  
  if(DISABLE != NewState)
  {
    /* Enable the option bytes page1 functions. */
    status = FLASH_ProgramWord(FLASH_OB1_PROGRAM_ADDR, 0x00000000);
  }
  else
  {
    /* Disable the soption bytes page1 functions. */
    status = FLASH_ProgramWord(FLASH_OB1_PROGRAM_ADDR, 0xFFFFFFFF);
  }
  
#elif defined (DPM32M026) || defined (DPM32T55x)
  
  uint32_t tmpBuf[DPM_PAGE_SIZE / 4u] = { 0u };
  uint32_t i = 0u;

  
  for(i = 0; i < DPM_PAGE_SIZE / 4u; i++)
  {
    tmpBuf[i] = *(uint32_t*)(FLASH_OB1_PROGRAM_ADDR + i * 4u);
  }
  
  if(DISABLE != NewState)
  {
    /* Enable the specifies option bytes functions. */
     tmpBuf[DPM_PAGE_SIZE / 4u - 1] = 0x00000000u;
  }
  else
  {
    /* Disable the specifies option bytes functions. */
     tmpBuf[DPM_PAGE_SIZE / 4u - 1] = 0xFFFFFFFFu;
  }
  
  /* Store the new value. */
  status = FLASH_ProgramPage(FLASH_OB1_PROGRAM_ADDR, tmpBuf);
  
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */
  /* Return the config Status. */
  return status;
  
}

/**
 *******************************************************************************
 * @brief   Read a word from specified address.
 * @param   [in]  FLASH_Addr: Specifies the address to be read.
 * @param   [in]  FLASH_DataBuf: Specifies the data buffer to be read.
 * @param   [in]  FLASH_DataLen: Read buffer length.
 * @retval  None.
 ******************************************************************************/
void FLASH_ReadBuffer(uint32_t FLASH_Addr, uint32_t* FLASH_DataBuf, uint32_t FLASH_DataLen)
{
  uint32_t i = 0u;

  /* Loop Read to flash. */
  for(i = 0; i < FLASH_DataLen; i++)
  {
    /* Read a word to flash. */
    FLASH_DataBuf[i] = FLASH_ReadWord(FLASH_Addr);

    /* Add one word to the address length. */
    FLASH_Addr += 4;
  }
}
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
/**
 *******************************************************************************
 * @brief   Programs buffer at specified address.
 * @param   [in]  FLASH_Addr: Specifies the address to be programmed.
 * @param   [in]  FLASH_DataBuf: Specifies the data buffer to be programmed.
 * @param   [in]  FLASH_DataLen: Programs buffer length.
 * @retval  FLASH_StatusEnum: Flash status.
 *                This parameter can be a value of @ref FLASH_StatusEnum.
 *                  @arg FLASH_OK: Flash OK.
 *                  @arg FLASH_BUSY: Flash busy.
 *                  @arg FLASH_ERROR_CR: Write configure register error.
 *                  @arg FLASH_ERROR_WP: Write protected error.
 *                  @arg FLASH_ERROR_ADDR: Program or erase address out of range error.
 *                  @arg FLASH_TIMEOUT: Write flash timeout.
 ******************************************************************************/
FLASH_StatusEnum FLASH_ProgramBufNotCheck(uint32_t FLASH_Addr, uint32_t* FLASH_DataBuf, uint32_t FLASH_DataLen)
{
  uint32_t i = 0u;
  FLASH_StatusEnum status = FLASH_OK;

  /* Loop write to flash. */
  for(i = 0u; i < FLASH_DataLen; i++)
  {
    /* Write a word to flash. */
    status = FLASH_ProgramWord(FLASH_Addr, FLASH_DataBuf[i]);

    /* Check status. */
    if(status != FLASH_OK)
    {
      break;
    }
    /* Add one word to the address length. */
    FLASH_Addr += 4u;
  }

  /* Return the config Status. */
  return status;
}
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */

/**
 *******************************************************************************
 * @brief   Flash restore factory mode.
 * @retval  None.
 * @note    Flash Restore Factory Mode.
 ******************************************************************************/
void FLASH_RestoreFactoryMode(void)
{
  /* The first write, the KEY field is written as 0x900D,the UNLK_DATA field is written as any number M. */
  FLASH->FAC_REC = FLASH_FAC_REC_KEY1;
  /* The second write, the KEY field is written as 0xBEEF, the UNLK_DATA field is written as ~M (invert M bit by bit). */
  FLASH->FAC_REC = FLASH_FAC_REC_KEY2;
  /* Finally write, the KEY field is written as any number, the UNLK_DATA field is written as 0x1.*/
  FLASH->FAC_REC = FLASH_FAC_REC_UNLK_DATA;
}
