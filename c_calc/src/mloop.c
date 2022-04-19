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


//начинает процедуру обработки векторного выражения
//принимает знак заданной операции, а также указатель на файл для чтения и на файл для записи
void vectorCalculation(char operation, FILE *inpFile, FILE *outFile)
{
	float vectorSize_temp; //после введения размерность ещё будет проходить проверку на целость
	float *vector_1;
	float *vector_2;
	float scalar;

	//читаем размерность векторов из файла
	fscanf(inpFile, "%f", &vectorSize_temp);

	//проводим небольшую проверку на целость числа - если число целое, fractionSize = 1
	int wholeSize = vectorSize_temp;
	float fractionSize = vectorSize_temp/wholeSize;
	if (fractionSize != 1) fprintf(outFile, "|!|Размерность должна быть целым числом|!|\n");

	int vectorSize = vectorSize_temp;
	//выделяем необходимое количество памяти под хранение координат
	vector_1 = malloc(vectorSize*sizeof(float));
	vector_2 = malloc(vectorSize*sizeof(float));

	//читаем координаты каждого вектора
	for(int i=0; i<vectorSize; i++) fscanf(inpFile, "%f", &vector_1[i]);
	for(int i=0; i<vectorSize; i++) fscanf(inpFile, "%f", &vector_2[i]);

	//операция, принимающая на вход массив чисел (вектор) и печатающая его в файл, с соблюдением принятого оформления (скобки, запятые)
	void printVector(float *vector)
	{
		fprintf(outFile, "(");
		for(int i = 0; i<(vectorSize - 1); i++) fprintf(outFile, "%g, ", vector[i]);
		fprintf(outFile, "%g)", vector[vectorSize - 1]);
	}

	//печатаем левую часть выражения
	printVector(vector_1);
	fprintf(outFile, " %c ", operation);
	printVector(vector_2);
	fprintf(outFile, " = ");

	float result[vectorSize]; //переменная для хранения результата в векторном виде

	switch (operation)
	{
	case '+':
		for(int i=0; i<vectorSize; i++) result[i] = vector_1[i]+vector_2[i];
		printVector(result);
		fprintf(outFile, "\n");
		break;
	case '-':
		for(int i=0; i<vectorSize; i++) result[i] = vector_1[i]-vector_2[i];
		printVector(result);
		fprintf(outFile, "\n");
		break;
	case '^':
		for(int i=0; i<vectorSize; i++) scalar += vector_1[i]*vector_2[i];
		fprintf(outFile, "%g\n", scalar);
		break;
	default:
		fprintf(outFile, "|!|Такая операция для векторов не определена: %c|!|\n", operation);

	}
	//освобождаем память, которую выделяли под хранение векторов
	free(vector_1);
	free(vector_2);
}


//начинает процедуру обработки класического арифметического выражения
//принимает знак заданной операции, а также указатель на файл для чтения и на файл для записи
void simpleArithmetic(char operation, FILE *inpFile, FILE *outFile)
{
	float a, b=1; //Переменные, хранящие значение будущих операндов. Второй операнд имеет значение по умолчанию -
				 //сделано это для того чтобы избежать ошибки при проверке числа на целость, которая выполняется в любом случае,
				 //хотя второй операнд не всегда получает значение при вводе с клавиатуры.

	fscanf(inpFile, "%f", &a);

	int wholeA = a; //получаем целую часть числа
	float fractionA = a/wholeA; //делим исходное число на целую. (если в результате получится единица - число целое)

	//Если выбрана операция факториала, второй операнд не запрашиваем
	if (operation != '!')
	{
		fscanf(inpFile, "%f", &b);
	}

	int wholeB = b; //также выполняем проверку числа b на предмет целости
	float fractionB = b/wholeB;

	switch (operation)
	{
	case '+':
		fprintf(outFile, "%g %c %g = %g\n", a, operation, b, a+b);
		break;

	case '-':
		fprintf(outFile, "%g %c %g = %g\n", a, operation, b, a-b);
		break;

	case '*':
		fprintf(outFile, "%g %c %g = %g\n", a, operation, b, a*b);
		break;

	case '/':
		if (b == 0) fprintf(outFile, "|!|Ошибка: деление на ноль не допустимо|!|\n");
		else fprintf(outFile, "%g %c %g = %g\n", a, operation, b, a/b);
		break;

	case '!':
		if ((fractionA != 1)||(a<0)) fprintf(outFile, "|!|Ошибка: под знаком факториала должно стоять целое неотрицательное число|!|\n");
		else fprintf(outFile, "%g%c = %lli\n", a, operation, factorial(a));
		break;

	case '^':
		if ((fractionB != 1)||(b<0)) fprintf(outFile, "|!|Ошибка: степень должна быть целым неотрицательным числом|!|\n");
		else fprintf(outFile, "%g%c%g = %g\n", a, operation, b, degree(a, b));
		break;

	default:
		fprintf(outFile, "|!|Была введена некорректная операция: %c|!|\n", operation);
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


		printf("Введите имя входного файла: ");
		scanf("%s", &inputFname);

		printf("Введите имя выходного файла: ");
		scanf("%s", &outputFname);

		FILE *input = fopen(inputFname, "r");
		FILE *output = fopen(outputFname, "w");

		while(fscanf(input, " %c %c", &operation, &type) != EOF) //читаем посторчно, пока не достигнем конца
		{
			if(type == 'v') vectorCalculation(operation, input, output);
			else if(type == 's') simpleArithmetic(operation, input, output);
			else
			{	//если данный тип операндов неизвестен - пропускаем строку
				fgets(outputFname, 100, input); //"сжигаем" оставшуюся часть строки (пожалуй можно реализовать гораздо умнее)
				fprintf(output, "|!|Неизвестный тип данных: %c|!|\n", type);
			}
		}

		//закрываем файлы после завершения работы с ними
		fclose(input);
		fclose(output);

		//предлагаем пользователю продолжить использование программы
		printf("Хотите продолжить? (y/n) \n");
		scanf(" %c", &choice);
		if (choice == 'n') break;

	}


	return 0;
}
