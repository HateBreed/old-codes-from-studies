
import java.io.IOException;
import java.io.ObjectInputStream;
import java.net.ServerSocket;
import java.net.Socket;


public class server {

	private static ServerSocket srv=null;
	private static Socket clientSocket=null;
	
	
	public static void main(String[] args) {
		
		try {
			srv = new ServerSocket(5000);
			System.out.println("Listeninh");
			clientSocket=srv.accept();
			ObjectInputStream ois = new ObjectInputStream(clientSocket.getInputStream());
			Object incoming=ois.readObject();
			//System.out.println(ois.readObject().getClass());
			String objname=new String(incoming.getClass().toString());
			if(objname.equals("class MessageHeader")){
				System.out.println("Tullut luokka oli messageheader");
			}
			srv.close();
			clientSocket.close();
			System.exit(0);
		} 
		catch (IOException e) {
			System.out.println(e.getMessage());
		} catch (ClassNotFoundException e) {
			System.out.println(e.getMessage());
		} 		
	}

}
