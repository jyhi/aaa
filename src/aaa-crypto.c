#include <stdio.h>
#include <stdint.h>

int aaa_message_encrypt(uint8_t *cipher,
                        size_t  *cipher_length,
                        uint8_t *nonce,
                        size_t  *nonce_length,
                        uint8_t *mac,
                        size_t  *mac_length,
                        const struct AaaMessage * const message)
{
  debug("encrypting message sending to %s", message->id);

  // unimpl

  return 0;
}


int aaa_message_decrypt(struct AaaMessage *message,
                        const uint8_t * const cipher,
                        const size_t  * const cipher_length,
                        const uint8_t * const nonce,
                        const size_t  * const nonce_length,
                        const uint8_t * const mac,
                        const size_t  * const mac_length)
{
  debug("decrypting message sent from %s", message->id);

  // unimpl

  return 0;
}
