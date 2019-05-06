namespace Aaa {
  public class Daemon : Object {
    private SocketConnection conn;
    private bool close_before_return;
    private ChatUI view;
    private string id;
    private uint8[] peer_public_key;

    public Daemon(SocketConnection conn, bool close_before_return, ChatUI view) {
      this.conn = conn;
      this.close_before_return = close_before_return;
      this.view = view;
    }

    public string get_remote_ip() {
      return (this.conn.get_remote_address() as InetSocketAddress).get_address().to_string();
    }

    public uint16 get_remote_port() {
      return (this.conn.get_remote_address() as InetSocketAddress).get_port();
    }

    public string get_remote_id() {
      return this.id;
    }

    public uint8[] get_remote_public_key() {
      return this.peer_public_key;
    }

    public string receive() throws IOError {
      var buffer = new uint8[4096];

      debug("receiving bytes from remote...");

      size_t size = this.conn.get_input_stream().read(buffer);

      message("read %zu bytes", size);

      // Manually NUL-terminate the data to prevent accidents
      buffer[buffer.length] = 0;

      return (string)buffer;
    }

    public void send(string packet) throws IOError {
      size_t size = 0;

      debug("writing bytes to remote...");

      bool r = this.conn.get_output_stream().write_all(packet.data, out size);

      if (r)
        message("written %zu bytes", size);
    }

    public bool handshake(bool is_server) {
      if (is_server) {

        // Receive, then send
        debug("handshaking with peer in server (positive) mode...");

        // - Receive packet
        string recv;
        try {
          recv = this.receive();
        } catch (IOError e) {
          warning("handshake failed during receive");
          return false;
        }

        // - Deserialize packet
        Packet *packet = packet_deserialize(recv);

        // - Deserialize message (hello)
        Message *message = message_deserialize((string)(*packet).message);
        if ((*message).type != MessageType.HELLO) {
          warning("unexpected non-hello packet during handshake");
          return false;
        }

        // - Verify packet (hello)
        // XXX: Since sender public key (certificate) is used during
        // verification, this stage is moved after message deserialzation (it
        // should have been before it).
        uint8[] bin_cert = base642bin((*message).cert);
        int verified = message_verify(packet.message, packet.signature, bin_cert);
        if (verified == 0) {
          warning("failed to verify digital signature");
          return false;
        }

        // - Get self info
        string self_id = config_get_id();
        string b64_self_cert = bin2base64(config_get_cert());
        uint8[] self_key = config_get_key();

        // - Serialize message (hello)
        string message_tosend = message_serialize(Message() {
          type = MessageType.HELLO,
          id   = self_id,
          cert = b64_self_cert
        });

        // - Sign packet (hello)
        uint8[] signature;
        int signed = message_sign(out signature, self_key, message_tosend.data);
        if (signed == 0) {
          warning("message signing error");
          return false;
        }

        // - Serialize packet
        string packet_tosend = packet_serialize(Packet() {
          message = message_tosend.data,
          nonce = null,
          signature = signature
        });

        // - Send packet
        try {
          this.send(packet_tosend);
        } catch (IOError e) {
          warning("failed to send peer hello packet: %s", e.message);
          return false;
        }

        // Remember peer information
        this.id = message.id;
        this.peer_public_key = bin_cert;

      } else {

        // Send, then receive
        debug("handshaking with peer in client (active) mode...");

        // - Get self info
        string self_id = config_get_id();
        string b64_self_cert = bin2base64(config_get_cert());
        uint8[] self_key = config_get_key();

        // - Serialize message (hello)
        string message_tosend = message_serialize(Message() {
          type = MessageType.HELLO,
          id   = self_id,
          cert = b64_self_cert
        });

        // - Sign packet (hello)
        uint8[] signature;
        int signed = message_sign(out signature, self_key, message_tosend.data);
        if (signed == 0) {
          warning("message signing error");
          return false;
        }

        // - Serialize packet
        string packet_tosend = packet_serialize(Packet() {
          message = message_tosend.data,
          nonce = null,
          signature = signature
        });

        // - Send packet
        try {
          this.send(packet_tosend);
        } catch (IOError e) {
          warning("failed to send peer hello packet: %s", e.message);
          return false;
        }

        // - Receive packet
        string recv;
        try {
          recv = this.receive();
        } catch (IOError e) {
          warning("handshake failed during receive");
          return false;
        }

        // - Deserialize packet
        Packet *packet = packet_deserialize(recv);

        // - Deserialize message (hello)
        Message *message = message_deserialize((string)(*packet).message);
        if ((*message).type != MessageType.HELLO) {
          warning("unexpected non-hello packet during handshake");
          return false;
        }

        // - Verify packet (hello)
        // XXX: Since sender public key (certificate) is used during
        // verification, this stage is moved after message deserialzation (it
        // should have been before it).
        int verified = message_verify(packet.message, packet.signature, (*message).cert.data);
        if (verified == 0) {
          warning("failed to verify digital signature");
          return false;
        }

        // Remember peer information
        this.id = (*message).id;
        this.peer_public_key = base642bin((*message).cert);
      }

      message("handshake succedded, connected with peer %s at %s:%u", this.id, this.get_remote_ip(), this.get_remote_port());
      return true;
    }

    public void disconnect() {
      this.conn.close();
    }

    public void bye(bool wait_for_bye) throws Error {
      if (wait_for_bye) {
        // Send bye, then wait for a bye
        // - Serialize message (bye)
        string message_tosend = message_serialize(Message() {
          type = MessageType.BYE,
          id   = this.id
        });

        // - Encrypt packet
        uint8[] cipher;
        uint8[] nonce;
        uint8[] mac;
        int r = message_encrypt(out cipher, out nonce, out mac, this.peer_public_key, config_get_key(), message_tosend);
        if (r == 0) {
          warning("encryption failed");
          return;
        }

        // - Serialize packet
        string packet_tosend = packet_serialize(Packet() {
          message = cipher,
          nonce = nonce,
          signature = mac
        });

        // - Send packet
        try {
          this.send(packet_tosend);
        } catch (IOError e) {
          warning("failed to send peer hello packet: %s", e.message);
          return;
        }

        // - Receive packet
        string recv;
        try {
          recv = this.receive();
        } catch (IOError e) {
          warning("handshake failed during receive");
          return;
        }

        // - Deserialize packet
        Packet *packet = packet_deserialize(recv);

        // - Decrypt packet (bye)
        string msg;
        int decrypted = message_decrypt(out msg, this.peer_public_key, config_get_key(), packet->message, packet->nonce, packet->signature);
        if (decrypted == 0) {
          warning("message decryption failed");
          return;
        }

        // - Deserialize message (hello)
        Message *message = message_deserialize(msg);
        if (message->type != MessageType.BYE) {
          warning("unexpected non-bye packet during handshake");
          return;
        }

        // If bye, we disconnect
        this.disconnect();
      } else {
        // Send bye
        // - Serialize message (bye)
        string message_tosend = message_serialize(Message() {
          type = MessageType.BYE,
          id   = this.id
        });

        // - Encrypt packet
        uint8[] cipher;
        uint8[] nonce;
        uint8[] mac;
        int r = message_encrypt(out cipher, out nonce, out mac, this.peer_public_key, config_get_key(), message_tosend);
        if (r == 0) {
          warning("encryption failed");
          return;
        }

        // - Serialize packet
        string packet_tosend = packet_serialize(Packet() {
          message = cipher,
          nonce = nonce,
          signature = mac
        });

        // - Send packet
        try {
          this.send(packet_tosend);
        } catch (IOError e) {
          warning("failed to send peer hello packet: %s", e.message);
          return;
        }

        // Then we just return
      }
    }

    public int loop() {
      string incoming;

      for (;;) {
        try {
          incoming = this.receive(); // Blocks
        } catch (IOError e) {
          warning("error during receive: %s", e.message);
          return 0;
        }

        debug("new incoming message");

        // unimpl:
        // - Deserialize packet
        // - Verify packet (msg / bye)
        // - Deserialize message (msg / bye)
        // - Update UI
        this.view.push_message("alice", "Someone is knocking the door...");
      }
    }
  }
}
