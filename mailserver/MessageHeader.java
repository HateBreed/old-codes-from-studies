import java.io.Serializable;




public class MessageHeader implements Serializable {

	private String to;
	private String from;
	private String subject;
	private String date;
	private boolean isRead;
	
	public MessageHeader(String to_, String from_, String subject_, String date_, boolean isRead_) {
		to = to_;
		from = from_;
		subject = subject_;
		date = date_;
		isRead = isRead_;
	}
	
	String getTo() {
		return to;
	}
	
	String getFrom() {
		return from;
	}
	
	String getSubject() {
		return subject;
	}
	
	String getDate() {
		return date;
	}
	
	boolean getStatus() {
		return isRead;
	}
}
