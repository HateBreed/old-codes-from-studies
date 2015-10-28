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

And of course linux man pages.

*/

#include "client.h"

int sockCl;

/* Address to which the client connects */
struct sockaddr_in conn_address;

/* Own address, used for receiving (binding port) */
struct sockaddr_in own_address;

unsigned int addr_length;

/* Mode of the client (upload/download) */
enum cl_mode clientmode;

/* For select */
fd_set s_set;

int main(int argc, char *argv[])
{
	int port;
	struct hostent *host;
	
	/* Check the given parameters and report error if any then quit */ 
	if(argc != PARAMS) param_err();
	else
	{
		if((host = gethostbyname(argv[1])) == 0) error("Error: Unknown hostname");
		if((port = atoi(argv[2])) == 0) error("Error: Invalid port");
		
		/* Set mode */
		if(strcmp(argv[3],"upload") == 0) clientmode = UPLOAD;
		else if(strcmp(argv[3],"download") == 0) clientmode = DOWNLOAD;
		else error_minor("Error: Unknown mode");
	}
	
	/* Run the client */
	if(runClient(host,port,argv[4]) != 0) error_minor("Error: starting error, unknown reason");

	return 0;
}

/*	Starts running the client in given mode, parameters:
		struct hostent *hostname	- host information
		int port			- port used by client
		char *filename			- file to be uploaded / downloaded
	
	Return 0 if successful.
*/
int runClient(struct hostent *hostname, int port, char *filename)
{
	int n = 0, returnval;
	packet *pck = NULL;
	filedata *files = NULL;
	
	/* Create socket */
	if((sockCl = socket(AF_INET,SOCK_DGRAM,0)) < 0) error("Error creating socket");
	
	/* Set own address data */
	own_address.sin_family = AF_INET;
	own_address.sin_addr.s_addr = INADDR_ANY;
	own_address.sin_port = htons(port);
	
	/* Set host address data */
	conn_address.sin_addr = *((struct in_addr *)hostname->h_addr);
	conn_address.sin_port = htons(TFTP_SERVER_PORT);
	conn_address.sin_family = AF_INET;
	
	addr_length = sizeof(struct sockaddr_in);
	
	/* Bind socket to own address */
	if(bind(sockCl,(struct sockaddr *)&own_address,addr_length) < 0) error("Error in socket binding");
	
	/* Create and send initialization packet in correct mode */
	pck = encode_init_packet(clientmode,filename,"octet");
	n = send_packet(pck);

	switch(clientmode)
	{
		/* Upload mode, read file to blocks, start the loop and finally free filedata */
		case UPLOAD: 
			files = read_file_to_blocks(filename);
			printf("The size of the file in bytes: %d\n",get_filesize());
			returnval = upload_mode_loop(files);
			free_filedata(files);
			break;

		/* Download mode, start the loop and write blocks to file */
		case DOWNLOAD: 
			files = download_mode_loop();
			n = write_blocks_to_file(files,filename);
			printf("Wrote: %d bytes to file \"%s\"\n",n,filename);
			break;

		/* Unknown mode - should not happen */
		default:
			returnval = -1;
	};
	free(pck->content);
	free(pck);
	close(sockCl);
	return returnval;
}

