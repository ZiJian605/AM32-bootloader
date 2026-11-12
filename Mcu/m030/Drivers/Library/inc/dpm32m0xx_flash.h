/**
 *******************************************************************************
 * @file    dpm32m0xx_flash.h
 *
 * @brief   Header file for FLASH firmware driver.
 *
 * @author  DPM
 *
 * @version V1.0.0
 *
 * @date    2023-11-01
 *
 *******************************************************************************/

#ifndef __DPM32M0XX_FLASH_H__
#define __DPM32M0XX_FLASH_H__


#ifdef __cplusplus
extern "C" {
#endif

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

#include "dpm32m0xx.h"


/** @defgroup FLASH_Parame_definition
  * @{
  * @}
  */

#define DPM_FLASH_BASE                        ((uint32_t)0x18000000UL)

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) || \
    defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
#define DPM_PAGE_SIZE                         ((uint32_t)512UL)
#elif defined (DPM32M026) || defined (DPM32T55x)
#define DPM_PAGE_SIZE                         ((uint32_t)256UL)
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */


#define FLASH_ER_PRG_TIMEOUT                  ((uint32_t)0x0FFFFFFFUL)
#define FLASH_CR_KEY                          ((uint32_t)0xACCE0000UL)
#define FLASH_CR_PAGE_PROGRAME_MODE           ((uint32_t)0xACCE0003UL)
#define FLASH_CR_PAGE_PROGRAME_DATA_DONE      ((uint32_t)0xACCE0080UL)




#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) || \
    defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
#define BOOTLOADER_ENTRY_ADDR                 ((uint32_t)0x10001800UL)
#define FLASH_OB0_PROGRAM_ADDR                ((uint32_t)0x100019FCUL)
#define FLASH_OB0_FUNCTION_ADDR               ((uint32_t)0x10001804UL)
#define FLASH_OB0_WP_PAGE_START_ADDR          ((uint32_t)0x10001808UL)

#define FLASH_OB1_PROGRAM_ADDR                ((uint32_t)0x10001BFCUL)
#define FLASH_OB1_USR_DATA_START_ADDR         ((uint32_t)0x10001A00UL)

#define FLASH_FAC_REC_KEY1                    ((uint32_t)0x900D0000UL)
#define FLASH_FAC_REC_KEY2                    ((uint32_t)0xBEEFFFFFUL)
#define FLASH_FAC_REC_UNLK_DATA               ((uint32_t)0x00000001UL)

#elif defined (DPM32M026) || defined(DPM32T55x)

#define FLASH_OB0_START_ADDR                  ((uint32_t)0x10000200UL)
#define FLASH_OB0_PROGRAM_ADDR                ((uint32_t)0x100002FCUL)
#define FLASH_OB0_FUNCTION_ADDR               ((uint32_t)0x10000204UL)
#define FLASH_OB0_WP_PAGE_START_ADDR          ((uint32_t)0x10000208UL)

#define FLASH_OB1_PROGRAM_ADDR                ((uint32_t)0x100003FCUL)
#define FLASH_OB1_USR_DATA_START_ADDR         ((uint32_t)0x10000300UL)

#define FLASH_FAC_REC_KEY1                    ((uint32_t)0x900D0000UL)
#define FLASH_FAC_REC_KEY2                    ((uint32_t)0xBEEFFFFFUL)
#define FLASH_FAC_REC_UNLK_DATA               ((uint32_t)0x00000001UL)

#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */





/**
 ******************************************************************************
 * @brief Flash status enumeration.
 *****************************************************************************/
typedef enum
{
  FLASH_OK            = 0x00UL,  /*!< Flash OK. */
  FLASH_BUSY,                    /*!< Flash busy. */
  FLASH_ERROR_CR,                /*!< Write configure register error. */
  FLASH_ERROR_WP,                /*!< Write protected error. */
  FLASH_ERROR_ADDR,              /*!< Program or erase address out of range error. */
  FLASH_TIMEOUT,                 /*!< Write flash timeout. */
  FLASH_ERROR_DATA,              /*!< Data err. */
} FLASH_StatusEnum;

