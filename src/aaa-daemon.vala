namespace Aaa {
  public class Daemon : Object {
    private SocketConnection conn;
    private bool close_before_return;
    private ChatUI view;

    public Daemon(SocketConnection conn, bool close_before_return, ChatUI view) {
      this.conn = conn;
      this.close_before_return = close_before_return;
      this.view = view;
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

    public void handshake(bool is_server) throws Error {
      if (is_server) {
        debug("handshaking with peer in server (positive) mode...");
        // Receive, then send:
        // - Receive packet
        // - Deserialize packet
        // - Verify packet (hello)
        // - Deserialize message (hello)
        // - Set peer info
        // - Get self info
        // - Serialize message (hello)
        // - Sign packet (hello)
        // - Serialize packet
        // - Send packet
      } else {
        debug("handshaking with peer in client (active) mode...");
        // Send, then receive
        // - Get self info
        // - Serialize message (hello)
        // - Sign packet (hello)
        // - Serialize packet
        // - Send packet
        // - Receive packet
        // - Deserialize packet
        // - Verify packet (hello)
        // - Deserialize message (hello)
        // - Set peer info
      }
    }

    public int loop() {
      for (;;) {
        debug("daemon: waiting for events...");
        // Look for user action and socket event
        // unimpl
      }
    }
  }
}
