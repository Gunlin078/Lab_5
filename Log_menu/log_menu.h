#pragma once
#ifndef LOG_MENU_H
#define LOG_MENU_H
#include "reg_menu.h"
#include "main_menu.h"
#include "hash.h"
#include "colorutils.h"

#include <filesystem>
#include <QMainWindow>
#include <QMessageBox>
#include <QFile>         //для работы с файлами: чтения, записи, открытия, удаления и проверки существования
#include <QTextStream>   //для удобного чтения и записи текстовых данных в различные источники
#include <QDir>          //обеспечивает доступ к каталогам и их содержимому

class Reg_menu; //forward declaration
class Main_menu;

QT_BEGIN_NAMESPACE
namespace Ui {
class Log_menu;
}
QT_END_NAMESPACE

class Log_menu : public QMainWindow
{
    Q_OBJECT

public:
    Log_menu(QWidget *parent = nullptr);
    ~Log_menu();

private slots:
    void on_Open_RegMenu_PB_clicked(); // Открыть меню регистрации
    void on_Sign_in_PB_clicked(); // Кнопка ввода данных
    bool isUsernameExists(const QString& username); // Существует ли пользователь
    bool isUserExists(const QString& username, const QString& password); //Верный ли он ввёл пароль
    bool user_login();             // Вход в основное меню

private:
    Ui::Log_menu *ui;
    Reg_menu *reg_menu;
    Main_menu *main_menu;
};
#endif
