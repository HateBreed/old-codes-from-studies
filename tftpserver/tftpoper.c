/* Author: Jussi Laakkonen /  */

#include "tftpoper.h"

/* Address of the other party */
struct sockaddr_in conn_address;

/* For select */
fd_set s_set;

/*	Sets the connecting address struct, parameter:
		struct sockaddr_in c_addr - The connection address information
*/
void set_conn_addr(struct sockaddr_in c_addr)
{
	conn_address = c_addr;
	return;
}

/* 	Loop for downloading file, parameters:
		int sock	- socket to be used
		int type	- type of the instance (server or client)

	Returns pointer to first filedata block
*/
filedata *download_mode_loop(int sock, int type)
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

		FD_SET(sock,&s_set);

		/* Check if there is something in socket */
		rv = select(sock+1,&s_set,NULL,NULL,&to);

		if(rv < 0) error("Error in select()");

		/* If got something */
		else if(rv > 0)
		{
			if(FD_ISSET(sock,&s_set)) /* Used socket has something */
			{
				/* Read packet and backup length */
				pck = read_packet(sock);
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
							//printf("Got DATA: %d\n",ntohs(d_pck->blockno));
							
							/* Add data packet to fileblock list */
							files = add_data_packet(files,d_pck,packet_len);
							
							/* Create ack packet for this packet and send it */
							pck = encode_ack_packet(ntohs(d_pck->blockno));
							n = send_packet(sock,pck);
							//printf("Sent ACK: %d\n",ntohs(d_pck->blockno));

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
			/* If client, cannot send ack with block number 0 */
			if((type != TYPE_CLIENT) && (cur_block != 1))
			{
				pck = encode_ack_packet(cur_block-1);
				n = send_packet(sock,pck);
				//printf("TIMEOUT ACK: %d\n",cur_block-1);
			}
		}
	}

	free(pck);
	free(d_pck);

	return files;
}

/*	Loop for uploading a file , parameters:
		int sock 		- socket used for sending
		filedata *files - pointer to first fileblock

	Returns 0 if successful
*/
int upload_mode_loop(int sock,filedata *files)
{
	int n = 0, rv = 0, returnval = 0;
	struct timeval to;
	packet *pck = NULL;
	ack_packet *a_pck = NULL;

	FD_ZERO(&s_set);
	/* Start uploading */
	while(files != NULL)
	{
		/* Timeout values */
		to.tv_sec = TIMEOUT_SEC;
		to.tv_usec = TIMEOUT_USEC;

		FD_SET(sock,&s_set);

		/* Create data packet and send it, quit if sent less than package size */
		pck = encode_data_packet(files);
		if((n = send_packet(sock,pck)) < pck->length) error_minor("Sent less than package size!");
		//printf("Sent DATA: %d\n",files->block_id);
		
		/* Check socket */
		rv = select(sock+1,&s_set,NULL,NULL,&to);

		if(rv < 0) error("Error in select()");

		/* If got something */
		else if (rv > 0)
		{
			if(FD_ISSET(sock,&s_set))
			{
				/* Read packet */
				pck = read_packet(sock);

				/* Check length */
				if(pck->length == 4)
				{
					/* Decode ack packet and check that the blocknumbers match */
					a_pck = decode_ack_packet(pck);
					if((ntohs(a_pck->opcode) == OPCODE_ACK) && (ntohs(a_pck->blockno) == files->block_id))
					{
						//printf("Got ACK: %d - file: %d\n", ntohs(a_pck->blockno), files->block_id);
						files = files->next; /* Move to next block */
					}
				}
			}
		}
	}

	return returnval;
}

/*	Reads the data from socket, parameter:
		int sock	- socket to be used for sending

	Returns the received data as struct packet
*/
packet *read_packet(int sock)
{
	int n = 0;
	unsigned int address_length = sizeof(struct sockaddr_in);

	packet *pck = (packet *)malloc(sizeof(packet));
	pck->content = (char *)malloc(MAX_MSG);
	
	bzero(pck->content,MAX_MSG);

	/* Read packet (as void pointer) from socket */	
	if((n = recvfrom(sock,(void*)pck->content,MAX_MSG,0,(struct sockaddr *)&conn_address,&address_length)) < 0) error("Receiving");

	/* Set length */
	pck->length = n;

	return pck;
}

/*	Sends given packet to socket, parameters:
		int sock	- socket to be used for sending
		packet *pck	- packet to be sent

	Returns amount of bytes written
*/
int send_packet(int sock, packet *pck)
{
	int n = 0;
	unsigned int address_length = sizeof(struct sockaddr_in);
	
	/* Send the content as void pointer */
	if((n = sendto(sock,(void *)pck->content,pck->length,0,(struct sockaddr *)&conn_address,address_length)) < 0) error("Sending");
	
	return n;
}
