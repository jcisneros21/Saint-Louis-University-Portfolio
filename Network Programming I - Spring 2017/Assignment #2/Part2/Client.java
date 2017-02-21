import java.net.*;
import java.io.*;
import java.util.*;

public class Client {
  
  // Checks all server messages for close messages
  public static boolean exitSocket(String reply){
    String exit_messages[] = {"404 ERROR: Invalid Connection Setup Message",
                           "404 ERROR: Invalid Measurement Message",
                           "404 ERROR: Invalid Connection Termination Message",
                           "200 OK: Closing Connection"};
    int i;
    for(i = 0; i < exit_messages.length; i++){
      if(reply.equals(exit_messages[i])){
        return true;
      }
    }
    return false;
  }

  // Creates the payload for measurement
  public static String create_payload(int msg_size){
    int size = msg_size / 2;
    char[] payload_array = new char[size];
    Arrays.fill(payload_array, '0');
    String payload = new String(payload_array);
    return payload;
  }

  // Creates the payload message  
  public static String create_probe_message(int seq_num, int msg_size){
    String sequence_number = Integer.toString(seq_num);
    return "m " + sequence_number + " " + create_payload(msg_size);
  }

  public static int measure_tput(long time_sent, long server_recieved, int byte_amount){
    int sent_time = (int) (server_recieved - time_sent);
    return ( byte_amount / sent_time );
  }

  public static int measure_rtt(long sent_time, long recieved_time){
    System.out.println(recieved_time);
    System.out.println(sent_time);
    return (int) (recieved_time - sent_time);
  }

  public static void print_measurements(int measurements[]){
    for(int i = 0; i < measurements.length; i++){
      System.out.println("Test #" + i + ": " + measurements[i]);
    }
  }

  public static void main(String[] args) {
    
    if(args.length != 2){
      System.out.println("Usage: java Client <hostname> <port>");
    }

    // Retrieve Hostname and Port number
    String hostname = args[0]; 
    int port = Integer.parseInt(args[1]);

    try {
      
      // Create and Bind the Socket with the hostname and port number
      Socket server = new Socket(hostname,port);
  
      // Create the Input and Ouput Data Streams to the server
      DataInputStream in = new DataInputStream(server.getInputStream());
      DataOutputStream out = new DataOutputStream(server.getOutputStream());
     
      // Wait to see if Server Connects
      System.out.println(in.readUTF());
     
      Scanner scanner = new Scanner(System.in);
      boolean ConnectionOpen = true;
      boolean ready_flag = false;

      int i;
      int probe_amount = 0;
      int message_size = 0;
      int sequence_num = 0;
      String m_type = "";
      String server_message = "";

      int measurement_amounts[];
      long recieved_time = 0;
      Date sent_date;
      Date recieved_date;

      while(ConnectionOpen){
        // Continuing retrieving input until connection closes
        String line = scanner.nextLine();
        System.out.println(line);
               
        if(ready_flag && line.equals("go")){
          System.out.println("Inside the go");
          measurement_amounts = new int[probe_amount];
          for(i=0; i < probe_amount; i++){
            line = create_probe_message(i+1, message_size);
            out.writeUTF(line);
            sent_date = new Date();
   
            // This is under the presumption that the server will recieve
            // the correct information
            server_message = in.readUTF();
            System.out.println(server_message);
            server_message = in.readUTF();
            System.out.println(server_message);
            if(m_type.equals("rtt")){
              recieved_date = new Date();
              measurement_amounts[i] = measure_rtt(sent_date.getTime(), recieved_date.getTime());
            }
            else if(m_type.equals("tput")){
              recieved_time = Long.parseLong(in.readUTF());
              measurement_amounts[i] = measure_tput(sent_date.getTime(), recieved_time, message_size + 10);
            }
          }
          print_measurements(measurement_amounts);   
        }
        else{
          out.writeUTF(line);
          server_message = in.readUTF();
          System.out.println(server_message);
        }

        if(server_message.equals("200 OK: Ready")){
          System.out.println("Client ready");
          String strip_input[] = line.split(" ");
          m_type = strip_input[1];
          probe_amount = Integer.parseInt(strip_input[2]);
          message_size = Integer.parseInt(strip_input[3]);
          
          ready_flag = true;
        }
   
        if(exitSocket(server_message)){
          ConnectionOpen = false;
        }
      }

      server.close();
    }
    catch(IOException e) {
      System.out.println("IO Exception occurred");
    }
  }  
}

