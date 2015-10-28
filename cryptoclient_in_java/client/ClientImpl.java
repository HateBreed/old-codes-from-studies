package client;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.math.BigInteger;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.UnknownHostException;
import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.Iterator;


import javax.crypto.BadPaddingException;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;

import cryptomodule.*;

/**
 * 
 * @author Jussi Laakkonen / 
 *
 */
public class ClientImpl implements Client {

	private UI uiOut;
	
	private ListenThread listen = null;
	private ArrayList<ClThread> clients = null;
	
	public ClientImpl(UI out)
	{
		uiOut = out;
		
		clients = new ArrayList<ClThread>();
		
	}
	
	/** (non-Javadoc)
	 * @see client.Client#startListen(int)
	 */
	public void startListen(int port)
	{
		listen = new ListenThread(port);
		listen.start();
	}
	
	/** (non-Javadoc)
	 * @see client.Client#connectTo(java.lang.String, int)
	 */
	public void connectTo(String address,int port)
	{
		try {
			// Create socket and thread for client
			Socket s = new Socket(address,port);
			ClThread cl = new ClThread(s,false);
			
			// Init and start
			cl.recver();
			cl.start();
			clients.add(cl);
		}
		catch (UnknownHostException e) { uiOut.info(e.getMessage() + "("+ address + ":" + port + ")"); }
		catch (IOException e) { uiOut.info(e.getMessage());	}
	}
	
	/** (non-Javadoc)
	 * @see client.Client#sendMsg(java.lang.String, java.net.Socket)
	 */
	public void sendMsg(String s,Socket sender)
	{
		try
		{
			// Go through list of clients
			for(Iterator<ClThread> i = clients.iterator(); i.hasNext();)
			{
				ClThread c = i.next();
				
				if(!c.getSocket().isClosed())
				{
					if(sender == null) // If sender not specified
					{
						// Encrypt message and send
						Envelope envl = c.getCrypter().encrypt(s.getBytes());
						sendEnvelope(envl, c);
						uiOut.info("Encrypted: " + new BigInteger(envl.getMessage()));
					}
					// If not sender
					else if(!c.getSocket().getLocalSocketAddress().equals(sender.getLocalSocketAddress()))
					{
						// Encrypt message and send
						Envelope envl = c.getCrypter().encrypt(s.getBytes());
						sendEnvelope(envl, c);
						uiOut.info("Encrypted: " + new BigInteger(envl.getMessage()));
					}
				}
			}
			
		}
		catch (NoSuchAlgorithmException e){ uiOut.info(e.getMessage()); }
		catch (IllegalBlockSizeException e){ uiOut.info(e.getMessage()); }
		catch (BadPaddingException e){ uiOut.info(e.getMessage()); }
		catch (NullPointerException e){ uiOut.info(e.getMessage()); }
	}
	
	/** (non-Javadoc)
	 * @see client.Client#close()
	 */
	public void close()
	{
		if(listen != null) listen.close();
		
		for(Iterator<ClThread> i = clients.iterator(); i.hasNext();)
		{
			try { i.next().getSocket().close();	}
			catch (IOException e) { uiOut.info(e.getMessage()); }
		}
	}
	
	/** Writes the given envelope to cipher output stream
	 * 
	 * @param envl Envelope to be written to socket
	 * @param cl Client thread
	 */
	private void sendEnvelope(Envelope envl,ClThread cl)
	{
		try {
			
			//ObjectOutputStream out = new ObjectOutputStream(cl.getSocket().getOutputStream());
			
			// Create cipher object stream
			ObjectOutputStream out = cl.getCrypter().encryptChannel(cl.getSocket());
			
			// write
			out.writeObject(envl);
		}
		catch (IOException e)
		{
			uiOut.info(e.getMessage());
			
			// Error, try to close socket
			try
			{
				cl.getSocket().close();
			}
			catch (IOException e1) { uiOut.info("Closing " + e.getMessage()); }
		}
		catch (NullPointerException e){ uiOut.info(e.getMessage()); }
	}
	
