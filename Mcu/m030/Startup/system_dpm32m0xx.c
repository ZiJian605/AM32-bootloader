#include "main.h"

uint32_t SystemCoreClock = M030_CLOCK_HZ;
extern const uint32_t __Vectors[];

void SystemInit(void)
{
  RCC_Unlock();
  RCC_SetVectorTableAddr((uint32_t)__Vectors >> RCC_SYSTEM_CR_VTOR_Pos);
  RCC_Lock();
  __DSB();
  __ISB();
}

void SystemCoreClockUpdate(void)
{
  SystemCoreClock = RCC_GetHCLKFrequency();
}
