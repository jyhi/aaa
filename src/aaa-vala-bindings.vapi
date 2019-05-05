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
  public int message_encrypt(out Message cipher, Message plain);

  [CCode]
  public int message_decrypt(out Message plain, Message cipher);
}
