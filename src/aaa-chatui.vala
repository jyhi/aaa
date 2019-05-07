namespace Aaa {
  public interface ChatUI : Object {
    public abstract void push_message(string id, string msg);
    public abstract void push_user(string id, string ip);

    // public abstract void remove_user(string id, string ip);
    public abstract void remove_user(string? id, string? ip);
  }
}
