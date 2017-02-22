import java.net.*;
import java.io.*;

public class Client {
  
  public static void main(String[] args) {
    String addr = "";
    String message = "";
    int port = 8000;
    
    // 1) Port number is required
    // 2) If the user doesn't specify hostname, then
    //    it represents localhost.
    // 3) The user can also enter a message, any text,
    //    to send to the server
    if(args.length == 2) {
      addr = args[0];
      port = Integer.parseInt(args[1]);
    }
    else if(args.length == 1) {
      port = Integer.parseInt(args[0]);
    }
    else if(args.length == 3){
      addr = args[0];
      port = Integer.parseInt(args[1]);
      message = args[2];
    }
    else{
      // Specify the three usages
      System.out.println("Usage: java Client <hostname> <port>");
      System.out.println("Usage: java Client <port>");
      System.out.println("Usage: java Client <hostname> <port> <\"message\">");
      System.exit(0);
    }
      
    try {
      // When we create a new socket, we also bind the properties.
      Socket client = new Socket(addr,port);

      // Create the output stream to send to the server
      OutputStream outToServer = client.getOutputStream();
      DataOutputStream out = new DataOutputStream(outToServer);
     
      // Send the default message or command line message 
      if(message.equals("")){
        out.writeUTF("Hello. Its me the client!");
      }
      else{
        out.writeUTF(message);
      }
      
      // Create the input stream to recieve from the server
      InputStream inFromServer = client.getInputStream();
      DataInputStream in = new DataInputStream(inFromServer);
      
      // Print the echo message
      System.out.println(in.readUTF());
      // Close Connection to the Server
      client.close();
    }
    catch(IOException e) {
      System.out.println("IO Exception occurred");
    }
  }  
}

