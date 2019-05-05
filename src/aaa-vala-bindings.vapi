/**
 * AAA Vala bindings to C methods.
 */

[CCode (cheader_filename = "aaa-message.h,aaa-crypto.h,aaa-packet.h,aaa-configmgr.h")]
namespace Aaa {

  // aaa-message

  [CCode (cname = "enum AaaMessageType", has_type_id = false)]
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

  public string? message_serialize(Message message);

  // FIXME: Vala compiles a struct returned from a function to an outward
  // parameter, which does not fit the C function.
  public Message *message_deserialize(string message_str);

  // aaa-crypto

  [CCode (cname = "struct AaaKeyPair", destroy_function = "aaa_key_pair_free", has_type_id = false)]
  public struct KeyPair {
    [CCode (array_length_type = "size_t", array_length_cname = "public_key_length")] uint8[] public_key;
    [CCode (array_length_type = "size_t", array_length_cname = "secret_key_length")] uint8[] secret_key;
  }

  [CCode (cname = "struct AaaUserKey", destroy_function = "aaa_user_key_free", has_type_id = false)]
  public struct UserKey {
    KeyPair *sign;
    KeyPair *encrypt;
  }

  public int keypair_gen(out UserKey *key);

  public int message_encrypt(
    [CCode (array_length_type = "size_t")] out uint8[] cipher,
    [CCode (array_length_type = "size_t")] out uint8[] nonce,
    [CCode (array_length_type = "size_t")] out uint8[] mac,
    [CCode (array_length_type = "size_t")] uint8[] recipient_pk,
    [CCode (array_length_type = "size_t")] uint8[] sender_sk,
    string message
  );

  public int message_decrypt(
    out string message,
    [CCode (array_length_type = "size_t")] uint8[] sender_pk,
    [CCode (array_length_type = "size_t")] uint8[] recipient_sk,
    [CCode (array_length_type = "size_t")] uint8[] cipher,
    [CCode (array_length_type = "size_t")] uint8[] nonce,
    [CCode (array_length_type = "size_t")] uint8[] mac
  );

  public int message_sign(
    [CCode (array_length_type = "size_t")] out uint8[] signature,
    [CCode (array_length_type = "size_t")] uint8[] sender_sk,
    [CCode (array_length_type = "size_t")] uint8[] message
  );

  public int message_verify(
    [CCode (array_length_type = "size_t")] uint8[] message,
    [CCode (array_length_type = "size_t")] uint8[] signature,
    [CCode (array_length_type = "size_t")] uint8[] sender_pk
  );

  public string bin2base64([CCode (array_length_type = "size_t")] uint8[] bin);

  [CCode (array_length_pos = 0.9, array_length_type = "size_t")]
  public uint8[] base642bin(string base64);

  // aaa-packet

  [CCode (cname = "struct AaaPacket", destroy_function = "aaa_packet_free", has_type_id = false)]
  public struct Packet {
    [CCode (array_length_type = "size_t", array_length_cname = "message_length")] uint8[] message;
    [CCode (array_length_type = "size_t", array_length_cname = "nonce_length")] uint8[] nonce;
    [CCode (array_length_type = "size_t", array_length_cname = "signature_length")] uint8[] signature;
  }

  public string packet_serialize(Packet packet);

  // FIXME: Vala compiles a struct returned from a function to an outward
  // parameter, which does not fit the C function.
  public Packet *packet_deserialize(string packet_str);

  // aaa-configmgr

  public string config_get_id();

  public void config_set_id(string id);

  [CCode (array_length_pos = 0.9, array_length_type = "size_t")]
  public uint8[] config_get_cert();

  public void config_set_cert([CCode (array_length_type = "size_t")] uint8[] cert);

  [CCode (array_length_pos = 0.9, array_length_type = "size_t")]
  public uint8[] config_get_key();

  public void config_set_key([CCode (array_length_type = "size_t")] uint8[] key);

  public int config_load();

  public int config_save();
}
