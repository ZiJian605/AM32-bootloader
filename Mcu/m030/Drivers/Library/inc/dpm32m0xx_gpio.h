/**
 *******************************************************************************
 * @file    dpm32m0xx_gpio.h
 *
 * @brief   Header file for GPIO firmware driver.
 *
 * @author  DPM
 *
 * @version V1.0.0
 *
 * @date    2025-05-20
 *
 *******************************************************************************/

#ifndef __DPM32M0XX_GPIO_H__
#define __DPM32M0XX_GPIO_H__


#ifdef __cplusplus
extern "C" {
#endif

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

#include "dpm32m0xx.h"


/*
 ******************************************************************************
 * @brief GPIO mode enumeration.
 *****************************************************************************/
typedef enum
{
  GPIO_MODE_INPUT                 = 0x00UL,   /*!< Input mode.              */
  GPIO_MODE_OUTPUT                = 0x01UL,   /*!< Output mode.             */
  GPIO_MODE_ALT_FUNC              = 0x02UL,   /*!< Alternate function Mode. */
  GPIO_MODE_ANALOG                = 0x03UL    /*!< Analog in/out Mode.      */
} GPIO_ModeEnum;

/**
 ******************************************************************************
 * @brief GPIO pull enumeration.
 *****************************************************************************/
typedef enum
{
  GPIO_PUPD_NOPULL                = 0x00UL,   /*!< No pull-up or pull-down. */
  GPIO_PUPD_UP                    = 0x01UL,   /*!< Pull-up.                 */
  GPIO_PUPD_DOWN                  = 0x02UL    /*!< Pull-down.               */
} GPIO_PuPdEnum;

/**
 ******************************************************************************
 * @brief GPIO output type enumeration.
 *****************************************************************************/
typedef enum
{
  GPIO_OTYPE_PP                   = 0x00UL,   /*!< Push pull output type.   */
  GPIO_OTYPE_OD                   = 0x01UL    /*!< Open drain output type.  */
} GPIO_OTypeEnum;

/**
 ******************************************************************************
 * @brief GPIO output speed enumeration.
 *****************************************************************************/
typedef enum
{
  GPIO_SPEED_LOW                  = 0x00UL,   /*!< Output low speed.        */
  GPIO_SPEED_MIDDLE               = 0x01UL,   /*!< Output middle speed.     */
  GPIO_SPEED_HIGH                 = 0x02UL,   /*!< Output high speed.       */
  GPIO_SPEED_MAX                  = 0x03UL    /*!< Output max speed.        */
} GPIO_SpeedEnum;

/**
 ******************************************************************************
 * @brief GPIO alternate function enumeration.
 *****************************************************************************/
typedef enum
{
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
  GPIO_ALT_FUNC_0                 = 0x00UL,   /*!< Alternate function 0 selection. */
  GPIO_ALT_FUNC_1                 = 0x01UL,   /*!< Alternate function 1 selection. */
  GPIO_ALT_FUNC_2                 = 0x02UL,   /*!< Alternate function 2 selection. */
  GPIO_ALT_FUNC_3                 = 0x03UL,   /*!< Alternate function 3 selection. */
  GPIO_ALT_FUNC_4                 = 0x04UL,   /*!< Alternate function 4 selection. */
  GPIO_ALT_FUNC_5                 = 0x05UL,   /*!< Alternate function 5 selection. */
  GPIO_ALT_FUNC_6                 = 0x06UL,   /*!< Alternate function 6 selection. */
  GPIO_ALT_FUNC_7                 = 0x07UL    /*!< Alternate function 7 selection. */
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */
  
#if defined (DPM32M026) || defined (DPM32T55x)
  GPIO_ALT_FUNC_EPWM_CH0_P        = 0x00UL,   /*!< Alternate function select epwm ch0 postive channel. */
  GPIO_ALT_FUNC_EPWM_CH0_N        = 0x01UL,   /*!< Alternate function select epwm ch0 negtive channel. */
  GPIO_ALT_FUNC_EPWM_CH1_P        = 0x02UL,   /*!< Alternate function select epwm ch1 postive channel. */
  GPIO_ALT_FUNC_EPWM_CH1_N        = 0x03UL,   /*!< Alternate function select epwm ch1 negtive channel. */
  GPIO_ALT_FUNC_EPWM_CH2_P        = 0x04UL,   /*!< Alternate function select epwm ch2 postive channel. */
  GPIO_ALT_FUNC_EPWM_CH2_N        = 0x05UL,   /*!< Alternate function select epwm ch2 negtive channel. */
  GPIO_ALT_FUNC_TIM0_PWM          = 0x06UL,   /*!< Alternate function select tim0 pwm out. */
  GPIO_ALT_FUNC_TIM1_PWM          = 0x07UL,   /*!< Alternate function select tim1 pwm out. */
  GPIO_ALT_FUNC_SPI_SCK           = 0x08UL,   /*!< Alternate function select spi clock. */
  GPIO_ALT_FUNC_SPI_MOSI          = 0x09UL,   /*!< Alternate function select spi mosi. */
  GPIO_ALT_FUNC_SPI_MISO          = 0x0AUL,   /*!< Alternate function select spi miso. */
  GPIO_ALT_FUNC_CCT0_CH0          = 0x0CUL,   /*!< Alternate function select cct0 channel 0. */
  GPIO_ALT_FUNC_CCT0_CH1          = 0x0DUL,   /*!< Alternate function select cct0 channel 1. */
  GPIO_ALT_FUNC_CCT0_CH2          = 0x0EUL,   /*!< Alternate function select cct0 channel 2. */
  GPIO_ALT_FUNC_SPI_NSS           = 0x13UL,   /*!< Alternate function select spi nss. */
  GPIO_ALT_FUNC_SWCLK             = 0x14UL,   /*!< Alternate function select swclk. */
  GPIO_ALT_FUNC_ADC_TRIGGER       = 0x15UL,   /*!< Alternate function select adc trigger. */
  GPIO_ALT_FUNC_UART0_TX          = 0x18UL,   /*!< Alternate function select uart0 tx. */
  GPIO_ALT_FUNC_SWDIO             = 0x19UL,   /*!< Alternate function select swdio. */
  GPIO_ALT_FUNC_SYS_DBG           = 0x1AUL,   /*!< Alternate function select system debug. */
  GPIO_ALT_FUNC_UART0_RX          = 0x1CUL,   /*!< Alternate function select uart0 rx. */
  GPIO_ALT_FUNC_BUTTON_RSTN       = 0x1FUL,   /*!< Alternate function select button reset, PD9 exclusive. */
#endif /* DPM32M026 || DPM32T55x */
} GPIO_AltFuncEnum;

#if defined (DPM32M026) || defined (DPM32T55x)
/**
 ******************************************************************************
 * @brief GPIO alternate function enumeration.
 *****************************************************************************/
typedef enum
{
  GPIO_ALT_SYS_DBG_MCO            = 0x00UL,   /*!< Alternate function select system debug, mco function. */
  GPIO_ALT_SYS_DBG_EPMW_ADC_TRIG  = 0x01UL,   /*!< Alternate function select system debug, epwm adc triggrt function. */
  GPIO_ALT_SYS_DBG_ACMP0_OUT      = 0x02UL,   /*!< Alternate function select system debug, acmp0 output function. */
  GPIO_ALT_SYS_DBG_ACMP1_OUT      = 0x03UL,   /*!< Alternate function select system debug, acmp1 output function. */
} GPIO_AltSysDbgEnum;
#endif /* DPM32M026 || DPM32T55x */


/**
 ******************************************************************************
 * @brief GPIO analog alternate function enumeration.
 *****************************************************************************/
typedef enum
{
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
  GPIO_ANA_AF_0                   = 0x00UL,   /*!< Analog alternate function 0 selection. */
  GPIO_ANA_AF_1                   = 0x01UL    /*!< Analog alternate function 1 selection. */
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */
  
#if defined (DPM32M026) || defined (DPM32T55x)
  GPIO_ANA_AF_ADC                 = 0x00UL,   /*!< Analog alternate function select adc. */
  GPIO_ANA_AF_TOUCH               = 0x01UL,   /*!< Analog alternate function select touch. */
  GPIO_ANA_AF_PGA                 = 0x02UL,   /*!< Analog alternate function select pga. */
  GPIO_ANA_AF_ACMP                = 0x03UL,   /*!< Analog alternate function select acmp. */
#endif /* DPM32M026 || DPM32T55x */
} GPIO_AnaAltFuncEnum;

/**
 ******************************************************************************
 * @brief GPIO pin index enumeration.
 *****************************************************************************/
typedef enum
{
  GPIO_PIN_0                      = 0x0001UL,   /*!< Pin index 0 of each port group.  */
  GPIO_PIN_1                      = 0x0002UL,   /*!< Pin index 1 of each port group.  */
  GPIO_PIN_2                      = 0x0004UL,   /*!< Pin index 2 of each port group.  */
  GPIO_PIN_3                      = 0x0008UL,   /*!< Pin index 3 of each port group.  */
  GPIO_PIN_4                      = 0x0010UL,   /*!< Pin index 4 of each port group.  */
  GPIO_PIN_5                      = 0x0020UL,   /*!< Pin index 5 of each port group.  */
  GPIO_PIN_6                      = 0x0040UL,   /*!< Pin index 6 of each port group.  */
  GPIO_PIN_7                      = 0x0080UL,   /*!< Pin index 7 of each port group.  */
  GPIO_PIN_8                      = 0x0100UL,   /*!< Pin index 8 of each port group.  */
  GPIO_PIN_9                      = 0x0200UL,   /*!< Pin index 9 of each port group.  */
  GPIO_PIN_10                     = 0x0400UL,   /*!< Pin index 10 of each port group. */
  GPIO_PIN_11                     = 0x0800UL,   /*!< Pin index 11 of each port group. */
  GPIO_PIN_12                     = 0x1000UL,   /*!< Pin index 12 of each port group. */
  GPIO_PIN_13                     = 0x2000UL,   /*!< Pin index 13 of each port group. */
  GPIO_PIN_14                     = 0x4000UL,   /*!< Pin index 14 of each port group. */
  GPIO_PIN_15                     = 0x8000UL,   /*!< Pin index 15 of each port group. */
  GPIO_PIN_ALL                    = 0xFFFFUL    /*!< All pins selected                */
} GPIO_PinEnum;

/**
 ******************************************************************************
 * @brief GPIO bit set and bit reset enumeration
 *****************************************************************************/
typedef enum
{
  BIT_RESET                       = 0,          /*!< bit reset */
  BIT_SET                         = !BIT_RESET  /*!< bit set   */
} GPIO_BitState;

/**
 ******************************************************************************
 * @brief GPIO interrupt type enumeration.
 *****************************************************************************/
typedef enum
{
#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
  GPIO_INT_RISING                 = 0x00UL,   /*!< interrupt mode with rising edge trigger         */
  GPIO_INT_FALLING                = 0x01UL,   /*!< interrupt mode with falling edge trigger        */
  GPIO_INT_HIGH                   = 0x02UL,   /*!< interrupt mode with high level edge trigger     */
  GPIO_INT_LOW                    = 0x03UL,   /*!< interrupt mode with low level edge trigger      */
  GPIO_INT_RISING_FALLING         = 0x04UL    /*!< interrupt mode with rising/falling edge trigger */
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */
  
#if defined (DPM32M026) || defined (DPM32T55x)
  GPIO_INT_DISABLE                = 0x00UL,   /*!< diable interrupt         */
  GPIO_INT_RISING                 = 0x01UL,   /*!< interrupt mode with rising edge trigger         */
  GPIO_INT_FALLING                = 0x02UL,   /*!< interrupt mode with falling edge trigger        */
  GPIO_INT_RISING_FALLING         = 0x03UL    /*!< interrupt mode with rising/falling edge trigger */
#endif /* DPM32M026 || DPM32T55x */
} GPIO_IntTypeEnum;

/**
 ******************************************************************************
 * @brief GPIO initialization structure definition.
 *****************************************************************************/
typedef struct
{
  uint32_t          GPIO_Pin;          /*!< Specifies the GPIO pins to be configured.
                                            This parameter can be any combination of @ref GPIO_PinEnum. */
  GPIO_ModeEnum     GPIO_Mode;         /*!< Specifies the mode for the selected pins.
                                            This parameter can be a value of @ref GPIO_ModeEnum.        */
  GPIO_OTypeEnum    GPIO_OType;        /*!< Specifies the output type for the selected pins.
                                            This parameter can be a value of @ref GPIO_OTypeEnum.       */
  GPIO_SpeedEnum    GPIO_Speed;        /*!< Specifies the output speed for the selected pins.
                                            This parameter can be a value of @ref GPIO_SpeedEnum.       */
  GPIO_PuPdEnum     GPIO_PuPd;         /*!< Specifies the pull for the selected pins.
                                            This parameter can be a value of @ref GPIO_PuPdEnum.        */
} GPIO_InitTypeStruct;



/* GPIO param check. */

#define IS_GPIO_PIN(x) ((x) != (uint16_t)0x00)


#if defined (DPM32M08x) || defined (DPM32M05x)

#define IS_GPIO_PORT(x)            (((x) == GPIOA) || ((x) == GPIOB) || ((x) == GPIOC) || ((x) == GPIOD))

#else

#define IS_GPIO_PORT(x)            (((x) == GPIOA) || ((x) == GPIOB) || ((x) == GPIOD))

#endif /* DPM32M08x || DPM32M05x */



#define IS_GPIO_MODE(x)            (((x) == GPIO_MODE_INPUT) || \
                                    ((x) == GPIO_MODE_OUTPUT) || \
                                    ((x) == GPIO_MODE_ALT_FUNC) || \
                                    ((x) == GPIO_MODE_ANALOG))


