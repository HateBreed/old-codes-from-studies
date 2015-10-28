//******************************************************\\
// 		Ti5212500 OBJECT ORIENTED PROGRAMMING			\\
// 														\\
// 		Second Project - JokeServer						\\
//														\\
// 		Name: 				Jussi Laakkonen				\\
// 		Student number: 							\\
// 		Email: 				jussi.laakkonen@lut.fi		\\
//														\\
//******************************************************\\

// Help was searched from http://javaalmanac.com/egs/java.lang/StopThread.html to
// start and stop the thread

package server;

import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.*;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;
import java.util.ArrayList;
import java.util.Random;

import javax.swing.*;

public class JokeServer extends JFrame implements Runnable {

	private static final long serialVersionUID = 1L;
	private ServerSocket listeningSocket;
	private int openPort = 6545;
	private ArrayList jokeList;
	private ArrayList clThreadList = new ArrayList();
	private Thread serverThread;
	private boolean threadHasDoneAll = false;
	
	private JButton start, exit, set, show, showC;
	private JTextArea area;
	private JLabel infoLabel, portLabel;
	private JTextField portField;
	
	private String jokeFile = new String("jokes.txt");
	private String loading = new String("Loading jokes from file: ");
	private String loaded = new String(" jokes loaded.");
	private String started = new String("Server started, name: ");
	private String stopped = new String("Server stopped");
	private String jokeSent = new String("A joke was sent to: ");
	private String newConnection = new String("New client from: ");
	private String notRunning = new String("Server isn't running");
	private String newPortSet = new String("New server port set to: ");
	private String clremove = new String("Client closed connection while server was stopping.");
	private String stillConnected = new String(" clients were connected to server, closing those connections now.");
	private String noClients = new String("No connections to server.");
	private String clients = new String("Clients connected to server:");
	private String errorClosing = new String("Error closing the connection, reason: ");
	private String serverStillRunning = new String("Cannot close, server is running. Close it first!");
	private String listError = new String("Unknown error while accessing the threadlist.");
	
	public JokeServer() {
		
		super("Joke server");
		
		Container container = getContentPane();
		container.setLayout(new BorderLayout());
		
		serverListener listener = new serverListener();
		
		// GUI buttons and other components
		start = new JButton("start");
		start.addActionListener(listener);
		exit = new JButton("exit");
		exit.addActionListener(listener);
		set = new JButton("set");
		set.addActionListener(listener);
		show = new JButton("show jokes");
		show.addActionListener(listener);
		showC = new JButton("show clients");
		showC.addActionListener(listener);
		showC.setEnabled(false);
		area = new JTextArea("");
		area.setEditable(false);
		area.setLineWrap(true);
		area.setWrapStyleWord(true);
		infoLabel = new JLabel(notRunning);
		portLabel = new JLabel("port:");
		portField = new JTextField(Integer.toString(openPort),4);
		
		JPanel buttonPanel = new JPanel();
		buttonPanel.add(start);
		buttonPanel.add(exit);
		buttonPanel.add(portLabel);
		buttonPanel.add(portField);
		buttonPanel.add(set);
		buttonPanel.add(show);
		buttonPanel.add(showC);
		JScrollPane textScrollPane = new JScrollPane(area);
		textScrollPane.setAutoscrolls(true);
		JPanel infoFieldPanel = new JPanel();
		infoFieldPanel.add(infoLabel);
		
		container.add(buttonPanel, BorderLayout.NORTH);
		container.add(textScrollPane, BorderLayout.CENTER);
		container.add(infoFieldPanel, BorderLayout.SOUTH);
		
		setSize(500,300);
		show();
		
		loadJokes();
	}
	
	public static void main(String[] args) {
		
		new JokeServer();
	}
	
	// Override method for closing the window
	public void processWindowEvent(java.awt.event.WindowEvent clickEvent) {
	    
		switch(clickEvent.getID()) {
		// When X is pressed
	        case java.awt.event.WindowEvent.WINDOW_CLOSING :
	        	// If the server isn't running we can quit the application
	            if(serverThread == null) {
	            	System.exit(0);
	            }
	            // If it's running
	            else {
	            	area.append(serverStillRunning + "\n");
	            }
	            break;
	    }
	  }


	public void run() {
		
		while(true) {
			
			// If the server is stopped then the serverthread is also being stopped
			if(threadHasDoneAll) {
				return;
			}
			// While running, server listens for connection and starts a new thread for
			// each connecting cilent, client threads are added into a list
			// Also information is added to server screen.
			try {
				Socket clientConnection = listeningSocket.accept();
				area.append(newConnection + clientConnection.getInetAddress().getHostAddress() + "\n");
				// Every new client thread gets its placenumber in the list (needed for removing)
				threadForClient clT = new threadForClient(clientConnection, clThreadList.size());
				clT.start();
				clThreadList.add(clT);
			}
			
			catch (Exception srE) {
				infoLabel.setText(stopped + ", " + srE.getMessage());
			}
		}
	}
	
