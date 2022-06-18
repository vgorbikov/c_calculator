#include <stdio.h>
#include <stdlib.h>
#include "custom_stack.h"

void sPutElement(stack *s, sElement *e)
{
	e->next = s->head;
	s->head = e;
}

sElement *sGetElement(stack *s)
{
	sElement *ret = s->head;
	s->head = s->head->next;
	return ret;
}


void delSElement(sElement *deleted)
{
	if(deleted != NULL) free(deleted); //а что ещё нужно?
}
