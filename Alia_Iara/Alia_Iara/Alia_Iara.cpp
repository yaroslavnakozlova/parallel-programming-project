// Alia_Iara.cpp: определяет точку входа для консольного приложения.
//
#include <iostream>
#include <cstdlib>
#include "stdafx.h"
using namespace std;

//эта функция заполняет матрицу рандомными числами
void RandomDataInitialization(double* &pMatrix, double *&pVector, int &Size)
{
	for (int i = 0; i < Size; i++)
	{
		pVector[i] = rand() % 100;
	}
	for (int i = 0; i < Size; i++) {
		for (int j = 0; j < Size; j++) {
			pMatrix[i, j] = rand() % 100;
		}
	}
}

// Эта функция определяет размер и элементы для матрицы A и вектора b.Значения для матрицы A и вектора b определяются в функции RandomDataInitialization.
void ProcessInitialization(double* &pMatrix, double* &pVector, double* &pResult, int &Size) {
	int i; // Переменная цикла
	do {
		printf("\nВведите размер инициализируемых объектов: ");
		scanf_s("%d", &Size);
		if (Size <= 0) {
			printf("Размер объектов должен быть больше 0! \n ");
		}
	} while (Size <= 0);
	pMatrix = new double[Size*Size];
	pVector = new double[Size];
	pResult = new double[Size];
	for (i = 0; i<Size; i++)
		pResult[i] = 0;
	RandomDataInitialization(pMatrix, pVector, Size);
}

int main() {
	double* pMatrix; // Матрицы линейной системы уровнений
	double* pVector; // Вектор матрицы (правая часть)
	double* pResult; // Результирующий вектор
	int Size; //Размеры изначальной матрицы и вектора
	printf("Метод сопряженных градиентов для решения линейных уравнений\n");
	//ProcessInitialization определяет исходные данные решаемой задачи (число неизвестных), выделяет память для хранения данных, осуществляет ввод матрицы
	//коэффициентов системы линейных уравнений и вектора правых частей(или формирует эти данные при помощи датчика случайных чисел)
	ProcessInitialization(pMatrix, pVector, pResult, Size);

	// Execution of Conjugate Gradient Method

	//ParallelResultCalculation(pMatrix, pVector, pResult, Size);

	// Computational process termination

	//ProcessTermination(pMatrix, pVector, pResult);
	return 0;
}



/*
// Поcледовательный алгоритм умножения матрицы на вектор
for (i = 0; i < m; i++){
c[i] = 0;
for (j = 0; j < n; j++){
c[i] += A[i][j]*b[j]
}
}
*/
