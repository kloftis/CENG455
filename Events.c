/* ###################################################################
**     Filename    : Events.c
**     Project     : project2
**     Processor   : MK64FN1M0VLL12
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-02-03, 13:20, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file Events.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         
/* MODULE Events */

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
**     Callback    : myUART_RxCallback
**     Description : This callback occurs when data are received.
**     Parameters  :
**       instance - The UART instance number.
**       uartState - A pointer to the UART driver state structure
**       memory.
**     Returns : Nothing
** ===================================================================
*/
void myUART_RxCallback(uint32_t instance, void * uartState)
{
	_queue_id user_id = _msgq_open((_queue_number)(USER_QUEUE), 0);
	HANDLE_MESSAGE* msg_ptr = (HANDLE_MESSAGE*)_msg_alloc(message_pool);

	if(msg_ptr == NULL) {
		char* err = "Could not allocate message";
		UART_DRV_SendData(uart1_IDX, err, sizeof(err));
		_mqx_exit(0);
	}

	// Message meta data
	msg_ptr->HEADER.SOURCE_QID = user_id;
	msg_ptr->HEADER.TARGET_QID = _msgq_get_id(0, HANDLER_QUEUE);
	msg_ptr->HEADER.SIZE = sizeof(MESSAGE_HEADER_STRUCT) + strlen((char*)msg_ptr->DATA) + 1;

	for(int i = 0; i < 255; i++){
		msg_ptr->DATA[i] = NULL;
	}

	// Copy data from buffer to DATA
	msg_ptr->DATA[0] = myRxBuff[0];

	_msgq_send(msg_ptr);

	return;
}

/* END Events */

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
