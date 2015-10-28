/* Author: Jussi Laakkonen /  */
/* Description:	Header file for generic functions */

#ifndef __GENOPER_H
#define __GENOPER_H

#include "fileoper.h"

int create_port();
struct sockaddr_in parse_address(char*);
char *create_port_param(int);
int getcode(char*);
char *getparam(char*,int);

void usage(int);
void error();
void commands();

#endif
