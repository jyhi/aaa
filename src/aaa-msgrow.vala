using Gtk;

namespace Aaa {
  [GtkTemplate (ui = "/hk/edu/uic/aaa/ui/aaa-msgrow.ui")]
  public class MsgRow : Box {
    [GtkChild]
    private Label id;
    [GtkChild]
    private Label msg;

    public MsgRow(string id, string msg) {
      this.id.set_text(id);
      this.msg.set_text(msg);
    }
  }
}
