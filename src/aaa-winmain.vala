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

    [GtkCallback]
    private void btn_user_add_clicked_cb() {
      var dlg_add_contact = new DlgAddContact();
      int ret = dlg_add_contact.run();

      switch (ret) {
        case ResponseType.OK:
          dlg_add_contact.set_status("Connecting...");

          // Try connecting the peer
          // Get peer ID
          // Insert an online user in contact list
          // unimpl

          break;
        case ResponseType.CANCEL: // fall through
        default:
          break;
      }

      dlg_add_contact.destroy();
    }
  }
}
