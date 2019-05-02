#ifndef AAA_PACKET_H
#define AAA_PACKET_H

#include <stdint.h>

/**
 * Data structure representing an AAA packet.
 *
 * Note that all the `uint8_t *` fields may or may not contain a trailing NUL
 * character, so it is important to have a correct corresponding _length size
 * to tell the length of each field.
 */
struct AaaPacket {
  uint8_t *message;          ///< The message field.
  size_t   message_length;   ///< Length of the message field.
  uint8_t *nonce;            ///< The nonce field.
  size_t   nonce_length;     ///< Length of the nonce field.
  uint8_t *signature;        ///< The signature field.
  size_t   signature_length; ///< Length of the signature field.
};

/**
 * Transform an AaaPacket structure into a string.
 *
 * @param packet [in] `struct AaaPacket` to transform.
 * @return String representing the packet in ASCII string. NULL if there is something wrong.
 */
char *aaa_packet_serialize(const struct AaaPacket packet);

/**
 * Transform a valid string representation of AaaPacket into the AaaPacket structure.
 *
 * @param packet_str [in] The string representation of a packet.
 * @return A `struct AaaPacket`. NULL if there is something wrong.
 */
struct AaaPacket *aaa_packet_deserialize(const char * const packet_str);

#endif
