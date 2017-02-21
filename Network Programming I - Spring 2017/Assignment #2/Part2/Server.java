import java.net.*;
import java.io.*;
import java.util.Date;

/* In Java, the socket contains a function called
   getInputStream and getOutputStream to recieve 
   and send packets. I can access these streams with
   other classes that Java utilizes these streams on.
   This stream also has a main stream called InputStream */

// Bits/seconds is too calculate the throughput
// Round time trip is the time it takes to send the
// message, the server recieves the message and
// the server sends it back for the client to recieve it

// To calculate the throughput I would need to get the
// save the message time and save the time it would take
// for the server to recieve the message. bits/sec
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
    //socket.setSoTimeout(10000);
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

        out.writeUTF("Connection Made");

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
          continue;
        }
     
        // Transisition to Measurement Phase
        this.state = new MeasurementPhase(this.probe_amount, this.message_size);
        
        int i;
        boolean closed_flag = false;
        for(i=0; i < this.probe_amount; i++){
          System.out.println("\n\n" + Integer.toString(i) + "\n\n");
          // Read Measurement Message
          clientMessage = in.readUTF();
          System.out.println(clientMessage);
          
      
          if(this.state.parseMessage(clientMessage)){
            out.writeUTF("Correct Message!!!!");
            out.writeUTF(clientMessage);
            if(measurement_type.equals("tput")){
              Date date = new Date();
              String strLong = Long.toString(date.getTime());
              out.writeUTF(strLong);
            } 
           
          }
          else{
            System.out.println("breaked out of the for loop");
            out.writeUTF(this.state.incorrectMessage());
            client.close();
            closed_flag = true;
            break;
          }
        }
        
        if(closed_flag){
          continue;
        }
        
        System.out.println("I left Measurement Phase!");
        // End the connection
        this.state = new TerminationPhase();
        clientMessage = in.readUTF();

        if(this.state.parseMessage(clientMessage)){
          out.writeUTF(this.state.correctMessage());
        }
        else{
          out.writeUTF(this.state.incorrectMessage());
        }   
     
        client.close();
      }
      //catch(SocketTimeoutException s) {
        //System.out.println("Socket Broke!");
        //break;
      //}
      catch(IOException e) {
        System.out.println("IO Exception occured");
        break;
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
