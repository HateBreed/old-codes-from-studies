package mailPack;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;

public class Account {

	private String name;
	private String pass;
	private ArrayList<Message> messages;
	private ArrayList<MessageHeader> messageHeaders;
	private boolean loggedin;
	private boolean folderCreated;
	
	public Account(String name_, String pass_) {
		loggedin = false;
		name = name_;
		pass = pass_;
		messages = new ArrayList<Message>();
		messageHeaders = new ArrayList<MessageHeader>();
		createFolder();
	}
	
	String getName() {
		return name;
	}
	
	String getPass() {
		return pass;
	}
	
	boolean isLoggedin() {
		return loggedin;
	}
	
	boolean hasFolder() {
		return folderCreated;
	}
	
	void login() {
		loggedin = true;
		messages = new ArrayList<Message>();
		messageHeaders = new ArrayList<MessageHeader>();
		loadMessages();
	}
	
	void logout() {
		loggedin = false;
		saveMessages();
		messages.clear();
		messageHeaders.clear();
	}
	
	void createFolder() {
		if(new File(name).exists()) {
			folderCreated = true;
		}
		else {
			folderCreated = (new File(name).mkdir());
		}
	}
	
	// Load the messages
	void loadMessages() {
		
		try {
			String readMsgPath;
			BufferedReader messageReader;
			boolean status;
			String subject, from, date, content;
			
			// Get all files (filenames) in the users directory
			File userDirectory = new File(name);
			String[] userMessages = userDirectory.list();
			
			// If there is any files
			if(userMessages != null) {
				// Read all files and the content in the directory
				for(int index = 0; index < userMessages.length; index++) {
					// The path of the file (/name/message)
					readMsgPath = new String(name + File.separator + userMessages[index]);
					// Create reader and read the content of the file
					messageReader = new BufferedReader(new FileReader(readMsgPath));
					if(messageReader.readLine().equals("true")) {
						status = true;
					}
					else {
						status = false;
					}
					subject = new String(messageReader.readLine());
					from = new String(messageReader.readLine());
					date = new String(messageReader.readLine());
					content = new String(messageReader.readLine());
					// Create new message and add it into list
					messages.add(new Message(name,from,subject,date,status,content));
					messageReader.close();
				}
			}
		}
			
		catch (Exception lmEx) {
			
		}
	}
	
	// Save all messages to users folder
	void saveMessages() {
		try {
			PrintWriter messageWriter;
			String filePath;
			// Write all messages
			for(int index = 0; index < messages.size(); index++) {
				// the path where the message will be written ( for example /name/year-month-day-hour:minute:second.txt )
				filePath = new String(name + File.separator + messages.get(index).getHeader().getDate() + ".txt");
				// Check if file already exists (no need to write)
				boolean fileExists = (new File(filePath).exists());
				if(fileExists == false) {
					messageWriter = new PrintWriter(new FileWriter(filePath));
					
					// Write the information of the message into the file line by line
					// Line1: true or false, depending of the boolean inside the message
					if(messages.get(index).getHeader().getStatus() == true) {
						messageWriter.println("true");
					}
					else {
						messageWriter.println("false");
					}
					messageWriter.println(messages.get(index).getHeader().getSubject());	// Line2: subject of the message
					messageWriter.println(messages.get(index).getHeader().getFrom());		// Line3: sender
					messageWriter.println(messages.get(index).getHeader().getDate());		// Line4: the date
					messageWriter.println(messages.get(index).getBody().getContent());		// Line5: the content
					messageWriter.close();
				}
			}
		}
		
		catch (Exception smEx) {
			
		}
	}
	
	// Create the headerlist and return the list
	ArrayList getHeaders() {

		messageHeaders = new ArrayList<MessageHeader>();
		try {
			// Get all headers one by one from the messages list and add the header
			// into messageHeaders list
			for(int index = 0; index < messages.size(); index++) {
				messageHeaders.add(messages.get(index).getHeader());
				System.out.println(messages.get(index).getHeader().getDate());
			}
		}
		
		catch (Exception ghEx) {

		}
		return messageHeaders;
	}
	
	// Return the body of desired message and set the sent message to read
	MessageBody getMessage(int msgID) {
		messages.get(msgID).getHeader().setRead();
		return messages.get(msgID).getBody();
	}
	
	// add a message to messagelist
	void addMessage(Message msg) {
		messages.add(msg);
	}
	
	void delMessage(int msgID) {
		// Check if there is a message as a file
		File delFile = new File(messages.get(msgID).getHeader().getDate());
		if(delFile.exists()) {
			delFile.delete();
		}
 		messages.remove(msgID);
	}
}
