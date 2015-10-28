/* Author: Jussi Laakkonen /  */

#ifndef __SERVER_H
#define __SERVER_H

#define PARAMS 2

#include <time.h>

#include "tftpoper.h"

int runServer(int);

char *parse_filename(char[]);

int create_port();

#endif
