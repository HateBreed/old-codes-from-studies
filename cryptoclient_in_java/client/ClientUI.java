package client;

import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;

/**
 * 
 * @author Jussi Laakkonen / 
 *
 */
public class ClientUI extends JFrame implements UI {

	private JButton buttonNet = null;
	private JButton buttonSend = null;
	private JButton buttonQuit = null;
	private JButton buttonOk = null;
	private JComboBox selector = null;
	private JTextField inputField = null;
	private JTextField textField = null;
	private JTextArea area = null;
	private JScrollPane scrollPane = null;
	private JLabel label = null;
	
	private final static String[] selectItems = {"Connect","Listen"};
	
	private JFrame frame = null;
	
	private static ClientUI clUI = null;
	
	private int port = 0;
	
	private String address = null;
	
	private Client client = null;
	
	public static void main(String args[])
	{
		clUI = new ClientUI();
	}
	
	public ClientUI()
	{
		Container cont = getContentPane();
		cont.setLayout(new BorderLayout());
		
		ActListener listener = new ActListener();
		
		buttonNet = new JButton("Connect");
		buttonNet.addActionListener(listener);
		
		buttonSend = new JButton("Send");
		buttonSend.addActionListener(listener);
		buttonSend.setEnabled(false);
		
		buttonQuit = new JButton("Quit");
		buttonQuit.addActionListener(listener);
		
		buttonOk = new JButton("Ok");
		buttonOk.addActionListener(listener);
		
		selector = new JComboBox(selectItems);
		selector.addActionListener(listener);
		
		area = new JTextArea();
		area.setEditable(false);
		area.setAutoscrolls(true);
		area.setLineWrap(true);

		scrollPane = new JScrollPane(area);
		scrollPane.setAutoscrolls(true);
		
		inputField = new JTextField(10);
		
		textField = new JTextField(46);
		textField.setEnabled(false);
		textField.addKeyListener(new KeybListener());
		
		JPanel panelTop = new JPanel();
		panelTop.add(selector);
		panelTop.add(buttonNet);
		panelTop.add(buttonQuit);
		
		JPanel panelBot = new JPanel();
		panelBot.add(textField);
		panelBot.add(buttonSend);
		
		cont.add(panelTop,BorderLayout.NORTH);
		cont.add(scrollPane,BorderLayout.CENTER);
		cont.add(panelBot,BorderLayout.SOUTH);
		
		setSize(600,400);
		setVisible(true);
		setTitle("Not connected");
	}
	
	public void processWindowEvent(java.awt.event.WindowEvent clickEvent)
	{
		switch(clickEvent.getID())
		{
	        case java.awt.event.WindowEvent.WINDOW_CLOSING :
	        	close();
	    }
	}
	
	private void close()
	{
		if(client != null) client.close();
		System.exit(0);
	}
	
	
	private void sendText()
	{
		String message = textField.getText();
		
		if(message.length() > 0)
		{
			area.append(message + "\n");
			client.sendMsg(message,null);
			textField.setFocusable(true);
			textField.setText("");
		}
	}
	
	public void info(String msg)
	{
		area.append(msg + "\n");
	}
	
	private void initInputFrame()
	{
		frame = new JFrame();
		frame.addWindowListener(new WindowAdapter()
		{
			public void windowClosing(WindowEvent e)
			{
				inputField.setText("");
				clUI.setEnabled(true);
				frame.setVisible(false);
			}
		});
		frame.setAlwaysOnTop(true);
		Container c = frame.getContentPane();
		c.setLayout(new BorderLayout());
		JPanel panel = new JPanel();
		panel.add(label);
		panel.add(inputField);
		panel.add(buttonOk);
		c.add(panel,BorderLayout.CENTER);
		frame.setSize(240, 60);
		frame.setVisible(true);
		
		clUI.setEnabled(false);
	}
	
	private void connect()
	{
		client = new ClientImpl(this);
		client.connectTo(address, port);
	}
	
	private void listen()
	{
		client = new ClientImpl(this);
		client.startListen(port);
	}
	
	/**
	 * 
	 * @author Jussi Laakkonen / 
	 *
	 */
	private class ActListener implements ActionListener
	{
		public void actionPerformed(ActionEvent e) {
			
			String command = e.getActionCommand();
			if(command.equals("comboBoxChanged"))
			{
				JComboBox box = (JComboBox)e.getSource();
				
				// Connect
				if(box.getSelectedItem().toString().equals(selectItems[0]))
				{
					buttonNet.setText("Connect");
				}
				// Listen
				else if(box.getSelectedItem().toString().equals(selectItems[1]))
				{
					buttonNet.setText("Listen");
				}
			}
			else if(command.equals("Connect"))
			{
				label = new JLabel("Address:");
				inputField.setToolTipText("e.g. server.host.net:8888");
				initInputFrame();
				frame.setTitle("Give address of peer");
				
			}
			else if(command.equals("Disconnect"))
			{
				if(client != null) client.close();
				
				selector.setEnabled(true);
				buttonNet.setText("Connect");
				
				setTitle("Not connected");
				
				buttonSend.setEnabled(false);
				textField.setEnabled(false);
			}
			else if(command.equals("Listen"))
			{

				label = new JLabel("Port:");
				inputField.setToolTipText("e.g. 8888");
				initInputFrame();
				
				frame.setTitle("Give port number");
				
			}
			else if(command.equals("Stop"))
			{
				if(client != null) client.close();
				
				selector.setEnabled(true);
				buttonNet.setText("Listen");
				
				setTitle("Not connected");
			}
			else if(command.equals("Quit"))
			{
				close();
			}
			else if(command.equals("Send"))
			{
				sendText();
			}
			
			else if(command.equals("Ok"))
			{
				frame.setVisible(false);
				clUI.setEnabled(true);
				clUI.setFocusableWindowState(true);
				
				if(buttonNet.getText().equals("Connect"))
				{
					if(separate(inputField.getText()))
					{
						buttonNet.setText("Disconnect");
						selector.setEnabled(false);
											
						connect();
						
						setTitle("Connected to: " + address);
					}
				}
				else if(buttonNet.getText().equals("Listen"))
				{
					String s = inputField.getText();
					
					if(s.length() == 0)
					{
						info("ERROR, specify a port");
						return;
					}
					
					
					try { port = Integer.parseInt(s); }
					catch (NumberFormatException e1) {
						info("ERROR, invalid port number.");
						return;
					}
					
					selector.setEnabled(false);
					buttonNet.setText("Stop");
										
					listen();
					
					setTitle("Listening connections to port " + port);
				}
				
				buttonSend.setEnabled(true);
				textField.setEnabled(true);
				
				inputField.setText("");
			}

		}
		
		private boolean separate(String s)
		{
			if(s.length() == 0) return false;
			int position = s.lastIndexOf(":");
			if(position < 0)
			{
				info("ERROR. No port specified.");
				return false;
			}
			address = s.substring(0, position);
			port = Integer.parseInt(s.substring(position+1, s.length()));
			
			return true;
		}
	}
	
	/**
	 * 
	 * @author Jussi Laakkonen / 
	 *
	 */
	private class KeybListener implements KeyListener
	{
		public void keyPressed(KeyEvent e) {
			if(e.getKeyChar() == KeyEvent.VK_ENTER) sendText();
		}

		public void keyReleased(KeyEvent e) {}

		public void keyTyped(KeyEvent e) {}
	}
}