#define IS_GPIO_PUPD(x)            (((x) == GPIO_PUPD_NOPULL) || \
                                    ((x) == GPIO_PUPD_UP) || \
                                    ((x) == GPIO_PUPD_DOWN))

#define IS_GPIO_OUTPUT_TYPE(x)     (((x) == GPIO_OTYPE_PP) || \
                                    ((x) == GPIO_OTYPE_OD))

#define IS_GPIO_SPEED(x)           (((x) == GPIO_SPEED_LOW) || \
                                    ((x) == GPIO_SPEED_MIDDLE) || \
                                    ((x) == GPIO_SPEED_HIGH) || \
                                    ((x) == GPIO_SPEED_MAX))



/* GPIO configuration values. */

#define GPIO_MODE_MASK(n)               (0x3U << (2U * (n)))
#define GPIO_MODE_SET(n, mode)          ((uint32_t)((uint32_t)(mode) << (2U * (n))))

#define GPIO_PUPD_MASK(n)               (0x3U << (2U * (n)))
#define GPIO_PUPD_SET(n, pull)          ((uint32_t)((uint32_t)(pull) << (2U * (n))))

#define GPIO_SPEED_MASK(n)              (0x3U << (2U * (n)))
#define GPIO_SPEED_SET(n, speed)        ((uint32_t)((uint32_t)(speed) << (2U * (n))))

