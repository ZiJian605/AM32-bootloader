/**
 *******************************************************************************
 * @file    dpm32m0xx_rcc.h
 *
 * @brief   Header file for RCC firmware driver.
 *
 * @author  DPM
 *
 * @version V1.0.0
 *
 * @date    2023-11-01
 *
 *******************************************************************************/

#ifndef __DPM32M0XX_RCC_H__
#define __DPM32M0XX_RCC_H__


#ifdef __cplusplus
extern "C" {
#endif

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif


#include "dpm32m0xx.h"


/* ########################## Oscillator Values adaptation ####################*/
#if defined (DPM32M08x)
/**
  * @brief Adjust the value of External High Speed oscillator (HSE) used in your application.
  *        This value is used by the RCC HAL module to compute the system frequency
  *        (when HSE is used as system clock source, directly or through the PLL).
  */
#if !defined  (HSE_VALUE)
#define HSE_VALUE    8000000UL /*!< Value of the External oscillator in Hz */
#endif /* HSE_VALUE */

#if !defined  (HSE_STARTUP_TIMEOUT)
#define HSE_STARTUP_TIMEOUT    0x0FFFFFFFUL   /*!< Time out for HSE start up. */
#endif /* HSE_STARTUP_TIMEOUT */
#endif /* DPM32M08x */

/**
  * @brief Internal High Speed oscillator (HSI) value.
  *        This value is used by the RCC HAL module to compute the system frequency
  *        (when HSI is used as system clock source, directly or through the PLL).
  */

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
 
#if !defined  (HSI_VALUE)
#define HSI_VALUE    48000000UL /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */
#else
#if !defined  (HSI_VALUE)
#define HSI_VALUE    36000000UL /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */

#if defined (DPM32M08x) || defined (DPM32M05x)
#if !defined  (PLL_HSI_DIV)
#define PLL_HSI_DIV    (8) /*!< Frequency division when HSI is used as PLL clock source. */
#endif /* PLL_HSI_DIV */
#endif /* DPM32M08x || DPM32M05x */

/**
  * @brief Internal Low Speed oscillator (LSI) value.
  */
#if !defined  (LSI_VALUE)
#define LSI_VALUE               32768UL    /*!< LSI Typical Value in Hz */
#endif /* LSI_VALUE */                     /*!< Value of the Internal Low Speed oscillator in Hz
The real value may vary depending on the variations
in voltage and temperature. */

/**
  * @brief External Low Speed oscillator (LSE) value.
  *        This value is used by the UART, RTC HAL module to compute the system frequency
  */
#if !defined  (LSE_VALUE)
#define LSE_VALUE    32768UL /*!< Value of the External oscillator in Hz*/
#endif /* LSE_VALUE */

#if !defined  (LSE_STARTUP_TIMEOUT)
#define LSE_STARTUP_TIMEOUT    0x0FFFFFFFUL   /*!< Time out for LSE start up. */
#endif /* LSE_STARTUP_TIMEOUT */

/* Tip: To avoid modifying this file each time you need to use different HSE,
   ===  you can define the HSE value in your toolchain compiler preprocessor. */

#define LSE_TIMEOUT_VALUE               LSE_STARTUP_TIMEOUT

#if defined (DPM32M08x)
#define HSE_TIMEOUT_VALUE               HSE_STARTUP_TIMEOUT
#endif /* DPM32M08x */

#define HSI_TIMEOUT_VALUE               0x0FFFFFFFUL      /* Startup timeout value of HSI. */
#define LSI_TIMEOUT_VALUE               0x0FFFFFFFUL      /* Startup timeout value of LSI. */

#if defined (DPM32M08x) || defined (DPM32M05x)
#define PLL_TIMEOUT_VALUE               0x0FFFFFFFUL      /* Startup timeout value of PLL. */
#endif /* DPM32M08x || DPM32M05x */


/**
 ******************************************************************************
 * @brief RCC status enumeration.
 *****************************************************************************/
typedef enum
{
  RCC_OK       = 0x00UL,            /*!< RCC configuration ok. */
  RCC_ERROR    = 0x01UL,            /*!< RCC configuration err. */
  RCC_BUSY     = 0x02UL,            /*!< RCC configuration busy. */
  RCC_TIMEOUT  = 0x03UL             /*!< RCC configuration timeout. */
} RCC_StatusEnum;

/**
 ******************************************************************************
 * @brief RCC clock ready flag enumeration.
 *****************************************************************************/
typedef enum
{
  RCC_CLOCK_FLAG_LSIRDY          = RCC_COMMON_CR_LSI_RDY_Msk,   /*!< LSI ready flag. */

#if defined (DPM32M08x)
  RCC_CLOCK_FLAG_HSERDY          = RCC_COMMON_CR_HSE_RDY_Msk,   /*!< HSE ready flag. */
#endif /* DPM32M08x */

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
  RCC_CLOCK_FLAG_LSERDY          = RCC_COMMON_CR_LSE_RDY_Msk,   /*!< LSE ready flag. */
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */

#if defined (DPM32M08x) || defined (DPM32M05x)
  RCC_CLOCK_FLAG_PLLRDY          = RCC_PLL_CR_RDY_Msk           /*!< PLL ready flag. */
#endif /* DPM32M08x || DPM32M05x */
} RCC_ClockFlagEnum;

#if defined (DPM32M08x) || defined (DPM32M05x)
/**
 ******************************************************************************
 * @brief PLL clock source enumeration.
 *****************************************************************************/
typedef enum
{
  RCC_PLL_SOURCE_HSI_DIV8       = 0x00UL,   /*!< HSI divided by 8 selected as PLL entry clock source */
#if defined (DPM32M08x)
  RCC_PLL_SOURCE_HSE                        /*!< HSE selected as PLL entry clock source. */
#endif /* DPM32M08x */
} RCC_PLLSourceEnum;

/**
 ******************************************************************************
 * @brief PLL multiplication factor enumeration.
 *****************************************************************************/
typedef enum
{
#if defined (DPM32M08x)
  RCC_PLL_MUL2               = 0x01U,   /*!< PLL 2 Multiplication Factor. */
  RCC_PLL_MUL3,                         /*!< PLL 3 Multiplication Factor. */
  RCC_PLL_MUL4,                         /*!< PLL 4 Multiplication Factor. */
  RCC_PLL_MUL5,                         /*!< PLL 5 Multiplication Factor. */
  RCC_PLL_MUL6,                         /*!< PLL 6 Multiplication Factor. */
  RCC_PLL_MUL7,                         /*!< PLL 7 Multiplication Factor. */
#endif
  RCC_PLL_MUL8               = 0x07U,   /*!< PLL 8 Multiplication Factor. */
  RCC_PLL_MUL9,                         /*!< PLL 9 Multiplication Factor. */
  RCC_PLL_MUL10,                        /*!< PLL 10 Multiplication Factor. */
  RCC_PLL_MUL11,                        /*!< PLL 11 Multiplication Factor. */
  RCC_PLL_MUL12,                        /*!< PLL 12 Multiplication Factor. */
  RCC_PLL_MUL13,                        /*!< PLL 13 Multiplication Factor. */
  RCC_PLL_MUL14,                        /*!< PLL 14 Multiplication Factor. */
  RCC_PLL_MUL15,                        /*!< PLL 15 Multiplication Factor. */
  RCC_PLL_MUL16                         /*!< PLL 16 Multiplication Factor. */
} RCC_PLLMulEnum;

/**
 ******************************************************************************
 * @brief PLL state enumeration.
 *****************************************************************************/
typedef enum
{
  RCC_PLL_NONE          = 0x00UL,   /*!< PLL is not configured. */
  RCC_PLL_OFF           = 0x01UL,   /*!< PLL deactivation. */
  RCC_PLL_ON            = 0x02UL    /*!< PLL activation. */
} RCC_PLLStateEnum;

/**
 ******************************************************************************
 * @brief SYSCLK source enumeration.
 *****************************************************************************/
typedef enum
{
  RCC_SYSCLK_SOURCE_HSI          = 0x00UL,    /*!< HSI as SYSCLK source. */
  RCC_SYSCLK_SOURCE_PLL          = 0x01UL,    /*!< PLL as SYSCLK source. */
#if defined (DPM32M08x)
  RCC_SYSCLK_SOURCE_HSCLK        = 0x02UL     /*!< HSCLK as SYSCLK source. */
#endif /* DPM32M08x */
} RCC_SYSCLKSourceEnum;

/**
 ******************************************************************************
 * @brief CCT time base initialization structure definition.
 *****************************************************************************/
typedef struct
{
  RCC_PLLStateEnum                PLLState;         /*!< The new state of the LSI.
                                                         This parameter can be a value of @ref RCC_PLLStateEnum */
  RCC_PLLSourceEnum               PLLSource;        /*!< The source of the PLL clock.
                                                         This parameter can be a value of @ref RCC_PLLSourceEnum .*/
  RCC_PLLMulEnum                  PLLMUL;           /*!< Multiplication factor for PLL input clock.
                                                         This parameter must be a value of @ref RCC_PLLMulEnum.*/
} RCC_PLLInitTypeStruct;

#endif /* DPM32M08x || DPM32M05x */

/**
 ******************************************************************************
 * @brief Oscillator type enumeration.
 *****************************************************************************/
typedef enum
{
  RCC_OSCILLATOR_TYPE_NONE          = 0x00UL,   /*!< None clock configured. */

#if defined (DPM32M08x)
  RCC_OSCILLATOR_TYPE_HSE           = 0x01UL,   /*!< HSE configured. */
#endif /* DPM32M08x */

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
  RCC_OSCILLATOR_TYPE_LSE           = 0x02UL,   /*!< LSE configured. */
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */

  RCC_OSCILLATOR_TYPE_LSI           = 0x04UL    /*!< LSI configured. */
} RCC_OscillatorTypeEnum;

/**
 ******************************************************************************
 * @brief Clock type enumeration.
 *****************************************************************************/
typedef enum
{
  RCC_CLOCK_TYPE_NONE           = 0x0000UL,   /*!< None clock configured. */

#if defined (DPM32M08x)
  RCC_CLOCK_TYPE_HSCLK          = 0x0001UL,   /*!< HSCLK to configure. */
#endif /* DPM32M08x */

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
  RCC_CLOCK_TYPE_LSCLK          = 0x0002UL,   /*!< LSCLK to configure. */
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */

#if defined (DPM32M08x) || defined (DPM32M05x)
  RCC_CLOCK_TYPE_SYSCLK         = 0x0004UL,   /*!< SYSCLK to configure. */
#endif /* DPM32M08x || DPM32M05x */

  RCC_CLOCK_TYPE_HCLK           = 0x0008UL,   /*!< HCLK to configure. */
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
  RCC_CLOCK_TYPE_PCLK           = 0x0010UL    /*!< PCLK to configure. */
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */
} RCC_ClockTypeEnum;

#if defined (DPM32M08x)
/**
 ******************************************************************************
 * @brief HSE state enumeration.
 *****************************************************************************/
typedef enum
{
  RCC_HSE_OFF           = 0x00UL,  /*!< HSE deactivation. */
  RCC_HSE_ON                       /*!< HSE activation. */
} RCC_HSEStateEnum;

/**
 ******************************************************************************
 * @brief HSCLK source enumeration.
 *****************************************************************************/
typedef enum
{
  RCC_HSCLK_SOURCE_HSI          = 0x00UL,  /*!< HSE as HSCLK source. */
  RCC_HSCLK_SOURCE_HSE                     /*!< HSI as HSCKL source. */
} RCC_HSCLKSourceEnum;
#endif /* DPM32M08x */

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
/**
 ******************************************************************************
 * @brief LSE state enumeration.
 *****************************************************************************/
typedef enum
{
  RCC_LSE_OFF           = 0x00UL,  /*!< LSE deactivation. */
  RCC_LSE_ON                       /*!< LSE activation. */
} RCC_LSEStateEnum;

/**
 ******************************************************************************
 * @brief LSCLK source enumeration.
 *****************************************************************************/
typedef enum
{
  RCC_LSCLK_SOURCE_LSI          = 0x00UL,                       /*!< LSI as LSCLK source. */
  RCC_LSCLK_SOURCE_LSE                                          /*!< LSE as LSCKL source. */
} RCC_LSCLKSourceEnum;
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */

/**
 ******************************************************************************
 * @brief LSI state enumeration.
 *****************************************************************************/
typedef enum
{
  RCC_LSI_OFF           = 0x00UL,  /*!< LSI deactivation. */
  RCC_LSI_ON                       /*!< LSI activation. */
} RCC_LSIStateEnum;

/**
 ******************************************************************************
 * @brief HCLK source enumeration.
 *****************************************************************************/
typedef enum
{
  RCC_SYSCLK_DIV1                 = 0x00UL,   /*!< SYSCLK divider 1 as HCLK source. */
  RCC_SYSCLK_DIV2                 = 0x01UL,   /*!< SYSCLK divider 2 as HCLK source. */
  RCC_SYSCLK_DIV4                 = 0x02UL,   /*!< SYSCLK divider 4 as HCLK source. */
  RCC_SYSCLK_DIV8                 = 0x03UL,   /*!< SYSCLK divider 8 as HCLK source. */
  RCC_SYSCLK_DIV16                = 0x04UL,   /*!< SYSCLK divider 16 as HCLK source. */
  RCC_SYSCLK_DIV32                = 0x05UL,   /*!< SYSCLK divider 32 as HCLK source. */
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
  RCC_SYSCLK_DIV64                = 0x06UL,   /*!< SYSCLK divider 64 as HCLK source. */
  RCC_SYSCLK_DIV128               = 0x07UL    /*!< SYSCLK divider 128 as HCLK source. */
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */
} RCC_HCLKDividerEnum;

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
/**
 ******************************************************************************
 * @brief PCLK source enumeration.
 *****************************************************************************/
typedef enum
{
  RCC_HCLK_DIV1                 = 0x00UL,   /*!< HCLK divider 1 as PCLK source. */
  RCC_HCLK_DIV2                 = 0x01UL,   /*!< HCLK divider 2 as PCLK source. */
  RCC_HCLK_DIV4                 = 0x02UL,   /*!< HCLK divider 4 as PCLK source. */
  RCC_HCLK_DIV8                 = 0x03UL,   /*!< HCLK divider 8 as PCLK source. */
  RCC_HCLK_DIV16                = 0x04UL,   /*!< HCLK divider 16 as PCLK source. */
  RCC_HCLK_DIV32                = 0x05UL,   /*!< HCLK divider 32 as PCLK source. */
  RCC_HCLK_DIV64                = 0x06UL,   /*!< HCLK divider 64 as PCLK source. */
  RCC_HCLK_DIV128               = 0x07UL    /*!< HCLK divider 128 as PCLK source. */
} RCC_PCLKDividerEnum;
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */

/**
 ******************************************************************************
 * @brief MCO source enumeration.
 *****************************************************************************/
typedef enum
{
#if defined(DPM32M026) || defined (DPM32T55x)
  RCC_MCO_SOURCE_HSI_DIV4       = 0x00UL,    /*!< HSI/4 as MCO source. */
#else /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */  
  RCC_MCO_SOURCE_HSI_DIV6       = 0x00UL,    /*!< HSI/6 as MCO source. */
#endif /* DPM32M026 || DPM32T55x */

#if defined (DPM32M08x)
  RCC_MCO_SOURCE_HSE_DIV        = 0x01UL,    /*!< HSE as MCO source. */
#endif /* DPM32M08x */

#if defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015) || \
    defined (DPM32M026) || defined (DPM32T55x)   
  RCC_MCO_SOURCE_LSI            = 0x01UL,    /*!< LSI as MCO source. */
#else  /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M026 || DPM32T55x */
  RCC_MCO_SOURCE_LSI            = 0x02UL,    /*!< LSI as MCO source. */
#endif /* DPM32M036 || DPM32M030 || DPM32M015 */

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
  RCC_MCO_SOURCE_LSE            = 0x03UL,    /*!< LSE as MCO source. */
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */

#if defined (DPM32M08x) || defined (DPM32M05x)
  RCC_MCO_SOURCE_PLL_DIV6       = 0x04UL,    /*!< PLL/6 as MCO source. */
  RCC_MCO_SOURCE_PLL_DIV12      = 0x05UL     /*!< PLL/12 as MCO source. */
#endif /* DPM32M08x || DPM32M05x  */
} RCC_MCOSourceEnum;

/**
 ******************************************************************************
 * @brief AHB peripheral enumeration.
 *****************************************************************************/
typedef enum
{
  RCC_AHB_PERIPH_GPIO          = RCC_CLKEN_CR_GPIO_Msk,       /*!< Specifies the AHB peripheral GPIO clock. */

#if defined (DPM32M08x)
  RCC_AHB_PERIPH_DSP           = RCC_CLKEN_CR_DSP_Msk         /*!< Specifies the AHB peripheral DSP clock. */
#endif /* DPM32M08x */
  
#if defined (DPM32M05x) || defined (DPM32M03x) || defined (DPM32M036) 
  RCC_AHB_PERIPH_MATH_ACC      = RCC_CLKEN_CR_MATH_ACC_Msk    /*!< Specifies the AHB peripheral MATH_ACC clock. */
#endif /* DPM32M05x || DPM32M03x || DPM32M036 */
} RCC_AHBPeriphClockEnum;

/**
 ******************************************************************************
 * @brief APB peripheral enumeration.
 *****************************************************************************/
typedef enum
{
  RCC_APB_PERIPH_UART0         = RCC_CLKEN_CR_UART0_Msk,   /*!< Specifies the APB peripheral UART0 clock. */
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) || \
    defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
  RCC_APB_PERIPH_UART1         = RCC_CLKEN_CR_UART1_Msk,   /*!< Specifies the APB peripheral UART1 clock. */
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */
#if defined (DPM32M08x) || defined (DPM32M05x)
  RCC_APB_PERIPH_UART2         = RCC_CLKEN_CR_UART2_Msk,   /*!< Specifies the APB peripheral UART2 clock. */
#endif /* DPM32M08x || DPM32M05x  */
  RCC_APB_PERIPH_SPI0          = RCC_CLKEN_CR_SPI0_Msk,    /*!< Specifies the APB peripheral SPI0 clock. */
#if defined (DPM32M08x) || defined (DPM32M05x)
  RCC_APB_PERIPH_SPI1          = RCC_CLKEN_CR_SPI1_Msk,    /*!< Specifies the APB peripheral SPI1 clock. */
#endif /* DPM32M08x || DPM32M05x  */
  RCC_APB_PERIPH_TIM0          = RCC_CLKEN_CR_TIM0_Msk,    /*!< Specifies the APB peripheral TIM0 clock. */
  RCC_APB_PERIPH_TIM1          = RCC_CLKEN_CR_TIM1_Msk,    /*!< Specifies the APB peripheral TIM1 clock. */
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) || \
    defined (DPM32M036) 
  RCC_APB_PERIPH_TIM2          = RCC_CLKEN_CR_TIM2_Msk,    /*!< Specifies the APB peripheral TIM2 clock. */
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 */
#if defined (DPM32M08x) || defined (DPM32M05x)
  RCC_APB_PERIPH_TIM3          = RCC_CLKEN_CR_TIM3_Msk,    /*!< Specifies the APB peripheral TIM3 clock. */
