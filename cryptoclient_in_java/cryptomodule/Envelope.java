package cryptomodule;

import java.io.Serializable;
import java.math.BigInteger;

import com.dragongate_technologies.borZoi.ECDSA;
import com.dragongate_technologies.borZoi.ECDomainParameters;
import com.dragongate_technologies.borZoi.ECPointF2m;
import com.dragongate_technologies.borZoi.ECPubKey;

/**
 * 
 * @author Jussi Laakkonen / 
 *
 */
public class Envelope implements Serializable{

	private static final long serialVersionUID = 5268956348074043659L;
	
	private byte[] message;
	private byte[] signature_c;
	private byte[] signature_d;
	private byte[] pubkey_x;
	private byte[] pubkey_y;
	
	public Envelope(byte[] msg, ECDSA sign, ECPubKey pkey)
	{
		message = new byte[msg.length];
		message = msg;
		
		// Copy values from signature
		signature_c = sign.c.toByteArray();
		signature_d = sign.d.toByteArray();

		// Copy coordinates of public key
		pubkey_x = pkey.W.x.val.toByteArray();
		pubkey_y = pkey.W.y.val.toByteArray();
	}
	
	/** Generates the signature from signature valuepair in the message envelope
	 * 
	 * @return Signature of this message
	 */
	public ECDSA getSignature()
	{
		ECDSA signature = new ECDSA(new BigInteger(signature_c),new BigInteger(signature_d));
		
		return signature;
	}
	
	/** Returns the encrypted message 
	 * 
	 * @return message
	 */
	public byte[] getMessage()
	{
		return message;
	}
	
	/** Creates public key of the sender from coordinates inside the message envelope
	 * 
	 * @return the public key of the sender
	 */
	public ECPubKey getKey()
	{
		ECPubKey pubkey;
		ECPointF2m _W = new ECPointF2m();
		_W.x.val = new BigInteger(pubkey_x);
		_W.y.val = new BigInteger(pubkey_y);
		
		pubkey = new ECPubKey(ECDomainParameters.NIST_B_233(),_W);
		
		return pubkey;
	}
}
