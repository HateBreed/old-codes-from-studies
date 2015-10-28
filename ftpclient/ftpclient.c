/* Author: Jussi Laakkonen / 

Sources used as help:

RFC959 - FTP protocol description
	
Own previous excercise works (mainly ex3)
	
Linux man pages
	
Beej's Guide to Network Programming
	http://beej.us/guide/bgnet/
	
The World of select()
	http://www.lowtek.com/sockets/select.html

*/

#include "ftpclient.h"

int sock = -1;		/* Control connection socket */
fd_set socketset;
int highsocket;
int hostset = -1; 	/* If server address set */
 
enum states state; 			/* Connection state */
enum states state_t; 			/* Transfer state */
enum cmd_states cmd_state;		/* Command state */
enum list_states list_state;		/* LIST command state */
enum data_modes data_mode;		/* Data mode state */
enum transfer_modes transfer_mode;	/* Transfer mode state */
enum client_modes client_mode; 		/* Client mode state */

struct sockaddr_in serveraddr;		/* Server control address */

struct sockaddr_in transfer;		/* Data transfer address */
struct sockaddr_in transfer_s;		/* Address of server (in active mode, when server connects to client) */

/* Data transfer socket, listening socket for data transfer */
int sock_t = -1, listen_t = -1;

unsigned int addr_len;

char filename[BUFSIZE]; /* Filename, for storing and retrieving */

int main(int argc, char *argv[])
{
	int port_ok = 0, host_ok = 0, i = 0;
	int port;
	
	struct in_addr hostaddr;
	struct hostent *host;

	/* Program started with parameters */
	if(argc == 5)
	{
		for(i = 0; i < argc; i++)
		{
			/* Port */
			if(strcmp(argv[i],"-p") == 0)
			{
				if(i+1 < argc)
				{
					i++;
					if((port = atoi(argv[i])) == 0)
					{
						port_ok = 0;
						printf("Invalid port.\n");
					}
					else port_ok = 1;
				}
			}
			
			/* Host */
			else if(strcmp(argv[i],"-h") == 0)
			{
				if(i+1 < argc)
				{
					i++;
					
					/* Hostname given, check if ok */
					if((host = gethostbyname(argv[i])) == NULL)
					{
						/* Address of host given */
						if(inet_aton(argv[i],&hostaddr) == 0)
						{
							printf("Unknown host: %s.\n",argv[i]);
							host_ok = 0;
						}
						else
						{
							/* Is address of host ok */
							if((host = gethostbyaddr(&hostaddr, sizeof(hostaddr),AF_INET)) == NULL)
							{
								printf("Unknown host: %s.\n",argv[i]);
								host_ok = 0;
							}
							else host_ok = 1;
						}
					}
					else host_ok = 1;
				}
			}
		}
	}
	
	/* If no parameters */
	else if(argc == 1) hostset = 0;
	
	/* If "--help" as parameter */
	else if((argc == 2) && (strcasecmp(argv[1],"--help") == 0)) usage(1);
	
	/* Otherwise invalid */
	else
	{
		printf("Invalid parameters.\n");
		usage(0);
	}
	
	/* Host or port invalid */
	if(((host_ok == 0) || (port_ok == 0)) && (hostset == -1)) usage(0);
	
	/* Host and port set */
	else if((host_ok == 1) || (port_ok == 1))
	{
		hostset = 1;
	
		serveraddr.sin_family = AF_INET;
		serveraddr.sin_port = htons(port);
		serveraddr.sin_addr = *((struct in_addr *)host->h_addr);
	}
	
	addr_len = sizeof(struct sockaddr_in);
	
	run();
	
	return 0;
}

/*	Run loop for client */
void run()
{
	int rv = 0;
	struct timeval to;
	
	/* Set starting states */
	state = NOTCONNECTED;
	state_t = CLOSED;
	cmd_state = NONE;
	list_state = DONE;
	data_mode = ASCII;
	transfer_mode = STOP;
	client_mode = ACT;
	
	while(1)
	{
		/* Set sockets */
		setsockets();
		to.tv_sec = 0;
		to.tv_usec = 1000;
		
		/* Check if something received */
		rv = select(highsocket+1,&socketset, NULL, NULL, &to);
		
		if(rv < 0)
		{
			error("Error in select");
			break;
		}
		/* Got something */
		else if(rv > 0)
		{
			/* If error or quitting */
			if(checksockets() == -1)
			{
				break;
			}
		}
	}
}

