#define _GNU_SOURCE
#include "common.h"
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

/*
 * send_figlet_msg: sends a figlet message over a socket
 *  sock: the socket the message will be sent over
 *  msg:  the message that will be sent
 * 
 * returns: 1 on success, 0 on failure.
 */
int send_figlet_msg(int sock, figlet_message_t* msg)
{
    if (write(sock, &msg->type, sizeof(msg->type)) != sizeof(msg->type))
    {
        return 0;
    }
    if (write(sock, &msg->length, sizeof(msg->length)) != sizeof(msg->length))
    {
        return 0;
    }
    if (write(sock, msg->text, msg->length) != msg->length)
    {
        return 0;
    }
    
    return 1;
}

/*
 * read_figlet_msg: reads a figlet message from a socket
 *  sock: the socket the message will be read from
 *  msg:  where the read message will be read into.
 * 
 * returns: 1 on success, 0 on failure.
 */
int read_figlet_msg(int sock, figlet_message_t* msg)
{
    if (read(sock, &msg->type, sizeof(msg->type)) != sizeof(msg->type))
    {
        return 0;
    }
    if (read(sock, &msg->length, sizeof(msg->length)) != sizeof(msg->length))
    {
        return 0;
    }
    if (read(sock, msg->text, msg->length) != msg->length)
    {
        return 0;
    }
    
    return 1;
}

/*
 * print_figlet_message: prints a figlet message
 *  fd:  the file descriptor to print the message to
 *  msg: the message to be printed
 */
void print_figlet_message(int fd, figlet_message_t* msg)
{
    write(fd, msg->text, msg->length);
}


/*
 * addrinfo_connect_try_all: Try to connect
 *  sock:    the connect that we will use to connect
 *  results: the results returned from `getaddrinfo`
 * 
 * returns: 1 on success, 0 on failure.
 * 
 * This function takes in a list of potential ways to connect to
 * a server. It will iterate over the list and try each result
 * until one succeedes.
 * 
 */
int addrinfo_connect_try_all(int sock, struct addrinfo* results)
{
    struct addrinfo* current = results;
    while(current != NULL)
    {
        /* Call connect for the current result  */
        if (connect(sock, current->ai_addr, current->ai_addrlen) == 0)
        {
            /* success */
            return 1;
        }

        /* connect failed, let's try the next result */
        current = current->ai_next;
    }

    /* failed :( */
    return 0;
}