/* Author: Jussi Laakkonen /  */

#ifndef __TFTPOPER_H
#define __TFTPOPER_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "genoper.h"

void set_conn_addr(struct sockaddr_in);

filedata *download_mode_loop(int,int);

int upload_mode_loop(int, filedata *);

packet *read_packet(int);

int send_packet(int, packet *);

#endif
