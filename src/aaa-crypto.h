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

/**
 * Transform a binary sequence into its Base64 representation.
 *
 * This function is an encapsulation of libsodium.
 *
 * @param bin [in] Binary data.
 * @param bin_length [in] Length of the binary data.
 * @return A string containing Base64 representaion of bin. The caller is
 *         responsible to free the string.
 */
char *aaa_bin2base64(const uint8_t * const bin, const size_t bin_length);

/**
 * Transform a Base64 representation into its corresponding binary sequence.
 *
 * This function is an encapsulation of libsodium.
 *
 * @param bin_size [out] Size of the output.
 * @param base64   [in]  Base64 representation.
 * @return A binary sequence containing the original data in base64.
 */
uint8_t *aaa_base642bin(size_t *bin_size, const char * const base64);

#endif
