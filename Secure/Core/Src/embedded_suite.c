/*
 * libstcrypto-example-nucleo_l552ze-q
 * Stanislaw Grams <stanislawgrams@gmail.com>
 *
 * libstcrypto-example-nucleo_l552ze-q_Secure/Core/Src/embedded_suite.c
 * See LICENSE for license information
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <stcrypto.h>
#include <stcrypto-test.h>

#include <mbedtls/aes.h>
#include <mbedtls/chacha20.h>

#include "stm32l5xx_hal.h"
#include "core_cm33.h"

static uint8_t m_debug = 0;
static uint8_t m_ht[256 / 8];

extern HASH_HandleTypeDef hhash;

void
handle_embedded_debug (
  void
  )
{
  m_debug = !m_debug;
}

lstcrypto_status_t
benchmark_aes256_block (
  uint64_t *time,
  uint64_t *cycles,
  uint8_t  *dst,
  uint8_t  *src,
  uint32_t length
  )
{
  lstcrypto_status_t status = C_OK;

  static mbedtls_aes_xts_context aes;
  static uint8_t key[32];
  static uint8_t iv[16];

  __fill_data (key, 32);
  __fill_data (iv, 16);

  if (m_debug) printf ("F [%s] BEGIN\r\n", __FUNCTION__);

  *time   = __tm ();
  *cycles = __cm ();

  mbedtls_aes_xts_setkey_enc (&aes, key, 256);
  mbedtls_aes_xts_init (&aes);
  mbedtls_aes_crypt_xts(&aes, MBEDTLS_AES_ENCRYPT, &length, iv, src, dst);
  mbedtls_aes_xts_free (&aes);

  *time   = __tm () - *time;
  *cycles = __cm () - *cycles;

  if (m_debug) printf ("F [%s] END\r\n", __FUNCTION__);

  return status;
}

lstcrypto_status_t
benchmark_sha256_block (
  uint64_t *time,
  uint64_t *cycles,
  uint8_t  *src,
  uint32_t length
  )
{
  lstcrypto_status_t status = C_OK;

  if (m_debug) printf ("F [%s] BEGIN\r\n", __FUNCTION__);

  *time   = __tm ();
  *cycles = __cm ();

  HAL_HASH_Init(&hhash);
  status = (lstcrypto_status_t)HAL_HASHEx_SHA256_Start (&hhash, src, length, m_ht, HAL_MAX_DELAY);

  *time   = __tm () - *time;
  *cycles = __cm () - *cycles;

  if (m_debug) printf ("F [%s] END\r\n", __FUNCTION__);

  return status;
}

lstcrypto_status_t
benchmark_embedded_algorithms (
  uint64_t *time,
  uint64_t *cycles,
  uint8_t  *dst,
  uint8_t  *src,
  uint32_t length,
  int      rounds
  )
{
  lstcrypto_status_t status     = C_OK;
  uint64_t           tmp_time   = 0;
  uint64_t           tmp_cycles = 0;

  if (m_debug) printf ("F [%s] BEGIN\r\n", __FUNCTION__);

  //
  // AES256 BEGIN
  //
  if (m_debug) printf ("A [%s] BEGIN\r\n", "AES256");
  time[0]   = 0;
  cycles[0] = 0;
  for (int i = 0; i < rounds; i++) {
	if (m_debug) printf ("R [%02x] BEGIN\r\n", i);

	status = benchmark_aes256_block (&tmp_time, &tmp_cycles, src, dst, length);

	if (m_debug) printf ("R [%02x] END (0x%08x%08x) (0x%08x%08x)\r\n", i, (uint32_t)(tmp_time >> 32), (uint32_t)tmp_time, (uint32_t)(tmp_cycles >> 32), (uint32_t)tmp_cycles);

	// avg the time per rounds
	time[0] += tmp_time;
	if (i > 1) {
	  time[0] /= 2;
	}

	// avg cycles
	cycles[0] += tmp_cycles;
	if (i > 1) {
	  cycles[0] /= 2;
	}
  }
  if (m_debug) printf ("A [%s] END\r\n", "AES256");
  //
  // AES256 END
  //

  //
  // SHA256 BEGIN
  //
  if (m_debug) printf ("A [%s] BEGIN\r\n", "SHA256");
  time[1]   = 0;
  cycles[1] = 0;
  for (int i = 0; i < rounds; i++) {
	if (m_debug) printf ("R [%02x] BEGIN\r\n", i);

	status = benchmark_sha256_block (&tmp_time, &tmp_cycles, src, length);

	if (m_debug) printf ("R [%02x] END (0x%08x%08x) (0x%08x%08x)\r\n", i, (uint32_t)(tmp_time >> 32), (uint32_t)tmp_time, (uint32_t)(tmp_cycles >> 32), (uint32_t)tmp_cycles);

	// avg the time per rounds
	time[1] += tmp_time;
	if (i > 1) {
	  time[1] /= 2;
	}

	// avg cycles
	cycles[1] += tmp_cycles;
	if (i > 1) {
	  cycles[1] /= 2;
	}
  }
  if (m_debug) printf ("A [%s] END\r\n", "SHA256");
  //
  // SHA256 END
  //

  return status;
}


lstcrypto_status_t
handle_embedded_benchmark (
  void
  )
{
  lstcrypto_status_t status = C_OK;

  uint64_t time[2]   = {0};
  uint64_t cycles[2] = {0};

  uint32_t rounds = 100;
  uint8_t *dst = NULL; // only for aes
  uint8_t *src = NULL;

  for (int bs = 256; bs <= 16384; bs <<= 1) {
    // allocate blocks of a given bs
    dst = realloc (dst, bs);
    src = realloc (src, bs);

    // fill the src with random data
    status = __fill_data (src, bs);
    if (CRYPTO_ERROR (status)) {
      goto exit;
    }

    // run testing
    status = benchmark_embedded_algorithms (
      time,
	  cycles,
	  (uint8_t *)dst,
	  (uint8_t *)src,
	  bs,
	  rounds
	  );

    printf ("----------------------------------------------------\r\n");
    printf ("EMBEDDED BENCHMARK:\r\n");
    printf ("----------------------------------------------------\r\n");
    printf ("BLOCK LENGTH: %i, ROUNDS: %i\r\n", bs, rounds);
    printf ("----------------------------------------------------\r\n");
    printf ("AES256:\r\n");
    printf ("    Avg time of execution: 0x%08x%08x\r\n", (uint32_t)(time[0] >> 32), (uint32_t)time[1]);
    printf ("    Avg no. of cycles: 0x%08x%08x\r\n", (uint32_t)(cycles[0] >> 32), (uint32_t)cycles[1]);
    printf ("----------------------------------------------------\r\n");
    printf ("SHA256:\r\n");
    printf ("    Avg time of execution: 0x%08x%08x\r\n", (uint32_t)(time[1] >> 32), (uint32_t)time[0]);
    printf ("    Avg no. of cycles: 0x%08x%08x\r\n", (uint32_t)(cycles[1] >> 32), (uint32_t)cycles[0]);
    printf ("----------------------------------------------------\r\n");
  }

  if (src) free (src);
  if (dst) free (dst);

exit:
  return status;
}

lstcrypto_status_t
handle_embedded_benchmark_pp_aes256 (
  uint64_t bs
  )
{
  lstcrypto_status_t status = C_OK;

  uint8_t  *dst   = NULL;
  uint8_t  *src   = NULL;

  static mbedtls_aes_xts_context aes;
  static uint8_t key[32];
  static uint8_t iv[16];

  if (m_debug) printf ("F [%s] BEGIN\r\n", __FUNCTION__);

  // allocate blocks of a given bs
  dst = realloc (dst, bs);
  src = realloc (src, bs);

  // fill the src with random data
  status = __fill_data (src, bs);
  if (CRYPTO_ERROR (status)) {
    printf ("Unable to fill the block with random data!\r\n");
    goto exit;
  }
  status = __fill_data (key, sizeof (key));
  if (CRYPTO_ERROR (status)) {
    printf ("Unable to fill the key block with random data!\r\n");
    goto exit;
  }
  status = __fill_data (iv, sizeof (iv));
  if (CRYPTO_ERROR (status)) {
    printf ("Unable to fill the iv block with random data!\r\n");
    goto exit;
  }

  printf ("Performing AES-XTS 256 infinite loop!\r\n");
  while (1 < 2) {
    mbedtls_aes_xts_setkey_enc (&aes, key, 256);
    mbedtls_aes_xts_init (&aes);
    mbedtls_aes_crypt_xts(&aes, MBEDTLS_AES_ENCRYPT, &bs, iv, src, dst);
    mbedtls_aes_xts_free (&aes);
  }

  if (m_debug) printf ("F [%s] END\r\n", __FUNCTION__);

  if (src) {
    free (src);
  }

  if (dst) {
    free (dst);
  }

exit:
  return status;
}

lstcrypto_status_t
handle_embedded_benchmark_pp_sha256 (
  uint64_t bs
  )
{
  lstcrypto_status_t status = C_OK;

  uint8_t *src = NULL;

  if (m_debug) printf ("F [%s] BEGIN\r\n", __FUNCTION__);

  // allocate block of a given bs
  src = realloc (src, bs);

  // fill the src with random data
  status = __fill_data (src, bs);
  if (CRYPTO_ERROR (status)) {
    printf ("Unable to fill the block with random data!\r\n");
    goto exit;
  }

  printf ("Performing SHA 256 infinite loop!\r\n");
  while (1 < 2) {
	  HAL_HASH_Init (&hhash);
	  status = (lstcrypto_status_t)HAL_HASHEx_SHA256_Start (&hhash, src, bs, m_ht, HAL_MAX_DELAY);
  }

  if (m_debug) printf ("F [%s] END\r\n", __FUNCTION__);

  if (src) {
    free (src);
  }

exit:
  return status;
}


lstcrypto_status_t
embedded_example_nucleo_l552ze_q_suite (
  void
  )
{
  lstcrypto_status_t status = C_OK;

  printf ("[%s] BEGIN\r\n", __FILE__);

  //
  // embedded tests
  //
  // configuration
  //handle_embedded_debug ();
/*
  // benchmark
  status = handle_embedded_benchmark ();
  if (CRYPTO_ERROR (status)) {
    goto exit;
  }
*/
  // power performance
  //HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);

  //status = handle_embedded_benchmark_pp_aes256 (1024*8);
  //status = handle_embedded_benchmark_pp_aes256 (1024*16);
  //status = handle_embedded_benchmark_pp_aes256 (1024*32);

  status = handle_embedded_benchmark_pp_sha256 (1024*8);
  //status = handle_embedded_benchmark_pp_sha256 (1024*16);
  //status = handle_embedded_benchmark_pp_sha256 (1024*32);

  printf ("[%s] END (%02x)\r\n", __FILE__, status);

exit:
  return status;
}
