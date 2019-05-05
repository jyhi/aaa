#include <stdint.h>
#include <glib.h>
#include "aaa-message.h"

void aaa_user_key_free(struct AaaUserKey *key)
{
  g_debug("freeing user key");

  // unimpl
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
