#ifndef SOURCES_ACCESS_FUNCTIONS_H_
#define SOURCES_ACCESS_FUNCTIONS_H_

#include <mqx.h>
#include <message.h>
#include "server.h"
#include <bsp.h>
#include <mutex.h>

int OpenR(int uid, _queue_id stream_no);
int _getline(int uid, void* string);
_queue_id OpenW(int uid);
int _putline(_queue_id qid, char* string);
int Close(int uid);

MUTEX_STRUCT access_mutex;

#endif /* SOURCES_ACCESS_FUNCTIONS_H_ */
