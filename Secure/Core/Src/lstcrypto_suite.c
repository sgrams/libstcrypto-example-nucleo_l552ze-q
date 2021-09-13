/*
 * libstcrypto-example-nucleo_l552ze-q
 * Stanislaw Grams <stanislawgrams@gmail.com>
 *
 * libstcrypto-example-nucleo_l552ze-q_Secure/Core/Src/lstcrypto_suite.c
 * See LICENSE for license information
 */
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <stcrypto.h>
#include <stcrypto-test.h>

#include "stm32l5xx_hal.h"
#include "core_cm33.h"

extern RNG_HandleTypeDef hrng;

int
lstcrypto_example_nucleo_l552ze_q_suite (
  void
  )
{
  lstcrypto_status_t status = C_OK;

  printf ("[%s] BEGIN\r\n", __FILE__);

  //
  // configuration
  //

  handle_debug ();

  //
  // verification
  //

  /*
  // sc
  status = handle_sc_verification ();
  if (CRYPTO_ERROR (status)) {
    goto exit;
  }
  */
  // hash
  status = handle_hash_verification ();
  if (CRYPTO_ERROR (status)) {
    goto exit;
  }

  //
  // benchmarks
  //

  // sc
  /*
  status = handle_sc_benchmark ();
  if (CRYPTO_ERROR (status)) {
    goto exit;
  }
  */

  // hash
  status = handle_hash_benchmark ();
  if (CRYPTO_ERROR (status)) {
    goto exit;
  }

  printf ("[%s] END (%02x)\r\n", __FILE__, status);

exit:
  return status;
}


//
// hardware-specific test functions
//

uint32_t
__tm (
  void
  )
{
  return HAL_GetTick (); // in miliseconds
}

uint64_t
__cm (
  void
  )
{
  uint32_t       cycles = 0;
  static uint8_t has_been_started = 0;

  volatile uint32_t *LAR  = (uint32_t *) 0xE0001FB0;

  if (!has_been_started) {
	CoreDebug->DEMCR |= 0x01000000;
	*LAR = 0xC5ACCE55;
	DWT->CTRL = 0;
    DWT->CTRL |= 0x00000001;
    DWT->CYCCNT = 0;
    has_been_started = 1;
    cycles = DWT->CYCCNT;
  } else {
	cycles = DWT->CYCCNT;
	DWT->CTRL &= 0xFFFFFFFE;
	has_been_started = 0;
  }

  return cycles;
}

lstcrypto_status_t
__fill_data (
  uint8_t *block,
  int bs
  )
{
  lstcrypto_status_t status = C_OK;
  HAL_StatusTypeDef  hal_status = HAL_OK;

  for (int i = 0; i < bs / 4; i++) {
    uint32_t rand;
    hal_status = HAL_RNG_GenerateRandomNumber (&hrng, &rand);
    if (hal_status != HAL_OK) {
      status = C_INVALID;
      goto exit;
    }

    block[i + 0] = (uint8_t)(rand & 0x000000ff) >> (0 * 8);
    block[i + 1] = (uint8_t)(rand & 0x0000ff00) >> (1 * 8);
    block[i + 2] = (uint8_t)(rand & 0x00ff0000) >> (2 * 8);
    block[i + 3] = (uint8_t)(rand & 0xff000000) >> (3 * 8);
  }

exit:
  return status;
}
