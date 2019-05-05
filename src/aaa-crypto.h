#ifndef AAA_CRYPTO_H
#define AAA_CRYPTO_H

#include <stdint.h>

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
  struct AaaKeyPair *sign;    ///< Key pair for signing
  struct AaaKeyPair *encrypt; ///< Key pair for encryption
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
 * Encrypt an AAA message with the given recipient's public key.
 *
 * @param cipher              [out] Place to hold the cipher text.
 * @param cipher_length       [out] Place to hold the length of the cipher text.
 * @param nonce               [out] Place to hold the nonce.
 * @param nonce_length        [out] Place to hold the length of the nonce.
 * @param mac                 [out] Place to hold the Poly1305 MAC for authentication.
 * @param mac_length          [out] Place to hold the length of the Poly1305 MAC for authentication.
 * @param recipient_pk        [in]  Public key to the receipent of the message.
 * @param recipient_pk_length [in]  Length of the public key above.
 * @param message             [in]  Plaintext message.
 * @return 0 on error, 1 on success.
 */
int aaa_message_encrypt(uint8_t *cipher,
                        size_t  *cipher_length,
                        uint8_t *nonce,
                        size_t  *nonce_length,
                        uint8_t *mac,
                        size_t  *mac_length,
                        const uint8_t * const recipient_pk,
                        const size_t recipient_pk_length,
                        const char * const message);

/**
 * Decrypt an AAA message with the user's private key.
 *
 * @param message             [out] Plaintext message.
 * @param recipient_pk        [in]  Public key of the sender of the message.
 * @param recipient_pk_length [in]  Length of the public key above.
 * @param cipher              [in]  Cipher text.
 * @param cipher_length       [in]  Length of the cipher text.
 * @param nonce               [in]  Nonce.
 * @param nonce_length        [in]  Length of the nonce.
 * @param mac                 [in]  Poly1305 MAC for authentication.
 * @param mac_length          [in]  Length of the Poly1305 MAC for authentication.
 * @return 0 on error, 1 on success.
 */
int aaa_message_decrypt(char *message,
                        const uint8_t * const sender_pk,
                        const size_t sender_pk_length,
                        const uint8_t * const cipher,
                        const size_t          cipher_length,
                        const uint8_t * const nonce,
                        const size_t          nonce_length,
                        const uint8_t * const mac,
                        const size_t          mac_length);

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
