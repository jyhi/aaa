using Gtk;

namespace Aaa {
  public enum ContactStatus {
    ONLINE,
    OFFLINE,
  }

  [GtkTemplate (ui = "/hk/edu/uic/aaa/ui/aaa-contactrow.ui")]
  public class ContactRow : Box {
    [GtkChild]
    private Image icon;
    [GtkChild]
    private Label id;
    [GtkChild]
    private Label addr;

    public ContactRow(ContactStatus status, string id, string addr) {
      switch (status) {
        case ContactStatus.ONLINE:
          this.icon.set_from_icon_name("user-available-symbolic", IconSize.DIALOG);
          break;
        case ContactStatus.OFFLINE: // fall through
        default:
          this.icon.set_from_icon_name("user-offline-symbolic", IconSize.DIALOG);
          break;
      }

      this.id.set_text(id);
      this.addr.set_text(addr);
    }

    public string get_id() {
      return this.id.get_text();
    }

    public string get_addr() {
      return this.addr.get_text();
    }
  }
}