/*	Parses address and port from given parameter and sets them to server address,
	parameter:
		char *address	- the whole address in <addr>:<port> format
		
	Returns 0 if successful, -1 if error
*/
int set_address(char* address)
{
	int position = 0, i = 0, portno = 0;
	
	char *addr = NULL;
	char *port = NULL;
	
	struct in_addr hostaddr;
	struct hostent *host = NULL;
	
	/* Get position of colon */
	position = strcspn(address,":");
	
	addr = (char *)malloc(256);
	bzero(addr,256);
	
	strncpy(addr,address,position);
	
	/* Resolve host */
	if((host = gethostbyname(addr)) == NULL)
	{
		/* Address of host given */
		if(inet_aton(addr,&hostaddr) == 0)
		{
			printf("Unknown host: %s\n",addr);
			free(addr);
			free(port);
			hostset = 0;
			return -1;
		}
		else
		{
			/* Is address of host ok */
			if((host = gethostbyaddr(&hostaddr, sizeof(hostaddr),AF_INET)) == NULL)
			{
				printf("Unknown host: %s\n",addr);
				free(addr);
				free(port);
				hostset = 0;
				return -1;
			}
		}
	}
	
	/* Get port */
	port = (char *)malloc(6);
	bzero(port,6);
	for(i = position+1; i < strlen(address); i++) port[i-position-1] = address[i];
	
	if((portno = atoi(port)) == 0)
	{
		printf("Invalid port.\n");
		free(addr);
		free(port);
		hostset = 0;
		return -1;
	}
	
	//printf("%s:%d\n",addr,portno);
	
	/* Set as server address */
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(portno);
	serveraddr.sin_addr = *((struct in_addr *)host->h_addr);
	
	free(addr);
	free(port);
	
	hostset = 1; /* Server is set */
	return 0;
	
}

