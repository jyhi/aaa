/**
 * Tests for the aaa-crypto module.
 */

#include <stdio.h>
#include <glib.h>
#include <sodium.h>
#include "aaa-crypto.h"

int main(void)
{
  int r = 0;
  char linebuf[4096] = {0};
  struct AaaUserKey *key = NULL;

  // Key generation

  puts("> generating keys");
  r = aaa_keypair_gen(&key);
  if (!r) {
    puts("** failed on key generation");
    return EXIT_FAILURE;
  }

  // Base64 conversion

  // Forward
  char *b64_spk = aaa_bin2base64(key->sign->public_key, key->sign->public_key_length);
  char *b64_ssk = aaa_bin2base64(key->sign->secret_key, key->sign->secret_key_length);
  char *b64_epk = aaa_bin2base64(key->encrypt->public_key, key->encrypt->public_key_length);
  char *b64_esk = aaa_bin2base64(key->encrypt->secret_key, key->encrypt->secret_key_length);

  // Backward
  size_t bin_spk_len = 0, bin_ssk_len = 0, bin_epk_len = 0, bin_esk_len = 0;
  uint8_t *bin_spk = aaa_base642bin(&bin_spk_len, b64_spk);
  uint8_t *bin_ssk = aaa_base642bin(&bin_ssk_len, b64_ssk);
  uint8_t *bin_epk = aaa_base642bin(&bin_epk_len, b64_epk);
  uint8_t *bin_esk = aaa_base642bin(&bin_esk_len, b64_esk);

  puts("> keys generated:");
  printf("spk: %s\nssk: %s\nepk: %s\nesk: %s\n", b64_spk, b64_ssk, b64_epk, b64_esk);

  puts("> converting Base64 representation back to binary and compare");

  // Libsodium constant-time equality test
  if (sodium_memcmp(bin_spk, key->sign->public_key, bin_spk_len) != 0) {
    puts("** bin_spk != key->sign->public_key");
  } else {
    puts("> bin_spk == key->sign->public_key");
  }

  if (sodium_memcmp(bin_ssk, key->sign->secret_key, bin_ssk_len) != 0) {
    puts("** bin_ssk != key->sign->secret_key");
  } else {
    puts("> bin_ssk == key->sign->secret_key");
  }

  if (sodium_memcmp(bin_epk, key->encrypt->public_key, bin_epk_len) != 0) {
    puts("** bin_epk != key->encrypt->public_key");
  } else {
    puts("> bin_epk == key->encrypt->public_key");
  }

  if (sodium_memcmp(bin_esk, key->encrypt->secret_key, bin_esk_len) != 0) {
    puts("** bin_esk != key->encrypt->secret_key");
  } else {
    puts("> bin_esk == key->encrypt->secret_key");
  }

  g_free(b64_esk);
  g_free(b64_epk);
  g_free(b64_ssk);
  g_free(b64_spk);
  g_free(bin_esk);
  g_free(bin_epk);
  g_free(bin_ssk);
  g_free(bin_spk);

  puts("> input a line to encrypt / sign:");
  fgets(linebuf, 4096, stdin);

  // Encryption / Decryption

  puts("> (1/2) encrypting message to yourself...");

  uint8_t *cipher     = NULL;
  size_t   cipher_len = 0;
  uint8_t *nonce      = NULL;
  size_t   nonce_len  = 0;
  uint8_t *mac        = NULL;
  size_t   mac_len    = 0;

  r = aaa_message_encrypt(&cipher, &cipher_len, &nonce, &nonce_len, &mac, &mac_len, key->encrypt->public_key, key->encrypt->public_key_length, key->encrypt->secret_key, key->encrypt->secret_key_length, linebuf);
  if (!r) {
    puts("** error encrypting message!");
    aaa_user_key_free(key);
    return EXIT_FAILURE;
  }

  puts("> encryption succeed");

  char *b64_cipher = aaa_bin2base64(cipher, cipher_len);
  char *b64_nonce  = aaa_bin2base64(nonce, nonce_len);
  char *b64_mac    = aaa_bin2base64(mac, mac_len);

  printf("> cipher text:\n%s\n> nonce:\n%s\n> mac:\n%s\n", b64_cipher, b64_nonce, b64_mac);

  g_free(b64_mac);
  g_free(b64_nonce);
  g_free(b64_cipher);

  puts("> (1/2) decrypting message from yourself...");

  char *plain = NULL;

  r = aaa_message_decrypt(&plain, key->encrypt->public_key, key->encrypt->public_key_length, key->encrypt->secret_key, key->encrypt->secret_key_length, cipher, cipher_len, nonce, nonce_len, mac, mac_len);
  if (!r) {
    puts("** error decrypting message!");
    aaa_user_key_free(key);
    return 1;
  }

  puts("> decryption succeed");

  printf("> plain text:\n%s\n", plain);

  g_free(plain);

  // Sign / Verify

  puts("> (2/2) signing message...");

  uint8_t *sig     = NULL;
  size_t   sig_len = 0;

  r = aaa_message_sign(&sig, &sig_len, key->sign->secret_key, key->sign->secret_key_length, linebuf, strlen(linebuf) + 1);
  if (!r) {
    puts("** error signing message!");
    aaa_user_key_free(key);
    return 1;
  }

  char *b64_sig = aaa_bin2base64(sig, sig_len);

  printf("> signature text:\n%s\n", b64_sig);

  g_free(b64_sig);

  puts("> (2/2) verifying message...");

  r = aaa_message_verify(linebuf, strlen(linebuf) + 1, sig, sig_len, key->sign->public_key, key->sign->public_key_length);
  if (!r) {
    puts("** verification error!");
  } else {
    puts("> verification succeed");
  }

  aaa_user_key_free(key);

  return EXIT_SUCCESS;
}
