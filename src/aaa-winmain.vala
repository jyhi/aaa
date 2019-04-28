using Gtk;

namespace Aaa {
  [GtkTemplate (ui = "/hk/edu/uic/aaa/ui/aaa-winmain.ui")]
  public class WinMain : ApplicationWindow {
    [GtkChild]
    private HeaderBar headerbar_main;
    [GtkChild]
    private ListBox listbox_users;
    [GtkChild]
    private Button btn_user_add;
    [GtkChild]
    private Button btn_user_del;
    [GtkChild]
    private Stack stack_main;
    [GtkChild]
    private TextView textview_messages;
    [GtkChild]
    private TextView textview_tosend;

    public WinMain() {}
  }
}
