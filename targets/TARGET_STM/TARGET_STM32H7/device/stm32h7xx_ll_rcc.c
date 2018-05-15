/**
  ******************************************************************************
  * @file    stm32h7xx_ll_rcc.c
  * @author  MCD Application Team
  * @version $VERSION$
  * @date    $DATE$
  * @brief   RCC LL module driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_ll_rcc.h"
#include "stm32h7xx_ll_bus.h"

#if defined(USE_FULL_LL_DRIVER) // MBED patch

#ifdef  USE_FULL_ASSERT
  #include "stm32_assert.h"
#else
  #define assert_param(expr) ((void)0U)
#endif

/** @addtogroup STM32H7xx_LL_Driver
  * @{
  */

#if defined(RCC)

/** @addtogroup RCC_LL
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/** @addtogroup RCC_LL_Private_Macros
  * @{
  */
#define IS_LL_RCC_USART_CLKSOURCE(__VALUE__)  (((__VALUE__) == LL_RCC_USART16_CLKSOURCE) \
                                            || ((__VALUE__) == LL_RCC_USART234578_CLKSOURCE))


#define IS_LL_RCC_I2C_CLKSOURCE(__VALUE__)    (((__VALUE__) == LL_RCC_I2C123_CLKSOURCE) \
                                            || ((__VALUE__) == LL_RCC_I2C4_CLKSOURCE))

#define IS_LL_RCC_LPTIM_CLKSOURCE(__VALUE__)  (((__VALUE__) == LL_RCC_LPTIM1_CLKSOURCE)  \
                                            || ((__VALUE__) == LL_RCC_LPTIM2_CLKSOURCE)  \
                                            || ((__VALUE__) == LL_RCC_LPTIM345_CLKSOURCE))

#define IS_LL_RCC_SAI_CLKSOURCE(__VALUE__)    (((__VALUE__) == LL_RCC_SAI1_CLKSOURCE) \
                                            || ((__VALUE__) == LL_RCC_SAI23_CLKSOURCE) \
                                            || ((__VALUE__) == LL_RCC_SAI4A_CLKSOURCE) \
                                            || ((__VALUE__) == LL_RCC_SAI4B_CLKSOURCE))

#define IS_LL_RCC_SPI_CLKSOURCE(__VALUE__)    (((__VALUE__) == LL_RCC_SPI123_CLKSOURCE) \
                                            || ((__VALUE__) == LL_RCC_SPI45_CLKSOURCE) \
                                            || ((__VALUE__) == LL_RCC_SPI6_CLKSOURCE))

/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/
/** @defgroup RCC_LL_Private_Functions RCC Private functions
  * @{
  */
uint32_t RCC_GetSystemClockFreq(void);
uint32_t RCC_GetHCLKClockFreq(uint32_t SYSCLK_Frequency);
uint32_t RCC_GetPCLK1ClockFreq(uint32_t HCLK_Frequency);
uint32_t RCC_GetPCLK2ClockFreq(uint32_t HCLK_Frequency);
uint32_t RCC_GetPCLK3ClockFreq(uint32_t HCLK_Frequency);
uint32_t RCC_GetPCLK4ClockFreq(uint32_t HCLK_Frequency);

/**
  * @}
  */


/* Exported functions --------------------------------------------------------*/
/** @addtogroup RCC_LL_Exported_Functions
  * @{
  */

/** @addtogroup RCC_LL_EF_Init
  * @{
  */

/**
  * @brief  Resets the RCC clock configuration to the default reset state.
  * @note   The default reset state of the clock configuration is given below:
  *            - HSI ON and used as system clock source
  *            - HSE, PLL1, PLL2 and PLL3 OFF
  *            - AHB, APB Bus pre-scaler set to 1.
  *            - CSS, MCO1 and MCO2 OFF
  *            - All interrupts disabled
  * @note   This function doesn't modify the configuration of the
  *            - Peripheral clocks
  *            - LSI, LSE and RTC clocks
  * @retval None
  */
void LL_RCC_DeInit(void)
{
  /* Set HSION bit */
  SET_BIT(RCC->CR, RCC_CR_HSION);

  /* Wait for HSI READY bit */
  while(LL_RCC_HSI_IsReady() == 0U)
  {}

  /* Reset CFGR register */
  CLEAR_REG(RCC->CFGR);

  /* Reset CSION , CSIKERON, HSEON, HSI48ON, HSECSSON,HSIDIV, PLL1ON, PLL2ON, PLL3ON bits */
  CLEAR_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_HSIKERON| RCC_CR_HSIDIV| RCC_CR_HSIDIVF| RCC_CR_CSION | RCC_CR_CSIKERON |  RCC_CR_HSI48ON  \
  |RCC_CR_CSSHSEON | RCC_CR_PLL1ON | RCC_CR_PLL2ON | RCC_CR_PLL3ON);

  /* Wait for PLL1 READY bit to be reset */
  while(LL_RCC_PLL1_IsReady() != 0U)
  {}

  /* Wait for PLL2 READY bit to be reset */
  while(LL_RCC_PLL2_IsReady() != 0U)
  {}

  /* Wait for PLL3 READY bit to be reset */
  while(LL_RCC_PLL3_IsReady() != 0U)
  {}

  /* Reset D1CFGR register */
  CLEAR_REG(RCC->D1CFGR);

  /* Reset D2CFGR register */
  CLEAR_REG(RCC->D2CFGR);

  /* Reset D3CFGR register */
  CLEAR_REG(RCC->D3CFGR);

  /* Reset PLLCKSELR register */
  CLEAR_REG(RCC->PLLCKSELR);

  /* Reset PLLCFGR register */
  CLEAR_REG(RCC->PLLCFGR);

  /* Reset PLL1DIVR register */
  CLEAR_REG(RCC->PLL1DIVR);

  /* Reset PLL1FRACR register */
  CLEAR_REG(RCC->PLL1FRACR);

  /* Reset PLL2DIVR register */
  CLEAR_REG(RCC->PLL2DIVR);

  /* Reset PLL2FRACR register */
  CLEAR_REG(RCC->PLL2FRACR);

  /* Reset PLL3DIVR register */
  CLEAR_REG(RCC->PLL3DIVR);

  /* Reset PLL3FRACR register */
  CLEAR_REG(RCC->PLL3FRACR);

  /* Reset HSEBYP bit */
  CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);

  /* Disable all interrupts */
  CLEAR_REG(RCC->CIER);

  /* Clear all interrupts */
  SET_BIT(RCC->CICR, RCC_CICR_LSIRDYC | RCC_CICR_LSERDYC | RCC_CICR_HSIRDYC | RCC_CICR_HSERDYC
                   | RCC_CICR_CSIRDYC | RCC_CICR_HSI48RDYC | RCC_CICR_PLLRDYC | RCC_CICR_PLL2RDYC
                   | RCC_CICR_PLL3RDYC | RCC_CICR_LSECSSC | RCC_CICR_HSECSSC);

  /* Clear reset source flags */
  SET_BIT(RCC->RSR, RCC_RSR_RMVF);
}

/**
  * @}
  */

/** @addtogroup RCC_LL_EF_Get_Freq
  * @brief  Return the frequencies of different on chip clocks;  System, AHB, APB1, APB2, APB3 and APB4 buses clocks.
  *         and different peripheral clocks available on the device.
  * @note   If SYSCLK source is HSI, function returns values based on HSI_VALUE(*)
  * @note   If SYSCLK source is HSE, function returns values based on HSE_VALUE(**)
  * @note   If SYSCLK source is CSI, function returns values based on CSI_VALUE(***)
  * @note   If SYSCLK source is PLL, function returns values based on HSE_VALUE(**)
  *         or HSI_VALUE(*) multiplied/divided by the PLL factors.
  * @note   (*) HSI_VALUE is a constant defined in header file (default value
  *             64 MHz) divider by HSIDIV, but the real value may vary depending on
  *             on the variations in voltage and temperature.
  * @note   (**) HSE_VALUE is a constant defined in header file (default value
  *              25 MHz), user has to ensure that HSE_VALUE is same as the real
  *              frequency of the crystal used. Otherwise, this function may
  *              have wrong result.
  * @note   (***) CSI_VALUE is a constant defined in header file (default value
  *               4 MHz) but the real value may vary depending on the variations
  *               in voltage and temperature.
  * @note   The result of this function could be incorrect when using fractional
  *         value for HSE crystal.
  * @note   This function can be used by the user application to compute the
  *         baud-rate for the communication peripherals or configure other parameters.
  * @{
  */

