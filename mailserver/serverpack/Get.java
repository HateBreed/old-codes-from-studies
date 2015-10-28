package serverpack;

import java.io.Serializable;

public class Get implements Serializable {

	private int type;
	private int messageID;
	
	public Get(int type_, int messageID_) {
		type = type_;
		messageID = messageID_;
	}
	
	int getType() {
		return type;
	}
	
	int getMessageID() {
		return messageID;
	}
}
