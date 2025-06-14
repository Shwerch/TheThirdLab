#include "library.h"

int main() {
    // Начальное и конечное значения X
    const double xStart = -8.0;
    const double xEnd = 10.0;
    // Шаг изменения X
    const double dxVal = 0.1;

    // Вектор для хранения пар значений X и F(X)
    std::vector<Point> graphPoints;

    // Расчет значений функции и заполнение вектора
    calculateFunction(xStart, xEnd, dxVal, graphPoints);

    // Вывод таблицы с отформатированными данными
    printTable(graphPoints);

    return 0;
}
