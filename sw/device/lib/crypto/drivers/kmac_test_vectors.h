// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
#ifndef OPENTITAN_SW_DEVICE_LIB_CRYPTO_DRIVERS_KMAC_TEST_VECTORS_H_
#define OPENTITAN_SW_DEVICE_LIB_CRYPTO_DRIVERS_KMAC_TEST_VECTORS_H_

#include "sw/device/lib/crypto/drivers/kmac.h"

// This struct allows us to neatly pack NIST CAVP vectors
typedef struct kmac_test_vector {
  kmac_operation_t operation;
  char func_name[36];
  char custom_str[36];
  uint8_t *key;
  size_t key_len;
  uint8_t *input_str;
  size_t input_str_len;
  uint8_t *digest;
  size_t digest_len;
} kmac_test_vector_t;

extern size_t nist_kmac_nr_of_vectors;
extern kmac_test_vector_t *nist_kmac_vectors[];

#endif  // OPENTITAN_SW_DEVICE_LIB_CRYPTO_DRIVERS_KMAC_TEST_VECTORS_H_
