/* Author: Jussi Laakkonen / 

Sources used as help:

Beej's Guide to Network Programming
	http://beej.us/guide/bgnet/

Sockets tutorial
	http://www.cs.rpi.edu/courses/sysprog/sockets/sock.html

The World of select()
	http://www.lowtek.com/sockets/select.html

I also looked at the source code of the TFTP implementation in CURL library.
	http://curl.haxx.se/
	The code can be also found with codesearch.google.com with "curl tftp lang:c"

For using time as a seed for random number generator:
	http://www.cs.cf.ac.uk/Dave/C/node21.html

And of course linux man pages.

*/

#include "server.h"

/* Listening socket of server */
int sockSrv;

/* Address for file transfer */
struct sockaddr_in transfer_address;

/* Own address for listening connections */
struct sockaddr_in own_address;

unsigned int addr_length;

int main(int argc, char *argv[])
{
	int port;
	
	/* Check the given parameters and report error if any then quit */ 
	if(argc == PARAMS)
	{
		if((port = atoi(argv[1])) == 0) error("Error: Invalid port");
	}
	
	/* If no parameter (port) is defined, default TFTP server port is used (69) */
	else if(argc == PARAMS - 1) port = TFTP_SERVER_PORT;
	
	else param_err();
	
	/* Run the server */
	if(runServer(port) != 0) error_minor("Error: starting error, unknown reason");

	return 0;
}

/*	Starts running the client in given mode, parameters:
		struct hostent *hostname	- host information
		int port			- port used by client
		char *filename			- file to be uploaded / downloaded
	
	Return 0 if successful.
*/
int runServer(int port)
{
	int returnval, t_sock;
	long n = 0;
	packet *pck = NULL;
	init_packet *i_pck = NULL;
	filedata *files = NULL;
	char *filename = NULL;
	
	/* Create socket */
	if((sockSrv = socket(AF_INET,SOCK_DGRAM,0)) < 0) error("Error creating socket");
	
	/* Set own address data */
	own_address.sin_family = AF_INET;
	own_address.sin_addr.s_addr = INADDR_ANY;
	own_address.sin_port = htons(port);
	
	addr_length = sizeof(struct sockaddr_in);
	
	/* Bind socket to own address */
	if(bind(sockSrv,(struct sockaddr *)&own_address,addr_length) < 0) error("Error in socket binding");
	
	while(1)
	{
		/* Wait for a packet to arrive */
		pck = read_packet(sockSrv);
		
		/* Check that packet is correct size:
			minimum: opcode (2) + filename (1) + pad (1) + mode (min 5) + pad (1) = 10
		*/
		if((pck->length < 10) || (pck->length > MAX_MSG)) printf("Invalid init packet size\n");
		
		else
		{
			/* Decode packet and parse filename */
			i_pck = decode_init_packet(pck);
			filename = parse_filename(i_pck->content);
			
			/* Create new socket for this new connection */
			if((t_sock = socket(AF_INET,SOCK_DGRAM,0)) < 0) error("Error creating socket");
			
			/* Set address and create random port */
			transfer_address.sin_family = AF_INET;
			transfer_address.sin_addr.s_addr = INADDR_ANY;
			transfer_address.sin_port = htons(create_port());
			
			/* Bind new socket */
			if(bind(t_sock,(struct sockaddr *)&transfer_address,addr_length) < 0) error("Error in socket binding");
		
			/* If client wants to read a file */
			if(ntohs(i_pck->opcode) == OPCODE_RRQ)
			{
				//printf("Send \"%s\" to client\n", filename);
				
				/* Read the file */
				if((files = read_file_to_blocks(filename)) == NULL) printf("File not found\n");
				
				else
				{
					printf("The size of the requested file \"%s\" in bytes: %ld\n",filename,get_filesize());
				
					/* Start uploading */
					returnval = upload_mode_loop(t_sock,files);
				}
			}
		
			/* If client wants to write a file */
			else if(ntohs(i_pck->opcode) == OPCODE_WRQ)
			{
				//printf("Get \"%s\" from client\n", filename);
			
				/* Create ACK packet with block number 0 and send it */
				pck = encode_ack_packet(0);
				n = send_packet(t_sock,pck);
			
				/* Start downloading */
				files = download_mode_loop(t_sock,TYPE_SERVER);
				if((n = write_blocks_to_file(files,filename)) == -1) printf("Cannot write to file\n");
				else printf("Wrote: %ld bytes to file \"%s\"\n",n,filename);
			}
			
			/* Otherwise invalid packet */
			else
			{
				close(t_sock);
				printf("Invalid opcode\n");
				break;
			}
			
			free(filename);
			filename = NULL;
			free_filedata(files);
			files = NULL;
			close(t_sock);
		}
	}
	
	close(sockSrv);
	return returnval;
}

/* 	Parse the filename from the init packet, parameter:
		char init_content[TFTP_BLOCK] - The content of init packet
	
	Returns the parsed filename
*/
char *parse_filename(char init_content[TFTP_BLOCK])
{
	int i = 0, j = 0;
	char *filename;
	
	/* Get the length of the filename */
	for(i = 0; i < strlen(init_content) && init_content[i] != '\0'; i++);
	
	/* Reserve memory and clear it */
	filename = (char *)malloc(i);
	bzero(filename,i);
	
	/* Copy filename */
	for(j = 0; j <= i; j++) filename[j] = init_content[j];
	
	return filename;
}

/* 	Creates a random port between [1000,32000] for connections.

	Returns the port number
*/
int create_port()
{
	unsigned int port;
	time_t t;

	/* Get the time in milliseconds and use it as a seed for random number generator */
	(void) time(&t);
	srand((long)t);
	
	/* Create port */
	port = (unsigned int)rand();
	
	/* Create port until it is within the limits */
	while((port > 32000) || (port < 1000))
	{
		port = (unsigned int)rand();
	}
	//printf("random port: %d\n",port);
	
	return port;
}

