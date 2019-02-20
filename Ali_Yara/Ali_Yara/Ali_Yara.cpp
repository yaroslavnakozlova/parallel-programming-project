// Ali_Yara.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
#include <iostream>
#include <random>
#include <ctime>
#include <cstdlib>

using namespace std;
//возвращает рандомное число double
double getRandomNumber(int min, int max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	// равномерно распределяем рандомное число в нашем диапазоне
	return static_cast<double>(rand() * fraction * (max - min + 1) + min);
}

//заполняет математический объект, матрицу или вектор, рандомными числами
void randomizeSmth(double* vector, int size) {
	for (int i = 0; i < size; i++) {
		vector[i] = getRandomNumber(1, 10);
	}
}

//создает пустой объект - матрицу или вектор. Для создания квадратной матрицы достаточно в параметре передать квадрат размерности: size*size
double* createNullable(int size) {
	double* matrix = new double[size];
	for (int i = 0; i < size; i++)
		matrix[i] = 0;
	return matrix;
}

//вывод матрицы на экран
void printMatrix(double* matrix, int size) {
	printf_s("Matrix\n");
	for (int i = 0; i < size * size; i++)
	{
		printf_s("%f ", matrix[i]);
		if (i % size == size - 1)
			printf_s("\n");
	}
}
//вывод вектора на экран
void printVector(double* vector, int size) {
	printf_s("vector\n");
	for (int i = 0; i < size; i++) {
		printf_s("%f", vector[i]);
		printf_s(" ");
	}
	printf_s("\n");
}

// Генерация положительно-определенной симметричной матрицы с использованием рандомной генерации
double* randomSynchronousPositiveDefiniteMatrix(int size) {
	double* matrix = new double[size*size];
	int k = 0;
	for (int i = 0; i < size; i++) {
		k = 0;
		for (int j = 0; j < size; j++) {
			if (i == j) {
				matrix[(size + 1)*i] = getRandomNumber(size, (size + 1));
			}
			else if (j > i) {
				k++;
				matrix[(size + 1)*i + k] = getRandomNumber((size - k), (size - k + 1));
				matrix[(size + 1)*j - k] = matrix[(size + 1)*i + k];
			}
		}

	}
	return matrix;
}


// ТЕСТИРОВОЧНЫЙ МЕТОД
void initializeMatrix(double* &pMatrix, double* &pVector, double* &pResult) {
	pMatrix= new double[4]{ 3, -1, -1, 3 };
	pVector = new double[2] {3, 7};
	pResult = new double[2]{ 0,0 };
}

// Первый шаг алгоритма - вычисление градиента на текущем шаге (currentG)
void compute_gradient(double* pMatrix, double* pVector, double* previousX, double* &currentG, int size) {
	printf("\ncompute_gradient\n");
	for (int i = 0; i<size; i++) {
		currentG[i] = -pVector[i];
		for (int j = 0; j<size; j++)
			currentG[i] += pMatrix[i*size + j] * previousX[j];
	}
}

// Второй шаг алгоритма - вычисление вектора направления (currentD)
void compute_direction(double* currentG, double* previousG, double* previousD, double* &currentD, int size) {
	printf("\ncompute_direction\n");
	double IP1 = 0, IP2 = 0;
	for (int i = 0; i<size; i++) {
		IP1 += currentG[i] * currentG[i];
		IP2 += previousG[i] * previousG[i];
	}
	for (int i = 0; i<size; i++) {
		currentD[i] = -currentG[i] + previousD[i] * IP1 / IP2;
	}
}

// Третий шаг алгоритма - вычисление величины смещения по выбранному направлению (step)
void compute_scalyar_step(double* currentD, double* currentG, double* pMatrix, double &step, double* denom, int size) {
	double IP1 = 0, IP2 = 0;
	for (int i = 0; i<size; i++) {
		denom[i] = 0;
		for (int j = 0; j<size; j++)
			denom[i] += pMatrix[i*size + j] * currentD[j];
		IP1 += currentD[i] * currentG[i];
		IP2 += currentD[i] * denom[i];
	}
	step = -IP1 / IP2;
}

// Четвертый шаг алгоритма - вычисление нового приближения, промежуточного результата (currentX)
void compute_x(double* previousX, double step, double* currentD, double* &currentX, int size) {
	for (int i = 0; i<size; i++) {
		currentX[i] = previousX[i] + step * currentD[i];
	}
}

/* Сравнивает две последние итерации на факт выполнения условия остановки. Для этого вычисляется максимальное различие между координатами векторов 
 по модулю и потом делится на длину последнего вектора. В итоге получается относительная погрешность.*/
