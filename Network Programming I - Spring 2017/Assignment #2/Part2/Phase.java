import java.net.*;

public interface Phase{
  public boolean parseMessage(String message);
  public String correctMessage();
  public String incorrectMessage();
}

// Done
class SetupPhase implements Phase{
  private String m_type = null;

  public boolean correctValues(String type, String value){
    int i;
    int amount;
    boolean found_size;
    int[] rtt_sizes = {1, 100, 200, 400, 800, 1000};
    int[] tput_sizes = {1000, 2000, 4000, 8000, 16000, 32000};

    if(type.equals("phase")){
      if(value.equals("s")){
        return true;
      }
    }
    else if(type.equals("m-type")){
      if(value.equals("rtt")){
        this.m_type = "rtt";
        return true;
      }
      else if(value.equals("tput")){
        this.m_type = "tput";
        return true;
      }
    }
    else if(type.equals("probes")){
      try {
        Integer.parseInt(value);
        return true;
      }
      catch(NumberFormatException e){
        return false;
      }
    }
    else if(type.equals("msg_size")){
      try {
        amount = Integer.parseInt(value);
        if(this.m_type.equals("rtt")){
          for(i=0; i < rtt_sizes.length; i++){
            if(amount == rtt_sizes[i]){
              return true;
            }
          }
        }
        else if(this.m_type.equals("tput")){
          for(i=0; i < tput_sizes.length; i++){
            if(amount == tput_sizes[i]){
              return true;
            }
          }
        }
      }
      catch(NumberFormatException e){
        return false;
      }
    }
    else if(type.equals("delay")){
      try {
        Integer.parseInt(value);
        return true;
      }
      catch(NumberFormatException e){
        return false;
      }
    }
     
    return false;
  }

  public boolean parseMessage(String message){
    int i;
    String[] m_values = message.split(" ");
    String[] m_types = {"phase", "m-type", "probes", "msg_size", "delay"};
    
    if(m_values.length != 5){
      return false;
    }
    
    // Check each value to get correct measurement
    for(i=0; i < m_values.length; i++){
      if(!correctValues(m_types[i], m_values[i])){
        return false;
      }
    }
      
    System.out.println("Setting Up.");
    return true;
  }

  public String correctMessage(){
    return "200 OK: Ready";
  }

  public String incorrectMessage(){
    return "404 ERROR: Invalid Connection Setup Message";
  }
}

class MeasurementPhase implements Phase{
  
  private int sequence_num = 0;
  private int probe_amount;
  private int probe_size;
  
  public MeasurementPhase(int probe_amount, int probe_size){
    this.probe_amount = probe_amount;
    this.probe_size = probe_size;
  }

  public boolean checkMessage(String type, String value){
    int seq_holder = 0;

    if(type.equals("phase")){
      System.out.println("In phase");
      if(value.equals("m")){
        return true;
      } 
      else{
        return false;
      }
    }
    else if(type.equals("seq_num")){
      try{
        seq_holder = Integer.parseInt(value);
        if(seq_holder == this.sequence_num + 1){
          sequence_num = seq_holder;
          return true;
        }
        return false;
      }
      catch(NumberFormatException e){
        return false;
      }
    }
    else if(type.equals("payload")){
      char payload_array[] = value.toCharArray();
      int payload_size = payload_array.length * 2;
      System.out.println("The payload size is: " + Integer.toString(payload_size));
      System.out.println("The probe_amount is: " + Integer.toString(probe_size));
      if(payload_size == probe_size){
        return true;
      }
      else{
        return false;
      }
    }
    return false;
  }

  public boolean parseMessage(String message){
    String[] m_values = message.split(" ");
    String[] m_types = {"phase", "seq_num", "payload"};

    // Checks to see if there is more or less parameters needed
    if(m_values.length != 3){
      return false;
    }

    int i;
    for(i = 0; i < m_values.length; i++){
      System.out.println("In the loop");
      if(!checkMessage(m_types[i], m_values[i])){
        return false;
      }
    }
    
    System.out.println("Returning true");
    return true;
  }

  public String correctMessage(){
    return "WOW";
  }

  public String incorrectMessage(){
    return "404 ERROR: Invalid Measurement Message";
  }
}

// Done
class TerminationPhase implements Phase{

  public boolean parseMessage(String message){
    String[] m_values = message.split(" ");
    
    // If the client sent more than one parameter
    if(m_values.length != 1){
      return false;
    }
    
    // Check to see if the protocol phase is correct
    return m_values[0].equals("t") ? true : false;  
    
  }

  public String correctMessage(){
    return "200 OK: Closing Connection";
  }

  public String incorrectMessage(){
    return "404 ERROR: Invalid Connection Termination Message";
  }

}

class PhaseTesting {
  public static void main( String[] args ){
  }
}
