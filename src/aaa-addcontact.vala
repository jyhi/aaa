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
  }
}