#endif /* DPM32M08x || DPM32M05x */
  RCC_APB_PERIPH_CCT0          = RCC_CLKEN_CR_CCT0_Msk,    /*!< Specifies the APB peripheral CCT0 clock. */
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
  RCC_APB_PERIPH_CCT1          = RCC_CLKEN_CR_CCT1_Msk,    /*!< Specifies the APB peripheral CCT1 clock. */
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */
  RCC_APB_PERIPH_EPWM          = RCC_CLKEN_CR_EPWM_Msk,    /*!< Specifies the APB peripheral EPWM clock. */
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) || \
    defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
  RCC_APB_PERIPH_PVD           = RCC_CLKEN_CR_PVD_Msk,     /*!< Specifies the APB peripheral PVD clock. */
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */
  RCC_APB_PERIPH_ADC           = RCC_CLKEN_CR_ADC_Msk,     /*!< Specifies the APB peripheral ADC clock. */
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
  RCC_APB_PERIPH_DAC           = RCC_CLKEN_CR_DAC_Msk,     /*!< Specifies the APB peripheral DAC clock. */
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) || \
    defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
  RCC_APB_PERIPH_I2C           = RCC_CLKEN_CR_I2C_Msk,     /*!< Specifies the APB peripheral I2C clock. */
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
  RCC_APB_PERIPH_POSIF         = RCC_CLKEN_CR_POSIF_Msk,   /*!< Specifies the APB peripheral POSIF clock. */
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */
#if defined (DPM32M08x)
  RCC_APB_PERIPH_CAN           = RCC_CLKEN_CR_CAN_Msk,     /*!< Specifies the APB peripheral CAN clock. */
