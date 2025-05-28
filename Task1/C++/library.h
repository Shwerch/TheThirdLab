#pragma once

#include <iostream> // Для std::cout, std::endl
#include <vector>   // Для std::vector
#include <string>   // Для std::string
#include <iomanip>  // Для std::setprecision, std::fixed, std::setw
#include <cmath>    // Для std::sqrt
#include <clocale>  // Для std::setlocale

// Структура для хранения координат точки
struct Point {
    double xCoord; // Координата X
    double yCoord; // Координата Y
};

// Прототипы функций

// Вычисляет значение функции f(x) для заданного x
double calculateF(double currentX);

// Вычисляет значения функции в заданном диапазоне и шагом, заполняя вектор точек
void calculateFunction(double startX, double endX, double stepX, std::vector<Point>& pointsVec);

// Выводит данные из вектора точек в виде отформатированной таблицы
void printTable(const std::vector<Point>& pointsVec);