	/** Read a Envelope from socket
	 * 
	 * @param cl Client thread
	 * @return Read envelope
	 */
	private Envelope readEnvelope(ClThread cl)
	{
		Envelope envl;
		
		try {
			//ObjectInputStream in = new ObjectInputStream(cl.getSocket().getInputStream());
			
			// Create cipher object stream
			ObjectInputStream in = cl.getCrypter().decryptChannel(cl.getSocket());
			
			// Read object
			Object obj = in.readObject();
			
			// If envelope
			if(obj.getClass().getSimpleName().equals("Envelope"))
			{
				envl = (Envelope)obj;
				return envl;
			}
			else
			{
				uiOut.info("Got: " + obj.getClass().getSimpleName() + " object.");
				return null;
			}
			
		}
		catch (IOException e)
		{
			uiOut.info(e.getMessage());
			
			// Error, close socket
			try
			{
				cl.getSocket().close();
			}
			catch (IOException e1) { uiOut.info("Closing " + e1.getMessage()); }
		}
		catch (ClassNotFoundException e) { uiOut.info(e.getMessage()); }
		
		return null;
	}
	
	/** Inside class for listening connections
	 * 
	 * @author Jussi Laakkonen / 
	 *
	 */
	private class ListenThread extends Thread
	{
		ServerSocket sock = null;
		
		public ListenThread(int port)
		{
			try
			{
				sock = new ServerSocket(port);
			}
			catch (IOException e)
			{
				uiOut.info(e.getMessage());
			}
		}
		
		public void run()
		{
			uiOut.info("Listening port " + sock.getLocalPort());
			
			while(!sock.isClosed())
			{
				try {
					// New connection
					Socket clsock = sock.accept();
					uiOut.info("Accepted: " + clsock.getInetAddress().getHostName());
					
					// Initialize thread for client and start
					ClThread cl = new ClThread(clsock,true);
					cl.sender();
					cl.start();
					clients.add(cl);
				}
				catch (IOException e)
				{
					uiOut.info(e.getMessage());
					break;
				}
			}
			
			uiOut.info("Listening stopped");
		}
		
		public void close()
		{
			try { sock.close();}
			catch (IOException e) { uiOut.info(e.getMessage()); }
		}
	}
	
	/** Class for client threads
	 * 
	 * @author Jussi Laakkonen / 
	 *
	 */
	private class ClThread extends Thread
	{
	
		private Socket socket = null;
		private Crypter crypter = null;
		boolean isListener;
		
		public ClThread(Socket sock, boolean listener)
		{
			socket = sock;
			isListener = listener;
			crypter = new CrypterSystem();
			
		}
		
		/** Run the thread
		 * 
		 */
		public void run()
		{
			Envelope envl;
			
			while(!socket.isClosed())
			{
				// Read from socket
				envl = readEnvelope(this);

				try {
					uiOut.info("Encrypted: " + new BigInteger(envl.getMessage()));
					
					// Decrypt message
					byte[] decrypted = crypter.decrypt(envl.getMessage());
					uiOut.info("Decrypted: " + new BigInteger(decrypted));
					
					String valid;
					String message = new String(decrypted);

					// Validate message (hash)
					if(crypter.verify(envl,decrypted)) valid = "(VALID)";
					else valid = "(INVALID)";

					uiOut.info(valid + " " + message);

					// If listening connections, deliver to others
					if(isListener) sendMsg(message,socket);
					
				}catch (NoSuchAlgorithmException e) {
					uiOut.info(e.getMessage());
				} catch (IllegalBlockSizeException e) {
					uiOut.info(e.getMessage());
				} catch (BadPaddingException e) {
					uiOut.info(e.getMessage());
				} catch (NullPointerException e) {
					uiOut.info("Empty message");
					break;
				}
			}
		}
		
		/** Get socket
		 * 
		 * @return socket of this client thread
		 */
		public Socket getSocket()
		{
			return socket;
		}
		
