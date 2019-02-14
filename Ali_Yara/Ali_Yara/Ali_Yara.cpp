// Ali_Yara.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
#include <iostream>
#include <random>
#include <ctime>
#include <cstdlib>

using namespace std;

//Здесь определяются значения для матрицы A и вектора b
double getRandomNumber(int min, int max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	// равномерно распределяем рандомное число в нашем диапазоне
	return static_cast<double>(rand() * fraction * (max - min + 1) + min);
}
//Эта функция определяет размер и элементы для матрицы A и вектора b.
void ProcessInitialization(double * &pMatrix, double* &pVector, double* &pResult, int &Size) {//передаем штуку типа указатель со значением взятым через &)
	do {
		printf("\nEnter size of the initial objects: ");
		scanf_s("%d", &Size);
		if (Size <= 0) {
			printf("Size of the objects must be greater than 0! \n ");
		}
	} while (Size <= 0);
	pMatrix = new double[Size*Size];
	pVector = new double[Size];
	pResult = new double[Size];
	//заполняем массивы рандомными данными
	for (int i = 0; i < Size; i++) {
		pResult[i] = 0;
	}
	for (int i = 0; i < Size; i++) {
		for (int j = 0; j < Size; j++) {
			pMatrix[i, j] = getRandomNumber(1, 10);
		}
	}
	for (int i = 0; i < Size; i++) {
		pVector[i] = getRandomNumber(1, 10);
	}
	//RandomDataInitialization(pMatrix, pVector, Size);
}

int main(int argc, char **argv) {
	double *pMatrix; // Инициализируем матрицу (указатель на первый элемент матрицы)
	double *pVector; // Инициализируем вектор (указатель на вектор)
	double *pResult; // Инициализируем результирующий вектор для перемножения матрицы и вектора (указатель на вектор)
	int Size; // для хранения размера матрицы и вектора (число)
	srand(static_cast<unsigned int>(time(0))); //для рандома
											   //Инициализируем матрицы
	ProcessInitialization(pMatrix, pVector, pResult, Size);
	// Matrix-vector multiplication
	//SerialResultCalculation(pMatrix, pVector, pResult, Size);
	// Program termination
	//ProcessTermination(pMatrix, pVector, pResult);
	return 0;
}
