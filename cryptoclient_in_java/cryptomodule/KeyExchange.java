package cryptomodule;

import java.io.IOException;
import java.security.NoSuchAlgorithmException;

import javax.crypto.BadPaddingException;
import javax.crypto.IllegalBlockSizeException;

/** Interface for Shamir Three Pass key Exchange
 * 
 * @author Jussi Laakkonen / 
 *
 */
public interface KeyExchange {

	public final static int SENDER = 1;

	public final static int RECVER = 2;

	/** Initialize key exhange 
	 * 
	 * @throws NullPointerException
	 * @throws IllegalBlockSizeException
	 * @throws BadPaddingException
	 * @throws NoSuchAlgorithmException
	 * @throws IOException
	 */
	public abstract void init() throws NullPointerException, IllegalBlockSizeException,
	BadPaddingException, NoSuchAlgorithmException, IOException;

	/** Get the exchanged key 
	 * 
	 * @return exhanged key
	 */
	public abstract byte[] getKey();

}