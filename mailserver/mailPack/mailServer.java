package mailPack;



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
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

import javax.swing.*;

public class mailServer extends JFrame implements Runnable {

	private static final long serialVersionUID = 1L;
	private ServerSocket listeningSocket;
	private int openPort = 6545;
	private ArrayList<threadForClient> clThreadList = new ArrayList<threadForClient>();
	private Map<String,Account> accountMap = new HashMap<String,Account>();
	private Set<String> registeredUsers = new HashSet<String>();
 	private Thread serverThread;
	private boolean threadHasDoneAll = false;
	
	private JButton start, exit, set, showClients, showAccounts;
	private JTextArea area;
	private JLabel infoLabel, portLabel;
	private JTextField portField;
	
	private String started = new String("Server started, name: ");
	private String stopped = new String("Server stopped");
	private String newConnection = new String("New client connection from: ");
	private String notRunning = new String("Server isn't running");
	private String newPortSet = new String("New server port set to: ");
	private String clremove = new String("Client closed connection while server was stopping.");
	private String stillConnected = new String(" clients were connected to server, closing those connections now.");
	private String noClients = new String("No connections to server.");
	private String clients = new String("Clients connected to server:");
	private String errorClosing = new String("Error closing the connection, reason: ");
	private String serverStillRunning = new String("Cannot close, server is running. Close it first!");
	private String listError = new String("Unknown error while accessing the threadlist.");
	private String accountFile = new String("accounts.txt");
	private String noAccountFile = new String("Account file not found, there is no registered users.");
	private String emptyAccountFile = new String("Accountfile was empty.");
	private String accountsLoaded = new String(" accounts loaded");
	private String noAccounts = new String("No accounts to save.");
	private String accountsSaved = new String(" accounts saved.");
	private String noRegistered = new String("No registered users.");
	private String registered = new String("List of registered users:");
	private String newRegistered = new String("New registered user: ");
	
	public mailServer() {
		
		super("Infernal Mail Server");
		
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
		showClients = new JButton("show clients");
		showClients.addActionListener(listener);
		showClients.setEnabled(false);
		showAccounts = new JButton("show accounts");
		showAccounts.addActionListener(listener);
		showAccounts.setEnabled(false);
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
		buttonPanel.add(showClients);
		buttonPanel.add(showAccounts);
		JScrollPane textScrollPane = new JScrollPane(area);
		textScrollPane.setAutoscrolls(true);
		JPanel infoFieldPanel = new JPanel();
		infoFieldPanel.add(infoLabel);
		
		container.add(buttonPanel, BorderLayout.NORTH);
		container.add(textScrollPane, BorderLayout.CENTER);
		container.add(infoFieldPanel, BorderLayout.SOUTH);
		
		setSize(600,300);
		
		setVisible(true);

	}
	
