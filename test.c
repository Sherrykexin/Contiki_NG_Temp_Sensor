#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#define LINE_COUNT 50
#define LINE_LENGTH 1000
#define BUFFER_COUNT
#define PACKETBUF_SIZE 128

typedef struct
{
    uint8_t len;
    uint8_t data[255]
} message_t;

char buffer[LINE_COUNT][LINE_LENGTH];

uint16_t buf_len;
uint16_t buf_ptr;
uint8_t hdrlen;

/*Threads call this function to get an available message struct from the library.
If the library is unable to provide a free message struct then this function returns NULL.
*/
message_t *new_message()
{

    message_t *new_msg = (message_t *)malloc(sizeof(message_t));
    if (new_msg == NULL)
    {

        return NULL;
    }
    return new_msg;
}

/*Threads call this function to return a
message struct back to the message library. After calling delete_message the thread
will no longer use that message struct and the message library is free to give it out to
other threads that call new_message
*/
void delete_message(message_t *msg)
{
    if (msg != NULL)
    {
        free(msg);
    }
}

/*Threads may use this
function to receive any pending incoming messages. The receiver_id is the ID of the
thread that wishes to receive a message. The msg argument is an output, and the
implementation of this function should set it to point to a message destined to
receiver_id if one exists. A response code should be returned where 0 indicates
success and non-0 indicates error.
*/
int recv(uint8_t receiver_id, message_t *msg)
{
    if (receiver_id.isAlive())
        memcpy(receiver_id, &msg, sizeof(msg));
    return 0;
}

/*Threads may use this
function to send a message to another thread. The destination_id is the ID of the
thread the message should be delivered to and msg is the message to be delivered. A
response code should be returned where 0 indicates success and non-0 indicates an
error.
 */
int send(uint8_t destination_id, message_t *msg)
{

    memcpy(destination_id, &msg, sizeof(msg));
    return 0;
}

void clear_buffer(void)
{
}

int copyfrombuffer(const void *source, uint16_t len)
{
    uint16_t length;
    packetbuf_clear();
    length = MIN(PACKETBUF_SIZE, len);
    memcpy(packetbuf, source, length);
    buf_len = length;
    return length;
}

int copytobuffer(void *destination)
{
    if (buflen > PACKETBUF_SIZE)
    {
        return 0;
    }
}
