import java.io.IOException;

public class PerformanceRequest implements java.io.Serializable {
  private String phase;
  private String mtype;
  private int probe_amount;
  private int msize;
  private int delay;

  PerformanceRequest(String phase, String mtype, int probes, int msize, int delay){
    this.phase = phase;
    this.mtype = mtype;
    this.probe_amount = probes;
    this.msize = msize;
    this.delay = delay;
  }

  public String getPhase(){
    return this.phase;
  }
  
  public String getMType(){
    return this.mtype;
  }
  
  public int getProbeAmount(){
    return this.probe_amount;
  }
  
  public int getMSize(){
    return this.msize;
  }

  public int getDelay(){
    return this.delay;
  }

  private void writeObject(java.io.ObjectOutputStream stream)
      throws IOException {
    stream.writeObject(this.phase);
    stream.writeObject(this.mtype);
    stream.writeInt(this.probe_amount);
    stream.writeInt(this.msize);
    stream.writeInt(this.delay);
  }

  private void readObject(java.io.ObjectInputStream stream)
      throws IOException, ClassNotFoundException {
    this.phase = (String) stream.readObject();
    this.mtype = (String) stream.readObject();
    this.probe_amount = stream.readInt();
    this.msize = stream.readInt();
    this.delay = stream.readInt();
  }
}
