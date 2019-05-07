namespace Aaa {
  public class SocketService : ThreadedSocketService {
    // Bind the GUI (view) to socket
    // since we will update it from the daemon later
    private ChatUI view;

    public SocketService() {
      Object(max_threads: -1);
    }

    public ChatUI get_view() {
      return this.view;
    }

    public void set_view(ChatUI view) {
      this.view = view;
    }

    public bool listen(uint16 port) {
      debug("try binding on port %u", port);

      // source_object parameter of ThreadedSocketService.run is not nullable
      // from VAPI of GIO, so the only way is to pass something, otherwise the
      // type checking will fail.
      try {
        this.add_inet_port(port, this);
      } catch (Error e) {
        warning("failed to bind on port %u: %s", port, e.message);
        return false;
      }

      message("listening on port %u", port);
      return true; // later action is triggered in this.run()
    }

    public Daemon? connect(string ip, uint16 port) {
      debug("try connecting to %s:%u", ip, port);

      SocketClient     client = new SocketClient();
      SocketConnection conn;
      try {
        conn = client.connect(new InetSocketAddress.from_string(ip, port));
      } catch (Error e) {
        warning("failed to connect to %s:%u: %s", ip, port, e.message);
        return null;
      }

      message("connected to peer at %s:%u", ip, port);

      // Socket will be closed by the daemon
      var daemon = new Daemon(conn, true, this.view);

      // Handshake
      debug("handshaking with peer at %s:%u", ip, port);
      bool r = daemon.handshake(false); // is "client"
      if (!r) {
        message("handshaking failed with peer at %s:%u", ip, port);
        return null;
      }

      message("handshake succeeded with peer at %s:%u", ip, port);

      // Enter event loop
      new Thread<int>("daemon", daemon.loop);

      return daemon;
    }

    public override bool run(SocketConnection conn, Object source_object) {
      debug("signal run triggered, new incoming connection");

      // Get remote address
      SocketAddress remote_address;
      try {
        remote_address = conn.get_remote_address();
      } catch (Error e) {
        message("failed to get remote address: %s", e.message);
        message("closing connection");
        return true;
      }

      assert(remote_address is InetSocketAddress);

      string remote_ip   = (remote_address as InetSocketAddress).get_address().to_string();
      uint16 remote_port = (remote_address as InetSocketAddress).get_port();

      message("accepting connection from %s:%u", remote_ip, remote_port);

      // Socket is managed by the caller (this thread)
      var daemon = new Daemon(conn, false, this.view);

      // Handshake
      debug("handshaking with the peer at %s:%u", remote_ip, remote_port);
      bool r = daemon.handshake(true); // is "server"
      if (!r) {
        message("handshaking failed with peer at %s:%u", remote_ip, remote_port);
        return true;
      }

      message("handshake succeeded with peer at %s:%u", remote_ip, remote_port);

      // Enter event loop
      // Block, otherwise the socket will be closed
      new Thread<int>("daemon", (daemon.ref() as Daemon).loop).join();

      message("closing connection");
      return true;
    }
  }
}