#define GPIO_OTYPE_MASK(n)              (0x1U << (n))
#define GPIO_OTYPE_SET(n, OType)        ((uint32_t)((uint32_t)(OType) << (n)))

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
#define GPIO_AF_MASK(n)                 (0xFU << (4U * (n)))
#define GPIO_AF_SET(n, af)              ((uint32_t)((uint32_t)(af) << (4U * (n))))
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */

#if defined (DPM32M026) || defined (DPM32T55x)
#define GPIO_AF_MASK(n)                 (0x1FU << (8U * (n)))
#define GPIO_AF_SET(n, af)              ((uint32_t)((uint32_t)(af) << (8U * (n))))
#endif /* DPM32M026 || DPM32T55x */

#define GPIO_ANA_AF_MASK(n)             (0x3UL << (2UL * (n)))
#define GPIO_ANA_AF_SET(n, ana)         ((uint32_t)((uint32_t)(ana) << (2UL * (n))))

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
#define GPIO_INT_TYPE_MASK(n)           (0x7U << (4U * (n)))
#define GPIO_INT_TYPE_SET(n, intType)   ((uint32_t)((uint32_t)(intType) << (4U * (n))))

#define GPIO_INT_EN_MASK(n)             (0x1U << (n))
#define GPIO_INT_EN_SET(n, en)          ((uint32_t)((uint32_t)(en) << (n)))
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */

