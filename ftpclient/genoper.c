/* Author: Jussi Laakkonen /  */
/* Description:	Generic helper operations for FTP client */

#include "genoper.h"

/*	Creates a port number by using time as seed, parameter:
		int create	- 1 for using new seed, 0 for next random with previous seed
	
	Returns the random port number created
*/
int create_port(int create)
{
	unsigned int port = 0;
	time_t t;

	/* Get the time in milliseconds and use it as a seed for random number generator */
	(void) time(&t);
	if(create == 1) srand((long)t);
	
	/* Create port */
	port = (unsigned int)rand();
	
	/* Create port until it is within the limits */
	while((port > 65000) || (port < 5000))
	{
		port = (unsigned int)rand();
	}
	//printf("random port: %d\n",port);
	
	return port;
}

/*	Parses IP-address and port nubmer from "227 Entering Passive Mode (x1,x2,x3,x4,p1,p2)",
	parameter:
		char *row	- previously mentioned text line
		
	Returns a sockaddr_in struct with parsed address and port.
*/
struct sockaddr_in parse_address(char *row)
{
	int pos = 0, i = 0, count = 0, port = 0;
	char addr[15];
	char port_mp[4];
	char port_add[4];
	
	struct sockaddr_in address;
	struct in_addr hostaddr;
	struct hostent *host;
	
	/* Get position of opening bracket and move to next */
	pos = strcspn(row,"(") + 1;
	
	/* Clear address array and parse IP-address from row */
	bzero(&addr,15);
	for(i = pos; i < strlen(row) ; i++)
	{
		if(row[i] == ',') /* Replace with "." */
		{
			if(count == 3) break; /* If found third comma */
			addr[i-pos] = '.';
			count++;
		}
		else addr[i-pos] = row[i]; /* Copy char */
	}
	pos = i+1;
	
	bzero(&port_mp,4);

	/* Get port "multiplier" */
	for(i = pos; i < strlen(row); i++)
	{
		if(row[i] == ',') /* If comma found, add null to end */
		{
			port_mp[i-pos] = '\0';
			break;
		}
		else port_mp[i-pos] = row[i]; /* Copy char */
	}
	
	pos = i+1;
	bzero(&port_add,4);
	
	/* Get port "addition" */
	for(i = pos; i < strlen(row); i++)
	{
		if(row[i] == ')') /* If closing bracket found, add null to end */
		{
			port_add[i-pos] = '\0';
			break;
		}
		else port_add[i-pos] = row[i]; /* Copy char */
	}
	
	/* Calculate port ((p1 * 256) + p2) */
	port = (atoi(port_mp) * 256) + (atoi(port_add));
	
	/* Set get host from IP-address */
	inet_aton(addr,&hostaddr);
	host = gethostbyaddr(&hostaddr, sizeof(hostaddr),AF_INET);
	
	//printf("port:%d\n",port);
	
	/* Set address */
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr = *((struct in_addr *)host->h_addr);
	
	return address;
}

/*	Create parameter for PORT command, parameter:
		int portno	- port number

	Returns the parameter for PORT command
*/
char *create_port_param(int portno)
{
	int i = 0;
	int m, r;
	char *own_name = (char*)malloc(128);
	char *own_ip = (char*)malloc(16);
	char *port = (char*)malloc(3);
	
	char *full_addr = (char*)malloc(23);
	struct hostent *h;
	
	/* Get own hostname */
	gethostname(own_name,128);
	
	/* Error, maybe not possible */
	if((h = gethostbyname(own_name)) == NULL) return NULL;
	
	/* Put own IP address to char array */
	sprintf(own_ip,"%s",inet_ntoa(*((struct in_addr *)h->h_addr)));

	/* Copy numbers to full address and replace period with comma */
	bzero(full_addr,23);
	for(i = 0; i < strlen(own_ip); i++)
	{
		if(own_ip[i] == '.') full_addr[i] = ',';
		else full_addr[i] = own_ip[i];
	}

	/* Calculate port parameters */
	m = portno / 256; /* Multiplier */
	r = portno % 256; /* Addition (remainder */

	bzero(port,3);
	sprintf(port,",%d,",m);
	
	/* Add multiplier */
	strcat(full_addr,port);
	
	bzero(port,3);
	sprintf(port,"%d",r);
	
	/* Add addition */
	strcat(full_addr,port);

	//printf("%s\n",full_addr);
	
	free(own_name);
	free(own_ip);
	free(port);
	
	return full_addr;
}

/*	Parse code from reply, parameter:
		char *reply	- Retrieved message
	
	Returns the parsed code
*/
int getcode(char *reply)
{
	int code = 0;
	char *charcode = (char *)malloc(3);
	
	bzero(charcode,3);
	
	/* Copy first three characters */
	charcode = strncpy(charcode,reply,3);
	
	/* To integer */
	if((code = atoi(charcode)) == 0) code = -1;
	
	free(charcode);
	
	return code;
}

/*	Get parameter from given message, parameters:
		char *buffer	- contains the full parameter
		int position	- Starting position
		
	Returns the parameter
*/
char *getparam(char *buffer, int position)
{
	int i = 0;
	char *param = (char *)malloc(BUFSIZE);
	
	bzero(param,BUFSIZE);
	
	/* Copy parameters forward from position, if space, null or newline found stop */
	for(i = position; i < strlen(buffer); i++)
	{
		if((buffer[i] == ' ') || (buffer[i] == '\0') || (buffer[i] == '\n')) break;
		else param[i-position] = buffer[i];
	}
	
	return param;
}

/*	Prints the usage text, parameter:
		int help 	- print command: 1
*/
void usage(int help)
{
	if(help == 1) printf("FTPCLIENT HELP\n");
	printf("\nUsage:\n-with parameters:\t./ftpclient -p <port> -h <host>\n");
	printf("-without parameters:\t./ftpclient\n\n! If using without parameters, use: open <addr>:<port> when connecting !\n\n");
	printf("For help with program commands, use \"help\" command in program or ./ftpclient --help\n\n");
	if(help == 1) commands();
	exit(1);
}

/* Prints error message and exits, parameter:
		char *msg	- message to be printed
*/
void error(char *msg)
{
	perror(msg);
	exit(1);
}

/* Prints command list and help in alphabetical order */
void commands()
{
	printf("Command list in alphabetical order:\n\n");
	printf("active\t\t\tSet client to active mode\n");
	printf("ascii\t\t\tSet file transfer mode to ascii\n");
	printf("binary\t\t\tSet file transfer mode to binary\n");
	printf("cd <path>\t\tChange working directory to <path>\n");
	printf("close\t\t\tClose connection to server\n");
	printf("get <file>\t\tRetrieve file <file> from server\n");
	printf("help\t\t\tPrint this help\n");
	printf("ls\t\t\tShow files in current working directory\n");
	printf("open <addr>:<port>\tOpen connection to server <addr> in <port>\n");
	printf("passive\t\t\tSet client to passive mode\n");
	printf("put <file>\t\tStore file <file> to server\n");
	printf("quit\t\t\tQuit and close active connections\n\n");

}
