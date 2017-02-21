import java.net.*;
import java.io.*;

public class Client {
  
  public static void main(String[] args) {
    String addr = null;
    int port = 8000;
    
    System.out.println(args.length);
    if(args.length == 2) {
      addr = args[0];
      port = Integer.parseInt(args[1]);
    }
    else if(args.length == 1) {
      port = Integer.parseInt(args[0]);
    }
    
    try {
      // When we create a new socket, we bind to this socket
      Socket client = new Socket(addr,port);
      // 
      OutputStream outToServer = client.getOutputStream();
      DataOutputStream out = new DataOutputStream(outToServer);
      
      out.writeUTF("From the client!!!!");
      InputStream inFromServer = client.getInputStream();
      DataInputStream in = new DataInputStream(inFromServer);
      
      System.out.println(in.readUTF());
      client.close();
    }
    catch(IOException e) {
      System.out.println("IO Exception occurred");
    }
  }  
}

// bind - binds a hostname and port to a socket


// accept - waits until the server makes a connection
// read - reads from a stream
// write - writes to a stream
//  