#if defined (DPM32M026) || defined (DPM32T55x)
#define GPIO_INT_TYPE_MASK(n)           (0x3U << (2U * (n)))
#define GPIO_INT_TYPE_SET(n, intType)   ((uint32_t)((uint32_t)(intType) << (2U * (n))))
#endif /* DPM32M026 || DPM32T55x */



/* Deinitializes and initialization and configuration functions. */
void                    GPIO_DeInit(GPIO_Type *GPIOx);
void                    GPIO_Init(GPIO_Type *GPIOx, GPIO_InitTypeStruct* GPIO_InitType);
void                    GPIO_StructInit(GPIO_InitTypeStruct* GPIO_InitType);

/* Unlock and lock the GPIO configuration register functions. */
void                    GPIO_Lock(GPIO_Type *GPIOx);
void                    GPIO_Unlock(GPIO_Type *GPIOx);

/* GPIO bit operation functions. */
void                    GPIO_SetBit(GPIO_Type *GPIOx, uint16_t GPIO_Pin);
void                    GPIO_ResetBit(GPIO_Type *GPIOx, uint16_t GPIO_Pin);
void                    GPIO_ToggleBit(GPIO_Type *GPIOx, uint16_t GPIO_Pin);
void                    GPIO_WriteBit(GPIO_Type *GPIOx, uint16_t GPIO_Pin, GPIO_BitState PinValue);