#endif  /* DPM32M08x */
  RCC_APB_PERIPH_ACMP          = RCC_CLKEN_CR_ACMP_Msk,    /*!< Specifies the APB peripheral ACMP clock. */
#if defined (DPM32M08x) || defined (DPM32M05x)
  RCC_APB_PERIPH_TIM4          = RCC_CLKEN_CR_TIM4_Msk,    /*!< Specifies the APB peripheral TIM4 clock. */
  RCC_APB_PERIPH_TIM5          = RCC_CLKEN_CR_TIM5_Msk,    /*!< Specifies the APB peripheral TIM5 clock. */
  RCC_APB_PERIPH_DMA           = RCC_CLKEN_CR_DMA_Msk,     /*!< Specifies the APB peripheral DMA clock. */
#endif /* DPM32M08x || DPM32M05x */  
#if defined (DPM32T55x)
  RCC_APB_PERIPH_CRC           = RCC_CLKEN_CR_CRC_Msk,      /*!< Specifies the APB peripheral CRC clock. */
  RCC_APB_PERIPH_TSI           = RCC_CLKEN_CR_TSI_Msk         /*!< Specifies the APB peripheral TSI clock. */
#else
  RCC_APB_PERIPH_CRC           = RCC_CLKEN_CR_CRC_Msk       /*!< Specifies the APB peripheral CRC clock. */
