#include "library.h"

// Вычисляет значение функции f(x) для заданного x
double calculateF(double currentX) {
    if (currentX >= -8.0 && currentX <= -5.0) {
        return -3.0;
    } else if (currentX > -5.0 && currentX <= -3.0) {
        return (3.0 / 2.0) * currentX + 9.0 / 2.0;
    } else if (currentX > -3.0 && currentX <= 3.0) {
        // Убедимся, что аргумент sqrt не отрицателен из-за погрешностей float
        double argSqrt = 1.0 - std::pow(currentX / 3.0, 2);
        if (argSqrt < 0) {
            argSqrt = 0; // Предотвращаем NaN при небольших отрицательных значениях
        }
        return 3.0 * std::sqrt(argSqrt);
    } else if (currentX > 3.0 && currentX <= 8.0) {
        return (3.0 / 5.0) * currentX - 9.0 / 5.0;
    } else if (currentX > 8.0 && currentX <= 10.0) {
        return 3.0;
    }
    return 0.0; // Значение по умолчанию для X вне заданного диапазона
}

// Вычисляет значения функции в заданном диапазоне и шагом, заполняя вектор точек
void calculateFunction(double startX, double endX, double stepX, std::vector<Point>& pointsVec) {
    for (double currentX = startX; currentX <= endX + stepX / 2.0; currentX += stepX) { // Добавляем stepX / 2.0 для учета погрешностей чисел с плавающей точкой
        Point newPoint;
        newPoint.xCoord = currentX;
        newPoint.yCoord = calculateF(currentX);
        pointsVec.push_back(newPoint);
    }
}

// Выводит данные из вектора точек в виде отформатированной таблицы
void printTable(const std::vector<Point>& pointsVec) {
    std::cout << std::string(20, '-') << std::endl;
    std::cout << std::left << std::setw(10) << "X" << std::setw(10) << "F(X)" << std::endl;
    std::cout << std::string(20, '-') << std::endl;

    for (const auto& point : pointsVec) {
        std::cout << std::fixed << std::setprecision(4)
                  << std::setw(10) << point.xCoord
                  << std::setw(10) << point.yCoord << std::endl;
    }
    std::cout << std::string(20, '-') << std::endl;
}
