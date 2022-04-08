/*
 * mloop.c
 *
 *  Created on: 12 мар. 2022 г.
 *      Author: Vadim
 */

#include <stdio.h>
#include <stdlib.h>

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


void vectorCalculation(char operation)
{
	float vector_size;
	float *vector_1;
	float *vector_2;
	float scalar;

	while (1)
	{
	printf("Введите размерность векторов: ");
	scanf("%f", &vector_size);

	int wholeSize = vector_size;
	float fractionSize = vector_size/wholeSize;

	if (fractionSize == 1) break;
	else printf("|!|Размерность должна быть целым числом|!|\n");
	}

	vector_1 = malloc(vector_size*sizeof(float));
	vector_2 = malloc(vector_size*sizeof(float));

	printf("Выберите операцию (+ - s): ");
	scanf(" %c", &operation);

	printf("Введите первый вектор (каждая координата через перевод строки): ");
	for(int i=vector_size; i>0; i--) scanf("%f", &vector_1[i]);

	printf("Введите второй вектор (каждая координата через перевод строки): ");
		for(int i=vector_size; i>0; i--) scanf("%f", &vector_2[i]);

	printf("Ответ: ");

	switch (operation)
	{
	case '+':
		for(int i=vector_size; i>0; i--) printf("%g ", vector_1[i]+vector_2[i]);
		printf("\n");
		break;
	case '-':
		for(int i=vector_size; i>0; i--) printf("%g ", vector_1[i]-vector_2[i]);
		printf("\n");
		break;
	case 's':
		for(int i=vector_size; i>0; i--) scalar += vector_1[i]*vector_2[i];
		printf("%g\n", scalar);

	}
}



//Начинает стандартную процедуру ввода математического выражения
//В  формате запрос-ответ, с отдельным вводом каждого операнда и знака операции
void simpleArithmetic(char operation)
{
	float a, b=1; //Переменные, хранящие значение будущих операндов. Второй операнд имеет значение по умолчанию -
				 //сделано это для того чтобы избежать ошибки при проверке числа на целость, которая выполняется в любом случае,
				 //хотя второй операнд не всегда получает значение при вводе с клавиатуры.

	//Запрашивает ввод операнда, сохраняет ввод в переменную, указатель на которую получает при вызове
	void operandInput(float *variable)
	{
		printf("Введите операнд: ");
		scanf("%f", variable);
	}

	operandInput(&a);

	int wholeA = a; //получаем целую часть числа
	float fractionA = a/wholeA; //делим исходное число на целую. (если в результате получится единица - число целое)

	//Если выбрана операция факториала, второй операнд не запрашиваем
	if (operation != '!')
	{
		operandInput(&b);
	}

	int wholeB = b; //также выполняем проверку числа b на предмет целости
	float fractionB = b/wholeB;

	switch (operation)
	{
	case '+':
		printf("%g %c %g = %g\n", a, operation, b, a+b);
		break;

	case '-':
		printf("%g %c %g = %g\n", a, operation, b, a-b);
		break;

	case '*':
		printf("%g %c %g = %g\n", a, operation, b, a*b);
		break;

	case '/':
		if (b == 0) printf("|!|Ошибка: деление на ноль не допустимо|!|\n");
		else printf("%g %c %g = %g\n", a, operation, b, a/b);
		break;

	case '!':
		if ((fractionA != 1)||(a<0)) printf("|!|Ошибка: под знаком факториала должно стоять целое неотрицательное число|!|\n");
		else printf("%g%c = %lli\n", a, operation, factorial(a));
		break;

	case '^':
		if ((fractionB != 1)||(b<0)) printf("|!|Ошибка: степень должна быть целым неотрицательным числом|!|\n");
		else printf("%g%c%g = %g\n", a, operation, b, degree(a, b));
		break;

	default:
		printf("|!|Была введена некорректная операция|!|\n");
	}
}


int main( int argc, char* argv[])
{
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	printf ("Доступные операции:\n+ Сложение\n- Вычитание\n* Умножение\n/ Деление\n^ Возведение в степень\n! Факториал\n\n");
	printf ("В векторном меню (введите \"v\" для перехода) доступны операции:\n+ Сложение\n- Вычитание\ns Скалярное произведение\n\n");
	while (1) //Главный цикл, раз за разом запускает операцию ввода математического выражения.
	{
		char choice;
		char operation;

		printf("Введите операцию (+ - / * ^ !) или введите \"v\" для работы с векторами: ");
		scanf(" %c", &operation);

		//в зависимости от выбранной операции вызываем функцию для работы с векторами, либо с вещественными числами
		if ((operation == 'v')||(operation == 'V')) vectorCalculation(operation);
		else simpleArithmetic(operation);

		//предлагаем пользователю продолжить использование программы
		printf("Хотите продолжить? (y/n) \n");
		scanf(" %c", &choice);
		if (choice == 'n') break;
	}


	return 0;
}