#endif /* DPM32T55x */
} RCC_APBPeriphClockEnum;

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
/**
 ******************************************************************************
 * @brief Sleep mode peripheral enumeration.
 *****************************************************************************/
typedef enum
{
  RCC_DEEP_SLEEP_PERIPH_UART0         = RCC_DEEP_SLEEP_CR_UART0_Msk,   /*!< Specifies the deep sleep mode peripheral UART0 clock. */
  RCC_DEEP_SLEEP_PERIPH_UART1         = RCC_DEEP_SLEEP_CR_UART1_Msk,   /*!< Specifies the deep sleep mode peripheral UART1 clock. */
#if defined (DPM32M08x) || defined (DPM32M05x)
  RCC_DEEP_SLEEP_PERIPH_UART2         = RCC_DEEP_SLEEP_CR_UART2_Msk,   /*!< Specifies the deep sleep mode peripheral UART2 clock. */
#endif /* DPM32M08x || DPM32M05x  */
  RCC_DEEP_SLEEP_PERIPH_SPI0          = RCC_DEEP_SLEEP_CR_SPI0_Msk,    /*!< Specifies the deep sleep mode peripheral SPI0 clock. */
#if defined (DPM32M08x) || defined (DPM32M05x)
  RCC_DEEP_SLEEP_PERIPH_SPI1          = RCC_DEEP_SLEEP_CR_SPI1_Msk,    /*!< Specifies the deep sleep mode peripheral SPI1 clock. */
#endif /* DPM32M08x || DPM32M05x  */
  RCC_DEEP_SLEEP_PERIPH_TIM0          = RCC_DEEP_SLEEP_CR_TIM0_Msk,    /*!< Specifies the deep sleep mode peripheral TIM0 clock. */
  RCC_DEEP_SLEEP_PERIPH_TIM1          = RCC_DEEP_SLEEP_CR_TIM1_Msk,    /*!< Specifies the deep sleep mode peripheral TIM1 clock. */
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) || defined (DPM32M036) 
  RCC_DEEP_SLEEP_PERIPH_TIM2          = RCC_DEEP_SLEEP_CR_TIM2_Msk,    /*!< Specifies the deep sleep mode peripheral TIM2 clock. */
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 */
#if defined (DPM32M08x) || defined (DPM32M05x)
  RCC_DEEP_SLEEP_PERIPH_TIM3          = RCC_DEEP_SLEEP_CR_TIM3_Msk,    /*!< Specifies the deep sleep mode peripheral TIM3 clock. */
