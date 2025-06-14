#pragma once

#include <clocale>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

struct Point {
	double xCoord; // X
	double yCoord; // Y
};

// Вычисляет значение функции f(x) для заданного x
double calculateF(double currentX);

// Вычисляет значения функции в заданном диапазоне и шагом, заполняя вектор точек
void calculateFunction(double startX, double endX, double stepX, std::vector<Point> &pointsVec);

// Выводит данные из вектора точек в виде отформатированной таблицы
void printTable(const std::vector<Point> &pointsVec);

void calculateFunctionValues();
