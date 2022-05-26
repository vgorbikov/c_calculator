#ifndef custom_stack
#define custom_stack

typedef struct sElement
{
	float numData;
	struct sElement *next;
} sElement;

typedef struct stack
{
	sElement *head;
} stack;


void sPutElement(stack *s, sElement *e);

sElement *sGetElement(stack *s);

#endif //custom_stack
