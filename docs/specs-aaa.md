# AAA Protocol Specification

This document specifies the protocol used by aaa, which is an end-to-end encrypted peer-to-peer instant messaging software.

## Overall Structure

An aaa packet consists of 3 parts:

1. `Header` is the signature (magic number) of packet
2. `Message` is the message to deliver
3. `Signature` is the cryptographic signature signed upon `Message` by the sender

```
[Header]![Message],[Signature].
```

In which,

1. `Header` is `AAA`
2. `Message` is `Base64(Encrypt(Json(...)))`
3. `Signature` is `Base64(Sign(Message))`

`Header` and `Message` are divided with an exclamation mark (`!`), `Message` and `Signature` are divided with a comma (`,`). The whole message ends with a period (`.`). The serialized packet is an ASCII string.

### Header

Header acts as a magic number of an aaa packet:

```
AAA!
```

Every packet sent by aaa is prefixed with this header.

### Message

Message field contains messages sending from a sender to a receiver.

The message is encoded in base64. Content of the message is either encrypted with the receipent's public key, or unencrypted, depending on types of the message (see below on message types).

### Signature

Signature field contains cryptographic signature to the encrypted message with the sender's private key.

Every aaa packet comes with a vaild signature from the sender. The signature is made upon the message field: if the message is encrypted, then the signature is made upon the encrypted content; otherwise it is made upon the plaintext message.

## Types of AAA Packets

There are 3 types of AAA message:

1. Hello (`"hello"`)
2. Message (`"msg"`)
3. Bye (`"bye"`)

### Hello Packet

A hello packet initiates a chat from one client to the other, with public key exchanging.

```json
{
  "type": "hello",
  "id": "id",
  "cert": "cert"
}
```

- `"type"`: String value `hello`, indicating type of the message.
- `"id"`: String value, containing identity of the sender.
- `"cert"`: String value, containing the certificate of sender. The value is base64-encoded.

A hello packet is generally **NOT encrypted**, since there is no knowledge on the receiver's certificate (public key) in prior. The signature is made directly upon the JSON string.

### Message Packet

A message packet contains a message from a sender to a receiver.

```json
{
  "type": "msg",
  "msg": "How's it going?"
}
```

- `"type"`: String value `msg`, indicating type of the message.
- `"msg"`: String value, containing message body from a sender.

A message packet MUST be encrypted, otherwise it is invalid.

### Bye Packet

A bye packet is sent on chat termination to inform the other side to gracefully shut down the connection.

```json
{
  "type": "bye"
}
```

- `"type"`: String value `type`, indicating type of the message.

Suppose client `A` wants to end the chat. `A` sends a bye packet to client `B`, and then it waits until `B` sends a bye packet back. `B` also waits for connection closing (if applicable) after sending the bye packet. Then, `A` terminates the connection (if applicable).

```
A             B
| --- bye --> | <- A waits
| <-- bye --- | <- B waits
| X           | <- A terminates the connection
```

A bye packet MUST be encrypted, otherwise it is invalid.

## Unexpected Packets

If a client receives unexpected packets:

- not an aaa packet
- other types of packet during handshake (hello)
- a hello attempt during messaging (after handshaking)
- other types of packet during handwaving (bye)

It should immediately stops transmission and terminates connection.