/**
  * @brief  Return the frequencies of different on chip clocks;  System, AHB, APB1, APB2, APB3 and APB4 buses clocks.
  * @note   Each time SYSCLK, HCLK, PCLK1, PCLK2, PCLK3 and/or PCLK4 clock changes, this function
  *         must be called to update structure fields. Otherwise, any
  *         configuration based on this function will be incorrect.
  * @param  RCC_Clocks pointer to a @ref LL_RCC_ClocksTypeDef structure which will hold the clocks frequencies
  * @retval None
  */
void LL_RCC_GetSystemClocksFreq(LL_RCC_ClocksTypeDef *RCC_Clocks)
{
  /* Get SYSCLK frequency */
  RCC_Clocks->SYSCLK_Frequency = RCC_GetSystemClockFreq();

  /* HCLK clock frequency */
  RCC_Clocks->HCLK_Frequency   = RCC_GetHCLKClockFreq(RCC_Clocks->SYSCLK_Frequency);

  /* PCLK1 clock frequency */
  RCC_Clocks->PCLK1_Frequency  = RCC_GetPCLK1ClockFreq(RCC_Clocks->HCLK_Frequency);

  /* PCLK2 clock frequency */
  RCC_Clocks->PCLK2_Frequency  = RCC_GetPCLK2ClockFreq(RCC_Clocks->HCLK_Frequency);

  /* PCLK3 clock frequency */
  RCC_Clocks->PCLK3_Frequency  = RCC_GetPCLK3ClockFreq(RCC_Clocks->HCLK_Frequency);

  /* PCLK4 clock frequency */
  RCC_Clocks->PCLK4_Frequency  = RCC_GetPCLK4ClockFreq(RCC_Clocks->HCLK_Frequency);
}

/**
  * @brief  Return PLL1 clocks frequencies
  * @note   LL_RCC_PERIPH_FREQUENCY_NO returned for non activated output or oscillator not ready
  * @retval None
  */
void LL_RCC_GetPLL1ClockFreq(LL_PLL_ClocksTypeDef *PLL_Clocks)
{
  uint32_t pllinputfreq = LL_RCC_PERIPH_FREQUENCY_NO, pllsource;
  uint32_t m, n, fracn = 0U;

  /* PLL_VCO = (HSE_VALUE, CSI_VALUE or HSI_VALUE/HSIDIV) / PLLM * (PLLN + FRACN)
     SYSCLK = PLL_VCO / PLLP
  */
  pllsource = LL_RCC_PLL_GetSource();

  switch (pllsource)
  {
    case LL_RCC_PLLSOURCE_HSI:
      if (LL_RCC_HSI_IsReady() != 0U)
      {
        pllinputfreq = HSI_VALUE >> (LL_RCC_HSI_GetDivider()>> RCC_CR_HSIDIV_Pos);
      }
      break;

    case LL_RCC_PLLSOURCE_CSI:
      if (LL_RCC_CSI_IsReady() != 0U)
      {
        pllinputfreq = CSI_VALUE;
      }
      break;

    case LL_RCC_PLLSOURCE_HSE:
      if (LL_RCC_HSE_IsReady() != 0U)
      {
        pllinputfreq = HSE_VALUE;
      }
      break;

    case LL_RCC_PLLSOURCE_NONE:
    default:
      /* PLL clock disabled */
      break;
  }

  PLL_Clocks->PLL_P_Frequency = 0U;
  PLL_Clocks->PLL_Q_Frequency = 0U;
  PLL_Clocks->PLL_R_Frequency = 0U;

  m = LL_RCC_PLL1_GetM();
  n = LL_RCC_PLL1_GetN();
  if (LL_RCC_PLL1FRACN_IsEnabled() != 0U)
  {
    fracn = LL_RCC_PLL1_GetFRACN();
  }

  if (m != 0U)
  {
    if (LL_RCC_PLL1P_IsEnabled() != 0U)
    {
      PLL_Clocks->PLL_P_Frequency = LL_RCC_CalcPLLClockFreq(pllinputfreq, m, n, fracn, LL_RCC_PLL1_GetP());
    }

    if (LL_RCC_PLL1Q_IsEnabled() != 0U)
    {
      PLL_Clocks->PLL_Q_Frequency = LL_RCC_CalcPLLClockFreq(pllinputfreq, m, n, fracn, LL_RCC_PLL1_GetQ());
    }

    if (LL_RCC_PLL1R_IsEnabled() != 0U)
    {
      PLL_Clocks->PLL_R_Frequency = LL_RCC_CalcPLLClockFreq(pllinputfreq, m, n, fracn, LL_RCC_PLL1_GetR());
    }
  }
}

/**
  * @brief  Return PLL2 clocks frequencies
  * @note   LL_RCC_PERIPH_FREQUENCY_NO returned for non activated output or oscillator not ready
  * @retval None
  */
void LL_RCC_GetPLL2ClockFreq(LL_PLL_ClocksTypeDef *PLL_Clocks)
{
  uint32_t pllinputfreq = LL_RCC_PERIPH_FREQUENCY_NO, pllsource;
  uint32_t m, n, fracn = 0U;

  /* PLL_VCO = (HSE_VALUE, CSI_VALUE or HSI_VALUE/HSIDIV) / PLLM * (PLLN + FRACN)
     SYSCLK = PLL_VCO / PLLP
  */
  pllsource = LL_RCC_PLL_GetSource();

  switch (pllsource)
  {
    case LL_RCC_PLLSOURCE_HSI:
      if (LL_RCC_HSI_IsReady() != 0U)
      {
        pllinputfreq = HSI_VALUE >> (LL_RCC_HSI_GetDivider()>> RCC_CR_HSIDIV_Pos);
      }
      break;

    case LL_RCC_PLLSOURCE_CSI:
      if (LL_RCC_CSI_IsReady() != 0U)
      {
        pllinputfreq = CSI_VALUE;
      }
      break;

    case LL_RCC_PLLSOURCE_HSE:
      if (LL_RCC_HSE_IsReady() != 0U)
      {
        pllinputfreq = HSE_VALUE;
      }
      break;

    case LL_RCC_PLLSOURCE_NONE:
    default:
      /* PLL clock disabled */
      break;
  }

  PLL_Clocks->PLL_P_Frequency = 0U;
  PLL_Clocks->PLL_Q_Frequency = 0U;
  PLL_Clocks->PLL_R_Frequency = 0U;

  m = LL_RCC_PLL2_GetM();
  n = LL_RCC_PLL2_GetN();
  if (LL_RCC_PLL2FRACN_IsEnabled() != 0U)
  {
    fracn = LL_RCC_PLL2_GetFRACN();
  }

  if (m != 0U)
  {
    if (LL_RCC_PLL2P_IsEnabled() != 0U)
    {
      PLL_Clocks->PLL_P_Frequency = LL_RCC_CalcPLLClockFreq(pllinputfreq, m, n, fracn, LL_RCC_PLL2_GetP());
    }

    if (LL_RCC_PLL2Q_IsEnabled() != 0U)
    {
      PLL_Clocks->PLL_Q_Frequency = LL_RCC_CalcPLLClockFreq(pllinputfreq, m, n, fracn, LL_RCC_PLL2_GetQ());
    }

    if (LL_RCC_PLL2R_IsEnabled() != 0U)
    {
      PLL_Clocks->PLL_R_Frequency = LL_RCC_CalcPLLClockFreq(pllinputfreq, m, n, fracn, LL_RCC_PLL2_GetR());
    }
  }
}

