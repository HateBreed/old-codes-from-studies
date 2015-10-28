package serverpack;

import java.io.Serializable;

public class Error implements Serializable {

	private int errorID;
	private String[] errorMessages;
	
	public Error(int errorID_) {
		errorID = errorID_;
		errorMessages = new String[3];
	}
	
	int getID() {
		return errorID;
	}
	
	String getError() {
		return errorMessages[errorID];
	}
}
