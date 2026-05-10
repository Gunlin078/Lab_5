#define FIXED_SALT "MyFixedSalt123!@#"  //фиксированная соль

#include "hash.h"
#include "reg_menu.h"
#include "ui_reg_menu.h"
#include <QMessageBox>   //для быстрого создания стандартных модальных диалоговых окон
#include <QFile>         //для работы с файлами: чтения, записи, открытия, удаления и проверки существования
#include <QTextStream>   //для удобного чтения и записи текстовых данных в различные источники
#include <QDir>          //обеспечивает доступ к каталогам и их содержимому
#pragma comment(lib, "bcrypt.lib")  //подключает библиотеку напрямую в объектный файл

// Конструктор класса Reg_menu
Reg_menu::Reg_menu(Log_menu* log_menu, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Reg_menu)
{
    ui->setupUi(this);

    ColorUtils colorUtils;
    colorUtils.change_color(this); //Функция []

    log_menu_link = log_menu;// Указатель на объект Log_menu

    connect(      ui->Register_PB1,     &QPushButton::clicked,   this,            &Reg_menu::on_Register_PB_clicked);
}

// Функция проверки существования пользователя в CSV-файле
bool Reg_menu::isUsernameTaken(const QString& username)
{
    QFile file("users.csv");

    // Если не удалось открыть файл в режиме 'только для чтения' с обработкой как текстового файла
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream text(&file);   // Создаем текстовый поток для чтения из файла
    while (!text.atEnd()) {
        QString line = text.readLine();
        // Разбиваю строку запятой (Формат: "имя,пароль")
        QStringList parts = line.split(',');
        if (parts.size() >= 1) {  // Проверяем на пустоту
            if (parts[0] == username) {
                file.close();
                return true;
            }
        }
    }

    file.close();
    return false;
}

bool Reg_menu::saveToCSV(const QString& username, const QString& password)
{
    QFile file("users.csv");
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        return false;
    }

    QTextStream in(&file);
    in << username << "," << password << "\n";

    file.close();
    return true;
}

void Reg_menu::on_Register_PB_clicked()
{
    validateAndRegister();
}

// Основная функция
bool Reg_menu::validateAndRegister()
{
    // trimmed() удаляет пробелы в начале и конце строки
    QString username = ui->New_Login_text->text().trimmed();
    QString password = ui->New_Password_text->text();
    QString passwordRepeat = ui->New_Password_repeat_text->text();

    QString saltXpass = QString::fromStdString(Hash::hashString_s(password.toStdString(), FIXED_SALT));

    if (username.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите имя пользователя!");
        return false;
    }

    else if (password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите пароль!");
        return false;
    }

    else if (password != passwordRepeat) {
        QMessageBox::warning(this, "Ошибка", "Пароли не совпадают!");
        ui->New_Password_text->clear();
        ui->New_Password_repeat_text->clear();
        return false;
    }

    else if (isUsernameTaken(username)) {
        QMessageBox::warning(this, "Ошибка", "Пользователь с таким именем уже существует!");
        return false;
    }

    else if (saveToCSV(username, saltXpass)) {
        //QMessageBox::information(this, "Успех", "Регистрация прошла успешно!");

        ui->New_Login_text->clear();
        ui->New_Password_text->clear();
        ui->New_Password_repeat_text->clear();
        return true;
    }
    else {
        QMessageBox::critical(this, "Ошибка", "Не удалось сохранить данные!");
        return false;
    }
}

void Reg_menu::on_Open_LogMenu_PB_clicked()
{
    this->close();
    log_menu_link->show();
}

Reg_menu::~Reg_menu()
{
    delete ui;
}