/**
  * @brief  Return PLL3 clocks frequencies
  * @note   LL_RCC_PERIPH_FREQUENCY_NO returned for non activated output or oscillator not ready
  * @retval None
  */
void LL_RCC_GetPLL3ClockFreq(LL_PLL_ClocksTypeDef *PLL_Clocks)
{
  uint32_t pllinputfreq = LL_RCC_PERIPH_FREQUENCY_NO, pllsource;
  uint32_t m, n, fracn = 0U;

  /* PLL_VCO = (HSE_VALUE, CSI_VALUE or HSI_VALUE/HSIDIV) / PLLM * (PLLN + FRACN)
     SYSCLK = PLL_VCO / PLLP
  */
  pllsource = LL_RCC_PLL_GetSource();

  switch (pllsource)
  {
    case LL_RCC_PLLSOURCE_HSI:
      if (LL_RCC_HSI_IsReady() != 0U)
      {
        pllinputfreq = HSI_VALUE >> (LL_RCC_HSI_GetDivider()>> RCC_CR_HSIDIV_Pos);
      }
      break;

    case LL_RCC_PLLSOURCE_CSI:
      if (LL_RCC_CSI_IsReady() != 0U)
      {
        pllinputfreq = CSI_VALUE;
      }
      break;

    case LL_RCC_PLLSOURCE_HSE:
      if (LL_RCC_HSE_IsReady() != 0U)
      {
        pllinputfreq = HSE_VALUE;
      }
      break;

    case LL_RCC_PLLSOURCE_NONE:
    default:
      /* PLL clock disabled */
      break;
  }

  PLL_Clocks->PLL_P_Frequency = 0U;
  PLL_Clocks->PLL_Q_Frequency = 0U;
  PLL_Clocks->PLL_R_Frequency = 0U;

  m = LL_RCC_PLL3_GetM();
  n = LL_RCC_PLL3_GetN();
  if (LL_RCC_PLL3FRACN_IsEnabled() != 0U)
  {
    fracn = LL_RCC_PLL3_GetFRACN();
  }

  if ((m != 0U) && (pllinputfreq != 0U))
  {
    if (LL_RCC_PLL3P_IsEnabled() != 0U)
    {
      PLL_Clocks->PLL_P_Frequency = LL_RCC_CalcPLLClockFreq(pllinputfreq, m, n, fracn, LL_RCC_PLL3_GetP());
    }

    if (LL_RCC_PLL3Q_IsEnabled() != 0U)
    {
      PLL_Clocks->PLL_Q_Frequency = LL_RCC_CalcPLLClockFreq(pllinputfreq, m, n, fracn, LL_RCC_PLL3_GetQ());
    }

    if (LL_RCC_PLL3R_IsEnabled() != 0U)
    {
      PLL_Clocks->PLL_R_Frequency = LL_RCC_CalcPLLClockFreq(pllinputfreq, m, n, fracn, LL_RCC_PLL3_GetR());
    }
  }
}

/**
  * @brief  Helper function to calculate the PLL frequency output
  * @note ex: @ref LL_RCC_CalcPLLClockFreq (HSE_VALUE, @ref LL_RCC_PLL1_GetM (),
  *             @ref LL_RCC_PLL1_GetN (), @ref LL_RCC_PLL1_GetFRACN (), @ref LL_RCC_PLL1_GetP ());
  * @param  PLLInputFreq PLL Input frequency (based on HSE/(HSI/HSIDIV)/CSI)
  * @param  M      Between 1 and 63
  * @param  N      Between 4 and 512
  * @param  FRACN  Between 0 and 0x1FFF
  * @param  PQR    VCO output divider (P, Q or R)
  *                Between 1 and 128, except for PLL1P Odd value not allowed
  * @retval PLL1 clock frequency (in Hz)
  */
uint32_t LL_RCC_CalcPLLClockFreq(uint32_t PLLInputFreq, uint32_t M, uint32_t N, uint32_t FRACN, uint32_t PQR)
{
  float freq;

  freq = ((float)PLLInputFreq / (float)M) * ((float)N + ((float)FRACN/(float)0x2000));

  freq = freq/(float)PQR;

  return (uint32_t)freq;
}

/**
  * @brief  Return USARTx clock frequency
  * @param  USARTxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_USART16_CLKSOURCE
  *         @arg @ref LL_RCC_USART234578_CLKSOURCE
  * @retval USART clock frequency (in Hz)
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NO indicates that oscillator is not ready
  */
uint32_t LL_RCC_GetUSARTClockFreq(uint32_t USARTxSource)
{
  uint32_t usart_frequency = LL_RCC_PERIPH_FREQUENCY_NO;
  LL_PLL_ClocksTypeDef PLL_Clocks;

  /* Check parameter */
  assert_param(IS_LL_RCC_USART_CLKSOURCE(USARTxSource));

  switch (LL_RCC_GetUSARTClockSource(USARTxSource))
  {
    case LL_RCC_USART16_CLKSOURCE_PCLK2:
      usart_frequency = RCC_GetPCLK2ClockFreq(RCC_GetHCLKClockFreq(LL_RCC_CALC_SYSCLK_FREQ(RCC_GetSystemClockFreq(),LL_RCC_GetSysPrescaler())));
      break;

    case LL_RCC_USART234578_CLKSOURCE_PCLK1:
      usart_frequency = RCC_GetPCLK1ClockFreq(RCC_GetHCLKClockFreq(LL_RCC_CALC_SYSCLK_FREQ(RCC_GetSystemClockFreq(),LL_RCC_GetSysPrescaler())));
      break;

    case LL_RCC_USART16_CLKSOURCE_PLL2Q:
    case LL_RCC_USART234578_CLKSOURCE_PLL2Q:
      if (LL_RCC_PLL2_IsReady() != 0U)
      {
        LL_RCC_GetPLL2ClockFreq(&PLL_Clocks);
        usart_frequency = PLL_Clocks.PLL_Q_Frequency;
      }
      break;

    case LL_RCC_USART16_CLKSOURCE_PLL3Q:
    case LL_RCC_USART234578_CLKSOURCE_PLL3Q:
      if (LL_RCC_PLL3_IsReady() != 0U)
      {
        LL_RCC_GetPLL3ClockFreq(&PLL_Clocks);
        usart_frequency = PLL_Clocks.PLL_Q_Frequency;
      }
      break;

    case LL_RCC_USART16_CLKSOURCE_HSI:
    case LL_RCC_USART234578_CLKSOURCE_HSI:
      if (LL_RCC_HSI_IsReady() != 0U)
      {
        usart_frequency = HSI_VALUE >> (LL_RCC_HSI_GetDivider()>> RCC_CR_HSIDIV_Pos);
      }
      break;

    case LL_RCC_USART16_CLKSOURCE_CSI:
    case LL_RCC_USART234578_CLKSOURCE_CSI:
      if (LL_RCC_CSI_IsReady() != 0U)
      {
        usart_frequency = CSI_VALUE;
      }
      break;

    case LL_RCC_USART16_CLKSOURCE_LSE:
    case LL_RCC_USART234578_CLKSOURCE_LSE:
      if (LL_RCC_LSE_IsReady() != 0U)
      {
        usart_frequency = LSE_VALUE;
      }
      break;

    default:
      /* Kernel clock disabled */
      break;
  }

  return usart_frequency;
}

/**
  * @brief  Return LPUART clock frequency
  * @param  LPUARTxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_LPUART1_CLKSOURCE
  * @retval LPUART clock frequency (in Hz)
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NO indicates that oscillator is not ready
  */
