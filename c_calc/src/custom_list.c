#include <stdio.h>
#include <stdlib.h>
#include "custom_list.h"

//функция для добавления в список нового элемента
int listAppend(list *toThisList, lElement *newElement)
{
	if(toThisList->head == NULL)
	{
		toThisList->head = newElement;
		toThisList->current = newElement;
		return 0;
	}
	toThisList->current->nextElement = newElement;
	toThisList->current = newElement;
	return 0;
}


lElement *nextElement(list *inThisList)
{
	lElement *next = inThisList->current->nextElement;
	inThisList->current =  next;
	return next;
}

int goToHeadElement(list *inThisList)
{
	inThisList->current =  inThisList->head;
	return 0;
}

int delElement(list *inThisList)
{
	lElement *deleted = inThisList->current;
	if(deleted == inThisList->head)
	{
		inThisList->head = deleted->nextElement;
		nextElement(inThisList);
	}
	else
	{
		goToHeadElement(inThisList);
		while(inThisList->current->nextElement != deleted) nextElement(inThisList);
		inThisList->current->nextElement = deleted->nextElement;
		if(deleted->nextElement != NULL) inThisList->current = deleted->nextElement;
	}
	free(deleted);
	return 0;
}
