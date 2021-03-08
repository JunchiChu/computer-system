#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/* Hint: `common.h` has definitions and `common.c` has helper functions. */
#include "common.h" 

/*
 * cleanup: closes the tcp_socket, and exits with the specified return code.
 */
void cleanup(int tcp_sock, int ret)
{
    close(tcp_sock);
    exit(ret);
}

int main(int argc, char **argv) 
{
    /* 0. Check the number of arguments */
    if (argc != 3)
    {
	fprintf(stderr, "Usage: %s <servername> <serverport>\n", argv[0]);
	exit(1);
    }
    char* server_name = argv[1];
    char* server_port = argv[2];


    /* 1. Create a socket (man 2 socket)
     * 
     * Creating a socket allows a program to communicate with the outside world.
     * 
     * We're going to create a TCP socket using the `socket` system call.
     * The `socket` takes in 3 parameters: 
     * 
     * a. domain: There are many different "families" of sockets for different
     * things (e.g. communicating over the internet, over a radio, etc.).
     * In this case we want an internet socket. To do this we want to specify
     * the family of our socket as: AF_INET
     * 
     * b. type: We'll need to specify a different type for the TCP/UDP sockets.
     * TCP sockets are stream based so use SOCK_STREAM as the type.
     * 
     * c. protocol: We can specify 0 for this field. 
     */

    /* TODO: set `tcp_sock` to the result of the `socket` call */
    int tcp_sock=socket(AF_INET,SOCK_STREAM,0); 
    


    /* 2. Address lookup!
     * We need to turn the address and port, into an addrss the computer
     * can under stand. In this case we want to translate:
     * ("localhost", "1234") -> (0.0.0.0, 1234)
     *
     * For the TCP socket we want to translate the address and port of the
     * server
     * 
     * For the UDP socket we want to translate the address of 
     * (localhost, udpport), so we can reserve it for our program.
     * 
     * 
     * To get a list of potential translations we can use `getaddrinfo`.
     * `getaddrinfo` takes 4 paramters:
     *  1. a remote name
     *  2. a remote port
     *  3. a set of hints
     *  4. a pointer that will be set to the results
     * 
     * The tcp_hints structure should be setup as follows:
     *  We're trying to connect to an address that's setup just like ours.
     *  This means we want to set the `ai_family`, `ai_socketype,` and 
     * `ai_protocol` fields of `hints` to  the values we used in our call to 
     * `socket`. `ai_flags` isn't used in this case and should be set to 0.
     *  
     */

    struct addrinfo tcp_hints;
    
    tcp_hints.ai_flags = 0;
    /* TODO: fill in `ai_family`, `ai_socketype,` and `ai_protocol` for `tcp_hints`.
     */
   	tcp_hints.ai_family = AF_INET;
	tcp_hints.ai_socktype = SOCK_STREAM;
	tcp_hints.ai_protocol = 0;
       
    /* call `getaddrinfo` to get the translations. */
    int err;
    struct addrinfo* tcp_results;
    if ((err = getaddrinfo(server_name, server_port, &tcp_hints, &tcp_results)) != 0) 
    {
        fprintf(stderr, "getaddrinfo failed: %s\n", gai_strerror(err));
        exit(1);
    }

    /*
     * 3. Connecting (TCP socket)
     * 
     * Our socket is going to be establishing an outgoing connection, so we 
     * need to use the `connect` system call.
     * 
     * `getaddrinfo` gave us a list of potential ways to connect to the server.
     * We want to try each result until we find one that works. You can use
     * the `addrinfo_connect_try_all` function to do this.
     * 
     * `addrinfo_connect_try_all` takes in 2 arguments:
     *  1. the socket
     *  2. the set of `tcp_results` that we got from `getaddrinfo`.
     */
    
    /* 
     * TODO: 
     * 1. call `addrinfo_connect_try_all
     * 2. Don't forget to error check it!
     */
    int acta;
    if((acta=addrinfo_connect_try_all(tcp_sock,tcp_results))==0){
      fprintf(stderr,"fails addrinfo connect try all");
      exit(1);
    }
    while(1)
    {
        /* 5. File descriptor sets
         * `select` allows a program to wait on multiple file descriptors at the 
         * same time. `select` will return when any of the file descriptors are
         * ready.
         */
        
         /* TODO:
         * a. Zero the read_set by using `FD_ZERO(&read_set)`
         * b. You can add a file descriptor to a set by using `FD_SET(fd, &set)`.
         * Add the `STDIN_FILENO` and `tcp_sock` to the `read_set`.
         */
        fd_set read_set;
	FD_ZERO(&read_set);
	FD_SET(STDIN_FILENO,&read_set);
	  FD_SET(tcp_sock,&read_set);

        /* 6. select: 
         * Now it's time to call `select`. 
         * `select` takes in 5 parameters:
         *  1. the maximum file number used. In this case you can just pass in
         *     `FD_SETSIZE`.
         *  2. the read set, you should pass a pointer to `read_set`.
         *  3. the write set, we don't have a write set so you can pass `NULL`.
         *  4. the error set, we don't have a write set so you can pass `NULL`.
         *  5. a timeout, we're not using timeouts so you can pass `NULL`.
         */

        // TODO: uncomment and fill in the correct arguments:
	int ret = select(FD_SETSIZE, &read_set,NULL,NULL,NULL);
		if (ret < 0)
		{
			perror("select");
			break;
		}
        
        
        /* Is there any data from STDIN to read? */
        if (FD_ISSET(STDIN_FILENO, &read_set))
        {
            /* Read up to `FIGLET_MAX_TEXT_LENGTH` from stdin into buf */
            char buf[FIGLET_MAX_TEXT_LENGTH];
            ssize_t bytes_read = read(STDIN_FILENO, buf, sizeof(buf));
            
            /*
             * If bytes read is:
             *  < 0: Call `perror` and call `cleanup` with a non-zero code. 
             *  = 0: Call `cleanup` with a zero return code.
             *  > 0: Send a figlet request message with the text from stdin.
             */
            if (bytes_read < 0)
            {
                /* TODO: fill me in */
	      perror("byte read < 0 ");
	      cleanup(tcp_sock,1);
            }
            else if (bytes_read == 0)
            {
                /* TODO: fill me in */
	      cleanup(tcp_sock,0);
            }
            else
            {   
                /*
                 * TODO: 
                 *  1. Set the msg type to be a request 
                 *     (see the enum figlet_msg_type) 
                 *  2. Set length field to the number of bytes read from stdin.
                 *  3. `memcpy` the data from buf into the text field of msg. 
		 *     Be sure to `man memcpy` to check its arguments!!
                 *  4. Send the message with `send_figlet_msg`
                 */
                figlet_message_t msg;
		msg.type=FIGLET_REQUEST;
		msg.length=bytes_read;
		memcpy(msg.text,buf,bytes_read);
		send_figlet_msg(tcp_sock,&msg);
            }

        }

        /* Is there any TCP data to read? */
        if (FD_ISSET(tcp_sock, &read_set))
        {   
            figlet_message_t msg;
            /* TODO: read a figlet message from the socket into msg using 
             * `read_figlet_msg`.
             * 
             * if `read_figlet_msg` fails, cleanup with a non-zero error
             * code, otherwise, print the message to stderr
             * with `print_figlet_message`.
             */
	    int s;
	    s=read_figlet_msg(tcp_sock,&msg);
	    if(s==0){
	      cleanup(tcp_sock,1);
	    }else{
              print_figlet_message(STDERR_FILENO,&msg);
	    }
        }
    }
    
    return 0;
}
