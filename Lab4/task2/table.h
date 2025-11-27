/**
 * @file table.h
 * @author Соловьев Арсений Евгеньевич 
 * @version 1.0
 * @date 2025
 * @brief Заголовочный файл для модуля табличной маршрутной перестановки
 * @warning Реализация для русского языка
 */

#pragma once
#include <string>
#include <vector>
#include <stdexcept>
using namespace std;

/**
 * @brief Класс-исключение для ошибок шифрования
 * @details Наследуется от std::invalid_argument
 */
class cipher_error: public invalid_argument {
public:
    /**
     * @brief Конструктор с параметром string
     * @param what_arg сообщение об ошибке
     */
    explicit cipher_error (const string& what_arg):
        invalid_argument(what_arg) {}
    
    /**
     * @brief Конструктор с параметром const char*
     * @param what_arg сообщение об ошибке
     */
    explicit cipher_error (const char* what_arg):
        invalid_argument(what_arg) {}
};

/**
 * @brief Класс для шифрования табличной маршрутной перестановкой
 * @details Реализует шифрование и расшифрование текста методом табличной перестановки
 * с маршрутом записи: по горизонтали слева направо, сверху вниз
 * с маршрутом считывания: сверху вниз, справа налево
 */
class Table
{
private:
    int cols; ///< Количество столбцов таблицы (ключ шифрования)
    
    /**
     * @brief Валидация ключа (количества столбцов)
     * @param key исходный ключ
     * @return валидированный ключ
     * @throw cipher_error если ключ неположительный
     */
     
    int getValidKey(const int key);
    
    /**
     * @brief Валидация и нормализация открытого текста
     * @param s исходный открытый текст
     * @return валидированный открытый текст
     * @throw cipher_error если текст пустой после удаления не-букв
     */
     
    wstring getValidOpenText(const wstring& s);
    
    /**
     * @brief Валидация зашифрованного текста
     * @param s исходный зашифрованный текст
     * @return валидированный зашифрованный текст
     * @throw cipher_error если текст пустой или содержит недопустимые символы
     */
     
    wstring getValidCipherText(const wstring& s);
    
public:
    /**
     * @brief Конструктор с установкой ключа
     * @param key количество столбцов таблицы
     * @throw cipher_error если ключ невалиден
     */
     
    explicit Table(int key);
    
    /**
     * @brief Шифрование открытого текста
     * @param plain открытый текст для шифрования
     * @return зашифрованный текст
     * @throw cipher_error если текст невалиден
     */
     
    wstring encrypt(const wstring& plain);
    
    /**
     * @brief Расшифрование зашифрованного текста
     * @param cipher зашифрованный текст для расшифрования
     * @return расшифрованный текст
     * @throw cipher_error если текст невалиден
     */
     
    wstring decrypt(const wstring& cipher);
};
