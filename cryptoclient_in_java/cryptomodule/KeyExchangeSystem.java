package cryptomodule;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;

import javax.crypto.BadPaddingException;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;

/** Shamir Three Pass Key Exchange
 * 
 * @author Jussi Laakkonen / 
 *
 */
public class KeyExchangeSystem implements KeyExchange {

	// Sender or receiver
	private int role;
	
	// Key to be exchanged
	private byte[] secKey;
	
	private Socket socket;
	
	// Streams
	private OutputStream out;
	private InputStream in;
	
	private Crypter crypter;
	
	public KeyExchangeSystem(int mode, byte[] key, Socket connection)
	{
		role = mode;
		if(mode == SENDER) secKey = key;
		else secKey = null;
		socket = connection;
	}
	
	/**
	 * @see cryptomodule.KeyExchange#init()
	 */
	public void init() throws NullPointerException, IllegalBlockSizeException,
	BadPaddingException, NoSuchAlgorithmException, IOException
	{
		byte[] m,m1,m2,m3;
		// Create random key - use crypter operations
		initCrypter();
		
		switch(role)
		{
		case SENDER:

			// Send Triple DES encrypted secret key 
			m1 = crypter.encrypt(secKey).getMessage();
			out.write(m1,0,m1.length);

			// Receive key encrypted by peer 
			m = new byte[1024];
			int read = in.read(m, 0, m1.length);

			// Copy bytes
			m2 = new byte[read];
			for(int i = 0; i < read; i++) m2[i] = m[i];

			// Decrypt previous message and send
			m3 = crypter.decrypt(m2);
			out.write(m3,0,m1.length);

			break;

		case RECVER:
			
			m = new byte[1024];

			// Receive key encrypted by peer
			m1 = new byte[in.read(m, 0, 1024)];

			// Copy bytes received
			for(int i = 0; i < m1.length; i++) m1[i] = m[i];

			// Encrypt previous message and send
			m2 = crypter.encrypt(m1).getMessage();
			out.write(m2,0,m2.length);

			// Receive key decrypted by peer
			m3 = new byte[m2.length];
			in.read(m3, 0, m2.length);

			// Decrypt message
			byte[] secK = crypter.decrypt(m3);

			secKey = secK;

			break;
		}
	}
	
	/** Initialize crypter for shamir tree-pass key exchange
	 *
	 */
	private void initCrypter()
	{
		crypter = new CrypterSystem();
		
		try
		{
			crypter.createDESKey();
			crypter.createIV();
			crypter.initCiphers();
		}
		catch (NoSuchAlgorithmException e) { e.printStackTrace(); }
		catch (InvalidKeyException e) { e.printStackTrace(); }
		catch (NoSuchPaddingException e) { e.printStackTrace(); }
		catch (InvalidAlgorithmParameterException e) { e.printStackTrace(); }
		
		try
		{
			out = socket.getOutputStream();
			in = socket.getInputStream();	
		}
		catch (IOException e) {	e.printStackTrace(); }
	}
	
	/**
	 * @see cryptomodule.KeyExchange#getKey()
	 */
	public byte[] getKey()
	{
		return secKey;
	}
}
