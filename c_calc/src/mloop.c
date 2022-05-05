/*
 * mloop.c
 *
 *  Created on: 03.05.2022
 *      Author: Vadim
 */

#include <stdio.h>
#include <stdlib.h>

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


//Принимает на вход целое неотрицательное число
//Возвращает его факториал
long long int factorial(int a)
{
	if(a == 0) return 1; //Обрабатываем исключение: факториал нуля равен единице
	else
	{
		long long int fact = 1;
		for(int i = 1; i<=a; i++)
		{
			fact*=i;
		}
		return fact;
	}
}


//Возводит а в степень b, принимает на вход два вещественных числа
//Возвращает вещественное число
//Не предусмотренна работа с отрицательными и дробными степенями
double degree(float a, float b)
{
	if (b==0) return 1; //Обрабатываем исключение: любое число в нулевой степени равно единице
	else
	{
		float deg = a;
		for(int i = 1; i<b; i++)
		{
			deg *= a;
		}
		return deg;
	}
}


//начинает процедуру обработки векторного выражения
//принимает знак заданной операции, а также указатель на файл для чтения и на файл для записи
void vectorCalculation(char operation, lElement *task)
{
	//проводим небольшую проверку на целость числа - если число целое, fractionSize = 1
	int wholeSize = task->vSize;
	float fractionSize = task->vSize/wholeSize;
	if (fractionSize != 1) task->msg = "|!|Размерность должна быть целым числом|!|";

	int vectorSize = task->vSize;

	task->result = calloc(vectorSize, sizeof(float));
	switch (operation)
	{
	case '+':
		for(int i=0; i<vectorSize; i++) task->result[i] = task->v1[i]+task->v2[i];
		break;
	case '-':
		for(int i=0; i<vectorSize; i++) task->result[i] = task->v1[i]-task->v2[i];
		break;
	case '^':
		task->r = 0;
		for(int i=0; i<vectorSize; i++) task->r += task->v1[i]*task->v2[i];
		break;
	default:
		task->msg = "|!|Такая операция для векторов не определена|!|";
	}
}


//начинает процедуру обработки класического арифметического выражения
//принимает знак заданной операции, а также указатель на файл для чтения и на файл для записи
void simpleArithmetic(char operation, lElement *task)
{
	int wholeA = task->a; //получаем целую часть числа
	float fractionA = task->a/wholeA; //делим исходное число на целую. (если в результате получится единица - число целое)

	int wholeB = task->b; //также выполняем проверку числа b на предмет целости
	float fractionB = task->b/wholeB;

	switch (operation)
	{
	case '+':
		task->r = task->a + task->b;
		break;
	case '-':
		task->r = task->a - task->b;
		break;
	case '*':
		task->r = task->a*task->b;
		break;
	case '/':
		if (task->b == 0) task->msg = "|!|Ошибка: деление на ноль не допустимо|!|";
		else task->r = task->a/task->b;
		break;
	case '!':
		if ((fractionA != 1)||(task->a<0)) task->msg = "|!|Ошибка: под знаком факториала должно стоять целое неотрицательное число|!|";
		else task->fact = factorial(task->a);
		break;
	case '^':
		if ((fractionB != 1)||(task->b<0)) task->msg = "|!|Ошибка: степень должна быть целым неотрицательным числом|!|";
		else task->r = degree(task->a, task->b);
		break;
	default:
		task->msg = "|!|Была введена неизвестная операция|!|";
	}
}


