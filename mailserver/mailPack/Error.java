package mailPack;

import java.io.Serializable;

public class Error implements Serializable {

	private int errorID;
	private String[] errorMessages;
	
	public Error(int errorID_) {
		errorID = errorID_;
		errorMessages = new String[8];
		errorMessages[0]="LOGOUT";
		errorMessages[1]="ACCOUNT NOT FOUND"; 
		errorMessages[2]="USER ALREADY LOGGED IN" ;
		errorMessages[3]="INVALID PASSWORD";
		errorMessages[4]="USERNAME ALREADY RESERVED"; 
		errorMessages[5]="DIRECTORY CREATION FAILED";  
		errorMessages[6]="REGISTERED USER NOT FOUND"; 
		errorMessages[7]="MESSAGE NOT FOUND";				
						 
	}
	
	int getID() {
		return errorID;
	}
	
	String getError() {
		return errorMessages[errorID];
	}
}
