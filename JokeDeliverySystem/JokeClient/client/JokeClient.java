//******************************************************\\
// 		Ti5212500 OBJECT ORIENTED PROGRAMMING			\\
// 														\\
// 		Second Project - JokeClient						\\
//														\\
// 		Name: 				Jussi Laakkonen				\\
// 		Student number: 							\\
// 		Email: 				jussi.laakkonen@lut.fi		\\
//														\\
//******************************************************\\

package client;

import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.*;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.SocketException;
import java.net.UnknownHostException;

public class JokeClient extends JApplet {

	private static final long serialVersionUID = 1L;
	private Socket clSocket;
	private BufferedReader socketReader;
	private PrintWriter socketWriter;
	
	private JButton close, connect, get;
	private JTextField address;
	private JTextArea area;
	
	private String srvAddress = new String("127.0.0.1");
	private int srvPort = 6545;
	
	private String connected = new String("Successfully connected to server: ");
	private String disconnected = new String("Disconnected from server: ");
	private String serverClosed = new String("Server closed connection.");
	private String errorClosing = new String("Error occurred while closing the connection.");
	private String closedCon = new String("Connection closed.");
	private String conLost = new String("Connection lost.");
	
	public void init() {
		
		JPanel upperPanel = new JPanel();
		JPanel buttonPanel = new JPanel();
		JPanel addressPanel = new JPanel();
		
		JLabel addressLabel = new JLabel("Server address:");
		address = new JTextField(srvAddress + ":" + srvPort,12);
		
		clientListener listener = new clientListener();
		
		close = new JButton("close");
		close.addActionListener(listener);
		connect = new JButton("connect");
		connect.addActionListener(listener);
		get = new JButton("get joke");
		get.addActionListener(listener);
		get.setEnabled(false);
		
		area = new JTextArea();
		area.setEditable(false);
		area.setLineWrap(true);
		area.setWrapStyleWord(true);
		JScrollPane textPane = new JScrollPane(area);
		textPane.setAutoscrolls(true);
		
		Container container = getContentPane();
		container.setLayout(new BorderLayout());
		
		buttonPanel.add(connect);
		buttonPanel.add(get);
		buttonPanel.add(close);
		
		addressPanel.add(addressLabel);
		addressPanel.add(address);
		
		upperPanel.setLayout(new BorderLayout());
		upperPanel.add(buttonPanel, BorderLayout.NORTH);
		upperPanel.add(addressPanel, BorderLayout.CENTER);
		
		container.add(upperPanel, BorderLayout.NORTH);
		container.add(textPane, BorderLayout.CENTER);
		
		setSize(300,500);
	}
	
	// Connecting to server, first the server address is retrieved from textfield (and parsed),
	// then a new connection is created to desired server
	private void connectToServer() {
		try {
			parseAddress();
			clSocket = new Socket(srvAddress, srvPort);
			socketReader = new BufferedReader(new InputStreamReader(clSocket.getInputStream()));
			socketWriter = new PrintWriter(clSocket.getOutputStream(), true);
			connect.setText("disconnect");
			get.setEnabled(true);
			close.setEnabled(false);
			address.setEnabled(false);
			
			area.append(connected + clSocket.getInetAddress().getHostName() 
					+ "(" + clSocket.getInetAddress().getHostAddress() + ")\n");
		}
		
		catch (UnknownHostException hostE) {
			area.append(hostE.getMessage() + "\n\n");
		}
		
		catch (IOException ioE) {
			area.append(ioE.getMessage() + "ion attempt to:" 
					+ srvAddress + "\n");
		}

		catch (Exception oE) {
			area.append(oE.getMessage() + "\n\n");
		}
	}
	
	// Disconnecting from server, client send closing-message to server and tries to read 
	// the line from socket input, if server has closed the connection before an exception
	// is thrown
	private void disconnectFromServer() {
		try {
			socketWriter.println("closing");
			String temp = new String(socketReader.readLine());
			
			if (temp.equals("ok")) {
				area.append(disconnected + clSocket.getInetAddress().getHostName()
						+ " (" + clSocket.getInetAddress().getHostAddress() + ")\n");
			}
			finishDisconnect();
		}
		catch (NullPointerException npE) {
			area.append(conLost + "\n\n");
			finishDisconnect();
		}
		
		// when server has closed the connection before
		catch (SocketException sockE) {
			area.append(serverClosed + "\n\n");
			finishDisconnect();
		}
				
		catch (Exception oE) {
			area.append(oE.getMessage() + "\n\n");
			finishDisconnect();
		}
	}
	
	// Finish the disconnect procedure, close reader, writer and socket
	private void finishDisconnect() {
		try {
			socketReader.close();
			socketWriter.close();
			clSocket.close();
			connect.setText("connect");
			get.setEnabled(false);
			close.setEnabled(true);
			address.setEnabled(true);
		}
		
		catch (IOException closeE) {
			area.append(errorClosing + "\n\n");
		}
	}
	
	// Retrieve a joke from server
	private void getJoke() {
		
		// Send get -message to server, server respons with a joke which is read
		// and printed to the client screen
		try {
			socketWriter.println("get");
			area.append("\nJoke:\n" + socketReader.readLine() + "\n");
		}

		// If joke couldn't be recieved, apparently the server has closed connection
		// or connection was lost and disconnect procedure is executed
		catch (Exception ex) {
			area.append(ex.getMessage() + "\n" + closedCon + "\n");
			finishDisconnect();
		}
	}
	
	// Parses the address from the textfield
	private void parseAddress() {
		String addressTemp = new String(address.getText());
		String addressIP = new String();
		String portTemp = new String();
		Integer portNumber;
		
		int counter = 0;
		
		// Get the address
		while((addressTemp.charAt(counter)) != ':') {
			if(addressIP.length() == 0) {
				addressIP = String.valueOf(addressTemp.charAt(counter));
			}
			else {
				addressIP += addressTemp.charAt(counter);
			}
			counter++;
		}
		counter++;
		
		// Get the port number
		while(counter < addressTemp.length()) {
			if(portTemp.length() == 0) {
				portTemp = String.valueOf(addressTemp.charAt(counter));
			}
			else {
				portTemp += addressTemp.charAt(counter);
			}
			counter++;
		}	
		portNumber = Integer.valueOf(portTemp);
		if(portNumber.intValue() != 0) {
			srvPort = portNumber.intValue();
		}
		srvAddress = addressIP;
		
	}
	
	private class clientListener implements ActionListener {

		public void actionPerformed(ActionEvent action) {
						
			if(action.getActionCommand().equals("close")) {
				System.exit(0);
			}
			
			else if(action.getActionCommand().equals("connect")) {
				connectToServer();
			}
			
			else if(action.getActionCommand().equals("disconnect")) {
				disconnectFromServer();
			}
			
			else if(action.getActionCommand().equals("get joke")) {
				getJoke();
			}
		}
	}
}
