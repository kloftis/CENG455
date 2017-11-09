#include "server.h"

void clear_handler_buffer() {
	memset(&handler_buffer[0], 0, sizeof(handler_buffer));
}

int get_buffer_length() {
	int i = 0;
	while (handler_buffer[i] != 0) { i++; };
	return i;
}

void send_buffer() {
	int i;
	for (i = 0; i < MAX_USERS; i++) {
		if (users[i].read_permission) {
			USER_MESSAGE* msg_ptr = (USER_MESSAGE*)_msg_alloc(message_pool);

			// Message meta data
			msg_ptr->HEADER.SOURCE_QID = 0;
			//msg_ptr->HEADER.TARGET_QID = users[i].qid;
			msg_ptr->HEADER.TARGET_QID = users[i].qid;
			msg_ptr->HEADER.SIZE = sizeof(MESSAGE_HEADER_STRUCT) + strlen((char*)msg_ptr->DATA + 1);

			// Copy data from buffer to DATA
			sprintf(msg_ptr->DATA, handler_buffer);

			_msgq_send(msg_ptr);
		}
	}
}