uint32_t LL_RCC_GetLPUARTClockFreq(uint32_t LPUARTxSource)
{
  uint32_t lpuart_frequency = LL_RCC_PERIPH_FREQUENCY_NO;
  LL_PLL_ClocksTypeDef PLL_Clocks;

  switch (LL_RCC_GetLPUARTClockSource(LPUARTxSource))
  {
    case LL_RCC_LPUART1_CLKSOURCE_PCLK4:
      lpuart_frequency = RCC_GetPCLK4ClockFreq(RCC_GetHCLKClockFreq(LL_RCC_CALC_SYSCLK_FREQ(RCC_GetSystemClockFreq(),LL_RCC_GetSysPrescaler())));
      break;

    case LL_RCC_LPUART1_CLKSOURCE_PLL2Q:
      if (LL_RCC_PLL2_IsReady() != 0U)
      {
        LL_RCC_GetPLL2ClockFreq(&PLL_Clocks);
        lpuart_frequency = PLL_Clocks.PLL_Q_Frequency;
      }
      break;

    case LL_RCC_LPUART1_CLKSOURCE_PLL3Q:
      if (LL_RCC_PLL3_IsReady() != 0U)
      {
        LL_RCC_GetPLL3ClockFreq(&PLL_Clocks);
        lpuart_frequency = PLL_Clocks.PLL_Q_Frequency;
      }
      break;

    case LL_RCC_LPUART1_CLKSOURCE_HSI:
      if (LL_RCC_HSI_IsReady() != 0U)
      {
        lpuart_frequency = HSI_VALUE >> (LL_RCC_HSI_GetDivider()>> RCC_CR_HSIDIV_Pos);
      }
      break;

    case LL_RCC_LPUART1_CLKSOURCE_CSI:
      if (LL_RCC_CSI_IsReady() != 0U)
      {
        lpuart_frequency = CSI_VALUE;
      }
      break;

    case LL_RCC_LPUART1_CLKSOURCE_LSE:
      if (LL_RCC_LSE_IsReady() != 0U)
      {
        lpuart_frequency = LSE_VALUE;
      }
      break;

    default:
      /* Kernel clock disabled */
      break;
  }

  return lpuart_frequency;
}

/**
  * @brief  Return I2Cx clock frequency
  * @param  I2CxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_I2C123_CLKSOURCE
  *         @arg @ref LL_RCC_I2C4_CLKSOURCE
  * @retval I2C clock frequency (in Hz)
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NO indicates that oscillator is not ready
  */
uint32_t LL_RCC_GetI2CClockFreq(uint32_t I2CxSource)
{
  uint32_t i2c_frequency = LL_RCC_PERIPH_FREQUENCY_NO;
  LL_PLL_ClocksTypeDef PLL_Clocks;

  /* Check parameter */
  assert_param(IS_LL_RCC_I2C_CLKSOURCE(I2CxSource));

  switch (LL_RCC_GetI2CClockSource(I2CxSource))
  {
    case LL_RCC_I2C123_CLKSOURCE_PCLK1:
      i2c_frequency = RCC_GetPCLK1ClockFreq(RCC_GetHCLKClockFreq(LL_RCC_CALC_SYSCLK_FREQ(RCC_GetSystemClockFreq(),LL_RCC_GetSysPrescaler())));
      break;

    case LL_RCC_I2C4_CLKSOURCE_PCLK4:
      i2c_frequency = RCC_GetPCLK4ClockFreq(RCC_GetHCLKClockFreq(LL_RCC_CALC_SYSCLK_FREQ(RCC_GetSystemClockFreq(),LL_RCC_GetSysPrescaler())));
      break;

    case LL_RCC_I2C123_CLKSOURCE_PLL3R:
    case LL_RCC_I2C4_CLKSOURCE_PLL3R:
      if (LL_RCC_PLL3_IsReady() != 0U)
      {
        LL_RCC_GetPLL3ClockFreq(&PLL_Clocks);
        i2c_frequency = PLL_Clocks.PLL_R_Frequency;
      }
      break;

    case LL_RCC_I2C123_CLKSOURCE_HSI:
    case LL_RCC_I2C4_CLKSOURCE_HSI:
      if (LL_RCC_HSI_IsReady() != 0U)
      {
        i2c_frequency = HSI_VALUE >> (LL_RCC_HSI_GetDivider()>> RCC_CR_HSIDIV_Pos);
      }
      break;

    case LL_RCC_I2C123_CLKSOURCE_CSI:
    case LL_RCC_I2C4_CLKSOURCE_CSI:
      if (LL_RCC_CSI_IsReady() != 0U)
      {
        i2c_frequency = CSI_VALUE;
      }
      break;

    default:
      /* Nothing to do */
      break;
  }

  return i2c_frequency;
}

/**
  * @brief  Return LPTIMx clock frequency
  * @param  LPTIMxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_LPTIM1_CLKSOURCE
  *         @arg @ref LL_RCC_LPTIM2_CLKSOURCE
  *         @arg @ref LL_RCC_LPTIM345_CLKSOURCE
  * @retval LPTIM clock frequency (in Hz)
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NO indicates that oscillator is not ready
  */
uint32_t LL_RCC_GetLPTIMClockFreq(uint32_t LPTIMxSource)
{
  uint32_t lptim_frequency = LL_RCC_PERIPH_FREQUENCY_NO;
  LL_PLL_ClocksTypeDef PLL_Clocks;

  /* Check parameter */
  assert_param(IS_LL_RCC_LPTIM_CLKSOURCE(LPTIMxSource));

  switch (LL_RCC_GetLPTIMClockSource(LPTIMxSource))
  {
    case LL_RCC_LPTIM1_CLKSOURCE_PCLK1:
      lptim_frequency = RCC_GetPCLK1ClockFreq(RCC_GetHCLKClockFreq(LL_RCC_CALC_SYSCLK_FREQ(RCC_GetSystemClockFreq(),LL_RCC_GetSysPrescaler())));
      break;

    case LL_RCC_LPTIM2_CLKSOURCE_PCLK4:
    case LL_RCC_LPTIM345_CLKSOURCE_PCLK4:
      lptim_frequency = RCC_GetPCLK4ClockFreq(RCC_GetHCLKClockFreq(LL_RCC_CALC_SYSCLK_FREQ(RCC_GetSystemClockFreq(),LL_RCC_GetSysPrescaler())));
      break;

    case LL_RCC_LPTIM1_CLKSOURCE_PLL2P:
    case LL_RCC_LPTIM2_CLKSOURCE_PLL2P:
    case LL_RCC_LPTIM345_CLKSOURCE_PLL2P:
      if (LL_RCC_PLL2_IsReady() != 0U)
      {
        LL_RCC_GetPLL2ClockFreq(&PLL_Clocks);
        lptim_frequency = PLL_Clocks.PLL_P_Frequency;
      }
      break;

    case LL_RCC_LPTIM1_CLKSOURCE_PLL3R:
    case LL_RCC_LPTIM2_CLKSOURCE_PLL3R:
    case LL_RCC_LPTIM345_CLKSOURCE_PLL3R:
      if (LL_RCC_PLL3_IsReady() != 0U)
      {
        LL_RCC_GetPLL3ClockFreq(&PLL_Clocks);
        lptim_frequency = PLL_Clocks.PLL_R_Frequency;
      }
      break;

    case LL_RCC_LPTIM1_CLKSOURCE_LSE:
    case LL_RCC_LPTIM2_CLKSOURCE_LSE:
    case LL_RCC_LPTIM345_CLKSOURCE_LSE:
      if (LL_RCC_LSE_IsReady() != 0U)
      {
        lptim_frequency = LSE_VALUE;
      }
      break;

    case LL_RCC_LPTIM1_CLKSOURCE_LSI:
    case LL_RCC_LPTIM2_CLKSOURCE_LSI:
    case LL_RCC_LPTIM345_CLKSOURCE_LSI:
      if (LL_RCC_LSI_IsReady() != 0U)
      {
        lptim_frequency = LSI_VALUE;
      }
      break;

    case LL_RCC_LPTIM1_CLKSOURCE_CLKP:
    case LL_RCC_LPTIM2_CLKSOURCE_CLKP:
    case LL_RCC_LPTIM345_CLKSOURCE_CLKP:
      lptim_frequency = LL_RCC_GetCLKPClockFreq(LL_RCC_CLKP_CLKSOURCE);
      break;

    default:
      /* Kernel clock disabled */
      break;
  }

  return lptim_frequency;
}

