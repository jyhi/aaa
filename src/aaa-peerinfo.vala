using Gtk;

namespace Aaa {
  [GtkTemplate (ui = "/hk/edu/uic/aaa/ui/aaa-peerinfo.ui")]
  public class PeerInfo : Dialog {
    [GtkChild]
    private Label lbl_id;
    [GtkChild]
    private Label lbl_fingerprint;
    [GtkChild]
    private Label lbl_trust_prompt;
    [GtkChild]
    private Button btn_yes;
    [GtkChild]
    private Button btn_no;

    public PeerInfo(string id, string fp, bool trust) {
      this.lbl_id.set_text(id);
      this.lbl_fingerprint.set_text(fp);

      if (trust) {
        lbl_trust_prompt.set_visible(true);
        btn_no.set_visible(true);
      } else {
        lbl_trust_prompt.set_visible(false);
        btn_no.set_visible(false);
      }
    }
  }
}
