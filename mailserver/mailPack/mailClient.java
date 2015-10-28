package mailPack;

//import java.awt.Button;
//import java.awt.Component;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
//import java.io.BufferedReader;
import java.io.IOException;
//import java.io.InputStreamReader;
import java.io.ObjectInputStream;

import java.io.ObjectOutputStream;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
//import java.sql.Date;
//import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.ArrayList;
//import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.JScrollPane;
import javax.swing.JSeparator;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.JTree;
import javax.swing.MutableComboBoxModel;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.tree.DefaultMutableTreeNode;






public class mailClient extends JFrame implements Runnable {
	//Stream variables
	private static Socket sendSocket = null;
	private static ObjectOutputStream out=null;
	private static ObjectInputStream in=null;
	private static int connectionPort=6545;
	private static Thread listeningThread;
	
	//communication objects
	private static MessageHeader msgH;
	private static Error Er;
	private static MessageBody msgBody;
	private static ArrayList<MessageHeader> arr;
	private static Set<String> users;
	//Gui variables
	private static JTree tree;							//mail tree
	private static JButton del;
	private static JPanel contentPane;
	private static DefaultMutableTreeNode top; 
	private static JScrollPane treeView;
	private static JTextArea mailBody;
	private static DefaultMutableTreeNode category = null;
	private static JTextField servAddr;
	private static JLabel servAddrLab;
	private JLabel portl;
	private JTextField port;
	
	//new mail components
	private static JTextArea sendBody;
	private static JScrollPane body;
	JScrollPane sendBd;
	JButton send;
	static JPanel newMail; 
	JLabel to;
	JLabel from;
	JLabel subject;
	private static JTextField subj;
	private static Date date;
	static JComboBox MailAddr;
	private static String items[];				//address items for the combobox			

	//login components
	static JPanel loginPanel;
	JButton loginB;
	JButton reg;
	static JTextField username;
	static JPasswordField pass;
	JLabel usname;
	JLabel pw;
	
	//common components
	JButton exit;
	static JButton start;
	static JLabel status;
	JSeparator sp;
	JSeparator sp1;
	static JButton sendMail;
	JSeparator sp2;
	private static JLabel event;
	
	//Information for the packets
	private static String toUser; 
	private static String wantedBody;
	private static String loginName;					//stores user's login name
	
	//Listener for buttons
	private static clientListener listener;
	private static ActionListener akti;
	