/**
 ******************************************************************************
 * @brief Flash operation mode enumeration.
 *****************************************************************************/
typedef enum
{
  FLASH_OPERATION_PROGRAM            = 0x01UL,        /*!< Program. */
  FLASH_OPERATION_PAGE_ERASE,                         /*!< Page erase. */
  FLASH_OPERATION_CHIP_ERASE                          /*!< Chip erase. */
} FLASH_OperationModeEnum;

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
/**
 ******************************************************************************
 * @brief Flash read wait cycles enumeration.
 *****************************************************************************/
typedef enum
{
#if defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)   
  FLASH_READ_WAIT_HCLK_CYCLE0         = 0x00UL,             /*!< When   0Mhz < HCLK <= 24Mhz, wait 0 HCLK cycle. */
#endif /* DPM32M036 || DPM32M030 || DPM32M015 */
  FLASH_READ_WAIT_HCLK_CYCLE1         = 0x01UL,             /*!< When  24Mhz < HCLK <= 48Mhz, wait 1 HCLK cycle. */
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
  FLASH_READ_WAIT_HCLK_CYCLE2         = 0x02UL,             /*!< When 48Mhz < HCLK <= 72Mhz, wait 2 HCLK cycle. */
  FLASH_READ_WAIT_HCLK_CYCLE3         = 0x03UL              /*!< When 72Mhz < HCLK <= 96Mhz, wait 3 HCLK cycle. */
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */
} FLASH_ReadWaitCycleEnum;
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */

/**
 ******************************************************************************
 * @brief Flash flag type enumeration.
 *****************************************************************************/
typedef enum
{
  FLASH_FLAG_BUSY                 = 0x01UL,    /*!< Flash busy. */
  FLASH_FLAG_ERROR_CR_BSY         = 0x02UL,    /*!< Write configure register when flash busy error. */
  FLASH_FLAG_ERROR_CR_LK          = 0x04UL,    /*!< Write configure register when locked error. */
  FLASH_FLAG_ERROR_CPE_WP         = 0x08UL,    /*!< Chip erase when any main page write protected error. */
  FLASH_FLAG_ERROR_MN_WP          = 0x10UL,    /*!< Program or erase when main page write protected error. */
  FLASH_FLAG_ERROR_OP_WP          = 0x20UL,    /*!< Program or erase when option bytes page write protected error. */
  FLASH_FLAG_ERROR_ADDR           = 0x40UL,    /*!< Program or erase address out of range error. */
#if defined (DPM32M026) || defined (DPM32T55x)
  FLASH_FLAG_ALLOW_DATA_WR        = 0x80UL,    /*!< Indicate whether data is allowed to be written to the FLASH-DATA register. */
  FLASH_FLAG_ERROR_EEPROM_WP      = 0x100UL    /*!< EEPROM program or erase error. */
#endif /* DPM32M026 || DPM32T55x */
} FLASH_FlagTypeEnum;

/**
 ******************************************************************************
 * @brief Flash erase type enumeration.
 *****************************************************************************/
typedef enum
{
  FLASH_ERASE_TYPE_PAGE            = 0x01UL,        /*!< Page erase. */
  FLASH_ERASE_TYPE_CHIP                             /*!< Chip erase. */
} FLASH_EraseTypeEnum;

/**
 ******************************************************************************
 * @brief User Option Bytes page0 functions enumeration.
 *****************************************************************************/