	// Function that loads jokes from a textfile
	private void loadJokes() {
		
		// First a new ArrayList for the jokes is initialized 
		jokeList = new ArrayList();
		String joke = new String();
		area.append(loading + jokeFile + "\n");
		
		try {
			// Create a buffered reader for reading the file using filereader
			BufferedReader jokeFS = new BufferedReader(new FileReader(jokeFile));
			
			// Read jokes from the file line by line until the end (EOF)
			while((joke = jokeFS.readLine()) != null) {
				jokeList.add(joke);
			}
			
			// Add informative text to server screen and close the reader using file.
			area.append(jokeList.size() + loaded + "\n");
			jokeFS.close();	
		}
		
		catch (Exception jfE) {
			area.append("Error: " + jfE.getMessage());
		}	
	}
	
	// Function which creates a random number within jokeList's index and
	// then returns the selected joke as a String 
	private String selectRandomJoke() {
		
		Random randomNumber = new Random();
		int randomInt = randomNumber.nextInt(jokeList.size());
		return jokeList.get(randomInt).toString();
	}
	
	// Function that sets the server port to the desired value in the UI
	// This is called when "set" button is pressed
	private void setServerPort() {	
		// Gets the value from the JTextField and if it's the same as current port
		// number, nothing is done
		Integer openPort_ = Integer.valueOf(portField.getText());
		if(openPort != openPort_.intValue()) {
			openPort = openPort_.intValue();
			infoLabel.setText(newPortSet + openPort + ", " + notRunning);
		}
	}
	
	// Function that prints all jokes in the array to the server screen
	private void showJokes() {
		
		area.append("\nAll jokes:\n");
		for(int index = 0; index < jokeList.size(); index++) {
			area.append("\n");
			area.append(index+1 + ": " + jokeList.get(index).toString() + "\n");
		}
		area.append("\n");
	}
	
	// Function that prints all connected clients IP-adresses to server screen
	private void showClients() {
		if(clThreadList.isEmpty()) {
			area.append(noClients + "\n");
		}
		else {
			area.append(clients + "\n");
			for(int i = 0; i < clThreadList.size(); i++) {
				threadForClient showTemp = (threadForClient)clThreadList.get(i);
				area.append(i+1 + ": " + showTemp.getIP() + "\n");
			}
		}
	}
	
	// Starts the server and creates new socket
	private void startServer() {
		try {
			// initializes a new serverSocket and sets the boolean to false, 
			// so serverthread can be started
			listeningSocket = new ServerSocket(openPort);
			threadHasDoneAll = false;
			// New thread is initialized and started
			serverThread = new Thread(JokeServer.this);
			serverThread.start();
			
			// Disable the buttons and textfield and add text to infoLabel
			exit.setEnabled(false);
			set.setEnabled(false);
			portField.setEnabled(false);
			showC.setEnabled(true);
			infoLabel.setText(started + InetAddress.getLocalHost().getHostName()
					+ ", address (" + InetAddress.getLocalHost().getHostAddress() + ":" 
					+ listeningSocket.getLocalPort() + ")\n");
			start.setText("stop");
		}
		
		catch (Exception sE){
			threadHasDoneAll = false;
			infoLabel.setText(notRunning + ", " + sE.getMessage());
		}
	}
	
	private void stopServer() {
		try {
			// Closes the serversocket and stops the thread
			threadHasDoneAll = true;
			serverThread = null;
			listeningSocket.close();

			if(clThreadList.size() != 0) {
				area.append(clThreadList.size() + stillConnected + "\n");
			}
			// When server is stopped, all still connected clients will be disconnected by
			// closing the sockets which they are connected to and also stopping the threads
			// When socket is closed, threadForClient throws an exception which is
			// caught and closeAndClean() will be run, it closes the reader, writer and socket
			// if needed
			if(!clThreadList.isEmpty()) {
				while(!clThreadList.isEmpty()) {
					threadForClient tempThread = (threadForClient)clThreadList.get(0);
					tempThread.closeSocket();
				}
				// For safety, the list is cleared from objects
				clThreadList.clear();
			}
			
			// Enable the buttons and textfield and add text to infolabel
			exit.setEnabled(true);
			set.setEnabled(true);
			portField.setEnabled(true);
			showC.setEnabled(false);
			infoLabel.setText(stopped + "\n");
			start.setText("start");
		}
		
		catch (IndexOutOfBoundsException ioutbE) {
			area.append(listError + "\n");
			stopServer();
		}
		
		catch (Exception clE) {
			area.append("Error, " + clE.getMessage());
			stopServer();
		}
	}
	
