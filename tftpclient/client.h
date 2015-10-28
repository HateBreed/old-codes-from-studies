/* Author: Jussi Laakkonen /  */

#ifndef __CLIENT_H
#define __CLIENT_H

#define PARAMS 5

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "genoper.h"

/* Client mode enumeration - codes from definitions.h */
enum cl_mode {UPLOAD = OPCODE_WRQ, DOWNLOAD = OPCODE_RRQ};

int runClient(struct hostent *, int, char *);

filedata *download_mode_loop();

int upload_mode_loop(filedata *);

packet *read_packet();

int send_packet(packet *);

#endif