#endif /* DPM32M08x || DPM32M05x  */
  RCC_DEEP_SLEEP_PERIPH_CCT0          = RCC_DEEP_SLEEP_CR_CCT0_Msk,    /*!< Specifies the deep sleep mode peripheral CCT0 clock. */
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
  RCC_DEEP_SLEEP_PERIPH_CCT1          = RCC_DEEP_SLEEP_CR_CCT1_Msk,    /*!< Specifies the deep sleep mode peripheral CCT1 clock. */
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */
  RCC_DEEP_SLEEP_PERIPH_EPWM          = RCC_DEEP_SLEEP_CR_EPWM_Msk,    /*!< Specifies the deep sleep mode peripheral EPWM clock. */
  RCC_DEEP_SLEEP_PERIPH_PVD           = RCC_DEEP_SLEEP_CR_PVD_Msk,     /*!< Specifies the deep sleep mode peripheral PVD clock. */
  RCC_DEEP_SLEEP_PERIPH_ADC           = RCC_DEEP_SLEEP_CR_ADC_Msk,     /*!< Specifies the deep sleep mode peripheral ADC clock. */
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
  RCC_DEEP_SLEEP_PERIPH_DAC           = RCC_DEEP_SLEEP_CR_DAC_Msk,     /*!< Specifies the deep sleep mode peripheral DAC clock. */
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */
  RCC_DEEP_SLEEP_PERIPH_I2C           = RCC_DEEP_SLEEP_CR_I2C_Msk,     /*!< Specifies the deep sleep mode peripheral I2C clock. */
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
  RCC_DEEP_SLEEP_PERIPH_POSIF         = RCC_DEEP_SLEEP_CR_POSIF_Msk,   /*!< Specifies the deep sleep mode peripheral POSIF clock. */
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */

#if defined (DPM32M08x)
  RCC_DEEP_SLEEP_PERIPH_CAN           = RCC_DEEP_SLEEP_CR_CAN_Msk,     /*!< Specifies the deep sleep mode peripheral CAN clock. */
#endif  /* DPM32M08x */
  RCC_DEEP_SLEEP_PERIPH_ACMP          = RCC_DEEP_SLEEP_CR_ACMP_Msk,    /*!< Specifies the deep sleep mode peripheral ACMP clock. */
#if defined (DPM32M08x) || defined (DPM32M05x)
  RCC_DEEP_SLEEP_PERIPH_TIM4          = RCC_DEEP_SLEEP_CR_TIM4_Msk,    /*!< Specifies the deep sleep mode peripheral TIM4 clock. */
  RCC_DEEP_SLEEP_PERIPH_TIM5          = RCC_DEEP_SLEEP_CR_TIM5_Msk,    /*!< Specifies the deep sleep mode peripheral TIM5 clock. */
  RCC_DEEP_SLEEP_PERIPH_DMA           = RCC_DEEP_SLEEP_CR_DMA_Msk,     /*!< Specifies the deep sleep mode peripheral DMA clock. */
#endif /* DPM32M08x || DPM32M05x */
  RCC_DEEP_SLEEP_PERIPH_CRC           = RCC_DEEP_SLEEP_CR_CRC_Msk,     /*!< Specifies the deep sleeup mode peripheral CRC clock. */
  RCC_DEEP_SLEEP_PERIPH_GPIO          = RCC_DEEP_SLEEP_CR_GPIO_Msk,    /*!< Specifies the deep sleep mode peripheral GPIO clock. */
#if defined (DPM32M08x)
  RCC_DEEP_SLEEP_PERIPH_DSP           = RCC_DEEP_SLEEP_CR_DSP_Msk      /*!< Specifies the deep sleep mode peripheral DSP clock. */
#endif /* DPM32M08x */
#if defined (DPM32M05x) || defined (DPM32M03x) || defined (DPM32M036)
  RCC_DEEP_SLEEP_PERIPH_MATH_ACC      = RCC_DEEP_SLEEP_CR_MATH_ACC_Msk /*!< Specifies the deep sleep mode peripheral MATH_ACC clock. */
#endif /* DPM32M05x || DPM32M03x || DPM32M036 */
} RCC_DeepSleepPeriphClockEnum;
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */

/**
 ******************************************************************************
 * @brief In stop mode, functions enumeration.
 *****************************************************************************/
typedef enum
{
  RCC_STOP_FUNC_LSCLK           = 0x01UL,     /*!< LSCLK functions. */
} RCC_StopFuncEnum;

/**
 ******************************************************************************
 * @brief Low power mode type enumeration.
 *****************************************************************************/
typedef enum
{
  RCC_LOW_POWER_MODE_SLEEP        = 0x00UL,  /*!< Sleep low power mode. */
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
  RCC_LOW_POWER_MODE_DEEP_SLEEP   = 0x01UL,  /*!< Deep sleep low power mode. */
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */
  RCC_LOW_POWER_MODE_STOP         = 0x02UL,  /*!< Stop low power mode. */
} RCC_LowPowerModeEnum;

/**
 ******************************************************************************
 * @brief Enter low power mode way enumeration.
 *****************************************************************************/
typedef enum
{
  RCC_LOW_POWER_ENTRY_WFI    = 0x00UL,     /*!< Enter low power mode with WFI instruction. */
  RCC_LOW_POWER_ENTRY_WFE    = 0x01UL      /*!< Enter low power mode with WFE instruction. */
} RCC_LowPowerEntryEnum;

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
/**
 ******************************************************************************
 * @brief System strat up address remap enumeration.
 *****************************************************************************/
typedef enum
{
  RCC_STARTUP_ADDR_REMAP_FLASH     = 0x00UL,   /*!< System strat up address from flash. */
  RCC_STARTUP_ADDR_REMAP_SRAM      = 0x01UL,   /*!< System strat up address from SRAM. */
  RCC_STARTUP_ADDR_REMAP_BOOTROM   = 0x02UL    /*!< System strat up address from boot rom0. */
} RCC_StartupAddrRemapEnum;
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */


#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
/**
 ******************************************************************************
 * @brief External clock start up time wait cycle enumeration.
 *****************************************************************************/
typedef enum
{
  RCC_EX_CLK_STARTUP_TIME_1024       = 0x00UL,  /*!< External clock start up wait 1024 cycle. */
  RCC_EX_CLK_STARTUP_TIME_4096,                 /*!< External clock start up wait 4096 cycle. */
  RCC_EX_CLK_STARTUP_TIME_16384,                /*!< External clock start up wait 16384 cycle. */
  RCC_EX_CLK_STARTUP_TIME_0                     /*!< External clock start up wait 0 cycle. */
} RCC_ExCLKStartupTimeEnum;
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
/**
 ******************************************************************************
 * @brief Internal clock start up time wait cycle enumeration.
 *****************************************************************************/
