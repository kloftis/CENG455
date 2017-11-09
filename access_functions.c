#include "access_functions.h"

int OpenR(int uid, _queue_id stream_no) {
	if(_mutex_lock(&access_mutex) != MQX_OK){
		printf("%d Lock Failed", uid);
		_mqx_exit(0);
	}
	if (users[uid].read_permission) {
		return 0;
	} else {
		users[uid].read_permission = 1;
		users[uid].qid = stream_no;
		_mutex_unlock(&access_mutex);
		return 1;
	}
	_mutex_unlock(&access_mutex);
	return 0;
}

int _getline(int uid, void* string) {

	if (users[uid].read_permission) {
		USER_MESSAGE_PTR msg_ptr;
		msg_ptr = _msgq_receive(users[uid].qid, 0);
		if(_mutex_lock(&access_mutex) != MQX_OK){
			printf("%d Lock Failed", uid);
			_mqx_exit(0);
		}
		_queue_id user_id = msg_ptr->HEADER.TARGET_QID;
		// fix this
		sprintf(string, "%s", msg_ptr->DATA);
		_msg_free(msg_ptr);
		_mutex_unlock(&access_mutex);
		return 1;
	}
	_mutex_unlock(&access_mutex);
	return 0;
}

_queue_id OpenW(int uid) {
	if(_mutex_lock(&access_mutex) != MQX_OK){
		printf("%d Lock Failed", uid);
		_mqx_exit(0);
	}
	if (writeid == 0){
		writeid = uid;
		_mutex_unlock(&access_mutex);
		return writeid;
	}
	_mutex_unlock(&access_mutex);
	return 0;
}

int _putline(_queue_id qid, char* string) {
	if(_mutex_lock(&access_mutex) != MQX_OK){
		printf("%d Lock Failed", (int)qid);
		_mqx_exit(0);
	}
	if (writeid == qid){
		SYS_MESSAGE* msg_ptr = (SYS_MESSAGE*)_msg_alloc(message_pool);

		// Message meta data
		msg_ptr->HEADER.SOURCE_QID = 5;
		msg_ptr->HEADER.TARGET_QID = _msgq_get_id(0, SYS_QUEUE);
		msg_ptr->HEADER.SIZE = sizeof(MESSAGE_HEADER_STRUCT) + strlen((char*)msg_ptr->DATA + 1);

		// Copy data from string to DATA
			sprintf(msg_ptr->DATA, string);
			_msgq_send(msg_ptr);
			_mutex_unlock(&access_mutex);
			return 1;
		}
	//}
		//send message to global queue
		//containing string
	_mutex_unlock(&access_mutex);
	return 0;
}

int Close(int uid) {
	if(_mutex_lock(&access_mutex) != MQX_OK){
		printf("%d Lock Failed", uid);
		_mqx_exit(0);
	}
	if (users[uid].read_permission) {
		users[uid].read_permission = 0;
		users[uid].qid = 0;
		if (writeid == uid){
			writeid = 0;
			_mutex_unlock(&access_mutex);
			return 1;
		}
	} else if (writeid == uid){
		writeid = 0;
		_mutex_unlock(&access_mutex);
		return 1;
	}
	_mutex_unlock(&access_mutex);
	return 0;
}