	// Function that removes the thread from the list, it is called when a client 
	// disconnects so the thread for that client can be stopped and removed
	private void removeClThread(int clID) {
		try {
			// Go trough the list and set the new id for threads so they can remove
			// themselves from the correct place of list. Start from the item that is 
			// next to the removed and set new id to the thread
			for(int x = clID + 1; x < clThreadList.size(); x++) {
				threadForClient temp = (threadForClient)clThreadList.get(x);
				temp.setNewID(x-1);
			}
			clThreadList.remove(clID);
		}
		
		catch (Exception rctE) {
			area.append(clremove + "\n");
		}
	}
	
	// Thread for the client, uses selectRandomJoke to send a joke to client
	private class threadForClient extends Thread {
		
		private Socket clientSocket;
		private int id;
		private boolean clThreadHasDoneAll = false;
		private BufferedReader fromClient;		
		private PrintWriter toClient;
		private String rec;
		
		// Gets socket and id number as construction parameters, id number
		// is the place of the thread in the list (is updated when one thread 
		// is removed from the list)
		public threadForClient(Socket clSocket, int id_) {
			clientSocket = clSocket;
			id = id_;
		}
		
		public void run() {
			
			while(true) {
				
				// Client thread is stopped when the client disconnects
				// so the thread has done all and can stop
				if(clThreadHasDoneAll) {
					return;
				}
				try {
					fromClient = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
					toClient = new PrintWriter(clientSocket.getOutputStream(), true);
				
					rec = new String(fromClient.readLine());
					// When client sends get-message to server, thread will send a random joke
					// to the client it was created for and adds text to textArea field
					if(rec.equals("get")) {
						toClient.println(selectRandomJoke());
						area.append(jokeSent + clientSocket.getInetAddress().getHostAddress() + "\n");
					}
					// When client sends closing message meaning that client is disconnecting, information
					// is sent to screen and message sent to client, after this the thread is closed by 
					// executing closeAndClean()-function
					else if(rec.equals("closing")) {
						toClient.println("ok");
						area.append("Client from " + clientSocket.getInetAddress().getHostAddress() +
								" disconnected.\n");
						closeAndClean();
					}
				}
			
				// When client drops (the closing message isn't recieved and the client has closed the
				// connection)
				catch (NullPointerException npE) {
					area.append("Client from " + clientSocket.getInetAddress().getHostAddress() + 
							" dropped\n");
					closeAndClean();
				}
				
				// When connection between client and server is closed from unknown reason
				// (timeout, software/hardware failure) or server forces it to close 
				// SocketException is thrown.
				// Information text is sent to server screen and a cleaning function is called
				catch (SocketException sockE) {
					area.append("Client from " + clientSocket.getInetAddress().getHostAddress() + 
							" dropped, reason: " + sockE.getMessage() + "\n");
					closeAndClean();
				}
				
				// If some other (unknown) exception is thrown, infotext is set to infoLabel
				// and cleaning function is called
				catch (Exception rE) {
					infoLabel.setText(infoLabel.getText() + ", " + rE.getMessage());
					closeAndClean();
				}
			}
		}
		
		// Function that "cleans" all necessary variables, it is called when client disconnects
		// drops or any another exception is thrown
		private void closeAndClean() {
			
			try {
				// stops the thread and closes writer, reader and socket
				fromClient.close();
				toClient.close();
				clientSocket.close();
				clThreadHasDoneAll = true;
				removeClThread(id);
			}
			
			catch (IOException closeE) {
				area.append(errorClosing + closeE.getMessage() + "\n");
			}
		}
		
		// Closes the socket, called when server is closing down with clients still
		// connected, exception is thrown and caught in run()  
		private void closeSocket() {
			try {
				clientSocket.close();
			}
			
			catch (Exception csE) {
				area.append(errorClosing + csE.getMessage() + "\n");
			}
		}
		
		// Function that sets the new id for client when some other client is removed from the list
		private void setNewID(int newID) {
			id = newID;
		}
		
		// Returns the IP address of client connected to this thread
		private String getIP() {
			return clientSocket.getInetAddress().getHostAddress();
		}
	}
	
	// Listener for the buttonactions
	private class serverListener implements ActionListener {

		public void actionPerformed(ActionEvent action) {
			
			// If exit is pressed then server will be closed
			if(action.getActionCommand().equals("exit")) {
				System.exit(0);
			}
			
			// If start is pressed, then a new serversocket is initialized to
			// desired port
			else if(action.getActionCommand().equals("start")) {
				startServer();
			}
			
			// If stop is pressed, the server will be stopped
			else if(action.getActionCommand().equals("stop")) {
				stopServer();	
			}
			
			// If set is pressed
			else if(action.getActionCommand().equals("set")) {
				// Calls the function that sets new port for the server
				setServerPort();
			}
			
			// If show jokes is pressed
			else if(action.getActionCommand().equals("show jokes")) {
				// Calls a function that prints out the jokes to screen
				showJokes();
			}
			
			// If show clients is pressed
			else if(action.getActionCommand().equals("show clients")) {
				// Calls a function that prints out the connected clients to screen
				showClients();
			}
		}
	}
}