/* 	Loop for downloading file from server

	Returns pointer to first filedata block
*/
filedata *download_mode_loop()
{
	int n = 0, packet_len = 0, cur_block = 1, rv = 0;
	struct timeval to;
	packet *pck = NULL;
	data_packet *d_pck = NULL;
	filedata *files = NULL;

	FD_ZERO(&s_set);
	while(1)
	{
		/* Set timeout values */
		to.tv_sec = TIMEOUT_SEC;
		to.tv_usec = TIMEOUT_USEC;

		FD_SET(sockCl,&s_set);

		/* Check if there is something in socket */
		rv = select(sockCl+1,&s_set,NULL,NULL,&to);

		if(rv < 0) error("Error in select()");

		/* If got something */
		else if(rv > 0)
		{
			if(FD_ISSET(sockCl,&s_set)) /* Used socket has something */
			{
				/* Read packet and backup length */
				pck = read_packet();
				packet_len = pck->length;
		
				/* If packet is less than 4 bytes or greater than 516 bytes (MAX_MSG) discard it */
				if((packet_len < 4) || (packet_len > MAX_MSG)) error_minor("Invalid packet size, discarding");

				/* Otherwise check the packet */
				else
				{
					if((d_pck = decode_data_packet(pck)) == NULL) error("Error decoding packet");

					/* If it is a DATA packet */
					if(ntohs(d_pck->opcode) == OPCODE_DATA)
					{
						/* Check blocknumber that it is the same as waited block*/
						if(ntohs(d_pck->blockno) == cur_block)
						{
							if(packet_len == 4) break; /* If data packet with 0 data -> last packet */
							
							/* Add data packet to fileblock list */
							files = add_data_packet(files,d_pck,packet_len);
							
							/* Create ack packet for this packet and send it */
							pck = encode_ack_packet(ntohs(d_pck->blockno));
							n = send_packet(pck);

							/* Increase the block counter */
							cur_block = ntohs(d_pck->blockno) + 1;

							/* If got less data than max 512 bytes -> last packet */
							if(packet_len != MAX_MSG) break;
						}
					}

					/* Otherwise a errorous packet */
					else printf("Erroreus packet");
				}
			}
		}
		/* If timeout happened, send re-ack if not waiting for first block */
		else
		{
			if(cur_block > 1)
			{
				pck = encode_ack_packet(files->block_id);
				n = send_packet(pck);
			}
		}
	}

	free(pck);
	free(d_pck);

	return files;
}

/*	Loop for uploading a file to server, parameter:
		filedata *files - pointer to first fileblock

	Returns 0 if successful
*/
int upload_mode_loop(filedata *files)
{
	int n = 0, rv = 0, returnval = 0;
	struct timeval to;
	packet *pck = NULL;
	ack_packet *a_pck = NULL;

	/* Read the packet from socket */
	pck = read_packet();

	/* If length is 4 it is probably ack packet, decode and check block number */
	if(pck->length == 4)
	{
		a_pck = decode_ack_packet(pck);
		if((ntohs(a_pck->opcode) == OPCODE_ACK) && (ntohs(a_pck->blockno) == 0))
		{
			FD_ZERO(&s_set);
			/* Start uploading */
			while(files != NULL)
			{
				/* Timeout values */
				to.tv_sec = TIMEOUT_SEC;
				to.tv_usec = TIMEOUT_USEC;

				FD_SET(sockCl,&s_set);

				/* Create data packet and send it, quit if sent less than package size */
				pck = encode_data_packet(files);
				if((n = send_packet(pck)) < pck->length) error_minor("Sent less than package size!");
				
				/* Check socket */
				rv = select(sockCl+1,&s_set,NULL,NULL,&to);

				if(rv < 0) error("Error in select()");

				/* If got something */
				else if (rv > 0)
				{
					if(FD_ISSET(sockCl,&s_set))
					{
						/* Read packet */
						pck = read_packet();

						/* Check length */
						if(pck->length == 4)
						{
							/* Decode ack packet and check that the blocknumbers match */
							a_pck = decode_ack_packet(pck);
							if((ntohs(a_pck->opcode) == OPCODE_ACK) && (ntohs(a_pck->blockno) == files->block_id))
							{
								files = files->next; /* Move to next block */
							}
						}
					}
				}
			}
		}
		/* Otherwise server sent something else than ack packet */
		else
		{
			returnval = -1;
			printf("Server did not accept");
		}
	
		free(a_pck);
	}
	free(pck);
	return returnval;
}

/*	Reads the data from socket.

	Returns the received data as struct packet
*/
packet *read_packet()
{
	int n = 0;

	packet *pck = (packet *)malloc(sizeof(packet));
	pck->content = (char *)malloc(MAX_MSG);
	
	bzero(pck->content,MAX_MSG);

	/* Read packet (as void pointer) from socket */	
	if((n = recvfrom(sockCl,(void*)pck->content,MAX_MSG,0,(struct sockaddr *)&conn_address,&addr_length)) < 0) error ("Receiving");

	/* Set length */
	pck->length = n;
	return pck;
}

/*	Sends given packet to socket, parameter:
		packet *pck	- packet to be sent

	Returns amount of bytes written
*/
int send_packet(packet *pck)
{
	int n = 0;
	/* Send the content as void pointer */
	if((n = sendto(sockCl,(void *)pck->content,pck->length,0,(struct sockaddr *)&conn_address,addr_length)) < 0) error("Sending");
	return n;
}


