package mailPack;



import java.io.Serializable;

public class MessageBody implements Serializable {

	private String content;
	
	public MessageBody(String content_) {
		content = content_;
	}
	
	String getContent() {
		return content;
	}
}
