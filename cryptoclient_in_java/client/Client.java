package client;

import java.net.Socket;

/**
 * 
 * @author Jussi Laakkonen / 
 *
 */
public interface Client {

	/** Start listening the given port
	 * 
	 * @param port
	 */
	public abstract void startListen(int port);

	/** Connect to given address
	 * 
	 * @param address
	 * @param port
	 */
	public abstract void connectTo(String address, int port);

	/** Send message to all but sender 
	 * 
	 * @param s Message to be sent
	 * @param sender The socket of sender of the message
	 */
	public abstract void sendMsg(String s, Socket sender);

	/** Close client
	 * 
	 */
	public abstract void close();

}