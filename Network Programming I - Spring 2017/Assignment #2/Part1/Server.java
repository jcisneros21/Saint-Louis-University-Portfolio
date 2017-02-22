import java.net.*;
import java.io.*;

public class Server extends Thread {
  private ServerSocket socket;
  
  public Server(int port) throws IOException {
    // Binds port nuumber to the Server
    socket = new ServerSocket(port);
  }

  public void run() {
    while(true) {
      try { 
        String message = "";

        // Blocks until client connects
        Socket client = socket.accept();
      
        // Creates input stream to retrieve from the client 
        DataInputStream in = new DataInputStream(client.getInputStream());
 
        // Will block thread until it recieves a message
        message = in.readUTF();

        // Creates output stream to send back to the client
        DataOutputStream out = new DataOutputStream(client.getOutputStream());
        out.writeUTF(message);

        client.close();
      }
      catch(IOException e) {
        System.out.println("IO Exception occured");
        break;
      }
    }
  }

  public static void main(String[] args) {
    int port = Integer.parseInt(args[0]);
    try {
      // Creates an instance of server
      Thread t = new Server(port);
      t.start();
    }
    catch(Exception e){
      System.out.println("Exception Happened");
    }
  }
}
