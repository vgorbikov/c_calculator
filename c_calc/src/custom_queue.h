#ifndef custom_queue
#define custom_queue


//структура элементов ореди заданий
//громоздко, но понятно
typedef struct qElement{
	float *v1, *v2, *result; //поля для хранения исходных данных и результата векторных операция
	char operation, type; //поля для хранения информации о операции и типе операндов
	float a, b, r, vSize; //поля для хранения исходных данных и результата операций с числами
	long long int fact; //отдельно поле для результата факториала
	char *msg; //поле для вывода сообщений об ошибках
	struct qElement *nextElement;
} qElement;


//позволяет работать со очередью как с самостоятельной структурной единицей
//(при том всё ещё состоящей из отдельных элементов)
typedef struct queue
{
	qElement *head;
	qElement *end;
} queue;


void putElement(queue *toThisQueue, qElement *newItem);

qElement *getElement(queue *fromThisQueue);

void delQElement(qElement *deleted);

#endif //custom_queue
