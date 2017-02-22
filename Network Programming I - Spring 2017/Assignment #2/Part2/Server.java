import java.net.*;
import java.io.*;
import java.util.Date;

public class Server extends Thread {
  private ServerSocket socket;
  private Phase state;

  // Measurement Values 
  private String phase = "";
  private String measurement_type = "";
  private int probe_amount = 0;
  private int message_size = 0;
  private int delay_time = 0;
  
  public Server(int port) throws IOException {
    socket = new ServerSocket(port);
  }

  // Initialize Setup Values
  public void setupValues(String values){
    String[] split_values = values.split(" ");
    this.phase = split_values[0];
    this.measurement_type = split_values[1];
    this.probe_amount = Integer.parseInt(split_values[2]);
    this.message_size = Integer.parseInt(split_values[3]);
    this.delay_time = Integer.parseInt(split_values[4]);
  }

  // Reset Values for next Connection
  public void clearValues(){
    this.phase = null;
    this.measurement_type = null;
    this.probe_amount = 0;
    this.message_size = 0;
    this.delay_time = 0;
  } 

  public void run() {
    while(true) {
      try { 
        // Blocks until client connects
        Socket client = socket.accept();

        DataOutputStream out = new DataOutputStream(client.getOutputStream());
        DataInputStream in = new DataInputStream(client.getInputStream());

        System.out.println("Connection Made.");
        out.writeUTF("Connection Made.");

        // Initialize Setup Phase
        this.state = new SetupPhase();
        String clientMessage;

        clientMessage = in.readUTF();
        if(this.state.parseMessage(clientMessage)){
          // Correct Message has been recieved
          setupValues(clientMessage);
          out.writeUTF(this.state.correctMessage());   
        }
        else{
          // Incorrect Message recieve and terminate connection
          out.writeUTF(this.state.incorrectMessage());
          client.close();
          System.out.println("Connection Terminated.");
          continue;
        }
     
        // Transisition to Measurement Phase
        this.state = new MeasurementPhase(this.probe_amount, this.message_size);
        
        int i;
        boolean closed_flag = false;
        // Loop until it recieves all probe messages
        for(i=0; i < this.probe_amount; i++){
          clientMessage = in.readUTF();
      
          if(this.state.parseMessage(clientMessage)){
            // Sends different values depending on Measurement type
            if(measurement_type.equals("rtt")){
              Thread.sleep(this.delay_time);
              out.writeUTF(clientMessage);
            }
            else if(measurement_type.equals("tput")){
              Date date = new Date();
              Thread.sleep(this.delay_time);
              String strLong = Long.toString(date.getTime());
              out.writeUTF(strLong);
            } 
           
          }
          else{
            out.writeUTF(this.state.incorrectMessage());
            client.close();
            System.out.println("Connection Terminated.");
            closed_flag = true;
            break;
          }
        }
        
        // Skip the current while loop and look for new connection
        if(closed_flag){
          continue;
        }
        
        // End the connection
        this.state = new TerminationPhase();
        clientMessage = in.readUTF();

        // We always close the socket, just sends different messages
        // depending on what the client sends to the server
        if(this.state.parseMessage(clientMessage)){
          out.writeUTF(this.state.correctMessage());
        }
        else{
          out.writeUTF(this.state.incorrectMessage());
        }   
     
        client.close();
        System.out.println("Connection Terminated");
      }
      catch(IOException e) {
        System.out.println("IO Exception occured.");
        break;
      }
      catch(InterruptedException e){
        System.out.println("Interrupted Exception occured.");
      }
    }
  }

  public static void main(String[] args) {
    int port = Integer.parseInt(args[0]);
    try {
      Thread t = new Server(port);
      t.start();
    }
    catch(Exception e){
      System.out.println("Exception Happened");
    }
  }
}
