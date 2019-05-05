#ifndef AAA_CRYPTO_H
#define AAA_CRYPTO_H

#include <stdint.h>
#include "aaa-message.h"

/**
 * A pair of key (public key and private key).
 */
struct AaaKeyPair {
  uint8_t *public_key;        ///< Public key
  size_t   public_key_length; ///< Length of public key
  uint8_t *secret_key;        ///< Secret key
  size_t   secret_key_length; ///< Length of secret key
};

/**
 * A group of user keys.
 */
struct AaaUserKey {
  struct AaaKeyPair sign;    ///< Key pair for signing
  struct AaaKeyPair encrypt; ///< Key pair for encryption
};

/**
 * Securely free AaaUserKey.
 *
 * This usually involves several techniques to wipe the memory, but generally
 * just a wrapper of libsodium's safe memory APIs.
 *
 * @param key [in] The AaaUserKey.
 */
void aaa_user_key_free(struct AaaUserKey *key);

/**
 * Generate user key, which contains two key pairs, one for signing, and one for
 * encryption.
 *
 * @param key [out] The generated key.
 * @return 0 for failure, 1 for success. On failure, key is remained untouched.
 */
int aaa_keypair_gen(struct AaaUserKey *key);

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