/**
  * @brief  Return SAIx clock frequency
  * @param  SAIxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_SAI1_CLKSOURCE
  *         @arg @ref LL_RCC_SAI23_CLKSOURCE
  *         @arg @ref LL_RCC_SAI4A_CLKSOURCE
  *         @arg @ref LL_RCC_SAI4B_CLKSOURCE
  * @retval SAI clock frequency (in Hz)
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NO indicates that oscillator is not ready
  */
uint32_t LL_RCC_GetSAIClockFreq(uint32_t SAIxSource)
{
  uint32_t sai_frequency = LL_RCC_PERIPH_FREQUENCY_NO;
  LL_PLL_ClocksTypeDef PLL_Clocks;

  /* Check parameter */
  assert_param(IS_LL_RCC_SAI_CLKSOURCE(SAIxSource));

  switch (LL_RCC_GetSAIClockSource(SAIxSource))
  {
    case LL_RCC_SAI1_CLKSOURCE_PLL1Q:
    case LL_RCC_SAI23_CLKSOURCE_PLL1Q:
    case LL_RCC_SAI4A_CLKSOURCE_PLL1Q:
    case LL_RCC_SAI4B_CLKSOURCE_PLL1Q:
      if (LL_RCC_PLL1_IsReady() != 0U)
      {
        LL_RCC_GetPLL1ClockFreq(&PLL_Clocks);
        sai_frequency = PLL_Clocks.PLL_Q_Frequency;
      }
      break;

    case LL_RCC_SAI1_CLKSOURCE_PLL2P:
    case LL_RCC_SAI23_CLKSOURCE_PLL2P:
    case LL_RCC_SAI4A_CLKSOURCE_PLL2P:
    case LL_RCC_SAI4B_CLKSOURCE_PLL2P:
      if (LL_RCC_PLL2_IsReady() != 0U)
      {
        LL_RCC_GetPLL2ClockFreq(&PLL_Clocks);
        sai_frequency = PLL_Clocks.PLL_P_Frequency;
      }
      break;

    case LL_RCC_SAI1_CLKSOURCE_PLL3P:
    case LL_RCC_SAI23_CLKSOURCE_PLL3P:
    case LL_RCC_SAI4A_CLKSOURCE_PLL3P:
    case LL_RCC_SAI4B_CLKSOURCE_PLL3P:
      if (LL_RCC_PLL3_IsReady() != 0U)
      {
        LL_RCC_GetPLL3ClockFreq(&PLL_Clocks);
        sai_frequency = PLL_Clocks.PLL_P_Frequency;
      }
      break;

    case LL_RCC_SAI1_CLKSOURCE_I2S_CKIN:
    case LL_RCC_SAI23_CLKSOURCE_I2S_CKIN:
    case LL_RCC_SAI4A_CLKSOURCE_I2S_CKIN:
    case LL_RCC_SAI4B_CLKSOURCE_I2S_CKIN:
      sai_frequency = EXTERNAL_CLOCK_VALUE;
      break;

    case LL_RCC_SAI1_CLKSOURCE_CLKP:
    case LL_RCC_SAI23_CLKSOURCE_CLKP:
    case LL_RCC_SAI4A_CLKSOURCE_CLKP:
    case LL_RCC_SAI4B_CLKSOURCE_CLKP:
      sai_frequency = LL_RCC_GetCLKPClockFreq(LL_RCC_CLKP_CLKSOURCE);
      break;
    default:
      /* Kernel clock disabled */
      break;
  }

  return sai_frequency;
}

/**
  * @brief  Return ADC clock frequency
  * @param  ADCxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_ADC_CLKSOURCE
  * @retval ADC clock frequency (in Hz)
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NO indicates that oscillator is not ready
  */
uint32_t LL_RCC_GetADCClockFreq(uint32_t ADCxSource)
{
  uint32_t adc_frequency = LL_RCC_PERIPH_FREQUENCY_NO;
  LL_PLL_ClocksTypeDef PLL_Clocks;

  switch (LL_RCC_GetADCClockSource(ADCxSource))
  {
    case LL_RCC_ADC_CLKSOURCE_PLL2P:
      if (LL_RCC_PLL2_IsReady() != 0U)
      {
        LL_RCC_GetPLL2ClockFreq(&PLL_Clocks);
        adc_frequency = PLL_Clocks.PLL_P_Frequency;
      }
      break;

    case LL_RCC_ADC_CLKSOURCE_PLL3R:
      if (LL_RCC_PLL3_IsReady() != 0U)
      {
        LL_RCC_GetPLL3ClockFreq(&PLL_Clocks);
        adc_frequency = PLL_Clocks.PLL_R_Frequency;
      }
      break;

    case LL_RCC_ADC_CLKSOURCE_CLKP:
      adc_frequency = LL_RCC_GetCLKPClockFreq(LL_RCC_CLKP_CLKSOURCE);
      break;

    default:
      /* Kernel clock disabled */
      break;
  }

  return adc_frequency;
}

/**
  * @brief  Return SDMMC clock frequency
  * @param  SDMMCxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_SDMMC_CLKSOURCE
  * @retval SDMMC clock frequency (in Hz)
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NO indicates that oscillator is not ready
  */
uint32_t LL_RCC_GetSDMMCClockFreq(uint32_t SDMMCxSource)
{
  uint32_t sdmmc_frequency = LL_RCC_PERIPH_FREQUENCY_NO;
  LL_PLL_ClocksTypeDef PLL_Clocks;

  switch (LL_RCC_GetSDMMCClockSource(SDMMCxSource))
  {
    case LL_RCC_SDMMC_CLKSOURCE_PLL1Q:
      if (LL_RCC_PLL1_IsReady() != 0U)
      {
        LL_RCC_GetPLL1ClockFreq(&PLL_Clocks);
        sdmmc_frequency = PLL_Clocks.PLL_Q_Frequency;
      }
      break;

    case LL_RCC_SDMMC_CLKSOURCE_PLL2R:
      if (LL_RCC_PLL2_IsReady() != 0U)
      {
        LL_RCC_GetPLL2ClockFreq(&PLL_Clocks);
        sdmmc_frequency = PLL_Clocks.PLL_R_Frequency;
      }
      break;

    default:
      /* Nothing to do */
      break;
  }

  return sdmmc_frequency;
}

/**
  * @brief  Return RNG clock frequency
  * @param  RNGxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_RNG_CLKSOURCE
  * @retval RNG clock frequency (in Hz)
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NO indicates that oscillator is not ready
  */
uint32_t LL_RCC_GetRNGClockFreq(uint32_t RNGxSource)
{
  uint32_t rng_frequency = LL_RCC_PERIPH_FREQUENCY_NO;
  LL_PLL_ClocksTypeDef PLL_Clocks;

  switch (LL_RCC_GetRNGClockSource(RNGxSource))
  {
    case LL_RCC_RNG_CLKSOURCE_PLL1Q:
      if (LL_RCC_PLL1_IsReady() != 0U)
      {
        LL_RCC_GetPLL1ClockFreq(&PLL_Clocks);
        rng_frequency = PLL_Clocks.PLL_Q_Frequency;
      }
      break;

    case LL_RCC_RNG_CLKSOURCE_HSI48:
      if (LL_RCC_HSI48_IsReady() != 0U)
      {
        rng_frequency = 48000000U;
      }
      break;

    case LL_RCC_RNG_CLKSOURCE_LSE:
      if (LL_RCC_LSE_IsReady() != 0U)
      {
        rng_frequency = LSE_VALUE;
      }
      break;

    case LL_RCC_RNG_CLKSOURCE_LSI:
      if (LL_RCC_LSI_IsReady() != 0U)
      {
        rng_frequency = LSI_VALUE;
      }
      break;

    default:
      /* Nothing to do */
      break;
  }

  return rng_frequency;
}

