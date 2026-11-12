/**
  ******************************************************************************
  * @file      startup_dpm32m030.s
  * @brief     DPM32M030 vector table and reset handler for the GNU toolchain.
  *            This module performs:
  *                - Set the initial SP and PC through the vector table.
  *                - Define the Cortex-M0 exception and peripheral IRQ vectors.
  *                - Copy the .data initializers from Flash to SRAM.
  *                - Clear the .bss section.
  *                - Call SystemInit(), then enter main().
  *
  *            The vector order follows the DPM32M030 SDK startup file.
  *            Stack and section addresses are defined in the linker script.
  *            Interrupts remain disabled for the polling bootloader.
  ******************************************************************************
  */

  .syntax unified
  .cpu cortex-m0
  .thumb

/******************************************************************************
 * Cortex-M0 exception and DPM32M030 peripheral interrupt vectors.
 * The linker places this section at the start of the current firmware image.
 *****************************************************************************/
  .section .isr_vector,"a",%progbits
  .global __Vectors
  .type __Vectors, %object
__Vectors:
  .word _estack                    /* Initial stack pointer                */
  .word Reset_Handler              /* Reset handler                        */
  .word NMI_Handler                /* Non-maskable interrupt               */
  .word HardFault_Handler          /* Hard fault                           */
  .word 0                          /* Reserved                             */
  .word 0                          /* Reserved                             */
  .word 0                          /* Reserved                             */
  .word 0                          /* Reserved                             */
  .word 0                          /* Reserved                             */
  .word 0                          /* Reserved                             */
  .word 0                          /* Reserved                             */
  .word SVC_Handler                /* Supervisor call                      */
  .word 0                          /* Reserved                             */
  .word 0                          /* Reserved                             */
  .word PendSV_Handler             /* Pendable service request             */
  .word SysTick_Handler            /* System tick timer                    */

  /* External interrupts */
  .word WDG_IRQHandler             /* IRQ  0: Watchdog                     */
  .word PVD_IRQHandler             /* IRQ  1: Programmable voltage detector */
  .word TIM0_IRQHandler            /* IRQ  2: TIM0                         */
  .word TIM1_IRQHandler            /* IRQ  3: TIM1                         */
  .word 0                          /* IRQ  4: Reserved                     */
  .word 0                          /* IRQ  5: Reserved                     */
  .word CCT0_IRQHandler            /* IRQ  6: Capture/compare timer        */
  .word 0                          /* IRQ  7: Reserved                     */
  .word EPWM_IRQHandler            /* IRQ  8: Enhanced PWM                 */
  .word 0                          /* IRQ  9: Reserved                     */
  .word 0                          /* IRQ 10: Reserved                     */
  .word 0                          /* IRQ 11: Reserved                     */
  .word UART0_IRQHandler           /* IRQ 12: UART0                        */
  .word UART1_IRQHandler           /* IRQ 13: UART1                        */
  .word 0                          /* IRQ 14: Reserved                     */
  .word SPI0_IRQHandler            /* IRQ 15: SPI0                         */
  .word 0                          /* IRQ 16: Reserved                     */
  .word I2C_IRQHandler             /* IRQ 17: I2C                          */
  .word 0                          /* IRQ 18: Reserved                     */
  .word 0                          /* IRQ 19: Reserved                     */
  .word LPTIM_IRQHandler           /* IRQ 20: Low-power timer              */
  .word ADC0_IRQHandler            /* IRQ 21: ADC0                         */
  .word 0                          /* IRQ 22: Reserved                     */
  .word GPIOA_IRQHandler           /* IRQ 23: GPIO port A                  */
  .word GPIOB_IRQHandler           /* IRQ 24: GPIO port B                  */
  .word GPIOD_IRQHandler           /* IRQ 25: GPIO port D                  */
  .word 0                          /* IRQ 26: Reserved                     */
  .word 0                          /* IRQ 27: Reserved                     */
  .word ACMP_IRQHandler            /* IRQ 28: Analog comparators           */
  .word 0                          /* IRQ 29: Reserved                     */
  .word 0                          /* IRQ 30: Reserved                     */
  .word 0                          /* IRQ 31: Reserved                     */
  .size __Vectors, .-__Vectors