typedef enum
{
  RCC_IN_CLK_STARTUP_TIME_4       = 0x00UL,  /*!< Internal clock start up wait 4 cycle. */
  RCC_IN_CLK_STARTUP_TIME_16,                /*!< Internal clock start up wait 16 cycle. */
  RCC_IN_CLK_STARTUP_TIME_64,                /*!< Internal clock start up wait 64 cycle. */
  RCC_IN_CLK_STARTUP_TIME_256                /*!< Internal clock start up wait 256 cycle. */
} RCC_InCLKStartupTimeEnum;
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */

/**
 ******************************************************************************
 * @brief System reset source enumeration.
 *****************************************************************************/
typedef enum
{
  RCC_RST_SOURCE_POR_PDR        = RCC_RESET_SR_POR_Msk,       /*!< Power on or power off reset. */
  RCC_RST_SOURCE_BUTTON         = RCC_RESET_SR_BUTTON_Msk,    /*!< Button reset. */
  RCC_RST_SOURCE_SOFTWARE       = RCC_RESET_SR_SW_Msk,        /*!< Software reset. */
  RCC_RST_SOURCE_LOCKUP         = RCC_RESET_SR_LOCKUP_Msk,    /*!< Lock up reset. */
#if defined (DPM32M08x)
  RCC_RST_SOURCE_DSP            = RCC_RESET_SR_DSP_Msk,       /*!< DSP reset. */
#endif /* DPM32M08x */
  RCC_RST_SOURCE_WDG            = RCC_RESET_SR_WDG_Msk,       /*!< Watchdog reset. */
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
  RCC_RST_SOURCE_PVD            = RCC_RESET_SR_PVD_Msk,       /*!< PVD reset. */
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */
  RCC_RST_SOURCE_DSTB_DET       = RCC_RESET_SR_DSTB_DET_Msk   /*!< DSTB_DET reset. */
} RCC_RSTSourceEnum;

/**
 ******************************************************************************
 * @brief RCC Internal/External Oscillator (LSE and LSI) configuration structure definition.
 *****************************************************************************/
typedef struct
{
  uint32_t                        RCC_OscillatorType;       /*!< The oscillators to be configured.
                                                                 This parameter can be a value of @ref RCC_OscillatorTypeEnum .*/

#if defined (DPM32M08x)
  RCC_HSEStateEnum                RCC_HSEState;             /*!< The new state of the HSE.
                                                                 This parameter can be a value of @ref RCC_HSEStateEnum */
#endif /* DPM32M08x */

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
  RCC_LSEStateEnum                RCC_LSEState;             /*!< The new state of the LSE.
                                                                 This parameter can be a value of @ref RCC_LSEStateEnum */
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */

  RCC_LSIStateEnum                RCC_LSIState;             /*!< The new state of the LSI.
                                                                 This parameter can be a value of @ref RCC_LSIStateEnum */

#if defined (DPM32M08x) || defined (DPM32M05x)
  RCC_PLLInitTypeStruct           RCC_PLL;                  /*!< PLL structure parameters */
#endif /* DPM32M08x || DPM32M05x */
} RCC_OscInitTypeStruct;

/**
 ******************************************************************************
 * @brief RCC System, AHB and APB busses clock configuration configuration structure definition.
 *****************************************************************************/
typedef struct
{
  uint32_t                        RCC_ClockType;            /*!< The clock to be configured.
                                                                 This parameter can be a value of @ref RCC_ClockTypeEnum */

#if defined (DPM32M08x)
  RCC_HSCLKSourceEnum             HSCLKSource;              /*!< The clock source (HSCLK).
                                                                 This parameter can be a value of @ref RCC_HSCLKSourceEnum */
#endif /* DPM32M08x */

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
  RCC_LSCLKSourceEnum             LSCLKSource;              /*!< The clock source (LSCLK).
                                                                 This parameter can be a value of @ref RCC_LSCLKSourceEnum */
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */

#if defined (DPM32M08x) || defined (DPM32M05x)
  RCC_SYSCLKSourceEnum            SYSCLKSource;             /*!< The clock source (SYSCLKS) used as system clock.
                                                                 This parameter can be a value of @ref RCC_SYSCLKSourceEnum */
#endif /* DPM32M08x || DPM32M05x */

  RCC_HCLKDividerEnum             HCLKDivider;              /*!< The AHB clock (HCLK) divider. This clock is derived from the system clock (SYSCLK).
                                                                 This parameter can be a value of @ref RCC_HCLKDividerEnum */
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
  RCC_PCLKDividerEnum             PCLKDivider;              /*!< The APB1 clock (PCLK1) divider. This clock is derived from the AHB clock (HCLK).
                                                                 This parameter can be a value of @ref RCC_PCLKDividerEnum */
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */
} RCC_ClkInitTypeStruct;

/**
 *******************************************************************************
 * @brief  Enable or disable the High Speed AHB (AHB) peripheral clock.
 * @note   After reset, the peripheral clock (used for registers read/write access)
 *         is disabled and the application software has to enable this clock before
 *         using it.
 ******************************************************************************/
#if defined (DPM32M08x)
#define RCC_DSP_CLK_DISABLE()      (RCC->CLKEN_CR &= ~RCC_CLKEN_CR_DSP_Msk)
#endif /* DPM32M08x */

#if defined (DPM32M05x) || defined (DPM32M03x) || defined (DPM32M036)
#define RCC_MATH_ACC_CLK_DISABLE() (RCC->CLKEN_CR &= ~RCC_CLKEN_CR_MATH_ACC_Msk)
#endif /* DPM32M05x || DPM32M03x || DPM32M036 */
#define RCC_GPIO_CLK_DISABLE()     (RCC->CLKEN_CR &= ~RCC_CLKEN_CR_GPIO_Msk)

#if defined (DPM32M08x)
#define RCC_DSP_CLK_ENABLE()      (RCC->CLKEN_CR |= RCC_CLKEN_CR_DSP_Msk)
#endif /* DPM32M08x */

#if defined (DPM32M05x) || defined (DPM32M03x) || defined (DPM32M036)
#define RCC_MATH_ACC_CLK_ENABLE() (RCC->CLKEN_CR |= RCC_CLKEN_CR_MATH_ACC_Msk)
#endif /* DPM32M05x || DPM32M03x || DPM32M036 */
#define RCC_GPIO_CLK_ENABLE()     (RCC->CLKEN_CR |= RCC_CLKEN_CR_GPIO_Msk)
/**
 *******************************************************************************
 * @brief  Enable or disable the Low Speed APB (APB) peripheral clock.
 * @note   After reset, the peripheral clock (used for registers read/write access)
 *         is disabled and the application software has to enable this clock before
 *         using it.
 ******************************************************************************/
