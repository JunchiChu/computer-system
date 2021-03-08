#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define FIGLET_MAX_TEXT_LENGTH 255

typedef enum figlet_msg_type 
{
    FIGLET_REQUEST = 1,
    FIGLET_RESPONSE = 2
} figlet_message_type_t;

typedef struct message 
{
    /* Indicates request/response */
    figlet_message_type_t type;

    /* The length of the text */
    uint8_t length;

    /* The text itself */
    char text[FIGLET_MAX_TEXT_LENGTH];
} figlet_message_t;

int send_figlet_msg(int sock, figlet_message_t* msg);

int read_figlet_msg(int sock, figlet_message_t* msg);

void print_figlet_message(int fd, figlet_message_t* msg);

int addrinfo_connect_try_all(int sock, struct addrinfo* results);

#endif
