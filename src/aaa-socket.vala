using Gee;

namespace Aaa {
  public class Socket : ThreadedSocketService {
    private HashMap<string, SocketConnection> peers; // <id, socket>

    public override bool run(SocketConnection conn, Object source_object) {
      message("Oh!");
      return true;
    }

    public int send(string id) {
      return 0; // unimpl
    }
  }
}