/**
  * @brief  Return CEC clock frequency
  * @param  CECxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_RNG_CLKSOURCE
  * @retval CEC clock frequency (in Hz)
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NO indicates that oscillator is not ready
  */
uint32_t LL_RCC_GetCECClockFreq(uint32_t CECxSource)
{
  uint32_t cec_frequency = LL_RCC_PERIPH_FREQUENCY_NO;

  switch (LL_RCC_GetCECClockSource(CECxSource))
  {
    case LL_RCC_CEC_CLKSOURCE_LSE:
      if (LL_RCC_LSE_IsReady() != 0U)
      {
        cec_frequency = LSE_VALUE;
      }
      break;

    case LL_RCC_CEC_CLKSOURCE_LSI:
      if (LL_RCC_LSI_IsReady() != 0U)
      {
        cec_frequency = LSI_VALUE;
      }
      break;

    case LL_RCC_CEC_CLKSOURCE_CSI_DIV122:
      if (LL_RCC_CSI_IsReady() != 0U)
      {
        cec_frequency = CSI_VALUE / 122U;
      }
      break;

    default:
      /* Kernel clock disabled */
      break;
  }

  return cec_frequency;
}

/**
  * @brief  Return USB clock frequency
  * @param  USBxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_USB_CLKSOURCE
  * @retval USB clock frequency (in Hz)
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NO indicates that oscillator is not ready or Disabled
  */
uint32_t LL_RCC_GetUSBClockFreq(uint32_t USBxSource)
{
  uint32_t usb_frequency = LL_RCC_PERIPH_FREQUENCY_NO;
  LL_PLL_ClocksTypeDef PLL_Clocks;

  switch (LL_RCC_GetUSBClockSource(USBxSource))
  {
    case LL_RCC_USB_CLKSOURCE_PLL1Q:
      if (LL_RCC_PLL1_IsReady() != 0U)
      {
        LL_RCC_GetPLL1ClockFreq(&PLL_Clocks);
        usb_frequency = PLL_Clocks.PLL_Q_Frequency;
      }
      break;

    case LL_RCC_USB_CLKSOURCE_PLL3Q:
      if (LL_RCC_PLL3_IsReady() != 0U)
      {
        LL_RCC_GetPLL3ClockFreq(&PLL_Clocks);
        usb_frequency = PLL_Clocks.PLL_Q_Frequency;
      }
      break;

    case LL_RCC_USB_CLKSOURCE_HSI48:
      if (LL_RCC_HSI48_IsReady() != 0U)
      {
        usb_frequency = 48000000U;
      }
      break;

    case LL_RCC_USB_CLKSOURCE_DISABLE:
        usb_frequency = LL_RCC_PERIPH_FREQUENCY_NO;
        break;

    default:
      /* Nothing to do */
      break;
  }

  return usb_frequency;
}

/**
  * @brief  Return DFSDM clock frequency
  * @param  DFSDMxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_DFSDM1_CLKSOURCE
  * @retval DFSDM clock frequency (in Hz)
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NO indicates that oscillator is not ready
  */
uint32_t LL_RCC_GetDFSDMClockFreq(uint32_t DFSDMxSource)
{
  uint32_t dfsdm_frequency = LL_RCC_PERIPH_FREQUENCY_NO;

  switch (LL_RCC_GetDFSDMClockSource(DFSDMxSource))
  {
    case LL_RCC_DFSDM1_CLKSOURCE_SYSCLK:
      dfsdm_frequency = RCC_GetSystemClockFreq();
      break;

    case LL_RCC_DFSDM1_CLKSOURCE_PCLK2:
      dfsdm_frequency = RCC_GetPCLK2ClockFreq(RCC_GetHCLKClockFreq(LL_RCC_CALC_SYSCLK_FREQ(RCC_GetSystemClockFreq(),LL_RCC_GetSysPrescaler())));
      break;

    default:
      /* Nothing to do */
      break;
  }

  return dfsdm_frequency;
}


#if defined(DSI)
/**
  * @brief  Return DSI clock frequency
  * @param  DSIxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_DSI_CLKSOURCE
  * @retval DSI clock frequency (in Hz)
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NO indicates that oscillator is not ready
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NA indicates that external clock is used
  */
uint32_t LL_RCC_GetDSIClockFreq(uint32_t DSIxSource)
{
  uint32_t dsi_frequency = LL_RCC_PERIPH_FREQUENCY_NO;
  LL_PLL_ClocksTypeDef PLL_Clocks;

  switch (LL_RCC_GetDSIClockSource(DSIxSource))
  {
    case LL_RCC_DSI_CLKSOURCE_PLL2Q:
      if (LL_RCC_PLL2_IsReady() != 0U)
      {
        LL_RCC_GetPLL2ClockFreq(&PLL_Clocks);
        dsi_frequency = PLL_Clocks.PLL_Q_Frequency;
      }
      break;

    case LL_RCC_DSI_CLKSOURCE_PHY:
      dsi_frequency = LL_RCC_PERIPH_FREQUENCY_NA;
      break;

    default:
      /* Nothing to do */
      break;
  }

  return dsi_frequency;
}
#endif /* DSI */

/**
  * @brief  Return SPDIF clock frequency
  * @param  SPDIFxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_SPDIF_CLKSOURCE
  * @retval SPDIF clock frequency (in Hz)
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NO indicates that oscillator is not ready
  */
uint32_t LL_RCC_GetSPDIFClockFreq(uint32_t SPDIFxSource)
{
  uint32_t spdif_frequency = LL_RCC_PERIPH_FREQUENCY_NO;
  LL_PLL_ClocksTypeDef PLL_Clocks;

  switch (LL_RCC_GetSPDIFClockSource(SPDIFxSource))
  {
    case LL_RCC_SPDIF_CLKSOURCE_PLL1Q:
      if (LL_RCC_PLL1_IsReady() != 0U)
      {
        LL_RCC_GetPLL1ClockFreq(&PLL_Clocks);
        spdif_frequency = PLL_Clocks.PLL_Q_Frequency;
      }
      break;

    case LL_RCC_SPDIF_CLKSOURCE_PLL2R:
      if (LL_RCC_PLL2_IsReady() != 0U)
      {
        LL_RCC_GetPLL2ClockFreq(&PLL_Clocks);
        spdif_frequency = PLL_Clocks.PLL_R_Frequency;
      }
      break;

    case LL_RCC_SPDIF_CLKSOURCE_PLL3R:
      if (LL_RCC_PLL3_IsReady() != 0U)
      {
        LL_RCC_GetPLL3ClockFreq(&PLL_Clocks);
        spdif_frequency = PLL_Clocks.PLL_R_Frequency;
      }
      break;

    case LL_RCC_SPDIF_CLKSOURCE_HSI:
      if (LL_RCC_HSI_IsReady() != 0U)
      {
        spdif_frequency = HSI_VALUE >> (LL_RCC_HSI_GetDivider()>> RCC_CR_HSIDIV_Pos);
      }
      break;

    default:
      /* Nothing to do */
      break;
  }

  return spdif_frequency;
}

/**
  * @brief  Return SPIx clock frequency
  * @param  SPIxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_SPI123_CLKSOURCE
  *         @arg @ref LL_RCC_SPI45_CLKSOURCE
  *         @arg @ref LL_RCC_SPI6_CLKSOURCE
  * @retval SPI clock frequency (in Hz)
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NO indicates that oscillator is not ready
  */
