#pragma once
#ifndef REG_MENU_H
#define REG_MENU_H
#include "log_menu.h"
#include "user_manager.hpp"
#include <QMainWindow>
#include <QString>

class Log_menu;  //forward declaration

namespace Ui {
class Reg_menu;
}

class Reg_menu : public QMainWindow
{
    Q_OBJECT

public:
    explicit Reg_menu(Log_menu* log_menu, QWidget *parent = nullptr);
    ~Reg_menu();

private:
    bool validateAndRegister();                                           // Основная функция валидации и регистрации
    bool saveToCSV(const QString& username, const QString& password);      // Сохранение в CSV
    bool isUsernameTaken(const QString& username);                       // Проверка существования пользователя
    QString hashPassword(const QString& password);                        // Хэширование пароля
    Ui::Reg_menu *ui;
    Log_menu *log_menu_link = nullptr;
private slots:
    // Слот для обработки нажатия кнопок
    void on_Register_PB_clicked();
    void on_Open_LogMenu_PB_clicked();
};

#endif