	public static void main(String[] args) {
		
		new mailServer();
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
	
	// Load the accounts
	private void loadAccounts() {
		
		String user;
		String username;
		String password;
		int counter = 0;
		
		try {
			File file = new File(accountFile);
			
			// If there isn't a account file
			if(file.exists() == false) {
				area.append(noAccountFile + "\n");
			}
			
			// If there is something (size not 0)
			else if(file.length() != 0) {
				BufferedReader accountReader = new BufferedReader(new FileReader(accountFile));
			
				// Read all lines in the file and parse the username and password from the line
				for(user = new String(accountReader.readLine()); user != null; user = new String(accountReader.readLine())) {
					counter = 0;
					username = new String();
					password = new String();
				
					while((user.charAt(counter) != ':')) {
						if(username.length() == 0) {
							username = String.valueOf(user.charAt(counter));
						}
						else {
							username += String.valueOf(user.charAt(counter));
						}
						counter++;
					}
					counter++;
					while(counter < user.length()){
						if(password.length() == 0) {
							password = String.valueOf(user.charAt(counter));
						}
						else {
							password += String.valueOf(user.charAt(counter));
						}
						counter++;	
					}
					
				// Add the user into map
				accountMap.put(username,new Account(username, password));
				}
				
				accountReader.close();
			}
			
			// Otherwise
			else {
				area.append(emptyAccountFile + "\n");
			}
		}
		// When end of file is reached, nullpointerexception is thrown
		catch (NullPointerException npeEx) {
			//update the registeredUsers set
			updateRegisteredUsers();
			area.append(accountMap.size() + accountsLoaded + "\n");
		}
		
		catch (FileNotFoundException notfoundEx) {
			
		}
		
		catch (Exception loadEx) {
			area.append(loadEx.toString() + "\n");
		}
	}
	
	// Saves the accounts
	private void saveAccounts() {
		try {
			int counter = 0;
			if(accountMap.isEmpty()) {
				area.append(noAccounts + "\n");
			}
			else {
				PrintWriter accountWriter;
				// update the registeredUsers set
				accountWriter = new PrintWriter(new File(accountFile));
				// Go through the set and save the user name and password in to textfile
				for(Iterator mapIterator = accountMap.keySet().iterator(); mapIterator.hasNext(); counter++){
					Account tempAcc = accountMap.get(mapIterator.next());
					accountWriter.println(tempAcc.getName() + ":" + tempAcc.getPass());
				}
				accountWriter.close();
				area.append(counter + accountsSaved +"\n");
			}
		}
		
		catch (Exception saEx) {
			
		}
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
	
	
	// Function that prints all connected clients usernames and IP-adresses to server screen
	private void showClients() {
		if(clThreadList.isEmpty()) {
			area.append(noClients + "\n");
		}
		else {
			area.append(clients + "\n");
			for(int i = 0; i < clThreadList.size(); i++) {
				threadForClient showTemp = (threadForClient)clThreadList.get(i);
				area.append(i+1 + ": " + showTemp.getUsername() + " (" + showTemp.getIP() + ")\n");
			}
		}
	}
	
	// Function that prints all registered users to server screen
	private void showAccounts() {
		
		int index = 0;
		if(registeredUsers.isEmpty()) {
			area.append(noRegistered + "\n");
		}
		else {
			area.append(registered + "\n");
			for(Iterator rUiterator = registeredUsers.iterator(); rUiterator.hasNext(); index++) {
				if(index > 0) {
					area.append(", ");
				}
				area.append(rUiterator.next().toString());
			}
			area.append("\n");
		}
	}
	
	// Function for notifing the client (send messageheaderlist or list of registered users
	private void notifyClient(String user) {
		try {
			if(clThreadList.isEmpty() == false) {
				for(int i = 0; i < clThreadList.size(); i++) {
					threadForClient tempThread = (threadForClient)clThreadList.get(i);
					// If all users need to be informed about a new registered user, the 
					// updated set of registered users is sent
					if(user == "all") {
						tempThread.deliverUserSet();
					}
					// Otherwise we inform a single user that it has new message by 
					// sending the updated messageheaderlist to client
					else {
						if(tempThread.getName().equals(user)) {
							tempThread.deliverHeaders();
							break;
						}
					}
				}
			}
		}
		
		catch (Exception ncEx) {
			
		}
	}
	
	// Function that updates (creates new) the set of registered users
	private void updateRegisteredUsers() {
		registeredUsers = new HashSet<String>(accountMap.keySet());
	}
	
	// Starts the server and creates new socket
	private void startServer() {
		try {
			// initializes a new serverSocket and sets the boolean to false, 
			// so serverthread can be started and loads the accounts
			loadAccounts();
			registeredUsers = accountMap.keySet();
			listeningSocket = new ServerSocket(openPort);
			threadHasDoneAll = false;
			// New thread is initialized and started
			serverThread = new Thread(mailServer.this);
			serverThread.start();
			
			// Disable the buttons and textfield and add text to infoLabel
			exit.setEnabled(false);
			set.setEnabled(false);
			portField.setEnabled(false);
			showClients.setEnabled(true);
			showAccounts.setEnabled(true);
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
			// Saves accounts, closes the serversocket and stops the thread
			saveAccounts();
			accountMap.clear();
			registeredUsers.clear();
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
					tempThread.activeAccount.logout();
					tempThread.closeSocket();
				}
				// For safety, the list is cleared from objects
				clThreadList.clear();
			}
			
			// Enable the buttons and textfield and add text to infolabel
			exit.setEnabled(true);
			set.setEnabled(true);
			portField.setEnabled(true);
			showClients.setEnabled(false);
			showAccounts.setEnabled(false);
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
		private ObjectInputStream fromClient;
		private ObjectOutputStream toClient;
		private int id;
		private Object receivedObject;
		private Account activeAccount;
		private boolean clThreadHasDoneAll = false;
		
		// Gets socket and id number as construction parameters, id number
		// is the place of the thread in the list (is updated when one thread 
		// is removed from the list)
		public threadForClient(Socket clSocket, int id_) {
			clientSocket = clSocket;
			id = id_;
			try {
				fromClient = new ObjectInputStream(clientSocket.getInputStream());
				toClient = new ObjectOutputStream(clientSocket.getOutputStream());
			}
			
			catch (Exception constructorEx) {
				
			}
		}
		
		public void run() {
			
			while(true) {
				
				// Client thread is stopped when the client disconnects
				// so the thread has done all and can stop
				if(clThreadHasDoneAll) {
					return;
				}
				try {
					receivedObject = fromClient.readObject();
					String obClass=receivedObject.getClass().toString();
					
					// DEBUGGAUS - POISTA
					System.out.println("Paketti saatu clientiltä!!"); 
					System.out.println("saatu luokka " +receivedObject.getClass());
					// END DEBUGGAUS
					
					// Login/Register
					if(obClass.equals("class mailPack.Login")) {
						
						// DEBUGGAUS - POISTA
						System.out.println("logataan");
						// END DEBUGGAUS
						
						Login login = (Login)receivedObject;
						// Login
						if(login.isRegistered() == true) {
							// Account wasn't found
							if((activeAccount = accountMap.get(login.getName())) == null) {
								
								// DEBUGGAUS - POISTA
								System.out.println("eilöydy");
								// END DEBUGGAUS
								
								area.append("Client from " + clientSocket.getInetAddress().getHostAddress() +
										" tried to login with invalid username.\n" + "Connection was closed.\n");
								deliverError(1);
								closeAndClean();
							}
							// User already logged in
							else if((accountMap.get(login.getName()).isLoggedin()) == true) {
								
								// DEBUGGAUS - POISTA
								System.out.println("joko loggautunu");
								// END DEBUGGAUS
								
								area.append("Client from " + clientSocket.getInetAddress().getHostAddress() +
										" tried to login as user (" + activeAccount.getName() + 
										") who is already logged in.\n" + "Connection was closed.\n");
								deliverError(2);
								closeAndClean();
							}
							// Account found and it wasn't logged in
							else {
								// If password is incorrect
								if(!activeAccount.getPass().equals(login.getPass())) {
									
									// DEBUGGAUS - POISTA
									System.out.println("passu väärä");
									// END DEBUGGAUS
									
									area.append("Client from " + clientSocket.getInetAddress().getHostAddress() +
											" tried to login with invalid password.\n" + "Connection was closed.\n");
									deliverError(3);
									closeAndClean();
								}
								// Otherwise all ok, set account as logged in, load messages
								// and send messageHeaders (ArrayList) and set of users (HashSet)
								// to client
								else {
									activeAccount.login();
									area.append("User: " + activeAccount.getName() + " ("+
											clientSocket.getInetAddress().getHostAddress() +") logged in.\n");
									deliverHeaders();
									deliverUserSet();
								}
							}
						}
						
						// Register as new user, account creation
						else {
							
							// DEBUGGAUS - POISTA
							System.out.println("rekisteröinti");
							// END DEBUGGAUS
							
							// If account with same name already exists
							if((accountMap.containsKey(login.getName())) == true) {
								deliverError(4);
								closeAndClean();
							}
							// Otherwise it can be created by the information in Login-class
							// Create new account, set it as logged in, add it to accountMap 
							// and update the registeredUsers-set
							else {
								
								// DEBUGGAUS - POISTA
								System.out.println("luodaan uusi account");
								// END DEBUGGAUS
								
								activeAccount = new Account(login.getName(),login.getPass());
								activeAccount.login();
								accountMap.put(activeAccount.getName(), activeAccount);
								updateRegisteredUsers();
								notifyClient("all");
								// If creation of the folder wasn't succesfull, error is sent
								// and try to create the folder again
								if(activeAccount.hasFolder() == false) {
									deliverError(5);
									activeAccount.createFolder();
								}
								area.append(newRegistered + activeAccount.getName() + " ("+
										clientSocket.getInetAddress().getHostAddress() +")\n");
								// Send messageHeaders-list and set of registeredUsers to client
								deliverHeaders();
								deliverUserSet();
							}
						}
					}
					// Quitting 
					else if(obClass.equals("class mailPack.Error")) {
						Error error = (Error)receivedObject;
						// When client is going to disconnect
						if(error.getID() == 0) {
							// Log out the clients account and perform cleaning operation
							
							// DEBUGGAUS - POISTA
							System.out.println("Logout vastaaotettu.");
							// END DEBUGGAUS 
							
							area.append("User: " + activeAccount.getName() + " ("+
									clientSocket.getInetAddress().getHostAddress() +") logged out.\n");
							closeAndClean();
						}
					}
					// Sent Message
					else if(obClass.equals("class mailPack.Message")) {
						Message message = (Message)receivedObject;
						Account tempAccount;
						
						// If user not found from the server
						System.out.println(message.getHeader().getTo() + message.getHeader().getFrom());
						if((tempAccount = accountMap.get(message.getHeader().getTo())) == null) {
							deliverError(6);
						}
						// Otherwise it is found and the message is added into the messagelist of
						// that user, if the user is logged in, a notification (new messageheaderlist)
						// is sent 
						else {
							tempAccount.addMessage(message);
							tempAccount.saveMessages();
							if(tempAccount.isLoggedin()) {
								notifyClient(tempAccount.getName());
							}	
						}
					}
					
					// Fetch something
					else if(obClass.equals("class mailPack.Get")) {
						Get get = (Get)receivedObject;
						// get message
						if(get.getType() == 0) {
							
							// DEBUGGAUS - POISTA
							System.out.println("laitetaan body " + get.getMessageID());
							// END DEBUGGAUS
							
							deliverMessage(get.getMessageID());
						}
						// get messageheaders
						else if(get.getType() == 1) {
							deliverHeaders();
						}
						// get list of registered users
						else if(get.getType() == 2) {
							deliverUserSet();
						}
						// delete message
						else if(get.getType() == 3) {
							activeAccount.delMessage(get.getMessageID());
							deliverHeaders();
						}
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
				
				// If some other (unknown) exception is thrown, info added to server screen 
				// and cleaning function is called
				catch (Exception rE) {
					area.append("Client from " + clientSocket.getInetAddress().getHostAddress() + 
							" dropped, reason: " + rE.getMessage() + "\n");
					closeAndClean();
				}
			}
		}
		
		// Function that "cleans" all necessary variables, it is called when client disconnects
		// drops or any another exception is thrown
		private void closeAndClean() {
			
			try {
				// If account was active, logout() -method is executed
				if(activeAccount != null) {
					activeAccount.logout();
				}
				// stops the thread and closes writer, reader and socket
				toClient.close();
				fromClient.close();
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
		
		// Returns the username of the connected client
		private String getUsername() {
			return activeAccount.getName();
		}

		// Sends the list of messageheaders to client
		private void deliverHeaders() {
			try {
				toClient.writeObject(activeAccount.getHeaders());
			}
			
			catch (Exception nmaEx) {
				
			}
		}
		
		// Sends the list of registered users to client
		private void deliverUserSet() {
			try {
				updateRegisteredUsers();
				toClient.writeObject(registeredUsers);
			}
			catch (Exception dusEx) {
				
			}
		}
		
		// Sends the desired message to client
		private void deliverMessage(int msgID) {
			try {
				toClient.writeObject(activeAccount.getMessage(msgID));
			}
			
			catch (Exception dmEx) {
				
			}
		}
		
		// Sends the desired errormessage to client
		private void deliverError(int errorID) {
			try {
				toClient.writeObject(new Error(errorID));
			}
			
			catch (Exception deEx) {
				
			}
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
						
			// If show clients is pressed
			else if(action.getActionCommand().equals("show clients")) {
				// Calls a function that prints out the connected clients to screen
				showClients();
			}
			
			else if(action.getActionCommand().equals("show accounts")) {
				showAccounts();
			}
		}
	}
}