	public mailClient(){
		super();
		contentPane = (JPanel)this.getContentPane();			//get contentpane
		contentPane.setLayout(null);
	    listener = new clientListener();						//create a new listener
	
		//common buttons for the GUI
		start = new JButton("Connect");
		start.addActionListener(listener);
		start.setBounds(5, 5, 100, 23);
		contentPane.add(start);
		sendMail = new JButton("New");
		sendMail.addActionListener(listener);
		sendMail.setBounds(5, 47, 80, 23);
		sendMail.setEnabled(false);								//New button is disabled when starting. after login it's enabled
		contentPane.add(sendMail);
		
		portl = new JLabel(":");
		portl.setBounds(300, 5, 20, 23);
		contentPane.add(portl);
		
		port = new JTextField("6545");							//default port
		port.setBounds(320, 5, 45, 23);
		contentPane.add(port);
		
		exit = new JButton("Exit");
		exit.addActionListener(listener);
		exit.setBounds(610, 5, 80, 23);
		contentPane.add(exit);
		
		status = new JLabel(":: Disconnected "); //status
		status.setBounds(100, 47, 120, 20);
		contentPane.add(status);
		
		sp = new JSeparator();
		sp.setBounds(5, 40, 685, 5);
		contentPane.add(sp);
		
		sp1 = new JSeparator();
		sp1.setBounds(5, 75, 685, 5);
		contentPane.add(sp1);
		
		sp2 = new JSeparator();
		sp2.setBounds(5, 525, 685, 5);
		contentPane.add(sp2);
		
		event = new JLabel(":: Client running."); //n
		event.setBounds(5, 535, 600, 20);
		contentPane.add(event);
		
		servAddr = new JTextField("127.0.0.1");
		servAddr.setBounds(140, 5, 150, 23);
		contentPane.add(servAddr);
		
		servAddrLab = new JLabel("To: ");
		servAddrLab.setBounds(115, 5, 20, 23);
		contentPane.add(servAddrLab);
		
		
		
		//mail tree
		top=new DefaultMutableTreeNode("Inbox");				//create mail tree
		treeView = new JScrollPane(tree);
		treeView.setBounds(5,90,255,400);
											
		createTree();											//Add nodes
		tree.setEnabled(false);									//tree remains disabled until logging
		contentPane.add(treeView);
		
		del = new JButton("Delete mail");
		del.setBounds(5, 492, 100, 20);
		del.addActionListener(listener);
		del.setEnabled(false);									//disabled  until login is completed
		contentPane.add(del);
		
		
		//panel and components for the New Mail panel
		to = new JLabel("To:");
		to.setPreferredSize(new Dimension(50, 15));
		
		subject = new JLabel("Subject:");
		subject.setPreferredSize(new Dimension(50, 15));
		
		subj = new JTextField("");
		subj.setPreferredSize(new Dimension(320, 21));
		
		MailAddr = new JComboBox();
		MailAddr.setPreferredSize(new Dimension(320, 21));
		contentPane.add(MailAddr);
		
		sendBody = new JTextArea();
		sendBody.setLineWrap(true);
		sendBody.setWrapStyleWord(true);
		
		sendBd = new JScrollPane();
		sendBd.setViewportView(sendBody);
		sendBd.setPreferredSize(new Dimension(415, 330));
		
		send = new JButton("Send");
		send.setPreferredSize(new Dimension(90, 20));
		send.addActionListener(listener);
		
		newMail = new JPanel();
		newMail.setBounds(265,90,420,420);
		newMail.setLayout(new FlowLayout(FlowLayout.CENTER, 5, 5));
		
		newMail.add(to, 0);	//0
		newMail.add(MailAddr, 1);
		newMail.add(subject, 2);
		newMail.add(subj, 3);
		newMail.add(sendBd, 4);
		newMail.add(send, 5);
		newMail.setBorder(BorderFactory.createLoweredBevelBorder());
		newMail.setVisible(false);										//becomes visible after "new" button is pressed
		contentPane.add(newMail);
		
		//text area with scrollpane for showing mail bodies
		mailBody = new JTextArea();
		mailBody.setLineWrap(true);
		mailBody.setWrapStyleWord(true);
		mailBody.setFont(new Font("sansserif", Font.PLAIN, 14));
		
		body = new JScrollPane(mailBody);
		body.setBounds(265,90,420,420);
		body.setVisible(false);											//becomes visible after login
		contentPane.add(body);
		
		//panel for getting username and password
		username = new JTextField("username"); 
		username.setPreferredSize(new Dimension(200, 21)); 
		
		usname = new JLabel("Username: ");
		usname.setPreferredSize(new Dimension(80, 15));
		
		pass = new JPasswordField("password");
		pass.setPreferredSize(new Dimension(200, 21)); 
		
		pw = new JLabel("Password: ");
		pw.setPreferredSize(new Dimension(80, 15));
		
		loginB = new JButton("Login");
		loginB.setPreferredSize(new Dimension(90, 20));
		loginB.addActionListener(listener);
		
		reg = new JButton("Register");
		reg.setPreferredSize(new Dimension(90, 20));
		reg.addActionListener(listener);
		
		loginPanel = new JPanel();
		loginPanel.setBounds(350,91,230,160); //221,91,419 402
		loginPanel.setLayout(new FlowLayout(FlowLayout.CENTER, 5, 5));
		loginPanel.add(usname, 0);
		loginPanel.add(username, 1);
		loginPanel.add(pw, 2);
		loginPanel.add(pass, 3);
		loginPanel.add(loginB, 4);
		loginPanel.add(reg, 5);
		loginPanel.setBorder(BorderFactory.createLoweredBevelBorder());
		loginPanel.setVisible(false);								//visible when start pressend
		contentPane.add(loginPanel);
		
		akti=new ActionListener() { 
			public void actionPerformed(ActionEvent e)
			{
				toUser=MailAddr.getSelectedItem().toString();		//store selected username for the later use
			}
		};
		MailAddr.addActionListener(akti);							//add actionlistener
		
		
		this.setTitle("Mailclient");
		this.setLocation(new Point(7, 42));
		this.setSize(new Dimension(700, 600));
		this.setResizable(false);
		this.setVisible(true);
	}
	
	
	//main method
	public static void main(String[] args) {
		new mailClient();								//create mailClient
	}
	
