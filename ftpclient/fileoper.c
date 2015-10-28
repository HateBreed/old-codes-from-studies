/* Author: Jussi Laakkonen /  */
/* Description:	File operation functions for FTP client */

#include "fileoper.h"

/* File pointer for ASCII (text files) */
FILE *filep = NULL;

/* File descriptor for binary files */
int filed = -1;

/* ASCII or BINARY */
enum data_modes t_mode;

/* "Interface" for opening file for reading in correct mode, parameters:
		char *filename			- Name of the file to be opened
 		enum data_modes mode	- Opening mode

	returns the value returned by opening function (-1 error)
*/
int open_file_read(char *filename, enum data_modes mode)
{
	int rval = 0;
	
	/* Set the mode */
	t_mode = mode;
	
	switch(t_mode)
	{
		case ASCII:
		{
			rval = open_file_ascii_read(filename);
			break;
		}
		
		case BINARY:
		{
			rval = open_file_bin_read(filename);
			break;
		}
	}
	
	return rval;
}

/* "Interface" for opening file for writing in correct mode, parameters:
		char *filename			- Name of the file to be opened
 		enum data_modes mode	- Opening mode

	returns the value returned by opening function (-1 error)
*/
int open_file_write(char *filename, enum data_modes mode)
{
	int rval = 0;
	
	/* Set the mode */
	t_mode = mode;
	
	switch(t_mode)
	{
		case ASCII:
		{
			rval = open_file_ascii_write(filename);
			break;
		}
		
		case BINARY:
		{
			rval = open_file_bin_write(filename);
			break;
		}
	}
	return rval;
}

/* "Interface" for closing the file in correct mode

	Returns the value returned by closing function.
*/
int close_file()
{
	int rval = 0;
	
	switch(t_mode)
	{
		case ASCII:
		{
			rval = close_file_ascii();
			break;
		}
		
		case BINARY:
		{
			rval = close_file_bin();
			break;
		}
	}
	return rval;
}

/* "Interface" for reading data from file with correct mode, parameter:
		char *b	- Storage for data to be read
	
	Returns amount of data read in BINARY mode and length of data storage in ASCII
*/
int read_from_file(char *b)
{
	int rval = 0;
	switch(t_mode)
	{
		case ASCII:
		{
			rval = read_line(b);
			break;
		}
		
		case BINARY:
		{
			rval = read_bytes(b);
			break;
		}
	}
	return rval;
}


/*	"Interface" for writing data to file with correct mode, parameters:
		char *b		- Data to be written
		int length 	- Length of the data to be written (used only in BINARY)
		
	Returns amount of data read in BINARY and nonnegative if successfully wrote in ASCII
*/
int write_to_file(char *b,int length)
{
	int rval = 0;
	switch(t_mode)
	{
		case ASCII:
		{
			rval = write_line(b);
			break;
		}
		
		case BINARY:
		{
			rval = write_bytes(b,length);
			break;
		}
	}
	return rval;
}

/*	Opens the file for reading in ASCII mode (text), parameter:
		char *filename	- the name of the file to be opened
		
	Returns 0 if success, -1 if cannot open file (not found, no rights)
*/
int open_file_ascii_read(char *filename)
{
	if((filep = fopen(filename,"r")) == NULL)
	{
		printf("Cannot open file \"%s\" for reading\n",filename);
		perror("Reason");
		return -1;
	}
	else return 0;
}

/*	Opens the file for writing in ASCII mode (text), file is truncated to zero
	if exists and created if it doesn't exist, parameter:
		char *filename	- the name of the file to be opened
		
	Returns 0 if success, -1 if cannot open file (not found, no rights)
*/
int open_file_ascii_write(char *filename)
{
	if((filep = fopen(filename,"w+")) == NULL)
	{
		printf("Cannot open file \"%s\" for writing\n",filename);
		perror("Reason");
		return -1;
	}
	else return 0;
}

/*	Close the text file

	Returns the value of fclose function.
*/
int close_file_ascii()
{
	return fclose(filep);
}


/*	Opens the file for reading in BINARY mode , parameter:
		char *filename	- the name of the file to be opened
		
	Returns 0 if success, -1 if cannot open file (not found, no rights)
*/
int open_file_bin_read(char *filename)
{
	if((filed = open(filename,O_RDONLY)) == -1)
	{
		printf("Cannot open file \"%s\" for reading\n",filename);
		perror("Reason");
		return -1;
	}
	else return 0;
}

/*	Opens the file for writing in ASCII mode (text), file is truncated to zero
	if exists and created if it doesn't exist, parameter:
		char *filename	- the name of the file to be opened
		
	Returns 0 if success, -1 if cannot open file (not found, no rights)
*/
int open_file_bin_write(char *filename)
{
	if((filed = open(filename,O_CREAT|O_WRONLY,S_IRWXU)) == -1)
	{
		printf("Cannot open file \"%s\" for writing\n",filename);
		perror("Reason");
		return -1;
	}
	else return 0;
}

/* 	Close binary file

	Returns the value of close function
*/
int close_file_bin()
{
	return close(filed);
}

/*	Read bytes (amount defined by DATASIZE) from BINARY file, parameter:
		char *b	- Storage for data
		
	Returns the amount of data read.
*/
int read_bytes(char *b)
{
	int n;
	char buf[DATASIZE];
	
	/* Clear buffer and read */
	bzero(&buf,DATASIZE);
	n = read(filed,&buf,DATASIZE);
	
	/* If some data was read */
	if(n > 0)
	{
		/* Clear storage and copy data */
		bzero(b,DATASIZE);
		memcpy(b,buf,n);
	}
	
	return n;
}

/*	Read line of text from file, size defined by DATASIZE, parameter:
		char *line	- Storage for data
		
	Returns the lenght of read data
*/
int read_line(char *line)
{
	int n = 0;
	char buf[DATASIZE];
	
	bzero(&buf,DATASIZE);
	
	/* Read a line, terminated by newline marker or the length defined
		by DATASIZE, if cannot read, return 0
	*/
	if((fgets(buf,DATASIZE,filep)) == NULL) return 0;
	else
	{
		n = strlen(buf);
		
		/* Replace '\n' with '\r\n' if not already in correct form */
		if((n < DATASIZE) && (buf[n-1] == '\n') && (buf[n-2] != '\r'))
		{
			buf[n-1] = '\r';
			buf[n] = '\n';
			n++; /* size + 1*/
			
		}
		
		/* Clear storage and copy data */
		bzero(line,DATASIZE);
		memcpy(line,buf,n);
		return n;
	}
}

/*	Write given data to BINARY file, parameters:
		char *b		- Data to be written
		int length	- Length of the data to be written
		
	Returns the amount of data written to file
*/
int write_bytes(char *b, int length)
{	
	return write(filed,b,length);
}

/*	Write given data to ASCII file, parameter:
		char *line	- Data to be written
		
	Returns nonnegative integer if success
*/
int write_line(char *line)
{
	return fputs(line,filep);
}

