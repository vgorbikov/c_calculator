#ifndef custom_list
#define custom_list


//структура элементов списка заданий
//громоздко, но понятно
typedef struct listElement{
	float *v1, *v2, *result; //поля для хранения исходных данных и результата векторных операция
	char operation, type; //поля для хранения информации о операции и типе операндов
	float a, b, r, vSize; //поля для хранения исходных данных и результата операций с числами
	long long int fact; //отдельно поле для результата факториала
	char *msg; //поле для вывода сообщений об ошибках
	struct listElement *nextElement;
} lElement;


//позволяет работать со списком как с самостоятельной структурной единицей
//(при том всё ещё состоящей из отдельных элементов)
typedef struct list
{
	lElement *head;
	lElement *current;
} list;


int listAppend(list *toThisList, lElement *newElement);

lElement *nextElement(list *inThisList);

int goToHeadElement(list *inThisList);

int delElement(list *inThisList);


#endif //custom_list
