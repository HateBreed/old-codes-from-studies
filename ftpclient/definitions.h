/* Author: Jussi Laakkonen /  */

#ifndef __DEFINITIONS_H
#define __DEFINITIONS_H

#define PARAMS 5

#define BUFSIZE 1024
#define DATASIZE 128

#define OPENING_DATA 150
#define CMD_OK 200
#define SERVICE_READY 220
#define BYE 221
#define TRANSFER_COMPL 226
#define PASSIVE_MODE 227
#define LOGGED_IN 230
#define FILE_ACT_OK 250
#define PASS_REQ 331
#define CLOSING 421
#define SYNTAX_ERR 500
#define LOGIN_ERROR 530
#define ACTION_FAIL 550

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>


enum states {NOTCONNECTED,CONNECTED};

enum states_t {CLOSED,LISTEN,PASSIVE,TRANSFER};

enum cmd_states {NONE,OPEN,USER,PASS,LS,QUIT,DATAMODE,CWD,STOR,RETR};

enum list_states {MODE,PORT,LIST,DONE};

enum data_modes {ASCII,BINARY};

enum transfer_modes {START,OPENING,STOP};

enum client_modes {ACT,PASV};

#endif
