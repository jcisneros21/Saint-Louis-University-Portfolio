import java.net.*;

public interface Phase{
  // Parses the client's message and looks at the values
  // to see if they are correct depending on the format
  public boolean parseMessage(String message);
  // Returns a correct Message to the server
  public String correctMessage();
  // Returns a incorrect Message to the Server
  public String incorrectMessage();
}


class SetupPhase implements Phase{
  private String m_type = null;

  // Checks for correct values.
  // Represents the logic to analyze the values of the
  // setup message. I apologize for the mess. I believe
  // that by looking at each if statement you can determine
  // what it is checking so I don't need to write unnecessary
  // comments.
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
      // Looks to see if correct measurement type
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
      // Looks to see if it is an integer more than 10
      try {
        amount = Integer.parseInt(value);
        if(amount < 10){
          return false;
        }
        return true;
      }
      catch(NumberFormatException e){
        return false;
      }
    }
    else if(type.equals("msg_size")){
      // Looks to see if it is a correct message size
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
        // Checks to see if it is an integer
        amount = Integer.parseInt(value);
        if(amount < 0){
          return false;
        }
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
      if(value.equals("m")){
        return true;
      } 
      else{
        return false;
      }
    }
    else if(type.equals("seq_num")){
      // Makes sure the sequence number is in order
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
      // Measures how many bytes the payload is by
      // the number of characters in the payload
      char payload_array[] = value.toCharArray();
      int payload_size = payload_array.length * 2;
      // Since characters are 2 bytes in java, I will represent
      // the 1 byte message size to 2 bytes, since I can't send
      // a 1 byte character. There shouldn't be a huge differnce
      // between the two.
      if(payload_size == probe_size || payload_size == 2){
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
    
    // Checks each value of the probe message
    int i;
    for(i = 0; i < m_values.length; i++){
      if(!checkMessage(m_types[i], m_values[i])){
        return false;
      }
    }
    
    return true;
  }

  public String correctMessage(){
    return "WOW";
  }

  public String incorrectMessage(){
    return "404 ERROR: Invalid Measurement Message";
  }
}


class TerminationPhase implements Phase{

  public boolean parseMessage(String message){
    String[] m_values = message.split(" ");
   
    // Checks to see if the message size is correct 
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
