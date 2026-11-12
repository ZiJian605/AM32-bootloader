/* DPM32M030F6N7 bootloader utilities, following the F051 port interface. */
#pragma once

#include "main.h"
#include "board.h"

#if !defined(USE_PD0)
#error "The M030 board uses PD0 for bootloader communication"
#endif

#define GPIO_PIN(n) (1U << (n))
#define GPIO_PULL_NONE GPIO_PUPD_NOPULL
#define GPIO_PULL_UP GPIO_PUPD_UP
#define GPIO_PULL_DOWN GPIO_PUPD_DOWN
#define GPIO_OUTPUT_PUSH_PULL GPIO_OTYPE_PP

static inline void gpio_mode_set_input(uint32_t pin, uint32_t pull)
{
  (void)pin; 
  input_port->MODE &= ~GPIO_MODE_MODE0_Msk;
  input_port->PUPD = (input_port->PUPD & ~GPIO_PUPD_PUPD0_Msk) | pull;
}

static inline void gpio_mode_set_output(uint32_t pin, uint32_t mode)
{
  (void)pin;
  (void)mode;
  input_port->OUT_TYPE &= ~1UL;
  input_port->PUPD &= ~GPIO_PUPD_PUPD0_Msk;
  input_port->MODE = (input_port->MODE & ~GPIO_MODE_MODE0_Msk) | GPIO_MODE_OUTPUT;
}

static inline void gpio_set(uint32_t pin) { input_port->BIT_SET_RST = pin; }
static inline void gpio_clear(uint32_t pin) { input_port->BIT_RST = pin; }
static inline bool gpio_read(uint32_t pin) { return (input_port->DATA_IN & pin) != 0; }

/* TIM0 counts down at 48 MHz. Its period is exactly 65536 microseconds,
* so the public 16-bit microsecond counter wraps without a discontinuity. */
#define M030_TIMER_RELOAD (48UL * 65536UL - 1UL)

static inline void bl_timer_init(void)
{
  RCC_Unlock();
  RCC_APBPeriphClockCmd(RCC_APB_PERIPH_TIM0, ENABLE);
  RCC_Lock();
  TIM0->CR = 0;
  TIM0->ARR = M030_TIMER_RELOAD;
  TIM0->SR = TIM_SR_IF_Msk;
  TIM0->CR = TIM_CR_EN_Msk;
}

static inline void bl_timer_disable(void) { TIM0->CR = 0; }

static inline uint16_t bl_timer_us(void)
{
  static const uint16_t remainder_offset[3] = {0, 0x5556, 0xAAAB};
  const uint32_t product = (TIM0->CNT >> 4) * 0xAAAAAAABUL;
  return (uint16_t)~(product - remainder_offset[product >> 30]);
}

static inline void bl_clock_config(void)
{
  FLASH_Unlock();
  FLASH_SetReadWaitCycle(FLASH_READ_WAIT_HCLK_CYCLE1);
  FLASH_Lock();
  RCC_ClkInitTypeStruct clock;
  RCC_ClockStructInit(&clock);
  clock.RCC_ClockType = RCC_CLOCK_TYPE_HCLK | RCC_CLOCK_TYPE_PCLK;
  clock.HCLKDivider = RCC_SYSCLK_DIV1;
  clock.PCLKDivider = RCC_HCLK_DIV1;
  RCC_Unlock();
  RCC_ClockConfig(&clock);
  RCC_Lock();
  SystemCoreClock = M030_CLOCK_HZ;
}

static inline void bl_gpio_init(void)
{
  RCC_Unlock();
  RCC_AHBPeriphClockCmd(RCC_AHB_PERIPH_GPIO, ENABLE);
  RCC_Lock();
  GPIO_Unlock(GPIOA);
  GPIO_Unlock(GPIOB);
  GPIO_Unlock(GPIOD);

  GPIO_InitTypeStruct gpio;
  GPIO_StructInit(&gpio);
  gpio.GPIO_Mode = GPIO_MODE_OUTPUT;
  gpio.GPIO_PuPd = GPIO_PUPD_DOWN;
  GPIO_ResetBit(GPIOA, PHASE_HIGH_GPIOA_PINS | PHASE_LOW_GPIOA_PINS);
  GPIO_ResetBit(GPIOB, PHASE_LOW_GPIOB_PINS);
  gpio.GPIO_Pin = PHASE_HIGH_GPIOA_PINS | PHASE_LOW_GPIOA_PINS;
  GPIO_Init(GPIOA, &gpio);
  gpio.GPIO_Pin = PHASE_LOW_GPIOB_PINS;
  GPIO_Init(GPIOB, &gpio);

  /* QFN20 pin 19 bonds PB5 to PD0. PB5 must not drive the wire. */
  gpio.GPIO_Pin = GPIO_PIN_5;
  gpio.GPIO_Mode = GPIO_MODE_ANALOG;
  gpio.GPIO_PuPd = GPIO_PUPD_NOPULL;
  GPIO_Init(GPIOB, &gpio);
  GPIO_IntDisable(GPIOB, GPIO_PIN_5);
  GPIOD->INT_EN &= ~GPIO_PIN_0;
  GPIOD->OUT_SPEED = (GPIOD->OUT_SPEED & ~GPIO_OUT_SPEED_OSPEED0_Msk) | GPIO_SPEED_HIGH;
  gpio_mode_set_input(input_pin, GPIO_PULL_UP);
}

static inline bool bl_was_software_reset(void)
{
  return (RCC->RESET_SR & RCC_RESET_SR_SW_Msk) != 0;
}

static inline void jump_to_application(void)
{
  const uint32_t *app = (const uint32_t *)M030_APP_START;
  const uint32_t sp = app[0];
  const uint32_t entry = app[1];
  if ((sp & 7UL) != 0 || sp < 0x20000000UL || sp > 0x20001000UL ||
      (entry & 1UL) == 0 || entry < M030_APP_START || entry >= M030_FLASH_END) {
    return;
  }

  __disable_irq();
  SysTick->CTRL = 0;
  NVIC->ICER[0] = 0xFFFFFFFFUL;
  NVIC->ICPR[0] = 0xFFFFFFFFUL;
  SCB->ICSR = SCB_ICSR_PENDSTCLR_Msk | SCB_ICSR_PENDSVCLR_Msk;
  bl_timer_disable();

  RCC_Unlock();
  RCC_SetVectorTableAddr(M030_APP_START >> RCC_SYSTEM_CR_VTOR_Pos);
  RCC_Lock();
  __DSB();
  __ISB();
  __asm volatile("msr msp, %0\n bx %1" : : "r"(sp), "r"(entry) : "memory");
  __builtin_unreachable();
}