/**
 * @brief  Initialize the C data sections and enter the bootloader or updater.
 * @param  None
 * @retval None
 */
  .section .text.Reset_Handler,"ax",%progbits
  .global Reset_Handler
  .type Reset_Handler, %function
  .thumb_func
Reset_Handler:
  cpsid i

  /* Copy the .data initializers from Flash to SRAM. */
  ldr   r0, =_sidata
  ldr   r1, =_sdata
  ldr   r2, =_edata
LoopCopyDataInit:
  cmp   r1, r2
  bcs   InitBss
  ldr   r3, [r0]
  str   r3, [r1]
  adds  r0, #4
  adds  r1, #4
  b     LoopCopyDataInit

  /* Clear the .bss section in SRAM. */
InitBss:
  ldr   r1, =_sbss
  ldr   r2, =_ebss
  movs  r3, #0
LoopFillZeroBss:
  cmp   r1, r2
  bcs   StartMain
  str   r3, [r1]
  adds  r1, #4
  b     LoopFillZeroBss

  /* Initialize the vector mapping, then call the C entry point. */
StartMain:
  bl    SystemInit
  bl    main

LoopForever:
  b     LoopForever
  .size Reset_Handler, .-Reset_Handler

/**
 * @brief  Preserve the system state after an unexpected exception or interrupt.
 * @param  None
 * @retval None
 */
  .section .text.Default_Handler,"ax",%progbits
  .type Default_Handler, %function
  .thumb_func
Default_Handler:
  b     .

/******************************************************************************
 * Weak aliases allow an application-provided handler to override the default.
 *****************************************************************************/
  .weak      NMI_Handler
  .thumb_set NMI_Handler, Default_Handler

  .weak      HardFault_Handler
  .thumb_set HardFault_Handler, Default_Handler

  .weak      SVC_Handler
  .thumb_set SVC_Handler, Default_Handler

  .weak      PendSV_Handler
  .thumb_set PendSV_Handler, Default_Handler

  .weak      SysTick_Handler
  .thumb_set SysTick_Handler, Default_Handler

  .weak      WDG_IRQHandler
  .thumb_set WDG_IRQHandler, Default_Handler

  .weak      PVD_IRQHandler
  .thumb_set PVD_IRQHandler, Default_Handler

  .weak      TIM0_IRQHandler
  .thumb_set TIM0_IRQHandler, Default_Handler

  .weak      TIM1_IRQHandler
  .thumb_set TIM1_IRQHandler, Default_Handler

  .weak      CCT0_IRQHandler
  .thumb_set CCT0_IRQHandler, Default_Handler

  .weak      EPWM_IRQHandler
  .thumb_set EPWM_IRQHandler, Default_Handler

  .weak      UART0_IRQHandler
  .thumb_set UART0_IRQHandler, Default_Handler

  .weak      UART1_IRQHandler
  .thumb_set UART1_IRQHandler, Default_Handler

  .weak      SPI0_IRQHandler
  .thumb_set SPI0_IRQHandler, Default_Handler

  .weak      I2C_IRQHandler
  .thumb_set I2C_IRQHandler, Default_Handler

  .weak      LPTIM_IRQHandler
  .thumb_set LPTIM_IRQHandler, Default_Handler

  .weak      ADC0_IRQHandler
  .thumb_set ADC0_IRQHandler, Default_Handler

  .weak      GPIOA_IRQHandler
  .thumb_set GPIOA_IRQHandler, Default_Handler

  .weak      GPIOB_IRQHandler
  .thumb_set GPIOB_IRQHandler, Default_Handler

  .weak      GPIOD_IRQHandler
  .thumb_set GPIOD_IRQHandler, Default_Handler

  .weak      ACMP_IRQHandler
  .thumb_set ACMP_IRQHandler, Default_Handler

  .section .note.GNU-stack,"",%progbits
