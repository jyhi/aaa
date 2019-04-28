namespace Aaa {
  public class App : Gtk.Application {
    public App() {
      Object(application_id: "hk.edu.uic.aaa", flags: ApplicationFlags.FLAGS_NONE);
    }

    public override void activate() {
      var win_main = new WinMain();
      win_main.show_all();
      this.add_window(win_main);
    }
  }
}
