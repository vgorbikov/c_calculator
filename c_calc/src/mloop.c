/*
 * mloop.c
 *
 *  Created on: 03.05.2022
 *      Author: Vadim
 */

#include <stdio.h>
#include <stdlib.h>
#include "custom_queue.h"
#include "custom_stack.h"


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
void vectorCalculation(char operation, qElement *task)
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
void simpleArithmetic(char operation, qElement *task)
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

//функция для обработки выражений в обратной польской нотации
//на фход принимает строку, содержащую арифметическое выражение, и элемент очереди на запись, для сохранения ответа
void polishNotation(char* string, qElement *out)
{
	stack *stk = calloc(1, sizeof(stack));
	int prevCharIsNum = 0; //нулевое значение указывает, что предыдущий обрабатываемый символе не был чиислом

	for(int i = 0; string[i] != '\0'; i++) //пока не дойдём до конца строки
	{
		float aCode = string[i]; //ascii-код символа

		if((aCode - 48 <10)&(aCode - 48 >=0)) //определяем, с числом ли имем дело (код нуля в acii - 48)
		{
			//если предыдущий символ тоже был числом, продолжаем "собирать" его поразрядно
			if(prevCharIsNum > 0) stk->head->numData = stk->head->numData*10 + (aCode - 48);
			else if(prevCharIsNum < 0)
			{
				stk->head->numData = stk->head->numData + (aCode - 48)/(degree(10, prevCharIsNum*(-1)));
				prevCharIsNum -= 1;
			}
			else //если пердыдущие символ не был числом, добавляем новое число в стек
			{
			sElement *new = calloc(1, sizeof(sElement));
			new->numData = aCode - 48;
			prevCharIsNum = 1;
			sPutElement(stk, new);
			}
		}
		else //если имеем дело с символом
		{
			sElement *new = calloc(1, sizeof(sElement));
			switch (string[i])
			{
			case '.': //встретив точку после числа, готовимся дописывать дробную часть в след. итерациях
				if(prevCharIsNum > 0) prevCharIsNum = -1;
				break;
			case '+': //если это символ операции, то берём из стека операнды и результат записываем обратно в стек
				new->numData = sGetElement(stk)->numData + sGetElement(stk)->numData;
				sPutElement(stk, new);
				break;
			case '-':
				new->numData = (sGetElement(stk)->numData - sGetElement(stk)->numData)*(-1);
				sPutElement(stk, new);
				break;
			case '*':
				new->numData = sGetElement(stk)->numData*sGetElement(stk)->numData;
				sPutElement(stk, new);
				break;
			case '/':;
				float second = sGetElement(stk)->numData;
				new->numData = sGetElement(stk)->numData/second;
				sPutElement(stk, new);
				break;
			case '!':
				new->numData = factorial(sGetElement(stk)->numData);
				sPutElement(stk, new);
				break;
			case '^':;
				float dSecond = sGetElement(stk)->numData;
				new->numData = degree(sGetElement(stk)->numData, dSecond);
				sPutElement(stk, new);
				break;
			default:
				prevCharIsNum = 0;
			}
		}
	}
	out->r = stk->head->numData; //результат работы записываем в очередь на вывод
	if(stk != NULL) free(stk);
}

int main( int argc, char* argv[])
{
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	printf ("Программа поддерживает работу в двух режимах, в зависимости от формата данных:\n");
	printf ("- Когда данные оформленны в обратной польской нотации\n");
	printf ("  Данный режим позволяет обрабатывать длинные математические выражения с числами.\n");
	printf ("- Когда данные оформленны в префиксной нотации, как в предыдущих версиях\n");
	printf ("  Не поддерживаются операции более чем с двумя операндами, зато доступна работа с векторами.\n");
	printf ("Каждая строчка во входном файле, для работы в режиме префиксной нотации должна быть оформленна в формате:\n");
	printf ("ОПЕРАЦИЯ ТИП_ДАННЫХ РАЗМЕРНОСТЬ(только для векторов) ОПЕРАНДЫ(через пробел)\n");
	printf ("|!|Для получения более подробной информации читайте файл README|!|\n\n");
	while (1) //Главный цикл, раз за разом запускает операцию ввода математического выражения.
	{
		char choice, operation, type; //type - переменная для хранения типа операндов
		char inputFname[255];
		char outputFname[255];
		float firstNum;

		printf("Выберите режим работы (p - префиксная нотация, r - обратная польская нотация): ");
		scanf(" %c", &choice);

		printf("Введите имя входного файла: ");
		scanf("%s", &inputFname);

		printf("Введите имя выходного файла: ");
		scanf("%s", &outputFname);

		//создаём очередь, который будет хранить входные данные для подсчёта
		queue *tasks = calloc(1, sizeof(queue));
		FILE *input = fopen(inputFname, "r");

		while(feof(input) == 0) //пока не достигнем конца
		{
			qElement *newTask = calloc(1, sizeof(qElement));
			if(choice == 'p')
			{
				fscanf(input, " %c %c %f", &operation, &type, &firstNum);
				/*при создании нового элемента важно использовать именно calloc, так-как попадание в поле "nextElement"
				 * ненулевого значения черевато сбоем в логике работы программы
				 */

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
			}
			else if(choice == 'r')
			{
				newTask->msg = calloc(255, sizeof(char));
				fgets(newTask->msg, 255, input);
			}
			putElement(tasks, newTask);
		}
		//закрываем файл после завершения чтения
		fclose(input);

		queue *readyData = calloc(1, sizeof(queue)); //очередь на печать
		while(tasks->head != NULL) //пока не достигнем конца списка
		{
			qElement *complTask = tasks->head; //берём из очереди элемент для последующей обработки
			if(choice == 'p')
			{
				if(tasks->head->type == 'v') vectorCalculation(complTask->operation, complTask);
				else if(tasks->head->type == 's') simpleArithmetic(complTask->operation, complTask);
				else
				{	//если данный тип операндов неизвестен - пропускаем строку
					complTask->msg = "|!|Неизвестный тип данных|!|";
				}
			}
			else if(choice == 'r')
			{
				polishNotation(complTask->msg, complTask);
			}
			putElement(readyData, complTask); //после обработки переносим элемент в очередь на печать
			delQElement(getElement(tasks));
		}
		if(tasks != NULL) free(tasks); //очередь на обработку нам больше не нужна


		FILE *output = fopen(outputFname, "w");
		while(readyData->head != NULL) //пока не достигнем конца oчереди
		{
			qElement *curT = readyData->head; //указатель с более коротким именем чисто для удобства
			//операция, принимающая на вход массив чисел и печатающая его в файл, с соблюдением принятого оформления
			void printVector(float *vector, int size)
			{
				fprintf(output, "(");
				for(int i = 0; i<(size - 1); i++) fprintf(output, "%g, ", vector[i]);
				fprintf(output, "%g)", vector[size - 1]);
			}

			if(choice == 'p')
			{
				if(curT->msg != NULL)
				{
					fprintf(output, "%s\n", curT->msg);
					delQElement(getElement(readyData));
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
			}
			else if(choice == 'r')
			{
				fprintf(output, "%g\n", readyData->head->r);
			}
			delQElement(getElement(readyData));
		}
		fclose(output);
		if(readyData != NULL) free(readyData);


		//предлагаем пользователю продолжить использование программы
		printf("Хотите продолжить? (y/n) \n");
		scanf(" %c", &choice);
		if (choice == 'n') break;
	}
	return 0;
}
