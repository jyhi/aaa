#ifndef AAA_MESSAGE_H
#define AAA_MESSAGE_H

/**
 * Types of AAA messages.
 */
enum AaaMessageType {
  AAA_MESSAGE_TYPE_HELLO, ///< Hello message for key exchange.
  AAA_MESSAGE_TYPE_MSG,   ///< Message message for messaging.
  AAA_MESSAGE_TYPE_BYE,   ///< Bye message for closing connection.
};

/**
 * A piece of AAA message.
 */
struct AaaMessage {
  enum AaaMessageType type; ///< Type of the message.
  char *id;                 ///< Remote user ID.
  union {
    char *message;          ///< Message sent from user, used in AAA_MESSAGE_TYPE_MSG.
    char *cert;             ///< Certificate from user, used in AAA_MESSAGE_TYPE_HELLO.
  };
};

/**
 * Transform a `struct AaaMessage` into its corresponding string representation.
 *
 * @param message [in] The C structure representation of the AAA message.
 * @return A string representation of the message. NULL if there is something wrong.
 */
char *aaa_message_serialize(const struct AaaMessage * const message);

/**
 * Transform a valid AAA message string representation into C structure.
 *
 * @param message [in] The string representation of AaaMessage.
 * @return A corresponding `struct AaaMessage`. NULL if there is something wrong.
 */
struct AaaMessage *aaa_message_deserialize(const char * const message_str);

#endif
