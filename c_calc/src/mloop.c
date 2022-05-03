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
	goToHeadElement(inThisList);
	while(inThisList->current->nextElement != deleted) nextElement(inThisList);
	inThisList->current->nextElement = deleted->nextElement;
	if(deleted->nextElement != NULL) inThisList->current = deleted->nextElement;
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
	int vectorSize = task->vSize;
	//проводим небольшую проверку на целость числа - если число целое, fractionSize = 1
//	int wholeSize = task->vSize;
//	float fractionSize = task->vSize/wholeSize;
//	if (fractionSize != 1) fprintf(outFile, "|!|Размерность должна быть целым числом|!|\n");

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
//	default:
//		fprintf(outFile, "|!|Такая операция для векторов не определена: %c|!|\n", operation);
	}
	//printf("%g ", task->r);
}


//начинает процедуру обработки класического арифметического выражения
//принимает знак заданной операции, а также указатель на файл для чтения и на файл для записи
void simpleArithmetic(char operation, lElement *task)
{
//	int wholeA = a; //получаем целую часть числа
//	float fractionA = a/wholeA; //делим исходное число на целую. (если в результате получится единица - число целое)

//	int wholeB = b; //также выполняем проверку числа b на предмет целости
//	float fractionB = b/wholeB;

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
//		if (b == 0) fprintf(outFile, "|!|Ошибка: деление на ноль не допустимо|!|\n");
//		else
		task->r = task->a/task->b;
		break;
	case '!':
//		if ((fractionA != 1)||(a<0)) fprintf(outFile, "|!|Ошибка: под знаком факториала должно стоять целое неотрицательное число|!|\n");
//		else
		task->fact = factorial(task->a);
		break;
	case '^':
//		if ((fractionB != 1)||(b<0)) fprintf(outFile, "|!|Ошибка: степень должна быть целым неотрицательным числом|!|\n");
//		else
		task->r = degree(task->a, task->b);
		break;
//	default:
//		fprintf(outFile, "|!|Была введена некорректная операция: %c|!|\n", operation);
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
			lElement *newTask = malloc(sizeof(lElement));
			newTask->operation = operation;
			newTask->type = type;
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
			else
			{	//если данный тип операндов неизвестен - пропускаем строку
				fgets(inputFname, 100, input); //"сжигаем" оставшуюся часть строки (пожалуй можно реализовать гораздо умнее)
			}
			listAppend(tasks, newTask);
		}
		//закрываем файл после завершения чтения
		fclose(input);

		goToHeadElement(tasks);

		printf("ok r\n");

		while(tasks->current != NULL) //пока не достигнем конца списка
		{
			if(tasks->current->type == 'v') vectorCalculation(tasks->current->operation, tasks->current);
			else if(tasks->current->type == 's') simpleArithmetic(tasks->current->operation, tasks->current);
			else
			{	//если данный тип операндов неизвестен - пропускаем строку
				fgets(outputFname, 100, input); //"сжигаем" оставшуюся часть строки (пожалуй можно реализовать гораздо умнее)
//				fprintf(output, "|!|Неизвестный тип данных: %c|!|\n", type);
			}
			nextElement(tasks);
		}

		goToHeadElement(tasks);

		printf("ok c\n");

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
			nextElement(tasks);
		}
		fclose(output);

		printf("ok w\n");

		//предлагаем пользователю продолжить использование программы
		printf("Хотите продолжить? (y/n) \n");
		scanf(" %c", &choice);
		if (choice == 'n') break;
	}
	return 0;
}
