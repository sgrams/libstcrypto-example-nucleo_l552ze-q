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

//
// UNCOMMENT A SECTION PER REQUEST BEFORE BUILDING & FLASHING
//
int
lstcrypto_example_nucleo_l552ze_q_suite (
  void
  )
{
  lstcrypto_status_t status = C_OK;

  printf ("[%s] BEGIN\r\n", __FILE__);

  //
  // hash tests
  //

  // configuration
  //handle_hash_debug ();
 /*
  // verification
  status = handle_hash_verification ();
  if (CRYPTO_ERROR (status)) {
    goto exit;
  }

  // benchmark
  status = handle_hash_benchmark ();
  if (CRYPTO_ERROR (status)) {
    goto exit;
  }
*/
  // power performance
  //HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);

  //status = handle_hash_benchmark_pp_photon (1024*8);
  //status = handle_hash_benchmark_pp_photon (1024*16);
  //status = handle_hash_benchmark_pp_photon (1024*32);

  //status = handle_hash_benchmark_pp_quark (1024*8);
  //status = handle_hash_benchmark_pp_quark (1024*16);
  //status = handle_hash_benchmark_pp_quark (1024*32);

  //status = handle_hash_benchmark_pp_spongent (1024*8);
  //status = handle_hash_benchmark_pp_spongent (1024*16);
  //status = handle_hash_benchmark_pp_spongent (1024*32);


  //
  // sc tests
  //

  // configuration
  //handle_sc_debug ();
/*
  // verification
  status = handle_sc_verification ();
  if (CRYPTO_ERROR (status)) {
    goto exit;
  }

  // benchmark
  status = handle_sc_benchmark ();
  if (CRYPTO_ERROR (status)) {
    goto exit;
  }
*/
  // power performance
  //HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);

  //status = handle_sc_benchmark_pp_chacha20 (1024*8);
  //status = handle_sc_benchmark_pp_chacha20 (1024*16);
  //status = handle_sc_benchmark_pp_chacha20 (1024*32);

  //status = handle_sc_benchmark_pp_grain128a (1024*8);
  //status = handle_sc_benchmark_pp_grain128a (1024*16);
  //status = handle_sc_benchmark_pp_grain128a (1024*32);

  //status = handle_sc_benchmark_pp_trivium (1024*8);
  //status = handle_sc_benchmark_pp_trivium (1024*16);
  status = handle_sc_benchmark_pp_trivium (1024*32);

  printf ("[%s] END (%02x)\r\n", __FILE__, status);

exit:
  return status;
}
