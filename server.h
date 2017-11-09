#ifndef SOURCES_SERVER_H_
#define SOURCES_SERVER_H_

#include <mqx.h>
#include <message.h>

#define NUM_USERS 5
#define SYSTEM_QUEUE 2

// Queue ID's
#define HANDLER_QUEUE 8
#define SYS_QUEUE 10

// Queue size
#define USER_QUEUE 64

// Max users
#define MAX_USERS 64

char handler_buffer[255];

typedef struct {
	MESSAGE_HEADER_STRUCT HEADER;
	char DATA[255];
} HANDLE_MESSAGE;

typedef struct {
	MESSAGE_HEADER_STRUCT HEADER;
	char DATA[255];
} USER_MESSAGE, * USER_MESSAGE_PTR;

typedef struct {
	MESSAGE_HEADER_STRUCT HEADER;
	char DATA[255];
} SYS_MESSAGE, * SYS_MESSAGE_PTR;

// Which user task has read privilege
typedef struct user {
	char read_permission;
	_queue_id qid;
} user_t;

_queue_id writeid;

_pool_id message_pool;

user_t users[MAX_USERS];

void clear_handler_buffer();
int get_buffer_length();
void send_buffer();

#endif /* SOURCES_SERVER_H_ */
