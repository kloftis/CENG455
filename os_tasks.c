/* ###################################################################
**     Filename    : os_tasks.c
**     Project     : project2
**     Processor   : MK64FN1M0VLL12
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-02-03, 13:37, # CodeGen: 1
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         serial_task - void serial_task(os_task_param_t task_init_data);
**
** ###################################################################*/
/*!
** @file os_tasks.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup os_tasks_module os_tasks module documentation
**  @{
*/         
/* MODULE os_tasks */

#include "Cpu.h"
#include "Events.h"
#include "rtos_main_task.h"
#include "os_tasks.h"

#ifdef __cplusplus
extern "C" {
#endif 

/* User includes (#include below this line is not maintained by Processor Expert) */

/*
** ===================================================================
**     Callback    : serial_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void serial_task(os_task_param_t task_init_data)
{
  /* Write your local variable definition here */
//	printf("serialTask created!\n\r");
//	sprintf(handler_buffer, "\n\rType here: ");
//	UART_DRV_SendDataBlocking(myUART_IDX, handler_buffer, sizeof(handler_buffer), 1000);
	//int i;
	//for (i = 0; i < 2)
	MUTEX_ATTR_STRUCT mutexattr;
	if(_mutatr_init(&mutexattr) != MQX_OK){
		printf("Mutex Failed");
		_mqx_exit(0);
	}
	if(_mutex_init(&access_mutex, & mutexattr) != MQX_OK){
		printf("Mutex Failed");
		_mqx_exit(0);
	}

	_task_create(0, USER_TASK, 1);
	_task_create(0, USER_TASK, 2);
	_task_create(0, USER_TASK, 3);


#ifdef PEX_USE_RTOS
  while (1) {
#endif
    /* Write your code here ... */
    OSA_TimeDelay(10);                 /* Example code (for task release) */
    
    
    

#ifdef PEX_USE_RTOS   
  }
#endif    
}

/*
** ===================================================================
**     Callback    : handler_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void handler_task(os_task_param_t task_init_data)
{
	/* Write your local variable definition here */
	HANDLE_MESSAGE* msg_ptr;
	_queue_id handler_qid = _msgq_open(HANDLER_QUEUE, 0);
	message_pool = _msgpool_create(sizeof(HANDLE_MESSAGE), NUM_USERS, 0, 0);
	_queue_id sys_qid = _msgq_open(SYS_QUEUE, 0);

	int i = get_buffer_length() - 1;
	int j = 0;

#ifdef PEX_USE_RTOS
  while (1) {
#endif
    /* Write your code here ... */
	// Check handler queue
	msg_ptr = _msgq_receive(0, 0);
	_queue_id user_id = msg_ptr->HEADER.SOURCE_QID;
	int k = 0;
	char * string = NULL;
	char newline = '\n';
	string = msg_ptr->DATA;
	// If a user task has opened the device for reading
	// if (read_permissions[user_id]) {
	while(string[k] != '\0'){
		//Determine what the character is
		char input = string[k];
		k++;
		// If the character is printable put it in the notification queue
		i = get_buffer_length();
		if ((int)input > 31 && (int)input < 127) {
			//printf("%c", input);
			handler_buffer[i++] = input;
			UART_DRV_SendDataBlocking(myUART_IDX, &input, sizeof(input), 1000);
		}
		// Else the character is a special character, handle it
		else {
			switch((int)input) {
			case 8:
				// Backspace
				handler_buffer[--i] = 0;
				char backspace[3];
				sprintf(backspace, "\b \b");
				//printf("%s", backspace);
				UART_DRV_SendDataBlocking(myUART_IDX, backspace, 3, 1000);
				break;
			case 10:
				// New line
				j = 0;
				while (handler_buffer[j++] != 0) {
					char back = '\b';
					UART_DRV_SendDataBlocking(myUART_IDX, &back, 1, 1000);
				}
				//printf("%c", newline);
				UART_DRV_SendDataBlocking(myUART_IDX, &newline, 1, 1000);
				send_buffer();
				clear_handler_buffer();
				break;
			case 13:
				// New line
				j = 0;
				while (handler_buffer[j++] != 0) {
					char back = '\b';
					UART_DRV_SendDataBlocking(myUART_IDX, &back, 1, 1000);
				}
				//printf("%c", newline);
				UART_DRV_SendDataBlocking(myUART_IDX, &newline, 1, 1000);
				send_buffer();
				clear_handler_buffer();
				break;
			case 21:
				// Clear line
				clear_handler_buffer();
				char clearline[5];
				sprintf(clearline, "\33[2K\r");
				//printf("%s", clearline);
				UART_DRV_SendDataBlocking(myUART_IDX, clearline, 5, 1000);
				break;
			case 23:
				// Clear word
				j = get_buffer_length() - 1;
				while (handler_buffer[j] != ' ' && j != 0 && i != 0) {
					--j;
					handler_buffer[--i] = 0;
					char backspace[3];
					sprintf(backspace, "\b \b");
					//printf("%s", backspace);
					UART_DRV_SendDataBlocking(myUART_IDX, backspace, 3, 1000);
				}
				break;
			}
		}
	}
	*string = NULL;
		//}
	//} else {
		// Else discard the character
	//}

    _msg_free(msg_ptr);
    OSA_TimeDelay(10);                 /* Example code (for task release) */
    
#ifdef PEX_USE_RTOS   
  }
#endif    
}

/*
** ===================================================================
**     Callback    : user_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void user_task(os_task_param_t task_init_data)
{
  /* Write your local variable definition here */

	USER_MESSAGE* msg_ptr;
	_queue_id user_qid = _msgq_open((_queue_number)task_init_data, USER_QUEUE);
	_pool_id user_message_pool = _msgpool_create(sizeof(USER_MESSAGE), 1, 0, 0);
	int write_result = 0;
	int open_result = OpenR((int)task_init_data, user_qid);
	printf("User %d created, OpenR: %d\n", (int)task_init_data, open_result);


#ifdef PEX_USE_RTOS
  while (1) {
#endif
    /* Write your code here ... */
	char string[255];
	if (_getline((int)task_init_data, string)) {
		printf("User %d says: %s\n", (int)task_init_data, string);
		if(write_result && Close((int)task_init_data)){
			printf("Task %d closing\n", (int)task_init_data);
			_task_block();
		}
		//if (string == "start"){
			write_result = OpenW((int)task_init_data);
			printf("User %d Requesting Write Access, OpenW: %d\n", (int)task_init_data, write_result);
			if(_putline((int)task_init_data, "User ")){
				_putline((int)task_init_data, (char)task_init_data);
				_putline((int)task_init_data, "was here\n");
				printf("User %d Writing\n", (int)task_init_data);
			}
		//}
	}
    OSA_TimeDelay(10);                 /* Example code (for task release) */
   
    
    
    
#ifdef PEX_USE_RTOS   
  }
#endif    
}

/* END os_tasks */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
