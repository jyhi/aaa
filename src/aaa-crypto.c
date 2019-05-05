#include <stdint.h>
#include <glib.h>
#include "aaa-message.h"

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
