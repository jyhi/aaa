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

    [GtkCallback]
    private void btn_user_del_clicked_cb() {
      // Remove the selected row
      var selected_row = this.listbox_users.get_selected_row();
      if (selected_row != null) {
        selected_row.destroy();
      }
    }

    [GtkCallback]
    private void listbox_users_row_selected_cb(ListBoxRow? row) {
      if (row == null) {
        // Switch to the welcome page
        this.headerbar_main.set_title("Welcome");
        this.stack_main.set_visible_child_name("stackpage_welcome");
      } else {
        // Fill chat history
        // Show peer id as main title
        // Switch to the chat page
        // unimpl
        this.stack_main.set_visible_child_name("stackpage_messaging");
      }
    }

    [GtkCallback]
    private void btn_send_clicked_cb() {
      var buffer = this.textview_tosend.get_buffer();

      // We want the whole message so get an start iter and an end iter
      TextIter start, end;
      buffer.get_start_iter(out start);
      buffer.get_end_iter(out end);

      var text = buffer.get_text(start, end, false);

      // Clear the text view
      buffer.delete(ref start, ref end);

      // Send message
      // unimpl
    }
  }
}
