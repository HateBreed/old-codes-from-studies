/* Author: Jussi Laakkonen /  */
#ifndef __STRUCTURES_H
#define __STRUCTURES_H

#include "definitions.h"
#include "errors.h"

/* Structure for element in linked list of datablocks */
typedef struct filedata
{
	int block_id;

	int length;

	char data[TFTP_BLOCK];
	
	struct filedata *next;
	
} filedata;

/* Structure for TFTP init packet */
typedef struct init_packet
{
	uint16_t opcode;

	char content[TFTP_BLOCK];

} init_packet;
 
/* Structure for TFTP data packet */
typedef struct data_packet
{
	uint16_t opcode;
	
	uint16_t blockno;
	
	char data[TFTP_BLOCK];

} data_packet;

/* Structure for TFTP ack packet */
typedef struct ack_packet
{
	uint16_t opcode;
	
	uint16_t blockno;
	
} ack_packet;

/* Structure for general packet */
typedef struct packet
{
	int length;
	
	char *content;
	
} packet;

packet *encode_init_packet(int, char *,char *);
packet *encode_data_packet(filedata *);
packet *encode_ack_packet(int);

init_packet *decode_init_packet(packet *);
data_packet *decode_data_packet(packet *);
ack_packet *decode_ack_packet(packet *);

#endif
