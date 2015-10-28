package mailPack;


import java.io.Serializable;

public class Message implements Serializable {

	private MessageBody body;
	private MessageHeader header;
	
	public Message(String to_, String from_, String subject_, String date_, boolean isRead_, String content_) {
		body = new MessageBody(content_);
		header = new MessageHeader(to_, from_, subject_, date_, isRead_);
	}
	
	MessageBody getBody() {
		return body;
	}
	
	MessageHeader getHeader() {
		return header;
	}
}
