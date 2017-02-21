import java.net.*;
import java.io.*;

/* In Java, the socket contains a function called
   getInputStream and getOutputStream to recieve 
   and send packets. I can access these streams with
   other classes that Java utilizes these streams on.
   This stream also has a main stream called InputStream */

public class Server extends Thread {
  private ServerSocket socket;
  
  public Server(int port) throws IOException {
    socket = new ServerSocket(port);
    //socket.setSoTimeout(10000);
  }

  public void run() {
    while(true) {
      try { 
        // Blocks until client connects
        Socket server = socket.accept();
        byte testing[] = new byte[4096];

        DataInputStream in = new DataInputStream(server.getInputStream());
 
        System.out.println(in.readUTF());
        DataOutputStream out = new DataOutputStream(server.getOutputStream());

        out.writeUTF("Hello");
        server.close();
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
