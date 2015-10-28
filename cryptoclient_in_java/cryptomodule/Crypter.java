package cryptomodule;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;

import javax.crypto.BadPaddingException;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.SecretKey;

import com.dragongate_technologies.borZoi.ECDSA;

/**
 * 
 * @author Jussi Laakkonen / 
 *
 */
public interface Crypter {

	/** Create a random initialization vector
	 * 
	 * @throws NoSuchAlgorithmException
	 */
	public abstract void createIV() throws NoSuchAlgorithmException;
	
	/** Initialize ciphers (en/decrypter)
	 * 
	 * @throws NoSuchAlgorithmException
	 * @throws NoSuchPaddingException
	 * @throws InvalidKeyException
	 * @throws InvalidAlgorithmParameterException
	 */
	public void initCiphers() throws NoSuchAlgorithmException, NoSuchPaddingException, 
	InvalidKeyException, InvalidAlgorithmParameterException;
	
	/** Initialize stream ciphers
	 * 
	 * @throws NoSuchAlgorithmException
	 * @throws NoSuchPaddingException
	 * @throws InvalidKeyException
	 * @throws InvalidAlgorithmParameterException
	 */
	public void initStreamCiphers() throws NoSuchAlgorithmException, NoSuchPaddingException, 
	InvalidKeyException, InvalidAlgorithmParameterException;
	
	/** Returns the initialization vector
	 * 
	 * @return initialization vector
	 */
	public abstract byte[] getIV();
	
	/** Set new initialization vector
	 * 
	 * @param newIV
	 */
	public abstract void setIV(byte[] newIV);
	
	/** Creates symmetric key for Triple DES
	 * 
	 * @throws NoSuchAlgorithmException if algorithm was not found
	 */
	public abstract void createDESKey() throws NoSuchAlgorithmException;

	/** Creates key for stream encryption (AES)
	 * 
	 * @throws NoSuchAlgorithmException
	 */
	public void createStreamKey() throws NoSuchAlgorithmException;
	
	/** Returns the secret key
	 * 
	 * @return secret key
	 * @throws NullPointerException
	 */
	public abstract SecretKey getKey() throws NullPointerException;

	/** Returns the stream key
	 * 
	 * @return stream key
	 * @throws NullPointerException
	 */
	public SecretKey getStreamKey() throws NullPointerException;
	
	/** Sets the symmetric key.
	 * 
	 * @param key The new secret key. 
	 */
	public abstract void setKey(SecretKey key);

	/** Sets the stream key
	 * 
	 * @param key
	 */
	public void setStreamKey(SecretKey key);
	
	/** Generate secret key from byte array.
	 * 
	 * @param keybytes Key in byte array
	 * @return The generated secret key
	 */
	public SecretKey keyFromBytes(byte[] keybytes);
	
	/** Encrypt bytes
	 * 
	 * @param msg message to be encrypted
	 * @return Envelope containing encrypted message and hash
	 * @throws NullPointerException
	 * @throws NoSuchAlgorithmException
	 * @throws IllegalBlockSizeException
	 * @throws BadPaddingException
	 */
	public abstract Envelope encrypt(byte[] msg)
			throws NullPointerException, IllegalBlockSizeException,
			BadPaddingException, NoSuchAlgorithmException;

	/** Decrypt message
	 * 
	 * @param msg Message to be decrypted
	 * @return The decrypted message
	 * @throws NoSuchAlgorithmException
	 * @throws IllegalBlockSizeException
	 * @throws BadPaddingException
	 * @throws NullPointerException
	 */
	public abstract byte[] decrypt(byte[] msg)
			throws NullPointerException, IllegalBlockSizeException,
			BadPaddingException, NoSuchAlgorithmException;

	/** Create a encrypted output socket stream 
	 * 
	 * @param sock Socket for stream
	 * @return encrypting object output stream
	 * @throws IOException
	 */
	public ObjectOutputStream encryptChannel(Socket sock) throws IOException;
	
	/** Create a encrypted input socket stream
	 * 
	 * @param sock Socket for stream
	 * @return decrypting object input stream
	 * @throws IOException
	 */
	public ObjectInputStream decryptChannel(Socket sock) throws IOException;
	
	/** Calculates hash using SHA-1 from given byte array
	 * 
	 * @param bytes The byte array from which the hash will be calculated
	 * @return calculated hash as byte array
	 * @throws NoSuchAlgorithmException
	 */
	public abstract byte[] calculateHash(byte[] bytes)
			throws NoSuchAlgorithmException;
	
	/** Creates a digital signature.
	 * 
	 * @param hash The hash of message that needs to be signed
	 * @return The digital signature of hash
	 * @throws NoSuchAlgorithmException
	 */
	public abstract ECDSA sign(byte[] hash) throws NoSuchAlgorithmException;
	
	/** Verifies the digital signature of message.
	 * 
	 * @param envl The message envelope
	 * @param decrypted Decrypted message
	 * @return true if signature can be verified, false otherwise
	 * @throws NoSuchAlgorithmException
	 */
	public abstract boolean verify(Envelope envl, byte[] decrypted) throws NoSuchAlgorithmException;

}