int main( int argc, char* argv[])
{
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	printf ("Каждая строчка во входном файле должна быть оформленна в формате:\n");
	printf ("ОПЕРАЦИЯ ТИП_ДАННЫХ РАЗМЕРНОСТЬ(только для векторов) ОПЕРАНДЫ(через пробел)\n");
	printf ("|!|Для получения более подробной информации читайте файл README|!|\n\n");
	while (1) //Главный цикл, раз за разом запускает операцию ввода математического выражения.
	{
		char choice, operation, type; //type - переменная для хранения типа операндов
		char inputFname[255];
		char outputFname[255];
		float firstNum;


		printf("Введите имя входного файла: ");
		scanf("%s", &inputFname);

		printf("Введите имя выходного файла: ");
		scanf("%s", &outputFname);

		//создаём список, который будет хранить входные данные для подсчёта
		list *tasks = calloc(1, sizeof(list));
		FILE *input = fopen(inputFname, "r");

		while(fscanf(input, " %c %c %f", &operation, &type, &firstNum) != EOF) //читаем посторчно, пока не достигнем конца
		{
			/*при создании нового элемента важно использовать именно calloc, так-как попадание в поле "nextElement"
			 * ненулевого значения черевато сбоем в логике работы программы
			 */
			lElement *newTask = calloc(1, sizeof(lElement));
			newTask->operation = operation;
			newTask->type = type;
			newTask->msg = NULL;
			if(type == 'v')
			{
				newTask->vSize = firstNum;
				newTask->v1 = malloc(firstNum*sizeof(float));
				newTask->v2 = malloc(firstNum*sizeof(float));
				for(int i=0; i<firstNum; i++) fscanf(input, "%f", &newTask->v1[i]);
				for(int i=0; i<firstNum; i++) fscanf(input, "%f", &newTask->v2[i]);
			}
			else if(type == 's')
			{	//операнды-невектора будем хранить в списке v1
				newTask->a = firstNum;
				if(operation != '!') fscanf(input, "%f", &newTask->b);
			}
			else fgets(inputFname, 100, input); //"сжигаем" оставшуюся часть строки
			listAppend(tasks, newTask);
		}
		//закрываем файл после завершения чтения
		fclose(input);

		goToHeadElement(tasks);


		while(tasks->current != NULL) //пока не достигнем конца списка
		{
			if(tasks->current->type == 'v') vectorCalculation(tasks->current->operation, tasks->current);
			else if(tasks->current->type == 's') simpleArithmetic(tasks->current->operation, tasks->current);
			else
			{	//если данный тип операндов неизвестен - пропускаем строку
				tasks->current->msg = "|!|Неизвестный тип данных|!|";
			}
			nextElement(tasks);
		}

		goToHeadElement(tasks);

		FILE *output = fopen(outputFname, "w");
		while(tasks->current != NULL) //пока не достигнем конца списка
		{
			lElement *curT = tasks->current;
			//операция, принимающая на вход массив чисел и печатающая его в файл, с соблюдением принятого оформления
			void printVector(float *vector, int size)
			{
				fprintf(output, "(");
				for(int i = 0; i<(size - 1); i++) fprintf(output, "%g, ", vector[i]);
				fprintf(output, "%g)", vector[size - 1]);
				free(vector);
			}

			if(curT->msg != NULL)
			{
				fprintf(output, "%s\n", curT->msg);
				delElement(tasks);
				continue;
			}

			if(curT->type == 's')
			{
				if(curT->operation != '!') fprintf(output, "%g %c %g = %g", curT->a, curT->operation, curT->b, curT->r);
				else fprintf(output, "%g! = %lli", curT->a, curT->fact);
			}
			else if(curT->type == 'v')
			{
				printVector(curT->v1, curT->vSize);
				fprintf(output, " %c ", curT->operation);
				printVector(curT->v2, curT->vSize);
				fprintf(output, " = ");
				if(curT->operation != '^') printVector(curT->result, curT->vSize);
				else fprintf(output, "%g", curT->r);
			}
			fprintf(output, "\n");
			delElement(tasks);
		}
		fclose(output);
		free(tasks);

		//предлагаем пользователю продолжить использование программы
		printf("Хотите продолжить? (y/n) \n");
		scanf(" %c", &choice);
		if (choice == 'n') break;
	}
	return 0;
}
