#include "library.h"

int main() {
    // Ввод количества чисел и максимально стираемых чисел
    int totalNumbers;
    int maxEraseCount;

    // Безопасный ввод количества чисел
    std::cout << "Введите общее количество чисел (от 5 до 50000): ";
    while (!(std::cin >> totalNumbers) || totalNumbers < 5 || totalNumbers > 50000) {
        std::cout << "Некорректный ввод. Пожалуйста, введите число от 5 до 50000: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Безопасный ввод максимально стираемых чисел
    std::cout << "Введите максимальное количество чисел, которые можно стереть за ход (от 4 до 100): ";
    while (!(std::cin >> maxEraseCount) || maxEraseCount < 4 || maxEraseCount > 100) {
        std::cout << "Некорректный ввод. Пожалуйста, введите число от 4 до 100: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Ввод последовательности чисел
    std::vector<int> numbers(totalNumbers);
    std::cout << "Введите " << totalNumbers << " целых чисел, разделенных пробелами: " << std::endl;
    for (int i = 0; i < totalNumbers; ++i) {
        while (!(std::cin >> numbers[i])) {
            std::cout << "Некорректный ввод. Пожалуйста, введите целое число: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    // Вычисление победителя
    int winner = calculateWinner(numbers, maxEraseCount);

    // Вывод результата
    std::cout << winner << std::endl;

    return 0;
}