	//Override method for closing the window-
	//when user closes the program (by pressing "X") the streams are closed
	protected void processWindowEvent(WindowEvent e) 
	{
       if(e.getID()==WindowEvent.WINDOW_CLOSING)
       {
    	   	  closeAll();
		      System.exit(0);
	   }
	}
	
	
	//startClient method is called when the users wants to start communication (by pressing "Start"-button)
	//listening thread is created and all the streams are set
	private void startClient()
	{
		try {
			// get server address
			InetAddress srv = InetAddress.getByName(servAddr.getText());
			
			//get the connection port
			Integer connPort = Integer.valueOf(port.getText());
			
			// open socket 
			sendSocket = new Socket(srv,connPort);	
			
			// open in and out streams
			out = new ObjectOutputStream(sendSocket.getOutputStream());	
			
			in = new ObjectInputStream(sendSocket.getInputStream());
			
			// create a new listning thread
			listeningThread = new Thread(mailClient.this);	
			
			//and start it
			listeningThread.start();										
		}
		
		//catch UnknownHostException if the address is typed wrong or if
		//there is some other problem
		catch (UnknownHostException e) {
			System.out.println(e.getMessage());
			event.setText(":: Error: " + e.getMessage());		//inform about
			status.setText(":: Disconnected");
			setPanel("clearAll");
			
			
		//catch any io exceptions and inform about them	
		} catch (IOException e) {
			System.out.println("fdsa"+e.getMessage());
			event.setText(":: Error: Server not responding.");		//inform about
			status.setText(":: Disconnected");
			setPanel("clearAll");
		} 
		
		
		//if the port number is incorrect
		//for example string value
		catch (NumberFormatException e){
			event.setText(":: Error: " + e.getMessage());		//inform about
			status.setText(":: Disconnected");
			setPanel("clearAll");
		}
		
	}
	
	
	
	
	//clientThread listens all the packets that are coming from the server
	public void run()
	{
		while(true)
		{
			try {
				//listen objects and call processObject method to identify them
				processObject(in.readObject()); 
			}
			
			// if the server disconnects or the link becomes broken
			// we cathc exception and inform about it
			catch (IOException e) {
				
				//connection is lost. update the status text
				status.setText(":: Disconnected");
				
				//client is not any more logged or connected -> update start button text
				start.setText("Connect");							
				setPanel("clearAll");
				//stop the thread
				break;
				
			//if reading object causes some error	
			} catch (ClassNotFoundException e) {
				
				//inform about the error 
				event.setText(":: Error: "+e.getMessage());
				setPanel("clearAll");
				//stop the thread
				break;
			}	
		}
	}
	
	
	
	//send method is for sending objects to the server
	//send method is called when user presses button
	public void send(Object sendOb)
	{
		try {
			//try to write object to the server
			out.writeObject(sendOb);
		} 
		
		//if connection is for some reasin lost 
		catch (IOException e) {
			//inform about the error
			event.setText(":: Error: " + e.getMessage());	
			//set status to 
			status.setText(":: Disconnected");	
		
		// catch any other exception	
		}catch (Exception er){			
			//and inform about it
			event.setText(":: Error: " + er.getMessage());		
		}
	}
	
	
	
	
	
	//process object method processes all the incoming objects (from the server)
	//there are 3 different objects that can come: MessageBody for mail bodies
	//Error for errors
	//Arraylist for headers
	public static void processObject(Object ob)
	{
		// get object class
		String ObClass=ob.getClass().toString();	
		
		System.out.println("saatu luokka: "+ObClass);
		
		//body for the wanted mail header
		if(ObClass.equals("class mailPack.MessageBody"))
		{				
			msgBody = (MessageBody)ob;							//cast the object
			mailBody.append(msgBody.getContent()+"\n");			//get the content
			body.setViewportView(mailBody);						//and show it
		}
		
		//Error message
		else if(ObClass.equals("class mailPack.Error"))
		{		
			
			Er = (Error)ob;										//cast it
			event.setText(":: Error: " + Er.getError());		//and show the error
			
			System.out.println("errori: "+Er.getError());
			
			setPanel("clearAll");
			
		}
		
		//header list for mails
		else if(ObClass.equals("class java.util.ArrayList"))
		{	
			start.setText("Logout");							//client is connected, so set start button text to logout
			status.setText(":: Logged in.");					//set status to logged in
			arr=(ArrayList)ob;									//cast object
			sendMail.setEnabled(true);							//when we are logged, we can send new mails
			
			System.out.println("listan koko: "+arr.size());
			
			top.removeAllChildren();							//clear mail tree
			del.setEnabled(true);								//enable "Delete mail" button
			tree.setEnabled(true);								//set tree enabled
			//go through all the Headers and add them to the mail tree
			for(int i=0;i<arr.size();i++)
			{						
				msgH = (MessageHeader)arr.get(i);				//cast object to Header
				if(msgH.getStatus()==false)						//if it's a new message
				{					
					category = new DefaultMutableTreeNode("new: "+msgH.getSubject()+" (From: "+msgH.getFrom()+")");	
				}
				else 											//if it's not a new message
				{											
					category = new DefaultMutableTreeNode(msgH.getSubject()+" (From: "+msgH.getFrom()+")");	//update tree
				}
				top.add(category);								//add object to the root
			}
			createTree();										//and create tree
			setPanel("body");									//set panel type to body
		}
		
		//list of addresses where user can send mails
		else if(ObClass.equals("class java.util.HashSet"))
		{
			users=(Set)ob;										//cast object
			MailAddr.removeActionListener(akti);				//remove actionListener
			MailAddr.removeAllItems();							//remove all items
			MailAddr.addItem("Empty");							//set empty item first
			
			String userAddr;									//temp
			
			//get all the user addresses
			for(Iterator userIter = users.iterator(); userIter.hasNext();) 
			{
				userAddr=userIter.next().toString();	//get the userAddr
				if(userAddr.equals(loginName)==false)   //don't add own address
				{  
					MailAddr.addItem(userAddr); 		//else add addresses to the list
				}
			}
			MailAddr.addActionListener(akti);					//addActionListener after all the imtems are set
		}
	}
	
	
	
