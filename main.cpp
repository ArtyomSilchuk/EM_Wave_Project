// main.cpp
#include <iostream>
#include "physics/Vector3.h"  // Путь к заголовку

int main() {
    std::cout << "=== Тест Vector3 ===" << std::endl;

    // Тест 1: Конструктор
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(4.0f, 5.0f, 6.0f);
    std::cout << "a = (" << a.x << ", " << a.y << ", " << a.z << ")" << std::endl;
    std::cout << "b = (" << b.x << ", " << b.y << ", " << b.z << ")" << std::endl;

    // Тест 2: Сложение
    Vector3 c = a + b;
    std::cout << "a + b = (" << c.x << ", " << c.y << ", " << c.z << ")" << std::endl;

    // Тест 3: Вычитание
    Vector3 d = a - b;
    std::cout << "a - b = (" << d.x << ", " << d.y << ", " << d.z << ")" << std::endl;

    // Тест 4: Умножение на скаляр
    Vector3 e = a * 2.0f;
    std::cout << "a * 2 = (" << e.x << ", " << e.y << ", " << e.z << ")" << std::endl;

    // Тест 5: Умножение скаляра на вектор (свободная функция)
    Vector3 f = 3.0f * a;
    std::cout << "3 * a = (" << f.x << ", " << f.y << ", " << f.z << ")" << std::endl;

    // Тест 6: Деление на скаляр
    Vector3 g = a / 2.0f;
    std::cout << "a / 2 = (" << g.x << ", " << g.y << ", " << g.z << ")" << std::endl;

    // Тест 7: Длина вектора
    float len = a.length();
    std::cout << "length(a) = " << len << std::endl;

    // Тест 8: Нормализация
    Vector3 n = a.normalized();
    std::cout << "normalized(a) = (" << n.x << ", " << n.y << ", " << n.z << ")" << std::endl;
    std::cout << "length(normalized(a)) = " << n.length() << std::endl;

    std::cout << "\n=== Все тесты завершены ===" << std::endl;

    return 0;
}
