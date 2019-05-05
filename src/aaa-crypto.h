#ifndef AAA_CRYPTO_H
#define AAA_CRYPTO_H

#include <stdint.h>
#include "aaa-message.h"

/**
 * Encrypt an AAA message.
 *
 * @param cipher [out] Ciphertext message.
 * @param plain  [in]  Plaintext message.
 * @return 0 on error, 1 on success.
 */
int aaa_message_encrypt(struct AaaMessage *cipher, const struct AaaMessage * const plain);

/**
 * Decrypt an AAA message.
 *
 * @param plain  [out]  Plaintext message.
 * @param cipher [in] Ciphertext message.
 * @return 0 on error, 1 on success.
 */
int aaa_message_decrypt(struct AaaMessage *plain, const struct AaaMessage * const cipher);

#endif