bool checkStopCondition(double* previousX, double* currentX, int size, float accuracy) {
	float sum = 0;
	float max = 0;
	for (int i = 0; i < size; i++) {
		if (max < fabs(currentX[i] - previousX[i])) {
			max = fabs(currentX[i] - previousX[i]);
		}
		sum += currentX[i] * currentX[i];
	}
	sum = sqrt(sum);
	return (max / sum) > accuracy;
}

// Полное вычисление алгоритма метода сопряженных градиентов
void algorithmCalculation(double* pMatrix, double* pVector, double* pResult, int size, float accuracy) {
	double *currentX, *previousX;//x
	double *currentG, *previousG;//g
	double *currentD, *previousD;//d
	double *denom;//знаменатель?
	double step; // смещение
	// Точное предназначение подсчёта итераций и их ограничения по максимуму не до конца понятно - возможно придётся удалить в конце
	int numberOfIterations = 1, maxNumberOfIterations = size + 1;
	
	//инициализируем вектора для алгоритма
	currentX = createNullable(size);
	previousX = createNullable(size);//=0
	currentG = createNullable(size);
	previousG = createNullable(size);
	currentD = createNullable(size);//=0
	previousD = createNullable(size);
	denom = createNullable(size);

	//задаем изначальные значения векторам
	for (int i = 0; i<size; i++) {
		previousG[i] = -pVector[i];
	}

	// Организация итерационных вычислений при помощи цикла do-while 
	do {
		// Перед каждой итерацией (кроме первой) необходимо переопределять вектора. Те вектора, что были текущими станут предыдущими.
		if (numberOfIterations > 1) {
			for (int i=0; i < size; i++) {
				previousX[i] = currentX[i];
				previousG[i] = currentG[i];
				previousD[i] = currentD[i];
			}
		}
		// Последовательное выполнение четырёх шагов алгоритма с выводом промежуточных результаов
		compute_gradient(pMatrix, pVector, previousX, currentG, size);//определили градиент
			//printVector(currentG, size);
		compute_direction(currentG, previousG, previousD, currentD, size);
			//printVector(currentD, size);
		compute_scalyar_step(currentD, currentG, pMatrix, step, denom, size);
			//printf_s("%f", step);
		compute_x(previousX, step, currentD, currentX, size);
			//printVector(currentX,size);
		//Инкрементирование количества итераций
		numberOfIterations++;
	} while(checkStopCondition(previousX, currentX, size, accuracy) && (numberOfIterations < maxNumberOfIterations));

	// Запись в результирующий вектор
	for (int i = 0; i<size; i++)
		pResult[i] = currentX[i];
	//DeleteVectors TODO  - Необходимо организовтаь освобождение памяти, выделенной под матрицы и вектора после вычислений. Т.к. при организации вычислений на нескольких размерностях без этого перезойдёт переполнение оперативнй памяти.
}

// ГЛАВНЫЙ МЕТОД 
int main(int argc, char **argv) {
	srand(static_cast<unsigned int>(time(0))); //для рандома

	double* pMatrix; //объявили матрицу
	double* pVector; //вектор
	double* pResult; //и вектор для результата

	int size = 2;//Размер матрицы. Данный параметр мутабельый

	float accuracy = 0.0001f; // Погрешность вычислений - эпсилон. Данный параметр мутабельный

	initializeMatrix(pMatrix, pVector, pResult); // инициализация матрицы и векторов тестировочными значениями

	printMatrix(pMatrix, size);
	printVector(pVector, size);
	// Вызов основного метода вычислений
	algorithmCalculation(pMatrix, pVector, pResult, size, accuracy);
	// Вывод результата
	printVector(pResult, size);

	return 0;
}

/*
//умножение вектора и матрицы вектор слева - БОЛЕЕ НЕ В УПОТРЕБЛЕНИИ
void vectorMatrixMultiplication(double* pMatrix, double* pVector, double* pResult, int size) {
for (int i=0; i < size; i++) {
for (int j = 0; j < size; j++) {
pResult[i] += pVector[j] * pMatrix[j*size+i];//умножаем строку на столбец
}
}
}
//умножение вектора и матрицы вектор справа - БОЛЕЕ НЕ В УПОТРЕБЛЕНИИ
void matrixVectorMultiplication(double* pMatrix, double* pVector, double* pResult, int size) {
for (int i = 0; i < size; i++) {
for (int j = 0; j < size; j++) {
pResult[i] += pVector[j] * pMatrix[i*size + j];//умножаем строку на столбец
}
}
}
*/