		/** Get crypter
		 * 
		 * @return crypter of this client thread
		 */
		public Crypter getCrypter()
		{
			return crypter;
		}
		
		/** Act as a sender in key exchange (starts sending)
		 *
		 */
		public void sender()
		{
			uiOut.info("Starting key exchange");
			
			try {

				// Create TripleDES key
				crypter.createDESKey();

				// Initialize key exhange for symmetric key
				KeyExchange ke = new KeyExchangeSystem(KeyExchange.SENDER,crypter.getKey().getEncoded(),socket);
				ke.init();
				uiOut.info("sec key: " + new BigInteger(crypter.getKey().getEncoded()));
				
				// Generate IV
				crypter.createIV();
				
				// Exhange IV
				ke = new KeyExchangeSystem(KeyExchange.SENDER,crypter.getIV(),socket);
				ke.init();
				
				uiOut.info("IV: " + new BigInteger(crypter.getIV()));
				
				// Create stream key
				crypter.createStreamKey();
				
				// Exchange stream key
				ke = new KeyExchangeSystem(KeyExchange.SENDER,crypter.getStreamKey().getEncoded(),socket);
				ke.init();
				
				uiOut.info("Stream key: " + new BigInteger(crypter.getStreamKey().getEncoded()));
				
				// Initialize ciphers
				crypter.initCiphers();
				crypter.initStreamCiphers();
			}
			catch (InvalidKeyException e) { uiOut.info(e.getMessage()); }
			catch (NoSuchAlgorithmException e){ uiOut.info(e.getMessage()); }
			catch (NoSuchPaddingException e){ uiOut.info(e.getMessage()); }
			catch (InvalidAlgorithmParameterException e) { uiOut.info(e.getMessage()); }
			catch (NullPointerException e) { uiOut.info(e.getMessage()); }
			catch (IllegalBlockSizeException e) { uiOut.info(e.getMessage()); }
			catch (BadPaddingException e) { uiOut.info(e.getMessage()); }
			catch (IOException e) { uiOut.info(e.getMessage()); }
		}
		
		/** Act as a receiver in key exchange (wait)
		 *
		 */
		public void recver()
		{
			uiOut.info("Starting key exchange");
			
			try
			{
				// Initialize key exhange for symmetric key
				KeyExchange ke = new KeyExchangeSystem(KeyExchange.RECVER,null,socket);
				ke.init();
				uiOut.info("sec key: " + new BigInteger(ke.getKey()));

				// Set received key as symmetric key
				crypter.setKey(crypter.keyFromBytes(ke.getKey()));

				// Initialize key exchange for initialization vector
				ke = new KeyExchangeSystem(KeyExchange.RECVER,null,socket);
				ke.init();

				// Set IV
				crypter.setIV(ke.getKey());
				uiOut.info("IV: " + new BigInteger(ke.getKey()));
				
				// Initialize key exchange for stream key
				ke = new KeyExchangeSystem(KeyExchange.RECVER,null,socket);
				ke.init();
				
				// Set stream key
				crypter.setStreamKey(crypter.keyFromBytes(ke.getKey()));
				uiOut.info("stream key: " + new BigInteger(crypter.getStreamKey().getEncoded()));
				
				// Initialize ciphers
				crypter.initCiphers();
				crypter.initStreamCiphers();
				
			}
			catch (InvalidKeyException e) { uiOut.info(e.getMessage()); }
			catch (NoSuchAlgorithmException e){ uiOut.info(e.getMessage()); }
			catch (NoSuchPaddingException e){ uiOut.info(e.getMessage()); }
			catch (InvalidAlgorithmParameterException e) { uiOut.info(e.getMessage()); }
			catch (NullPointerException e) { uiOut.info(e.getMessage()); }
			catch (IllegalBlockSizeException e) { uiOut.info(e.getMessage()); }
			catch (BadPaddingException e) { uiOut.info(e.getMessage()); }
			catch (IOException e) { uiOut.info(e.getMessage()); }

		}
	}
}