/* GPIO port operation functions. */
void                    GPIO_SetPort(GPIO_Type* GPIOx);
void                    GPIO_ResetPort(GPIO_Type* GPIOx);
void                    GPIO_TogglePort(GPIO_Type* GPIOx);
void                    GPIO_WritePort(GPIO_Type *GPIOx, uint16_t GPIO_Value);

/* GPIO port and bit get functions. */
uint8_t                 GPIO_ReadInputBit(GPIO_Type *GPIOx, uint16_t GPIO_Pin);
uint16_t                GPIO_ReadInputPort(GPIO_Type *GPIOx);
uint8_t                 GPIO_ReadOutputBit(GPIO_Type *GPIOx, uint16_t GPIO_Pin);
uint16_t                GPIO_ReadOutputPort(GPIO_Type *GPIOx);

/* GPIO alternate function configuration functions. */
void                    GPIO_AltFuncConfig(GPIO_Type* GPIOx, uint16_t GPIO_Pin, uint32_t GPIO_AltFunc);

#if defined (DPM32M026) || defined (DPM32T55x)
void                    GPIO_SysDbgConfig(GPIO_Type* GPIOx, GPIO_AltSysDbgEnum GPIO_AltSysDbg);
#endif /* DPM32M026 || DPM32T55x */

/* GPIO analog alternate function configuration functions. */
void                    GPIO_AnaAFConfig(GPIO_Type* GPIOx, uint16_t GPIO_Pin, GPIO_AnaAltFuncEnum GPIO_AnaAF);

/* GPIO interrupt operation functions. */
void                    GPIO_IntConfig(GPIO_Type* GPIOx, uint16_t GPIO_Pin, GPIO_IntTypeEnum GPIO_IntType);

#if defined (DPM32M08x) || defined (DPM32M05x) || defined (DPM32M03x) \
 || defined (DPM32M036) || defined (DPM32M030) || defined (DPM32M015)
void                    GPIO_IntEnable(GPIO_Type* GPIOx, uint16_t GPIO_Pin);
void                    GPIO_IntDisable(GPIO_Type* GPIOx, uint16_t GPIO_Pin);
void                    GPIO_IntCmd(GPIO_Type* GPIOx, uint16_t GPIO_Pin, FunctionalState NewState);
#endif /* DPM32M08x || DPM32M05x || DPM32M03x || DPM32M036 || DPM32M030 || DPM32M015 */

/* GPIO interrupt status get and clear functions. */
FlagState               GPIO_GetIntFlagStatus(GPIO_Type* GPIOx, uint16_t GPIO_Pin);
void                    GPIO_ClearIntFlag(GPIO_Type* GPIOx, uint16_t GPIO_Pin);

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}

#endif

#endif /* __DPM32M0XX_GPIO_H__ */
