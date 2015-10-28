/* Author: Jussi Laakkonen /  */

#ifndef __GENOPER_H
#define __GENOPER_H

#include "structures.h"

filedata *add_file_block(filedata *,char *, int);
filedata *add_data_packet(filedata *,data_packet *, int);

unsigned long get_filesize();

unsigned long write_blocks_to_file(filedata *, char *);
filedata *read_file_to_blocks(char *);

void free_filedata(filedata *);

#endif
