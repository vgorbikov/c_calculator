#include <stdio.h>
#include <stdlib.h>
#include "custom_queue.h"

//функция для добавления в очередь нового элемента
void putElement(queue *toThisQueue, qElement *newItem)
{
	if(toThisQueue->head == NULL) toThisQueue->head = newItem;
	if(toThisQueue->end != NULL) toThisQueue->end->nextElement = newItem;
	toThisQueue->end = newItem;
}


//функция для получения лемента из головы очереди
qElement *getElement(queue *fromThisQueue)
{
	if(fromThisQueue->head == NULL) return NULL;
	qElement *ret = fromThisQueue->head;
	fromThisQueue->head = ret->nextElement;
	return ret;
}


void delQElement(qElement *deleted)
{
	if(deleted != NULL)
	{
		if(deleted->msg != NULL) free(deleted->msg);
		if(deleted->result != NULL) free(deleted->result);
		if(deleted->v1 != NULL) free(deleted->v1);
		if(deleted->v2 != NULL) free(deleted->v2);
		free(deleted);
	}
}
