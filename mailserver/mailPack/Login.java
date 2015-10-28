package mailPack;



import java.io.Serializable;

public class Login implements Serializable {
	
	private String name;
	private String pass;
	private boolean registered;	// If true, client is already registered 
	
	public Login(String name_, String pass_, boolean registered_) {
		name = name_;
		pass = pass_;
		registered = registered_;
	}
	
	String getName() {
		return name;
	}
	
	String getPass() {
		return pass;
	}
	
	boolean isRegistered() {
		return registered;
	}
}