#define RCC_UART0_CLK_DISABLE()   (RCC->CLKEN_CR &= ~RCC_CLKEN_CR_UART0_Msk)
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
#define RCC_UART1_CLK_DISABLE()   (RCC->CLKEN_CR &= ~RCC_CLKEN_CR_UART1_Msk)
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */
#if defined (DPM32M08x) || defined (DPM32M05x)
#define RCC_UART2_CLK_DISABLE()   (RCC->CLKEN_CR &= ~RCC_CLKEN_CR_UART2_Msk)
#endif /* DPM32M08x || DPM32M05x  */
#define RCC_SPI0_CLK_DISABLE()    (RCC->CLKEN_CR &= ~RCC_CLKEN_CR_SPI0_Msk)
#if defined (DPM32M08x) || defined (DPM32M05x)
#define RCC_SPI1_CLK_DISABLE()    (RCC->CLKEN_CR &= ~RCC_CLKEN_CR_SPI1_Msk)
#endif /* DPM32M08x || DPM32M05x  */
#define RCC_TIM0_CLK_DISABLE()    (RCC->CLKEN_CR &= ~RCC_CLKEN_CR_TIM0_Msk)
#define RCC_TIM1_CLK_DISABLE()    (RCC->CLKEN_CR &= ~RCC_CLKEN_CR_TIM1_Msk)
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) || defined (DPM32M036) 
#define RCC_TIM2_CLK_DISABLE()    (RCC->CLKEN_CR &= ~RCC_CLKEN_CR_TIM2_Msk)
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 */
#if defined (DPM32M08x) || defined (DPM32M05x)
#define RCC_TIM3_CLK_DISABLE()    (RCC->CLKEN_CR &= ~RCC_CLKEN_CR_TIM3_Msk)
#endif /* DPM32M08x || DPM32M05x  */
#define RCC_CCT0_CLK_DISABLE()    (RCC->CLKEN_CR &= ~RCC_CLKEN_CR_CCT0_Msk)
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
#define RCC_CCT1_CLK_DISABLE()    (RCC->CLKEN_CR &= ~RCC_CLKEN_CR_CCT1_Msk)
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */
#define RCC_EPWM_CLK_DISABLE()    (RCC->CLKEN_CR &= ~RCC_CLKEN_CR_EPWM_Msk)
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
#define RCC_PVD_CLK_DISABLE()     (RCC->CLKEN_CR &= ~RCC_CLKEN_CR_PVD_Msk)
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */
#define RCC_ADC_CLK_DISABLE()     (RCC->CLKEN_CR &= ~RCC_CLKEN_CR_ADC_Msk)
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
#define RCC_DAC_CLK_DISABLE()     (RCC->CLKEN_CR &= ~RCC_CLKEN_CR_DAC_Msk)
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
#define RCC_I2C_CLK_DISABLE()     (RCC->CLKEN_CR &= ~RCC_CLKEN_CR_I2C_Msk)
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
#define RCC_POSIF_CLK_DISABLE()   (RCC->CLKEN_CR &= ~RCC_CLKEN_CR_POSIF_Msk)
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */
#if defined (DPM32M08x)
#define RCC_CAN_CLK_DISABLE()     (RCC->CLKEN_CR &= ~RCC_CLKEN_CR_CAN_Msk)
#endif  /* DPM32M08x */
#define RCC_ACMP_CLK_DISABLE()    (RCC->CLKEN_CR &= ~RCC_CLKEN_CR_ACMP_Msk)
#if defined (DPM32M08x) || defined (DPM32M05x)
#define RCC_TIM4_CLK_DISABLE()    (RCC->CLKEN_CR &= ~RCC_CLKEN_CR_TIM4_Msk)
#define RCC_TIM5_CLK_DISABLE()    (RCC->CLKEN_CR &= ~RCC_CLKEN_CR_TIM5_Msk)
#define RCC_DMA_CLK_DISABLE()     (RCC->CLKEN_CR &= ~RCC_CLKEN_CR_DMA_Msk)
#endif /* DPM32M08x || DPM32M05x */
#define RCC_CRC_CLK_DISABLE()     (RCC->CLKEN_CR &= ~RCC_CLKEN_CR_CRC_Msk)
#if defined (DPM32T55x)
#define RCC_TSI_CLK_DISABLE()     (RCC->CLKEN_CR &= ~RCC_CLKEN_CR_TSI_Msk)
#define RCC_TSI_CLK_ENABLE()      (RCC->CLKEN_CR |=  RCC_CLKEN_CR_TSI_Msk)
#endif /* DPM32T55x */
#define RCC_UART0_CLK_ENABLE()    (RCC->CLKEN_CR |=  RCC_CLKEN_CR_UART0_Msk)
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
#define RCC_UART1_CLK_ENABLE()    (RCC->CLKEN_CR |=  RCC_CLKEN_CR_UART1_Msk)
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */
#if defined (DPM32M08x) || defined (DPM32M05x)
#define RCC_UART2_CLK_ENABLE()    (RCC->CLKEN_CR |=  RCC_CLKEN_CR_UART2_Msk)
#endif /* DPM32M08x || DPM32M05x  */
#define RCC_SPI0_CLK_ENABLE()     (RCC->CLKEN_CR |=  RCC_CLKEN_CR_SPI0_Msk)
#if defined (DPM32M08x) || defined (DPM32M05x)
#define RCC_SPI1_CLK_ENABLE()     (RCC->CLKEN_CR |=  RCC_CLKEN_CR_SPI1_Msk)
#endif /* DPM32M08x || DPM32M05x  */
#define RCC_TIM0_CLK_ENABLE()     (RCC->CLKEN_CR |=  RCC_CLKEN_CR_TIM0_Msk)
#define RCC_TIM1_CLK_ENABLE()     (RCC->CLKEN_CR |=  RCC_CLKEN_CR_TIM1_Msk)
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) || defined (DPM32M036) 
#define RCC_TIM2_CLK_ENABLE()     (RCC->CLKEN_CR |=  RCC_CLKEN_CR_TIM2_Msk)
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 */
#if defined (DPM32M08x) || defined (DPM32M05x)
#define RCC_TIM3_CLK_ENABLE()     (RCC->CLKEN_CR |=  RCC_CLKEN_CR_TIM3_Msk)
#endif /* DPM32M08x || DPM32M05x  */
#define RCC_CCT0_CLK_ENABLE()     (RCC->CLKEN_CR |=  RCC_CLKEN_CR_CCT0_Msk)
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
#define RCC_CCT1_CLK_ENABLE()     (RCC->CLKEN_CR |=  RCC_CLKEN_CR_CCT1_Msk)
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */
#define RCC_EPWM_CLK_ENABLE()     (RCC->CLKEN_CR |=  RCC_CLKEN_CR_EPWM_Msk)
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
#define RCC_PVD_CLK_ENABLE()      (RCC->CLKEN_CR |=  RCC_CLKEN_CR_PVD_Msk)
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */
#define RCC_ADC_CLK_ENABLE()      (RCC->CLKEN_CR |=  RCC_CLKEN_CR_ADC_Msk)
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
#define RCC_DAC_CLK_ENABLE()      (RCC->CLKEN_CR |=  RCC_CLKEN_CR_DAC_Msk)
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
#define RCC_I2C_CLK_ENABLE()      (RCC->CLKEN_CR |=  RCC_CLKEN_CR_I2C_Msk)
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
#define RCC_POSIF_CLK_ENABLE()    (RCC->CLKEN_CR |=  RCC_CLKEN_CR_POSIF_Msk)
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */
#if defined (DPM32M08x)
#define RCC_CAN_CLK_ENABLE()      (RCC->CLKEN_CR |=  RCC_CLKEN_CR_CAN_Msk)
#endif  /* DPM32M08x */
#define RCC_ACMP_CLK_ENABLE()     (RCC->CLKEN_CR |=  RCC_CLKEN_CR_ACMP_Msk)
#if defined (DPM32M08x) || defined (DPM32M05x)
#define RCC_TIM4_CLK_ENABLE()     (RCC->CLKEN_CR |=  RCC_CLKEN_CR_TIM4_Msk)
#define RCC_TIM5_CLK_ENABLE()     (RCC->CLKEN_CR |=  RCC_CLKEN_CR_TIM5_Msk)
#define RCC_DMA_CLK_ENABLE()      (RCC->CLKEN_CR |=  RCC_CLKEN_CR_DMA_Msk)
#endif /* DPM32M08x || DPM32M05x */
#define RCC_CRC_CLK_ENABLE()      (RCC->CLKEN_CR |=  RCC_CLKEN_CR_CRC_Msk)



