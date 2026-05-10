#include "Logger.h"
#include <iostream>
#include <fstream>            // Для работы с файлами
#include <chrono>             // Для получения текущего времени
#include <ctime>              // Для преобразования времени в читаемый формат
#include <stdlib.h>           // Стандартная библиотека C (может использоваться для вспомогательных функций)

// Статическая глобальная переменная - файловый поток для записи логов
static std::ofstream logFile("Logger.log", std::ios::app); // Поток для записи логов; виден только в этом файле
                                        //А-ля "append"
// Статическая функция для получения текущего времени в строковом формате
static std::string currentTime() { //Получение времени

    auto now = std::chrono::system_clock::now(); // Получаю время из системных часов

    // Преобразую тип из мутного "time_point" в числовой "time_t"
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    // Буфер для строки времени
    char buf[64];

    // Преобразуем "time_t" в нормальную С-строку вида "Wed Jun 30 21:30:00 2025\n" и кладём в буфер
    ctime_s(buf, sizeof(buf), &t);

    // Преобразуем C-строку в ещё более нормальный string
    std::string s(buf);

    // Удаляем последний символ ("\n", который добавляет ctime)
    s.pop_back();

    return s; //Получаю время
}

// Статическая функция для преобразования уровня логирования в строку
static const char* levelToString(LogLevel level) { // Преобразование уровня логирования в строку
    switch (level) {
    case LogLevel::Trace:   return "TRACE";   // Самый низкий уровень - детальная трассировка
    case LogLevel::Debug:   return "DEBUG";   // Отладочная информация
    case LogLevel::Info:    return "INFO";    // Информационные сообщения
    case LogLevel::Warning: return "WARNING"; // Предупреждения (не ошибки, но стоит обратить внимание)
    case LogLevel::Error:   return "ERROR";   // Ошибки, но приложение может продолжить работу
    case LogLevel::Fatal:   return "FATAL";   // Критические ошибки, после которых приложение завершается
    default: return "UNKNOWN";                // На всякий
    }
}

// Основной метод логирования: принимает уровень, сообщение, возможно файл и строку
void Logger::log(LogLevel level, const std::string& msg, const char* file, int line) {
    // Формируем полное сообщение: [время] [уровень] сообщение
    // currentTime() - текущее время
    // levelToString(level) - преобразование уровеня в строку
    std::string fullMessage = "[" + currentTime() + "] [" + levelToString(level) + "] " + msg;

    // Для ошибок и фатальных ошибок добавляем информацию о месте возникновения
    if (level == LogLevel::Error || level == LogLevel::Fatal) {
        fullMessage += " (" + std::string(file) + ":" + std::to_string(line) + ")";
    }

    // Выводим сообщение в консоль
    std::cout << fullMessage << std::endl;

    // Если файл для логов успешно открыт
    if (logFile.is_open())
        logFile << fullMessage << std::endl;  // Записываем сообщение в файл с переводом строки
}

void Logger::trace(const std::string& msg) {
    log(LogLevel::Trace, msg);
}

void Logger::debug(const std::string& msg) {
    log(LogLevel::Debug, msg);
}

void Logger::info(const std::string& msg) {
    log(LogLevel::Info, msg);
}

void Logger::warning(const std::string& msg) {
    log(LogLevel::Warning, msg);
}

void Logger::error(const std::string& msg, const char* file, int line) {
    log(LogLevel::Error, msg, file, line);
}

void Logger::fatal(const std::string& msg, const char* file, int line) {
    log(LogLevel::Fatal, msg, file, line);
}

void Logger::close() { // Для гарантированного закрытия файла
    if (logFile.is_open()) {
        logFile.close();
        std::cout << "File closed" << std::endl;
    }
}