typedef enum
{
  FLASH_OB0_FUNCTION_DISDEBUG     = 0x0001UL,       /*!< Disable debug functions. */
  FLASH_OB0_FUNCTION_READPRO      = 0x0002UL,       /*!< Flash read protect. */
  FLASH_OB0_FUNCTION_DISISP       = 0x0004UL,       /*!< Disable ISP download functions. */
  FLASH_OB0_FUNCTION_DISRECOVE    = 0x0008UL,       /*!< Disbale recover functions. */
  FLASH_OB0_FUNCTION_OB0WP        = 0x0100UL,       /*!< Option bytes page0 write protect functions. */
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
  FLASH_OB0_FUNCTION_BLVLD        = 0x8000UL        /*!< BOOTLOADER_ ENTRY_ ADDR valid flag. */
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */
} FLASH_OB0FunctionEnum;



#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
/* FLASH Interface configuration functions. */
void                    FLASH_SetReadWaitCycle(FLASH_ReadWaitCycleEnum FLASH_ReadWait);
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) || defined (DPM32M036)
void                    FLASH_PrefetchBufferCmd(FunctionalState NewState);
FlagState               FLASH_GetPrefetchBufferStatus(void);
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 */

/* FLASH Memory Programming and read functions. */
void                    FLASH_Unlock(void);
void                    FLASH_Lock(void);
FLASH_StatusEnum        FLASH_ErasePage(uint32_t FLASH_EraseAddr);
FLASH_StatusEnum        FLASH_EraseChip(void);

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
FLASH_StatusEnum        FLASH_ProgramWord(uint32_t FLASH_Addr, uint32_t FLASH_Data);
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */

#if defined (DPM32M026) || defined (DPM32T55x)
FLASH_StatusEnum        FLASH_ProgramPage(uint32_t FLASH_PageStartAddr, uint32_t* FLASH_Data);
#endif /* DPM32M026 || DPM32T55x */

/* Flash flags management functions. */
FLASH_StatusEnum        FLASH_GetStatus(void);
FLASH_StatusEnum        FLASH_WaitForLastOperation(uint32_t FLASH_Timeout);
FlagState               FLASH_GetFlagStatus(FLASH_FlagTypeEnum FLASH_FlagType);
void                    FLASH_ClearFlag(FLASH_FlagTypeEnum FLASH_FlagType);

/* FLASH Option Bytes Programming functions. */
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
FLASH_StatusEnum        FLASH_OB0_SetBootLoaderAddr(uint32_t FLASH_Addr);
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */

FLASH_StatusEnum        FLASH_OB0_Erase(void);
FLASH_StatusEnum        FLASH_OB0_FunctionConfig(FLASH_OB0FunctionEnum OB0_Func, FunctionalState NewState);
FLASH_StatusEnum        FLASH_OB0_WriteProConfig(uint8_t FlashPage, FunctionalState NewState);
FLASH_StatusEnum        FLASH_OB0_WriteProBufConfig(uint32_t* FlashPageProBuf, uint8_t ProBufLen);
FLASH_StatusEnum        FLASH_OB0_Cmd(FunctionalState NewState);
FLASH_StatusEnum        FLASH_OB1_Erase(void);
FLASH_StatusEnum        FLASH_OB1_ProgramUserData(uint32_t FLASH_Addr, uint32_t FLASH_Data);
FLASH_StatusEnum        FLASH_OB1_WritePro(FunctionalState NewState);

/* Flash extend functions. */
uint32_t                FLASH_ReadWord(uint32_t FLASH_Addr);
void                    FLASH_ReadBuffer(uint32_t FLASH_Addr, uint32_t* FLASH_DataBuf, uint32_t FLASH_DataLen);

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015) 
FLASH_StatusEnum        FLASH_ProgramBufNotCheck(uint32_t FLASH_Addr, uint32_t* FLASH_DataBuf, uint32_t FLASH_DataLen);
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */

/* Flash restore factory mode */
void                    FLASH_RestoreFactoryMode(void);
/* Compatible with older versions */
#define Flash_RestoreFactoryMode FLASH_RestoreFactoryMode


#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}

#endif

#endif /* __DPM32M0XX_FLASH_H__ */
