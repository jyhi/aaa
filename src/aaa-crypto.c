#include <stdint.h>
#include <glib.h>
#include <sodium.h>
#include "aaa-message.h"

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
  g_debug("generating user keys");

  // unimpl

  return 0;
}

int aaa_message_encrypt(struct AaaMessage *cipher, const struct AaaMessage * const plain)
{
  g_debug("encrypting message sending to %s", plain->id);

  // unimpl

  return 0;
}

int aaa_message_decrypt(struct AaaMessage *plain, const struct AaaMessage * const cipher)
{
  g_debug("decrypting message...");

  // unimpl

  return 0;
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
