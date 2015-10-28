/* Author: Jussi Laakkonen /  */
/* Description:	Header file for file operation functions */

#ifndef __FILEOPER_H
#define __FILEOPER_H

#include "definitions.h"

/* "Interface" functions */
int open_file_read(char*, enum data_modes);
int open_file_write(char*, enum data_modes);
int close_file();
int read_from_file(char*);
int write_to_file(char*,int);
/* End of "interface" functions */

int open_file_ascii_read(char*);
int open_file_ascii_write(char*);
int close_file_ascii();

int open_file_bin_read(char*);
int open_file_bin_write(char*);
int close_file_bin();

int read_bytes(char*);
int read_line(char*);

int write_bytes(char*,int);
int write_line(char*);

#endif