	//createTree object creates a new tree with given nodes
	private static void createTree()
	{
		tree = new JTree(top);								//create a new tree with nodes
		tree.addTreeSelectionListener(listener);			//add listener
		treeView.setViewportView(tree);
		tree.setEnabled(true);								//enabel tree that shows mail subjects
	}
	
	
	//treeSearc updates the subject. If the new mail is read the "new"-string is removed
	//else it just retuns index of the wanted header 
	private static int treeSearch(String subject,int option)
	{		
		for(int i=0;i<top.getLeafCount();i++)						//go through all the nodes
		{
			if(subject.equals(top.getChildAt(i).toString()))		//if we find matching subject
			{
				if(option==0)										//check option
				{		
					top.remove(i);									//remove old node
					category = new DefaultMutableTreeNode(subject.substring(5)); //create a new node without the "new"-string
					top.insert(category,i);							//add new node to the root
					wantedBody=subject.substring(4);				//update wantedBody after removing the "new"-string			
					createTree();									//create a new tree
				}
				return i;											//return header index
			}
		}
		return -1;													//if node is not found
	}
	
	
	
	// Createobject method creates a wanted object to be send to the server
	// method checks the type of the wanted object and then creates it
	private static Object createObject(String type)
	{
		//if type is login
		if(type.equals("Login"))
		{
			return new Login(username.getText(),pass.getText(),true);	//return login object
		}
		
		//if type is register
		else if(type.equals("Register"))
		{
			return new Login(username.getText(),pass.getText(),false);	//return login object with register option
		}
		
		//if type is getBody (for the mail)
		else if(type.equals("getBody"))
		{
			return new Get(0,treeSearch(wantedBody,3));					//return Get object with request for mail body id
		}
		
		//logout object
		else if(type.equals("Logout"))
		{
			return new Error(0);										//return error object with logout option 0
		} 
		
		//Delete mail object.
		else if(type.equals("Delete mail"))
		{
			return new Get(3, treeSearch(wantedBody,1)); 				//return get object with delete option 3. treeSearch finds wanted index for removing
		}
		
		//else the type is "new" for new mails to be send
		else //if(type.equals("Send"))
		{	
			date=new Date();						
			SimpleDateFormat currentDate = new SimpleDateFormat("yMddHms");
			System.out.println(currentDate.format(date));
			System.out.println("username: "+ loginName+"to "+toUser);
			return new Message(toUser,loginName,subj.getText(),currentDate.format(date),false,sendBody.getText());
		}
	}
	
	
	// method closes socket and other streams
	// closeAll is called when the user quits the program
	private static void closeAll()
	{
		try {
			out.close();
			in.close();
			sendSocket.close();
		} catch (IOException e) {
			event.setText(":: Error: " + e.getMessage());		//inform about the error
		}catch (NullPointerException e){
			event.setText(":: Error: " + e.getMessage());		//inform about the error
		}
	}
	
	
	//set panel set panels visible when needed
	private static void setPanel(String panel)
	{
		//if String panel gets "clearAll"-string we don't set any panel visible
		//first set all the panels not visible
		newMail.setVisible(false);						
		body.setVisible(false);
		loginPanel.setVisible(false);
		
		//if we want to show new mail panel
		if(panel.equals("newMail"))
		{					
			newMail.setVisible(true);			//set it enable
		}
		//if we want to show mail body panel
		else if(panel.equals("body"))
		{					
			body.setVisible(true);				//set it enable
		}
		//if we want to shoe login panel 
		else if(panel.equals("loginPanel"))
		{											
			loginPanel.setVisible(true);		//set it enable
		}
		else if(panel.equals("clearAll")){
			//clear tree
			//top.removeAllChildren();			//remove mails from the root
			//createTree();						//update tree
			//treeView.setViewportView(tree);		//set it visible
			
			//clear users
			//MailAddr.removeAllItems();
			//MailAddr.addItem("....");
			//MailAddr.removeAllItems();			//remove all the addresses 
			
			
			//disable everything
			tree.setEnabled(false);				//disable mail tree
			sendMail.setEnabled(false);			//set "new"-mail disabled
			del.setEnabled(false);				//set mail delete button disabled	
			closeAll();							//close streams
		}
	}
	
	
	//Clientlistener listens all the user commands and executes them 
	private class clientListener implements ActionListener,TreeSelectionListener
	{
		//if the mail tree nodes are pressed
		//setInfoToBody method adds information from the headers to the body
		public void valueChanged(TreeSelectionEvent e)
		{
			wantedBody=tree.getLastSelectedPathComponent().toString(); //get mail name
			if(wantedBody.equals("Inbox")==false)					   //we don't want to get Inbox body
			{					   
				if(wantedBody.substring(0, 3).equals("new"))		  // if it has a "new" label, we remove it and update
				{				
					setInfoToBody(treeSearch(wantedBody,0));		  // option 0: update
				}
				else
				{
					setInfoToBody(treeSearch(wantedBody,3));		 //else we just find wanted index
				}
				send(createObject("getBody"));						 //send get for wanted mail body
			}
		}
		
		
		//method sets mail information to the body 
		private void setInfoToBody(int index)
		{
			msgH = (MessageHeader)arr.get(index);					//cast object
			mailBody.setText("Date: " + msgH.getDate()+"\n");		//add information
			mailBody.append("From: " + msgH.getFrom()+"\n");		
			mailBody.append("Subject: " + msgH.getSubject()+"\n");
			mailBody.append("-----------------------------------------------------------------------------------"+"\n");
			body.setViewportView(mailBody);							//and show it
		}
		
		
		//listens the buttons
		public void actionPerformed(ActionEvent action) 
		{
			//logout button is pressed
			if(action.getActionCommand().equals("Logout")) 
			{			
				start.setText("Connect");							//update button
				send(createObject("Logout"));						//send Logout object
				setPanel("clearAll");								//set all the panels disabled
			}
			//if Login button is pressed
			else if(action.getActionCommand().equals("Login")) 
			{		
				send(createObject("Login"));					//send login object
				loginName=username.getText();					//store login name
			}
			
			else if(action.getActionCommand().equals("Register")) 
			{
				send(createObject("Register"));					//Register
			}
			
			else if(action.getActionCommand().equals("Exit")) {
				closeAll();										//close streams
				System.exit(0);									//and exit the program
			}
			
			else if(action.getActionCommand().equals("Send")) 
			{
				send(createObject("Send"));						//create a new mail and send it
			}
			
			else if(action.getActionCommand().equals("Delete mail")) 
			{
				if(wantedBody!=null&&wantedBody.equals("Inbox")==false) //we don't want to remove inbox or null messages
				{
					send(createObject("Delete mail"));			//create delete mail object	
					wantedBody=null;							//clear wantedbody
					mailBody.setText("Deleted..");				//clear mail body
					body.setViewportView(mailBody);				//and show it
				}
			}
			
			else if(action.getActionCommand().equals("New")) 
			{
				sendBody.setText("");							//clear sendBody for new mail
				setPanel("newMail");							//send a new mail
			}
			
			else if(action.getActionCommand().equals("Connect")) 
			{
				event.setText(":: Client running.");		//every time we start, we reset the event
				status.setText(":: Connected");				//update status
				pass.setText("");							//clear password and username field
				username.setText("");
				setPanel("loginPanel");						//set login panel
				startClient();								//and start the client
			}
		}	//actionperformed
	}//class listener
}
