#ifndef AAA_CRYPTO_H
#define AAA_CRYPTO_H

#include <stdint.h>
#include "aaa-message.h"

/**
 * Encrypt an AAA message.
 *
 * @param cipher  [out] Place to hold the cipher text.
 * @param nonce   [out] Place to hold the nonce.
 * @param mac     [out] Place to hold the Poly1305 MAC for authentication.
 * @param message [in]  Plaintext message.
 * @return 0 on error, 1 on success.
 */
int aaa_message_encrypt(uint8_t *cipher,
                        uint8_t *nonce,
                        uint8_t *mac,
                        const struct AaaMessage * const message);

/**
 * Decrypt an AAA message.
 *
 * @param message [out] Plaintext message.
 * @param cipher  [in]  Cipher text.
 * @param nonce   [in]  Nonce.
 * @param mac     [in]  Poly1305 MAC for authentication.
 * @return 0 on error, 1 on success.
 */
int aaa_message_decrypt(struct AaaMessage *message,
                        const uint8_t * const cipher,
                        const uint8_t * const nonce,
                        const uint8_t * const mac);

#endif
