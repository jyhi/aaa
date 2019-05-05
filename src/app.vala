namespace Aaa {
  public class App : Gtk.Application {
    public App() {
      Object(application_id: "hk.edu.uic.aaa", flags: ApplicationFlags.FLAGS_NONE);
    }

    public override void activate() {
      // Create front-end and back-end
      var socket_service = new SocketService();
      var win_main = new WinMain();

      // Bi-directional association
      socket_service.set_view(win_main);
      win_main.set_socket_service(socket_service);

      // Load configuration
      if (config_load() == 0) {
        // Let user set an initial one
        win_main.welcome();
      } else {
        // Configuration exists, go to chat page
        win_main.chat();
      }

      // Activate back-end
      socket_service.listen(1234);

      // Activate front-end
      win_main.show_all();
      this.add_window(win_main);
    }
  }
}