/* Initialization and de-initialization functions. */
void                    RCC_Unlock(void);
void                    RCC_Lock(void);
void                    RCC_DeInit(void);
RCC_StatusEnum          RCC_OscConfig(RCC_OscInitTypeStruct * RCC_OscInitType);
void                    RCC_OscStructInit(RCC_OscInitTypeStruct * RCC_OscInitType);
RCC_StatusEnum          RCC_ClockConfig(RCC_ClkInitTypeStruct  *RCC_ClkInitType);
void                    RCC_ClockStructInit(RCC_ClkInitTypeStruct * RCC_ClkInitType);
FlagState               RCC_GetClockFlag(RCC_ClockFlagEnum RCC_Clockflag);
RCC_StatusEnum          RCC_LSICmd(FunctionalState NewState);

/* Clock start up time functions. */
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
void                    RCC_SetHSIStartUpTime(RCC_InCLKStartupTimeEnum RCC_StartTime);
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
void                    RCC_SetLSIStartUpTime(RCC_InCLKStartupTimeEnum RCC_StartTime);
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */

#if defined (DPM32M08x)
void                    RCC_SetHSEStartUpTime(RCC_ExCLKStartupTimeEnum RCC_StartTime);
#endif /* DPM32M08x */

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
void                    RCC_SetLSEStartUpTime(RCC_ExCLKStartupTimeEnum RCC_StartTime);
RCC_StatusEnum          RCC_LSECmd(FunctionalState NewState);
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */

/* MCO configuration functions. */
void                    RCC_MCOConfig(RCC_MCOSourceEnum RCC_Source);
void                    RCC_MCODisable(void);
void                    RCC_MCOEnable(void);

/* Peripheral clocks configuration functions. */
void                    RCC_AHBPeriphClockCmd(RCC_AHBPeriphClockEnum RCC_AHBClk, FunctionalState NewState);
void                    RCC_APBPeriphClockCmd(RCC_APBPeriphClockEnum RCC_APBClk, FunctionalState NewState);

/* Get clock frequency functions. */
uint32_t                RCC_GetSYSCLKFrequency(void);
uint32_t                RCC_GetHCLKFrequency(void);
uint32_t                RCC_GetPCLKFrequency(void);

#if defined (DPM32M08x) || defined (DPM32M05x)
uint32_t                RCC_GetPLLFrequency(void);
#endif  /* DPM32M08x || DPM32M05x */
#if defined (DPM32M08x)
uint32_t                RCC_GetHSCLKFrequency(void);
#endif  /* DPM32M08x */

/* Reset and low power mode functions. */
RCC_RSTSourceEnum       RCC_GetSysResetSource(void);
void                    RCC_ClearSysResetSourceFlag(void);
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
void                    RCC_DeepSleepModePeriphCmd(RCC_DeepSleepPeriphClockEnum RCC_DeepSleepClk, FunctionalState NewState);
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */
void                    RCC_StopModeFunctionCmd(RCC_StopFuncEnum RCC_StopFunc, FunctionalState NewState);
void                    RCC_EnterLowPowerMode(RCC_LowPowerModeEnum RCC_LowPowerType, RCC_LowPowerEntryEnum RCC_LowPowerEntry);

/* Start up addr remap and lock up reset functions. */
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x)
void                    RCC_SysStartupAddrRemap(RCC_StartupAddrRemapEnum RCC_StartAddr);
#endif /* DPM32M08x || DPM32M05x || DPM32M03x */

#if defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015) || \
    defined (DPM32M026) || defined (DPM32T55x)  
void                    RCC_SetVectorTableAddr(uint32_t RCC_VectorAddr);
#endif /* DPM32M036 || DPM32M030 || DPM32M015 || DPM32M026 || DPM32T55x */

void                    RCC_LockupResetCmd(FunctionalState NewState);


#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}

#endif

#endif /* __DPM32M0XX_RCC_H__ */