uint32_t LL_RCC_GetSPIClockFreq(uint32_t SPIxSource)
{
  uint32_t spi_frequency = LL_RCC_PERIPH_FREQUENCY_NO;
  LL_PLL_ClocksTypeDef PLL_Clocks;

  /* Check parameter */
  assert_param(IS_LL_RCC_SPI_CLKSOURCE(SPIxSource));

  switch (LL_RCC_GetSPIClockSource(SPIxSource))
  {
    case LL_RCC_SPI123_CLKSOURCE_PLL1Q:
      if (LL_RCC_PLL1_IsReady() != 0U)
      {
        LL_RCC_GetPLL1ClockFreq(&PLL_Clocks);
        spi_frequency = PLL_Clocks.PLL_Q_Frequency;
      }
      break;

    case LL_RCC_SPI123_CLKSOURCE_PLL2P:
      if (LL_RCC_PLL2_IsReady() != 0U)
      {
        LL_RCC_GetPLL2ClockFreq(&PLL_Clocks);
        spi_frequency = PLL_Clocks.PLL_P_Frequency;
      }
      break;

    case LL_RCC_SPI123_CLKSOURCE_PLL3P:
      if (LL_RCC_PLL3_IsReady() != 0U)
      {
        LL_RCC_GetPLL3ClockFreq(&PLL_Clocks);
        spi_frequency = PLL_Clocks.PLL_P_Frequency;
      }
      break;

    case LL_RCC_SPI123_CLKSOURCE_I2S_CKIN:
      spi_frequency = EXTERNAL_CLOCK_VALUE;
      break;

    case LL_RCC_SPI123_CLKSOURCE_CLKP:
      spi_frequency = LL_RCC_GetCLKPClockFreq(LL_RCC_CLKP_CLKSOURCE);
      break;

    case LL_RCC_SPI45_CLKSOURCE_PCLK2:
      spi_frequency = RCC_GetPCLK2ClockFreq(RCC_GetHCLKClockFreq(LL_RCC_CALC_SYSCLK_FREQ(RCC_GetSystemClockFreq(),LL_RCC_GetSysPrescaler())));
      break;

    case LL_RCC_SPI6_CLKSOURCE_PCLK4:
      spi_frequency = RCC_GetPCLK4ClockFreq(RCC_GetHCLKClockFreq(LL_RCC_CALC_SYSCLK_FREQ(RCC_GetSystemClockFreq(),LL_RCC_GetSysPrescaler())));
      break;

    case LL_RCC_SPI45_CLKSOURCE_PLL2Q:
    case LL_RCC_SPI6_CLKSOURCE_PLL2Q:
      if (LL_RCC_PLL2_IsReady() != 0U)
      {
        LL_RCC_GetPLL2ClockFreq(&PLL_Clocks);
        spi_frequency = PLL_Clocks.PLL_Q_Frequency;
      }
      break;

    case LL_RCC_SPI45_CLKSOURCE_PLL3Q:
    case LL_RCC_SPI6_CLKSOURCE_PLL3Q:
      if (LL_RCC_PLL3_IsReady() != 0U)
      {
        LL_RCC_GetPLL3ClockFreq(&PLL_Clocks);
        spi_frequency = PLL_Clocks.PLL_Q_Frequency;
      }
      break;

    case LL_RCC_SPI45_CLKSOURCE_HSI:
    case LL_RCC_SPI6_CLKSOURCE_HSI:
      if (LL_RCC_HSI_IsReady() != 0U)
      {
        spi_frequency = HSI_VALUE >> (LL_RCC_HSI_GetDivider()>> RCC_CR_HSIDIV_Pos);
      }
      break;

    case LL_RCC_SPI45_CLKSOURCE_CSI:
    case LL_RCC_SPI6_CLKSOURCE_CSI:
      if (LL_RCC_CSI_IsReady() != 0U)
      {
        spi_frequency = CSI_VALUE;
      }
      break;

    case LL_RCC_SPI45_CLKSOURCE_HSE:
    case LL_RCC_SPI6_CLKSOURCE_HSE:
      if (LL_RCC_HSE_IsReady() != 0U)
      {
        spi_frequency = HSE_VALUE;
      }
      break;

    default:
      /* Kernel clock disabled */
      break;
  }

  return spi_frequency;
}

/**
  * @brief  Return SWP clock frequency
  * @param  SWPxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_SWP_CLKSOURCE
  * @retval SWP clock frequency (in Hz)
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NO indicates that oscillator is not ready
  */
uint32_t LL_RCC_GetSWPClockFreq(uint32_t SWPxSource)
{
  uint32_t swp_frequency = LL_RCC_PERIPH_FREQUENCY_NO;

  switch (LL_RCC_GetSWPClockSource(SWPxSource))
  {
    case LL_RCC_SWP_CLKSOURCE_PCLK1:
      swp_frequency = RCC_GetPCLK1ClockFreq(RCC_GetHCLKClockFreq(LL_RCC_CALC_SYSCLK_FREQ(RCC_GetSystemClockFreq(),LL_RCC_GetSysPrescaler())));
      break;

    case LL_RCC_SWP_CLKSOURCE_HSI:
      if (LL_RCC_HSI_IsReady() != 0U)
      {
        swp_frequency = HSI_VALUE >> (LL_RCC_HSI_GetDivider()>> RCC_CR_HSIDIV_Pos);
      }
      break;

    default:
      /* Nothing to do */
      break;
  }

  return swp_frequency;
}

/**
  * @brief  Return FDCAN clock frequency
  * @param  FDCANxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_FDCAN_CLKSOURCE
  * @retval FDCAN clock frequency (in Hz)
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NO indicates that oscillator is not ready
  */
uint32_t LL_RCC_GetFDCANClockFreq(uint32_t FDCANxSource)
{
  uint32_t fdcan_frequency = LL_RCC_PERIPH_FREQUENCY_NO;
  LL_PLL_ClocksTypeDef PLL_Clocks;

  switch (LL_RCC_GetFDCANClockSource(FDCANxSource))
  {
    case LL_RCC_FDCAN_CLKSOURCE_HSE:
      if (LL_RCC_HSE_IsReady() != 0U)
      {
        fdcan_frequency = HSE_VALUE;
      }
      break;

    case LL_RCC_FDCAN_CLKSOURCE_PLL1Q:
      if (LL_RCC_PLL1_IsReady() != 0U)
      {
        LL_RCC_GetPLL1ClockFreq(&PLL_Clocks);
        fdcan_frequency = PLL_Clocks.PLL_Q_Frequency;
      }
      break;

    case LL_RCC_FDCAN_CLKSOURCE_PLL2Q:
      if (LL_RCC_PLL2_IsReady() != 0U)
      {
        LL_RCC_GetPLL2ClockFreq(&PLL_Clocks);
        fdcan_frequency = PLL_Clocks.PLL_Q_Frequency;
      }
      break;

    default:
      /* Kernel clock disabled */
      break;
  }

  return fdcan_frequency;
}

/**
  * @brief  Return FMC clock frequency
  * @param  FMCxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_FMC_CLKSOURCE
  * @retval FMC clock frequency (in Hz)
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NO indicates that oscillator is not ready
  */
