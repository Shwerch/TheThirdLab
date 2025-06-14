#include "library.h" // Подключаем заголовочный файл нашей библиотеки

int main() {
    // Выводим приветствие
    std::cout << std::string(30, '-') << std::endl;
    std::cout << "Программа для вычисления суммы ряда" << std::endl;
    std::cout << std::string(30, '-') << std::endl;

    int valA = 0; // Переменная для хранения числа 'a'
    int valB = 0; // Переменная для хранения числа 'b'

    // Запрашиваем ввод 'a' и проверяем его
    while (true) {
        std::cout << "Введите целое число 'a' (от 1 до 10): ";
        if (!(std::cin >> valA) || valA < 1 || valA > 10) {
            std::cout << "Некорректный ввод. Пожалуйста, введите число от 1 до 10." << std::endl;
            std::cin.clear(); // Очищаем флаги ошибок ввода
            // Игнорируем оставшиеся символы в буфере ввода до конца строки
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            break; // Ввод корректен, выходим из цикла
        }
    }

    // Запрашиваем ввод 'b' и проверяем его
    while (true) {
        std::cout << "Введите целое число 'b' (от 1 до 10): ";
        if (!(std::cin >> valB) || valB < 1 || valB > 10) {
            std::cout << "Некорректный ввод. Пожалуйста, введите число от 1 до 10." << std::endl;
            std::cin.clear(); // Очищаем флаги ошибок ввода
            // Игнорируем оставшиеся символы в буфере ввода до конца строки
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            break; // Ввод корректен, выходим из цикла
        }
    }

    calculateAndPrintSeriesSum(valA, valB);

    return 0;
}
