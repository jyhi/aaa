/**
 * AAA Vala bindings to C methods.
 */

[CCode (cheader_filename = "aaa-message.h,aaa-crypto.h")]
namespace Aaa {
  [CCode (has_type_id = false)]
  public enum MessageType {
    HELLO,
    MSG,
    BYE,
  }

  [CCode (cname = "struct AaaMessage", destroy_function = "aaa_message_free", has_type_id = false)]
  public struct Message {
    MessageType type;
    string id;
    string? message;
    string? cert;
  }

  [CCode]
  public string? message_serialize(Message message);

  [CCode]
  public Message message_deserialize(string message_str);

  [CCode]
  public int message_encrypt(
    [CCode (array_length_type = "size_t")] out uint8[] cipher,
    [CCode (array_length_type = "size_t")] out uint8[] nonce,
    [CCode (array_length_type = "size_t")] out uint8[] mac,
    Message message
  );

  [CCode]
  public int message_decrypt(
    out Message message,
    [CCode (array_length_type = "size_t")] uint8[] cipher,
    [CCode (array_length_type = "size_t")] uint8[] nonce,
    [CCode (array_length_type = "size_t")] uint8[] mac
  );
}
