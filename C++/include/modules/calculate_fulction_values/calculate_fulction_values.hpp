#pragma once

#include <clocale>
#include <cmath>
#include <vector>

struct Point {
	double xCoord;
	double yCoord;
};

double calculateF(double currentX);

void calculateFunction(double startX, double endX, double stepX, std::vector<Point> &pointsVec);

void printTable(const std::vector<Point> &pointsVec);

void calculateFunctionValues();
