/* Author: Jussi Laakkonen /  */

#ifndef __FTPCLIENT_H
#define __FTPCLIENT_H

#include "genoper.h"

void run();

int set_address(char*);
int open_connection();
void close_connection();
int open_data_conn();
int open_data_conn_pasv();
void close_data_conn();

int send_cmd(char*,char*);

int checksockets();
void setsockets();

int checkcmd(char*);
void check_code(int);

#endif
