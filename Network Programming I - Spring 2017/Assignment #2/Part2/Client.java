import java.net.*;
import java.io.*;
import java.util.*;

public class Client {
  
  // Checks all server messages for close messages
  // If it see's one, it then reports to close the socket
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
    if(msg_size == 1){ msg_size = 2; }
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

  // Measure the Throughput 
  public static int measure_tput(long time_sent, long server_recieved, int byte_amount){
    int sent_time = (int) (server_recieved - time_sent);
    if(sent_time == 0){
      return byte_amount;
    }
    else{
      return ( byte_amount / sent_time );
    }
  }
  
  // Measure the Round Trip Time
  public static int measure_rtt(long sent_time, long recieved_time){
    return (int) (recieved_time - sent_time);
  }

  // Produces the mean from the measurements given.
  public static double measure_mean(int measurements[]){
    int i;
    double mean = 0;

    for(i=0; i < measurements.length; i++){
      mean += measurements[i];
    }
    return ( mean / measurements.length );    
  }

  // Prints the mean with its measurement type
  public static void print_mean(String m_type, double mean){
    if(m_type.equals("rtt")){
      System.out.println("The round trip time mean is " + mean + " miliseconds.");
    }
    else{
      System.out.println("The throughput mean is " + mean + " byte/miliseconds.");
    }
  }

  // Prints the measurement array of tests
  public static void print_measurements(int measurements[]){
    for(int i = 0; i < measurements.length; i++){
      System.out.println("Test #" + (i+1) + ": " + measurements[i]);
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
     
      // All variables needed
      Scanner scanner = new Scanner(System.in);
      boolean ConnectionOpen = true;
      boolean ready_flag = false;

      int i;
      int probe_amount = 0;
      int message_size = 0;
      String m_type = "";
      String server_message = "";

      int measurement_amounts[];
      long recieved_time = 0;
      Date sent_date;
      Date recieved_date;

      // Continue to retrieve user input until the client 
      // sees an error message
      while(ConnectionOpen){
        
        // Provide the user the option for measurement
        if(ready_flag){
          System.out.println("Type in \"Begin\" to send probes.");
        }

        // Scan for user input (message)
        String line = scanner.nextLine();
        
        // Measurement Phase
        if(ready_flag && line.equals("Begin")){
          server_message = "";
          ready_flag = false;
          
          // Send probes to the Server
          measurement_amounts = new int[probe_amount];
          for(i=0; i < probe_amount; i++){
            
            // Make probe message
            line = create_probe_message(i+1, message_size);
            // Record time before sending
            sent_date = new Date();
            out.writeUTF(line);

           
            // Seperate responses due to measurement type     
            if(m_type.equals("rtt")){
              // Wait until probe comes back to record time
              server_message = in.readUTF();
              recieved_date = new Date();
              measurement_amounts[i] = measure_rtt(sent_date.getTime(), recieved_date.getTime());
            }
            else if(m_type.equals("tput")){
              // Wait until the server sends its recieved time
              recieved_time = Long.parseLong(in.readUTF());
              measurement_amounts[i] = measure_tput(sent_date.getTime(), recieved_time, message_size + 10);
            }
          }
          
          // Compute and Print the mean to the user
          double mean = measure_mean(measurement_amounts);
          print_mean(m_type, mean);
        }
        else{
          // Send any input from the user to the server
          out.writeUTF(line);
          server_message = in.readUTF();
        }
     
        // If the setup was successful, set up values in Client
        if(server_message.equals("200 OK: Ready")){
          String strip_input[] = line.split(" ");
          m_type = strip_input[1];
          probe_amount = Integer.parseInt(strip_input[2]);
          message_size = Integer.parseInt(strip_input[3]);
          ready_flag = true;
        }
   
        // If any error message is recieved, terminate the while loop
        if(exitSocket(server_message)){
          System.out.println(server_message);
          ConnectionOpen = false;
        }
      }
      // After while loop is terminated
      server.close();
    }
    catch(IOException e) {
      System.out.println("IO Exception occurred");
    }
  }  
}

