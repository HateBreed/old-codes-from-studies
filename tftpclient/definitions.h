/* Author: Jussi Laakkonen /  */

#ifndef __DEFINITIONS_H
#define __DEFINITIONS_H

#define OPCODE_RRQ 1
#define OPCODE_WRQ 2
#define OPCODE_DATA 3
#define OPCODE_ACK 4
#define OPCODE_ERROR 5

#define TFTP_BLOCK 512
#define MAX_MSG 516

#define TIMEOUT_SEC 1
#define TIMEOUT_USEC 000

#define TFTP_SERVER_PORT 69

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <netinet/in.h>


#endif