uint32_t LL_RCC_GetFMCClockFreq(uint32_t FMCxSource)
{
  uint32_t fmc_frequency = LL_RCC_PERIPH_FREQUENCY_NO;
  LL_PLL_ClocksTypeDef PLL_Clocks;

  switch (LL_RCC_GetFMCClockSource(FMCxSource))
  {
    case LL_RCC_FMC_CLKSOURCE_HCLK:
      fmc_frequency = RCC_GetHCLKClockFreq(LL_RCC_CALC_SYSCLK_FREQ(RCC_GetSystemClockFreq(),LL_RCC_GetSysPrescaler()));
      break;

    case LL_RCC_FMC_CLKSOURCE_PLL1Q:
      if (LL_RCC_PLL1_IsReady() != 0U)
      {
        LL_RCC_GetPLL1ClockFreq(&PLL_Clocks);
        fmc_frequency = PLL_Clocks.PLL_Q_Frequency;
      }
      break;

    case LL_RCC_FMC_CLKSOURCE_PLL2R:
      if (LL_RCC_PLL2_IsReady() != 0U)
      {
        LL_RCC_GetPLL2ClockFreq(&PLL_Clocks);
        fmc_frequency = PLL_Clocks.PLL_R_Frequency;
      }
      break;

    case LL_RCC_FMC_CLKSOURCE_CLKP:
      fmc_frequency = LL_RCC_GetCLKPClockFreq(LL_RCC_CLKP_CLKSOURCE);
      break;

    default:
      /* Nothing to do */
      break;
  }

  return fmc_frequency;
}

/**
  * @brief  Return QSPI clock frequency
  * @param  QSPIxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_QSPI_CLKSOURCE
  * @retval QSPI clock frequency (in Hz)
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NO indicates that oscillator is not ready
  */
uint32_t LL_RCC_GetQSPIClockFreq(uint32_t QSPIxSource)
{
  uint32_t qspi_frequency = LL_RCC_PERIPH_FREQUENCY_NO;
  LL_PLL_ClocksTypeDef PLL_Clocks;

  switch (LL_RCC_GetQSPIClockSource(QSPIxSource))
  {
    case LL_RCC_QSPI_CLKSOURCE_HCLK:
      qspi_frequency = RCC_GetHCLKClockFreq(LL_RCC_CALC_SYSCLK_FREQ(RCC_GetSystemClockFreq(),LL_RCC_GetSysPrescaler()));
      break;

    case LL_RCC_QSPI_CLKSOURCE_PLL1Q:
      if (LL_RCC_PLL1_IsReady() != 0U)
      {
        LL_RCC_GetPLL1ClockFreq(&PLL_Clocks);
        qspi_frequency = PLL_Clocks.PLL_Q_Frequency;
      }
      break;

    case LL_RCC_QSPI_CLKSOURCE_PLL2R:
      if (LL_RCC_PLL2_IsReady() != 0U)
      {
        LL_RCC_GetPLL2ClockFreq(&PLL_Clocks);
        qspi_frequency = PLL_Clocks.PLL_R_Frequency;
      }
      break;

    case LL_RCC_QSPI_CLKSOURCE_CLKP:
      qspi_frequency = LL_RCC_GetCLKPClockFreq(LL_RCC_CLKP_CLKSOURCE);
      break;

    default:
      /* Nothing to do */
      break;
  }

  return qspi_frequency;
}

/**
  * @brief  Return CLKP clock frequency
  * @param  CLKPxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_CLKP_CLKSOURCE
  * @retval CLKP clock frequency (in Hz)
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NO indicates that oscillator is not ready
  */
uint32_t LL_RCC_GetCLKPClockFreq(uint32_t CLKPxSource)
{
  uint32_t clkp_frequency = LL_RCC_PERIPH_FREQUENCY_NO;

  switch (LL_RCC_GetCLKPClockSource(CLKPxSource))
  {
    case LL_RCC_CLKP_CLKSOURCE_HSI:
      if (LL_RCC_HSI_IsReady() != 0U)
      {
        clkp_frequency = HSI_VALUE >> (LL_RCC_HSI_GetDivider()>> RCC_CR_HSIDIV_Pos);
      }
      break;

    case LL_RCC_CLKP_CLKSOURCE_CSI:
      if (LL_RCC_CSI_IsReady() != 0U)
      {
        clkp_frequency = CSI_VALUE;
      }
      break;

    case LL_RCC_CLKP_CLKSOURCE_HSE:
      if (LL_RCC_HSE_IsReady() != 0U)
      {
        clkp_frequency = HSE_VALUE;
      }
      break;

    default:
      /* CLKP clock disabled */
      break;
  }

  return clkp_frequency;
}

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup RCC_LL_Private_Functions
  * @{
  */

/**
  * @brief  Return SYSTEM clock frequency
  * @retval SYSTEM clock frequency (in Hz)
  */
uint32_t RCC_GetSystemClockFreq(void)
{
  uint32_t frequency = 0U;
  LL_PLL_ClocksTypeDef PLL_Clocks;

  /* Get SYSCLK source -------------------------------------------------------*/
  switch (LL_RCC_GetSysClkSource())
  {
    /* No check on Ready: Won't be selected by hardware if not */
    case LL_RCC_SYS_CLKSOURCE_STATUS_HSI:
      frequency = HSI_VALUE >> (LL_RCC_HSI_GetDivider()>> RCC_CR_HSIDIV_Pos);
      break;

    case LL_RCC_SYS_CLKSOURCE_STATUS_CSI:
      frequency = CSI_VALUE;
      break;

    case LL_RCC_SYS_CLKSOURCE_STATUS_HSE:
      frequency = HSE_VALUE;
      break;

    case LL_RCC_SYS_CLKSOURCE_STATUS_PLL1:
      LL_RCC_GetPLL1ClockFreq(&PLL_Clocks);
      frequency = PLL_Clocks.PLL_P_Frequency;
      break;

    default:
      /* Nothing to do */
      break;
  }

  return frequency;
}

/**
  * @brief  Return HCLK clock frequency
  * @param  SYSCLK_Frequency SYSCLK clock frequency
  * @retval HCLK clock frequency (in Hz)
  */
uint32_t RCC_GetHCLKClockFreq(uint32_t SYSCLK_Frequency)
{
  /* HCLK clock frequency */
  return LL_RCC_CALC_HCLK_FREQ(SYSCLK_Frequency, LL_RCC_GetAHBPrescaler());
}

/**
  * @brief  Return PCLK1 clock frequency
  * @param  HCLK_Frequency HCLK clock frequency
  * @retval PCLK1 clock frequency (in Hz)
  */
uint32_t RCC_GetPCLK1ClockFreq(uint32_t HCLK_Frequency)
{
  /* PCLK1 clock frequency */
  return LL_RCC_CALC_PCLK1_FREQ(HCLK_Frequency, LL_RCC_GetAPB1Prescaler());
}

/**
  * @brief  Return PCLK2 clock frequency
  * @param  HCLK_Frequency HCLK clock frequency
  * @retval PCLK2 clock frequency (in Hz)
  */
uint32_t RCC_GetPCLK2ClockFreq(uint32_t HCLK_Frequency)
{
  /* PCLK2 clock frequency */
  return LL_RCC_CALC_PCLK2_FREQ(HCLK_Frequency, LL_RCC_GetAPB2Prescaler());
}

/**
  * @brief  Return PCLK3 clock frequency
  * @param  HCLK_Frequency HCLK clock frequency
  * @retval PCLK3 clock frequency (in Hz)
  */
uint32_t RCC_GetPCLK3ClockFreq(uint32_t HCLK_Frequency)
{
  /* PCLK3 clock frequency */
  return LL_RCC_CALC_PCLK3_FREQ(HCLK_Frequency, LL_RCC_GetAPB3Prescaler());
}

/**
  * @brief  Return PCLK4 clock frequency
  * @param  HCLK_Frequency HCLK clock frequency
  * @retval PCLK4 clock frequency (in Hz)
  */
uint32_t RCC_GetPCLK4ClockFreq(uint32_t HCLK_Frequency)
{
  /* PCLK4 clock frequency */
  return LL_RCC_CALC_PCLK4_FREQ(HCLK_Frequency, LL_RCC_GetAPB4Prescaler());
}

/**
  * @}
  */

/**
  * @}
  */

#endif /* defined(RCC) */

/**
  * @}
  */

#endif /* USE_FULL_LL_DRIVER */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
