package cryptomodule;

import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.security.spec.AlgorithmParameterSpec;

import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.CipherInputStream;
import javax.crypto.CipherOutputStream;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.KeyGenerator;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.SecretKey;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

import com.dragongate_technologies.borZoi.*;

/**
 * 
 * @author Jussi Laakkonen / 
 *
 */
public class CrypterSystem implements Crypter {
	
	// TripleDES key for packet encryption
	private SecretKey symmetricKey = null;
	
	// AES key for stream encryption
	private SecretKey streamKey = null;
	
	// Initialization vector
	private byte[] iv;
	
	// Private and public keys for Elliptic curves
	private ECPrivKey privKey;
	private ECPubKey ownPubKey;
	
	private static final String algorithm = "TripleDES";
	private static final String streamalg = "AES";
	private static final String algorithmParams = "/CTR/PKCS5Padding";
	private static final String hashAlgorihm = "SHA-1";
	private static final String nokey = "No key was found!";
	
	// Packet ciphers
	private Cipher encrypter;
	private Cipher decrypter;
	
	// Stream ciphers
	private Cipher streamencrypter;
	private Cipher streamdecrypter;
	
	public CrypterSystem()
	{
		privKey = new ECPrivKey(ECDomainParameters.NIST_B_233());
		ownPubKey = new ECPubKey(privKey);
	}
	
	/**
	 * @see cryptomodule.Crypter#createIV()
	 */
	public void createIV() throws NoSuchAlgorithmException
	{
		SecureRandom sr = null;
		
		iv = new byte[8];
		
		// Get secure random for IV creation
		sr = SecureRandom.getInstance("SHA1PRNG");
			
		sr.nextBytes(iv); // Fill IV with random bytes
	}
	
	/**
	 * @see cryptomodule.Crypter#initCiphers()
	 */
	public void initCiphers() throws NoSuchAlgorithmException, NoSuchPaddingException, 
	InvalidKeyException, InvalidAlgorithmParameterException
	{
		// Algorithm parameters
		AlgorithmParameterSpec algParamSpec = new IvParameterSpec(iv);
		
		// Packet encrypter
		encrypter = Cipher.getInstance(algorithm + algorithmParams);
		encrypter.init(Cipher.ENCRYPT_MODE, symmetricKey, algParamSpec);
		
		// Packet decrypter
		decrypter = Cipher.getInstance(algorithm + algorithmParams);
		decrypter.init(Cipher.DECRYPT_MODE, symmetricKey,algParamSpec);
	}
	
	/**
	 * @see cryptomodule.Crypter#initStreamCiphers()
	 */
	public void initStreamCiphers() throws NoSuchAlgorithmException, NoSuchPaddingException,
	InvalidKeyException, InvalidAlgorithmParameterException
	{
		// Create 16-byte long initialization vector for stream cipher (AES)
		byte[] iv2 = new byte[16];
		for(int i = 0; i < 16; i++)
		{
			if(i < 8) iv2[i] = iv[7-i];
			else iv2[i] = iv[i-8];
		}
		
		// Algorithm parameters
		AlgorithmParameterSpec algParamSpec = new IvParameterSpec(iv2);
		
		// Stream encrypter
		streamencrypter = Cipher.getInstance(streamalg + algorithmParams);
		streamencrypter.init(Cipher.ENCRYPT_MODE,streamKey,algParamSpec);
		
		// Stream decrypter
		streamdecrypter = Cipher.getInstance(streamalg + algorithmParams);
		streamdecrypter.init(Cipher.DECRYPT_MODE,streamKey,algParamSpec);
	}
	/**
	 * @see cryptomodule.Crypter#getIV()
	 */
	public byte[] getIV()
	{
		return iv;
	}
	
	/**
	 * @see cryptomodule.Crypter#setIV(byte[])
	 */
	public void setIV(byte[] newIV)
	{
		iv = newIV;
	}
	
	/**
	 * @see cryptomodule.Crypter#createDESKey()
	 */
	public void createDESKey() throws NoSuchAlgorithmException
	{
		KeyGenerator kg = KeyGenerator.getInstance(algorithm);
		
		symmetricKey = kg.generateKey();
	}
	
	/**
	 * @see cryptomodule.Crypter#createStreamKey()
	 */
	public void createStreamKey() throws NoSuchAlgorithmException
	{
		KeyGenerator kg = KeyGenerator.getInstance(streamalg);
		kg.init(128); // 128-bit long AES key
		streamKey = kg.generateKey();
	}
	/**
	 * @see cryptomodule.Crypter#getKey()
	 */
	public SecretKey getKey() throws NullPointerException
	{
		if(symmetricKey == null) throw new NullPointerException(nokey);
		else return symmetricKey;
	}
	