/*	Open connection to server

	Returns 1 if successfull
*/
int open_connection()
{
	/* Create socket */
	if((sock = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		perror("Error creating socket");
		fflush(stderr);
		return -1;
	}
	else
	{
		/* Connect */
		if(connect(sock,(struct sockaddr *)&serveraddr,addr_len) < 0)
		{
			perror("Cannot connect to server.\n");
			fflush(stderr);
			return -1;
		}
		else 
		{
			return 1;
		}
	}
}

/* Close connection to server */
void close_connection()
{
	printf("Connection closed to server.\n");
	state = NOTCONNECTED;
	close(sock);
	sock = -1;
}

/*	Open data connection (active mode),

	Returns the port that client listens
*/
int open_data_conn()
{
	/* Create listening socket */
	if((listen_t = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		perror("Cannot create listening socket");
		return -1;
	}
	
	/* Set address */
	transfer.sin_family = AF_INET;
	transfer.sin_addr.s_addr = htons(INADDR_ANY);
	transfer.sin_port = htons(create_port(1));
	
	/* Try to bind, if not successful create new port until it is successful */
	while(bind(listen_t,(struct sockaddr *)&transfer,addr_len) < 0)
	{
		transfer.sin_port = htons(create_port(0));
	}
	
	/* Listen */
	listen(listen_t,1);
	
	return ntohs(transfer.sin_port);
}

/*	Open data connection (Passive mode)

	Returns 1 when successful
*/
int open_data_conn_pasv()
{
	/* Create socket */
	if((sock_t = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		perror("Error creating socket");
		fflush(stderr);
		return -1;
	}
	else
	{
		/* Connect to server */
		if(connect(sock_t,(struct sockaddr *)&transfer,addr_len) < 0)
		{
			perror("Cannot connect to server in passive mode.\n");
			return -1;
		}
		else 
		{
			//printf("Connected in passive mode\n");
			return 1;
		}
	}
}

/* Close data connection */
void close_data_conn()
{
	if(sock_t != -1) printf("Data connection closed.\n");
	state_t = CLOSED;
	if(sock_t != -1) close(sock_t);
	sock_t = -1;
	
	/* If active, close listening socket */
	if(client_mode == ACT)
	{
		if(listen_t != -1) close(listen_t);
		listen_t = -1;
	}
}

/*	Send command to server, parameters:
		char *cmd	- Command
		char *param	- Command parameter, NULL if no parameter
		
	Returns the amount of bytes sent
*/
int send_cmd(char *cmd, char *param)
{
	int n;
	int length;
	
	char msg[BUFSIZE];
	
	bzero(&msg,BUFSIZE);
	
	/* If no parameter given */
	if(param == NULL)
	{
		sprintf(msg,"%s\r\n",cmd);
		length = strlen(cmd) + 2;
	}
	/* Parameter given */
	else
	{
		sprintf(msg,"%s %s\r\n",cmd,param);
		length = strlen(cmd) + strlen(param) + 3;
	}
	
	/* Send */
	n = send(sock,msg,length,0);

	//printf("Sent: %s", msg);
	
	return n;
}

/*	Checks sockets for incoming data that can be read

	Returns 0 or the value of command checking function (-1 when quitting)
*/
int checksockets()
{
	int returnvalue = 0,code = 0;
	int n = 0, position = 0, r_found = 0;
	unsigned long size = 0, speed = 0;
	long t1 = 0,t2 = 0;
	char *buffer;
	char b;
	char param[BUFSIZE];
	
	/* Standard input */
	if(FD_ISSET(0,&socketset))
	{
		buffer = (char *)malloc(BUFSIZE);
		bzero(buffer,BUFSIZE);
		
		fgets(buffer,BUFSIZE-1,stdin); /* Get input */
		
		returnvalue = checkcmd(buffer); /* Check input */
		free(buffer);
	}
	
	/* If connected and not transferring and there is something in command socket */
	if((state == CONNECTED) && (state_t == CLOSED) && (FD_ISSET(sock,&socketset)))
	{
		bzero(&param,BUFSIZE);
		while((n = recv(sock,&b,1,0)) > 0)
		{
			if(b == '\r') r_found = 1; /* If '\r' found */

			else
			{	
				param[position] = b;
				
				/* A full line, print and check it */
				if(r_found == 1 && b == '\n')
				{	
					code = getcode(param); /* Parse code */
					printf("%s",param);
					
					/* If in passive mode and server sent listening port info */
					if((code == PASSIVE_MODE) && (client_mode == PASV))
					{
						transfer = parse_address(param); /* Parse address and set it to transfer addr*/
					}
					
					/* Received closing */
					if(code == CLOSING)
					{
						close_connection();
						cmd_state = NONE;
						break;
					}
					check_code(code); /* Check the code */
					bzero(&param,BUFSIZE);
					break;
				}
				else position++;
			}
		}
	}
	
	/* If transferring and there is something in socket */
	if((state_t == TRANSFER) && (FD_ISSET(sock_t,&socketset)))
	{	
		/* Retrieving list */
		if(cmd_state == LS)
		{
			bzero(&param,BUFSIZE);
			while(1)
			{
				/* Receive byte by byte from socket */
				if((n = recv(sock_t,&b,1,0)) > 0)
				{
					if(b == '\r') r_found = 1;
					
					else
					{
						param[position] = b;
						
						/* When got full line, print */
						if((r_found == 1) && (b == '\n'))
						{
							printf("-%s",param);
							bzero(&param,BUFSIZE);
							position = 0;
						}
						else position++;
					}
				}
				else /* Stop */
				{
					state_t = CLOSED;
					list_state = DONE;
					break;
				}
			}
		}
		
		/* Receiving file */
		else if(cmd_state == RETR)
		{
			buffer = (char *)malloc(DATASIZE);
			printf("Transferring file..");
			fflush(stdout);
			
			bzero(buffer,DATASIZE);
			t1 = time(&t1); /* Get start time (seconds)*/
			
			/* Receive in DATASIZE blocks */
			while((n = recv(sock_t,buffer,DATASIZE,0)) > 0)
			{
				if(data_mode == ASCII) buffer[n] = '\0'; /* If ASCII, end with NULL */
				write_to_file(buffer,n); /* Write */
				size += n;
				bzero(buffer,DATASIZE);
			}
			
			t2 = time(&t2) - t1; /* Calculate difference */

			close_file();
			
			free(buffer);
			
			/* Calculate speed */
			if(t2 == 0) speed = size; /* 0 seconds, speed is size in one second */
			else speed = (size / t2);
			
			printf(". done.\nFile size: %ld bytes transferred at %ld bytes/s\n",size,speed);
			fflush(stdout);
			state_t = CLOSED;
			transfer_mode = STOP;
		}
	}

	/* If listening and socket is connected */
	if((state_t == LISTEN) && (FD_ISSET(listen_t,&socketset)))
	{
		/* Accept and proceed to transfer */
		sock_t = accept(listen_t,(struct sockaddr *)&transfer_s,&addr_len);
		state_t = TRANSFER;
	}
	
	return returnvalue;
}

/*	Set sockets depending on the states */
void setsockets()
{
	int n = 0,i = 0;
	char *b;
	unsigned long size = 0,speed = 0;
	long t1 = 0,t2 = 0;
	
	FD_ZERO(&socketset);
	FD_SET(0,&socketset); /* Set stdin */
	
	highsocket = 0;
	
	/* Connection state */
	switch(state)
	{
		case NOTCONNECTED: break;
		
		case CONNECTED:
		{
			FD_SET(sock,&socketset);
			if(sock > highsocket) highsocket = sock;
			break;
		}
		
		default: break;
	}
	
	/* Transfer state */
	switch(state_t)
	{
		case TRANSFER:
		{
			/* Transferring and storing a file to server */
			if(cmd_state == STOR)
			{
				printf("Transferring file..");
				fflush(stdout);
				b = (char*)malloc(DATASIZE);

				t1 = time(&t1); /* Start time */
				
				/* Read and send in blocks */
				while((i = read_from_file(b)) > 0)
				{
					n = send(sock_t,b,i,0);
					size += n;
					/* If BINARY data and read less than DATASIZE -> last packet */
					if((data_mode == BINARY) && (i < DATASIZE)) break;
				}
				t2 = time(&t2) - t1; /* Caclulate difference */

				close_file();
				free(b);
				
				/* Calculate speed */
				if(t2 == 0) speed = size;
				else speed = (size / t2);
				
				printf(". done.\nFile size: %ld bytes transferred at %ld bytes/s\n",size,speed);
				state_t = CLOSED;
				transfer_mode = STOP;
				close_data_conn();
			}
			
			/* Otherwise add transfer socket */
			else
			{
				FD_SET(sock_t,&socketset);
				if(sock_t > highsocket) highsocket = sock_t;
			}
			break;
		}
		
		/* Listening, set listening socket */
		case LISTEN:
		{
			FD_SET(listen_t,&socketset);
			if(listen_t > highsocket) highsocket = listen_t;
			break;
		}

		/* Passive mode enabled, set state to TRANSFER */
		case PASSIVE:
		{
			state_t = TRANSFER;
			break;
		}
		
		default: break;
	}
}

/*	Check the given command, parameter:
		char *buffer	- command
		
	Returns 0 when ok, -1 when quitting
*/
int checkcmd(char *buffer)
{
	int i = 0;
	char cmd[8];
	char *param;
	
	bzero(&cmd,8);
	
	/* Get the command */
	for(i = 0; i < 8; i++)
	{
		if((buffer[i] == ' ') || (buffer[i] == '\0') || (buffer[i] == '\n'))
		{
			cmd[i] = '\0'; /* End with null */
			break;
		}
		else
		{
			cmd[i] = buffer[i];
		}
	}
	i++; /* Move to next char, jump over space */
	
	/* To active mode */
	if(strcasecmp(cmd,"active") == 0)
	{
		if(state != CONNECTED) printf("Not connected to server\n");
		else
		{
			printf("Active mode enabled.\n");
			client_mode = ACT;
		}
	}
	
	/* Data mode to ascii */
	else if(strcasecmp(cmd,"ascii") == 0)
	{
		printf("ASCII transfer mode enabled\n");
		send_cmd("TYPE","A"); /* Send mode ascii*/
		data_mode = ASCII;
		cmd_state = DATAMODE;
	}
	
	/* Data mode to binary */
	else if(strcasecmp(cmd,"binary") == 0)
	{
		printf("Binary transfer mode\n");
		send_cmd("TYPE","I"); /* Send mode binary */
		data_mode = BINARY;
		cmd_state = DATAMODE;
	}
	
	/* Changing directory */
	else if(strcasecmp(cmd,"cd") == 0)
	{
		if(state != CONNECTED) printf("Not connected to server\n");
		else 
		{
			/* If no parameter */
			if((strlen(buffer) - i) <= 0)
			{
				printf("No parameter given for command: %s\n",cmd);
				return 0;
			}
			else
			{
				param = getparam(buffer,i); /* Get parameter */
				cmd_state = CWD;
				send_cmd("CWD",param); /* Send command */
				free(param);
			}
		}
	}
	
	/* Closing */
	else if(strcasecmp(cmd,"close") == 0)
	{
		if(state != CONNECTED) printf("Not connected to server\n");
		else
		{
			cmd_state = QUIT;
			send_cmd("QUIT",NULL); /* Send quit */
		}
	}
	
	/* Retrieving file from server */
	else if(strcasecmp(cmd,"get") == 0)
	{
		if(state != CONNECTED) printf("Not connected to server\n");
		else 
		{
			/* If no param */
			if((strlen(buffer) - i) <= 0)
			{
				printf("No parameter given for command: %s\n",cmd);
				return 0;
			}
			else param = getparam(buffer,i); /* Get param */

			cmd_state = RETR;
			
			/* Set filename */
			bzero(&filename,BUFSIZE);
			strncpy(filename,param,strlen(param));
			
			free(param);
			
			/* Try to open file for writing */
			if(open_file_write(filename,data_mode) == -1)
			{
				cmd_state = NONE;
				transfer_mode = STOP;
				return 0;
			}

			/* If in active mode, send PORT command */
			if(client_mode == ACT)
			{
				buffer = create_port_param(open_data_conn());
				send_cmd("PORT",buffer);
				
			}
			
			/* Otherwise in passive mode, send PASV command */
			else
			{
				send_cmd("PASV",NULL);
			}
			transfer_mode = START;
		}
	}
	
	/* Listing files in current working directory */
	else if(strcasecmp(cmd,"ls") == 0)
	{
		if(state != CONNECTED) printf("Not connected to server\n");
		else
		{	
			cmd_state = LS;
			send_cmd("TYPE","A"); /* Send type ascii */
			list_state = MODE;
			data_mode = ASCII;
		}
	}
	
	/* Opening connection */
	else if(strcasecmp(cmd,"open") == 0)
	{
		if(state == CONNECTED) printf("Already connected to server\n");
		else
		{
			/* If parameter given, get it and try to set server address */
			if((strlen(buffer) - i) > 0)
			{
				param = getparam(buffer,i);
	
				if(set_address(param) < 0)
				{
					free(param);
					return 0;
				}
				free(param);
			}
			
			/* If no server address set */
			if(hostset == 0)
			{
				printf("No server address specified. Give address and port with command:\nopen <address>:<port>\ne.g. open server.fi:21\n");
				return 0;
			}
			
			/* Open connection */
			if(open_connection() != 1) error("Connection error");
			
			cmd_state = OPEN;
			
			state = CONNECTED;
		}
	}
	
	/* Enabling passive mode */
	else if(strcasecmp(cmd,"passive") == 0)
	{
		if(state != CONNECTED) printf("Not connected to server\n");
		else
		{
			printf("Passive mode enabled.\n");
			client_mode = PASV;
		}
	}
	
	/* Storing file to server */
	else if(strcasecmp(cmd,"put") == 0)
	{
		if(state != CONNECTED) printf("Not connected to server\n");
		else 
		{
			/* If no parameter given */
			if((strlen(buffer) - i) <= 0)
			{
				printf("No parameter given for command: %s\n",cmd);
				return 0;
			}
			else param = getparam(buffer,i); /* Get parameter */
			
			cmd_state = STOR;
			
			/* Set filename */
			bzero(&filename,BUFSIZE);
			strncpy(filename,param,strlen(param));
			
			free(param);
			
			/* Open file for reading */
			if(open_file_read(filename,data_mode) == -1)
			{
				cmd_state = NONE;
				transfer_mode = STOP;
				return 0;
			}
			
			/* If in active mode, send PORT command */
			if(client_mode == ACT)
			{
				buffer = create_port_param(open_data_conn());
				send_cmd("PORT",buffer);
			}
			/* Else in passive mode, send PASV command */
			else send_cmd("PASV",NULL);
			
			transfer_mode = START;
		}
	}
	
	/* Quitting */
	else if(strcasecmp(cmd,"quit") == 0)
	{
		if(state == CONNECTED)
		{
			printf("Connected to server\nClosing connections.\n");
			cmd_state = QUIT;
			send_cmd("QUIT",NULL); /* Send quit, don't wait for server reply */
		}
		return -1; /* -1 - quitting */
	}

	/* print command help */
	else if(strcasecmp(cmd,"help") == 0)
	{
		commands();
	}
	
	/* Otherwise invalid, print commands */
	else
	{
		printf("INVALID COMMAND");
		if(strlen(cmd) > 0) printf(": %s\n",cmd);
		commands();
	}
	
	return 0;
}

/*	Check the code of received message based on command state, parameter:
		int code	- received message code
*/
void check_code(int code)
{
	char *buffer = (char *)malloc(BUFSIZE);
	char *param;
	
	switch(cmd_state)
	{
		/* Opening */
		case OPEN:
		{
			/* If service ready, can send username */
			if(code == SERVICE_READY)
			{
				printf("Username: ");
				bzero(buffer,BUFSIZE);
				fgets(buffer,BUFSIZE-1,stdin);
				
				param = getparam(buffer,0);
				
				/* Send username if given */
				if(strlen(param) > 0) send_cmd("USER",param);
				else /* No username, close connections */
				{
					free(param);
					printf("No username given!\n");
					cmd_state = NONE;
					close_connection();
					break;
				}
				free(param);
				
				cmd_state = USER;
				break;
			}
			/* Other codes -> close connection */
			else
			{
				cmd_state = NONE;
				close_connection();
				break;
			}
		}
		
		/* User sent */
		case USER:
		{
			switch(code)
			{
				/* No password needed */
				case LOGGED_IN:
				{
					state = CONNECTED;
					break;
				}
				
				/* Password needed */
				case PASS_REQ:
				{
					printf("Password: ");
					
					bzero(buffer,BUFSIZE);
					fgets(buffer,BUFSIZE-1,stdin);
	
					param = getparam(buffer,0);

					/* If no password defined, send only PASS command,
						some servers may not support this */
					if(strlen(param) == 0)
					{
						send_cmd("PASS",NULL);
					}
					else send_cmd("PASS",param);
					
					free(param);
					cmd_state = PASS;
					
					break;
				}

				/* Erroreous username */
				case LOGIN_ERROR:
				{
					close_connection();
					cmd_state = NONE;
					break;
				}

				/* Server closes */
				case BYE:
				{
					close_connection();
					cmd_state = NONE;
					break;
				}
				
				/* Command not supported */
				case SYNTAX_ERR:
				{
					cmd_state = NONE;
					close_connection();
					break;
				}
				default:
				{	
					break;
				}
			}
			break;
		}
		
		/* Password sent */
		case PASS:
		{
			switch(code)
			{
				/* all ok */
				case LOGGED_IN:
				{
					state = CONNECTED;
					cmd_state = NONE;
					break;
				}
				
				/* Erroreus username or password */
				case LOGIN_ERROR:
				{
					cmd_state = NONE;
					close_connection();
					break;
				}
				
				/* Command not supported */
				case SYNTAX_ERR:
				{
					cmd_state = NONE;
					close_connection();
					break;
				}
				default:
				{
					cmd_state = NONE;
					break;
				}
			}
			break;
		}
		
		/* Listing files in directory */
		case LS:
		{
			switch(list_state)
			{
				/* mode sent */
				case MODE:
				{
					/* If ok */
					if(code == CMD_OK)
					{
						/* Send port command in active */
						if(client_mode == ACT)
						{
							buffer = create_port_param(open_data_conn());

							send_cmd("PORT",buffer);
						}
						
						/* Send PASV command in passive */
						else send_cmd("PASV",NULL);
						
						list_state = PORT;
						break;
					}
					/* If not accepted */
					else
					{
						cmd_state = NONE;
						list_state = DONE;
						break;
					}
				}
				
				/* PORT or PASV sent */
				case PORT:
				{
					/* If ok and in active mode,
						or passive mode enabled and in passive mode */ 
					if(((code == CMD_OK) && (client_mode == ACT)) || 
						((code == PASSIVE_MODE) && (client_mode == PASV)))
					{
						send_cmd("LIST",NULL); /* send LIST command */
						
						list_state = LIST;
						if(client_mode == PASV) /* If in passive mode, open data connection */
						{
							if(open_data_conn_pasv() == -1)	cmd_state = NONE;
						}
						break;
					}
					else break;
				}
				
				/* LIST command sent */
				case LIST:
				{
					/* If opening data connection */
					if(code == OPENING_DATA)
					{
						/* Listen in active mode */
						if(client_mode == ACT) state_t = LISTEN;
						/* Start transfer in passive */
						else state_t = PASSIVE;
						break;
					}
					else break;
				}
				
				/* Done listing files */
				case DONE:
				{
					/* Completed -> close data connection */
					if(code == TRANSFER_COMPL)
					{
						close_data_conn();
						cmd_state = NONE;
						break;
					}
					else
					{
						break;
					}
				}
			}
			
			break;
		}
		
		/* Changed datamode */
		case DATAMODE:
		{
			/* If not ok */
			if(code != CMD_OK)
			{
				printf("Not supported command\n");
				cmd_state = NONE;
				break;
			}
			else
			{
				cmd_state = NONE;
				break;
			}
		}
		
		/* Closing connecting */
		case QUIT:
		{
			/* Got quitting reply */
			if(code == BYE)
			{
				close_connection();
				cmd_state = NONE;
				break;
			}
			else /* otherwise syntax error */
			{
				cmd_state = NONE;
				break;
			}
		}
		
		/* Changing directory */
		case CWD:
		{
			/* If ok */
			if(code == FILE_ACT_OK)
			{
				cmd_state = NONE;
			}
			/* If failed */
			else if(code == ACTION_FAIL)
			{
				printf("Changing directory not allowed\n");
				cmd_state = NONE;
			}
			else cmd_state = NONE;
			
			break;
		}
		
		/* Storing file to server */
		case STOR:
		{
			switch(transfer_mode)
			{
				/* Starting */
				case START:
				{
					/* If ok reply in active mode,
						or passive mode enabled reply in passive mode */
					if(((client_mode == ACT) && (code == CMD_OK)) ||
						((client_mode == PASV) && (code == PASSIVE_MODE)))
					{
						transfer_mode = OPENING;
						send_cmd("STOR",filename); /* Send store command */

						if(client_mode == PASV) /* In passive mode, open data connection */
						{
							if(open_data_conn_pasv() == -1) cmd_state = NONE;
						}
					}
					break;
				}
				
				/* Sent store command */
				case OPENING:
				{
					/* If opening data (data connection cannot be already open,
						it is closed after every transfer)*/
					if(code == OPENING_DATA)
					{
						if(client_mode == ACT) state_t = LISTEN; /* Active -> listen */
						else if(client_mode == PASV) state_t = PASSIVE; /* Passive -> transfer */
					}
					/* Failed */
					else if(code == ACTION_FAIL)
					{
						transfer_mode = STOP;
					}
					else transfer_mode = STOP;
					break;
				}
				
				/* Transfer done */
				case STOP:
				{
					/* Transfer completed */
					if(code == TRANSFER_COMPL)
					{
						bzero(&filename,BUFSIZE);
						close_data_conn(); /* Close if still active */
						cmd_state = NONE;
					}
					break;
				}
			}
			break;
		}
		
		/* Retrieving file from server */
		case RETR:
		{
			switch(transfer_mode)
			{
				/* Starting */
				case START:
				{
					/* If ok reply in active mode,
						or passive mode enabled reply in passive mode */
					if(((client_mode == ACT) && (code == CMD_OK)) ||
						((client_mode == PASV) && (code == PASSIVE_MODE)))
					{
						transfer_mode = OPENING;
						send_cmd("RETR",filename); /* Send RETR command */

						if(client_mode == PASV)
						{
							/* Open data connection in passive mode */
							if(open_data_conn_pasv() == -1) cmd_state = NONE;
						}
					}
					break;
				}
				
				/* RETR sent */
				case OPENING:
				{
					/* If opening data connection */
					if(code == OPENING_DATA)
					{
						/* listen in active, transfer in passive */
						if(client_mode == ACT) state_t = LISTEN;
						else if(client_mode == PASV) state_t = PASSIVE;
					}
					/* failed */
					else if(code == ACTION_FAIL)
					{
						transfer_mode = STOP;
					}
					else transfer_mode = STOP;
					break;
				}
				/* Stopped */
				case STOP:
				{
					/* Transfer complete */
					if(code == TRANSFER_COMPL)
					{
						close_data_conn();
						bzero(&filename,BUFSIZE);
						cmd_state = NONE;
					}
					break;
				}
			}
			break;
		}

		/* None, do nothing */
		case NONE:
		{
			if(code == CLOSING)
			{
				close_data_conn();
			}
			break;
		}
	}
	
	free(buffer);
}
