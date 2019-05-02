using Gtk;

namespace Aaa {
  [GtkTemplate (ui = "/hk/edu/uic/aaa/ui/aaa-addcontact.ui")]
  public class DlgAddContact : Dialog {
    [GtkChild]
    private Entry entry_remote;
    [GtkChild]
    private Label lbl_status;
    [GtkChild]
    private Button btn_cancel;
    [GtkChild]
    private Button btn_connect;

    public DlgAddContact() {}

    public void set_status(string? status) {
      if (status != null) {
        this.lbl_status.set_visible(true);
        this.lbl_status.set_text(status);
      } else {
        this.lbl_status.set_visible(false);
      }
    }

    public string get_ip() {
      return this.entry_remote.get_text();
    }
  }
}