	/**
	 * @see cryptomodule.Crypter#getStreamKey()
	 */
	public SecretKey getStreamKey() throws NullPointerException 
	{
		if(streamKey == null) throw new NullPointerException(nokey);
		else return streamKey;
	}
	/**
	 * @see cryptomodule.Crypter#setKey(javax.crypto.SecretKey)
	 */
	public void setKey(SecretKey key)
	{
		symmetricKey = key;
	}
	
	/**
	 * @see cryptomodule.Crypter#setStreamKey(SecretKey)
	 */
	public void setStreamKey(SecretKey key)
	{
		streamKey = new SecretKeySpec(key.getEncoded(),streamalg);
	}
	/**
	 * @see cryptomodule.Crypter#keyFromBytes(byte[])
	 */
	public SecretKey keyFromBytes(byte[] keybytes)
	{
		SecretKey secKey = new SecretKeySpec(keybytes,algorithm);
		
		return secKey;
	}
	/**
	 * @see cryptomodule.Crypter#encrypt(byte[])
	 */
	public Envelope encrypt(byte[] msg) throws NullPointerException, IllegalBlockSizeException,
	BadPaddingException, NoSuchAlgorithmException
	{
		if(symmetricKey == null) throw new NullPointerException(nokey);
		
		Envelope envelope = null;
		byte[] msgBytes = msg;
		
		// Encrypt bytes
		byte[] encrypted = encrypter.doFinal(msgBytes);
		
		// Create envelope containing encrypted message, signed hash and public key
		envelope = new Envelope(encrypted,sign(calculateHash(msgBytes)),ownPubKey);

		return envelope;
	}
	
	/**
	 * @see cryptomodule.Crypter#decrypt(cryptomodule.Envelope)
	 */
	public byte[] decrypt(byte[] msg) throws NullPointerException, IllegalBlockSizeException,
	BadPaddingException, NoSuchAlgorithmException
	{
		if(symmetricKey == null) throw new NullPointerException(nokey);
		
		// Decrypt bytes
		byte[] decrypted = decrypter.doFinal(msg);
		
		return decrypted;
	}
	
	/**
	 * @see cryptomodule.Crypter#encryptChannel(Socket)
	 */
	public ObjectOutputStream encryptChannel(Socket sock) throws IOException
	{
		// Create cipher output stream from outputstream
		OutputStream outC = new CipherOutputStream(sock.getOutputStream(),streamencrypter);
		
		// Wrap cipher stream inside an object stream
		ObjectOutputStream outObj = new ObjectOutputStream(outC);
		
		return outObj;
	}
	
	/**
	 * @see cryptomodule.Crypter#decryptChannel(Socket)
	 */
	public ObjectInputStream decryptChannel(Socket sock) throws IOException
	{
		// Create cipher stream from inputstream
		InputStream inC = new CipherInputStream(sock.getInputStream(),streamdecrypter);
		
		// Wrap cipher stream inside an object stream
		ObjectInputStream inObj = new ObjectInputStream(inC);
		
		return inObj;
	}
	/**
	 * @see cryptomodule.Crypter#calculateHash(byte[])
	 */
	public byte[] calculateHash(byte[] bytes) throws NoSuchAlgorithmException
	{
		MessageDigest md = MessageDigest.getInstance(hashAlgorihm);
		md.update(bytes);
		
		// Calculate hash
		byte[] hash = md.digest();
		
		return hash;
	}
	
	/**
	 * @see cryptomodule.Crypter#sign(byte[])
	 */
	public ECDSA sign(byte[] hash) throws NoSuchAlgorithmException
	{
		// Create signature
		ECDSA dsa = new ECDSA();		
		
		// Initialize private key for signing and set bytes for signing
		dsa.initSignature(privKey);
		dsa.update(hash);
		
		// sign
		dsa.sign();
		
		return dsa;
	}
	
	/**
	 * @see cryptomodule.Crypter#verify(Envelope, byte[])
	 */
	public boolean verify(Envelope envl, byte[] decrypted) throws NoSuchAlgorithmException
	{
		// Calculate hash from decrypted bytes
		byte[] hash = calculateHash(decrypted);
		
		// Get signature
		ECDSA dsa = envl.getSignature();
		
		// Initialize public key of sender and verify hash
		dsa.initVerify(envl.getKey());
		dsa.update(hash);
		
		return dsa.verify();
	}
}
