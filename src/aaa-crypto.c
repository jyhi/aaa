#include <stdint.h>
#include <glib.h>
#include <sodium.h>
#include "aaa-configmgr.h"
#include "aaa-crypto.h"

void aaa_user_key_free(struct AaaUserKey *key)
{
  g_debug("freeing user key pairs");

  if (key->sign->public_key_length > 0)
    sodium_free(key->sign->public_key);
  if (key->sign->secret_key_length > 0)
    sodium_free(key->sign->secret_key);

  sodium_free(key->sign);

  if (key->encrypt->public_key_length > 0)
    sodium_free(key->encrypt->public_key);
  if (key->encrypt->secret_key_length > 0)
    sodium_free(key->encrypt->secret_key);

  sodium_free(key->encrypt);

  sodium_free(key);
}

int aaa_keypair_gen(struct AaaUserKey *key)
{
  if (sodium_init() == -1) {
    g_warning("%s: sodium_init failed", __func__);
    return 0;
  }

  g_debug("generating user keys");

  int r = 0;

  /* Memory allocation */

  struct AaaUserKey *ret = sodium_malloc(sizeof(struct AaaUserKey));
  if (!ret)
    goto mem_fail;

  ret->sign = sodium_malloc(sizeof(struct AaaKeyPair));
  if (!(ret->sign))
    goto mem_fail;

  ret->encrypt = sodium_malloc(sizeof(struct AaaKeyPair));
  if (!(ret->encrypt))
    goto mem_fail;

  // Ed25519 for signing

  ret->sign->public_key = sodium_malloc(crypto_sign_ed25519_PUBLICKEYBYTES);
  if (!(ret->sign->public_key))
    goto mem_fail;

  ret->sign->secret_key = sodium_malloc(crypto_sign_ed25519_SECRETKEYBYTES);
  if (!(ret->sign->secret_key))
    goto mem_fail;

  ret->sign->public_key_length = crypto_sign_ed25519_PUBLICKEYBYTES;
  ret->sign->secret_key_length = crypto_sign_ed25519_SECRETKEYBYTES;

  // Curve25519 for encryption

  ret->encrypt->public_key = sodium_malloc(crypto_scalarmult_curve25519_BYTES);
  if (!(ret->encrypt->public_key))
    goto mem_fail;

  ret->encrypt->secret_key = sodium_malloc(crypto_scalarmult_curve25519_BYTES);
  if (!(ret->encrypt->secret_key))
    goto mem_fail;

  ret->encrypt->public_key_length = crypto_scalarmult_curve25519_BYTES;
  ret->encrypt->secret_key_length = crypto_scalarmult_curve25519_BYTES;

  g_debug("keygen: memory allocation succeeded");

  // The generation process is described in:
  // https://download.libsodium.org/doc/advanced/ed25519-curve25519

  r = crypto_sign_ed25519_keypair(ret->sign->public_key, ret->sign->secret_key);
  if (r < 0) {
    g_warning("keygen: crypto_sign_ed25519_keypair returned %d, indicating an error.", r);
    aaa_user_key_free(ret);
    return 0;
  }

  g_debug("keygen: Ed25519 keypair generation succeeded, converting to Curve25519");

  r = crypto_sign_ed25519_pk_to_curve25519(ret->encrypt->public_key, ret->sign->public_key);
  if (r < 0) {
    g_warning("keygen: crypto_sign_ed25519_pk_to_curve25519 returned %d, indicating an error.", r);
    aaa_user_key_free(ret);
    return 0;
  }

  r = crypto_sign_ed25519_sk_to_curve25519(ret->encrypt->secret_key, ret->encrypt->secret_key);
  if (r < 0) {
    g_warning("keygen: crypto_sign_ed25519_sk_to_curve25519 returned %d, indicating an error.", r);
    aaa_user_key_free(ret);
    return 0;
  }

  g_debug("keygen: process finished without errors");

  return 1;

mem_fail:
  g_error("keygen: secure memory allocation failed, this is unacceptable.");
  return 1; // unreachable
}

int aaa_message_encrypt(uint8_t *cipher,
                        size_t  *cipher_length,
                        uint8_t *nonce,
                        size_t  *nonce_length,
                        uint8_t *mac,
                        size_t  *mac_length,
                        const uint8_t * const recipient_pk,
                        const size_t recipient_pk_length,
                        const char * const message)
{
  if (sodium_init() == -1) {
    g_warning("%s: sodium_init failed", __func__);
    return 0;
  }

  g_debug("encrypting message");

  int r = 0;

  // Calculate length of the plaintext message
  size_t message_len = strlen(message);

  // Generate one-time number (nonce) for encryption use
  uint8_t *nonce = crypto_box_NONCEBYTES;
  randombytes_buf(nonce, crypto_box_NONCEBYTES);

  // Get user private key
  size_t key_size = 0;
  uint8_t key = aaa_config_get_key(&key_size);

  // Ignite
  r = crypto_box_detached(cipher, mac, message, message_len, nonce, recipient_pk, key);
  if (r < 0) {
    g_warning("%s: crypto_box_detached returned %d, indicating an error.", r);
    return 0;
  }

  // Fill lengths
  *cipher_length = message_len;
  *nonce_length  = crypto_box_NONCEBYTES;
  *mac_length    = crypto_box_MACBYTES;

  g_debug("encryption succeeded");

  return 1;
}

int aaa_message_decrypt(char *message,
                        const uint8_t * const sender_pk,
                        const size_t          sender_pk_length,
                        const uint8_t * const cipher,
                        const size_t          cipher_length,
                        const uint8_t * const nonce,
                        const size_t          nonce_length,
                        const uint8_t * const mac,
                        const size_t          mac_length)
{
  if (sodium_init() == -1) {
    g_warning("%s: sodium_init failed", __func__);
    return 0;
  }

  g_debug("decrypting message");

  int r = 0;

  // Calculate length of the plaintext message
  size_t message_len = strlen(message);

  // Get user private key
  size_t key_size = 0;
  uint8_t key = aaa_config_get_key(&key_size);

  // Nike
  r = crypto_box_open_detached(message, cipher, mac, cipher_length, nonce, sender_pk, key);
  if (r < 0) {
    g_warning("%s: crypto_box_open_detached returned %d, indicating an error.", r);
    return 0;
  }

  g_debug("decryption succeeded");

  return 1;
}

char *aaa_bin2base64(const uint8_t * const bin, const size_t bin_length)
{
  g_debug("transforming binary data to base64...");

  // unimpl

  return NULL;
}

uint8_t *aaa_base642bin(size_t *bin_size, const char * const base64)
{
  g_debug("transforming base64 to binary data...");

  // unimpl

  return NULL;
}
