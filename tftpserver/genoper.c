/* Author: Jussi Laakkonen /  */

#include "genoper.h"

/* Size of the file */
unsigned long filesize = 0;

/*	Adds new data block to linked list, parameters:
		filedata *first	- pointer to first data block element
		char *newdata	- new data to be added
		int length	- length of data

	Returns the pointer to first element in data block list
*/
filedata *add_file_block(filedata *first,char *newdata, int length)
{
	filedata *current;
	
	/* Create new element and set data */
	filedata *newFD = (filedata *)malloc(sizeof(filedata));
	memcpy(newFD->data,newdata,length);
	newFD->length = length;
	newFD->next = NULL;
	
	filesize += newFD->length; /* Increase file size counter */

	/* Search place */
	if(first != NULL)
	{
		current = first;
		while(current->next != NULL) current = current->next;
		
		/* When place found, set block number */
		newFD->block_id = current->block_id + 1;
		
		/* Add to list as last */
		current->next = newFD;
		return first;
	}
	else
	{
		/* Otherwise list is empty, block added as first */
		newFD->block_id = 1;
		return newFD;
	}
}

/*	Adds new data in data packet to linked list, parameters:
		filedata *first		- pointer to first data block element
		data_packet *d_packet	- Data packet to be added
		int length		- total length of the packet

	Returns the pointer to first element in data block list
*/
filedata *add_data_packet(filedata *first, data_packet *d_packet, int length)
{
	filedata *current;

	/* Create new element */
	filedata *newFD = (filedata *)malloc(sizeof(filedata));	

	/* Set data */
	newFD->block_id = ntohs(d_packet->blockno);
	newFD->length = length - sizeof(uint16_t) - sizeof(uint16_t);
	memcpy(newFD->data,d_packet->data,length);
	newFD->next = NULL;

	filesize += newFD->length; /* Increase filesize */

	/* Search place and add to correct place */
	if(first != NULL)
	{
		current = first;
		
		while(current->next != NULL) current = current->next;

		current->next = newFD;

		return first;
	}
	/* Otherwise added as first */
	else return newFD;
}

/*	Returns value of the filesize counter */
unsigned long get_filesize()
{
	return filesize;
}

/*	Writes the given data to file, parameters:
		filedata *first	- pointer to first fileblock
		char *filename	- name of the file to be written

	Returns the amount of bytes written
*/		
unsigned long write_blocks_to_file(filedata *first,char *filename)
{
	int filed;
	unsigned long bytes_written = 0;

	/* Open the file for writing, create the file and grant rwx permissions to user */
	if((filed = open(filename,O_CREAT|O_WRONLY,S_IRWXU)) == -1)
	{
		printf("Cannot create file or get writing rights to file\n");
		bytes_written = -1; 
	}
	
	else
	{
		/* Go throuhg blocks and free the written one */
		while(first != NULL)
		{
			/* Write data to file */
			bytes_written += write(filed,first->data,first->length);

			/* Move to next and free previous */
			first = first->next;
		}
		close(filed);
	}
	return bytes_written;
}

/*	Reads the given file to data blocks, parameter:
		char *filename	- the name of the file to be read

	Returns the pointer to first datablock element
*/
filedata *read_file_to_blocks(char *filename)
{
	int filed;
	int bytes_read = 0;
	char buf[TFTP_BLOCK];
	filedata *files = NULL;

	/* Open the file for reading */
	if((filed = open(filename,O_RDONLY)) == -1) printf("Cannot open file for reading\n");

	else
	{
		bzero(&buf,TFTP_BLOCK);

		/* Read the file to TFTP_BLOCK size blocks */
		while((bytes_read = read(filed,&buf,TFTP_BLOCK)) > 0)
		{
			/* Add data */
			files = add_file_block(files,buf,bytes_read);
			bzero(&buf,TFTP_BLOCK);

			/* If blocksize is less than TFTP_BLOCK, it is last block */
			if(bytes_read < TFTP_BLOCK) break;
		}
		close(filed);
	}
	return files;
}

/* Clear the list */
void free_filedata(filedata *first)
{
	int count = 0;
	filedata *prev;
	filesize = 0;
	
	while(first != NULL)
	{
		prev = first;
		first = first->next;
		free(prev);
		count++;
	}

	//printf("Free'd %d blocks\n",count);
	return;
}
