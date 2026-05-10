#include "log_menu.h"
#include "./ui_log_menu.h"
#include <QToolTip>

#define HAVE_ORIGINAL_BUTTON_STEEL
namespace fs = std::filesystem;

Log_menu::Log_menu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Log_menu)
{
    ui->setupUi(this);

    reg_menu = new Reg_menu(this);

    ColorUtils colorUtils;
    colorUtils.change_color(this); // Функция '[]'

    // Устанавливаем стиль для всех ToolTip в этом приложении
    qApp->setStyleSheet(
        "QToolTip {"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   background-color: #eeeeee;"
        "   border: 2px solid black;"
        "   padding: 10px;"
        "}"
        );
    connect(ui->Lost_Password_PB, &QPushButton::clicked, this, []() {
        QToolTip::showText(QCursor::pos(), "Очень-очень жаль :-(");
    });
}

Log_menu::~Log_menu()
{
    delete ui;
}

void Log_menu::on_Open_RegMenu_PB_clicked()
{
    this->hide();
    reg_menu->show();
}

bool Log_menu::isUsernameExists(const QString& username)
{
    QFile file("users.csv");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream text(&file);
    while (!text.atEnd()) {
        QString line = text.readLine();
        QStringList parts = line.split(',');
        if (parts.size() >= 1) { 
            if (parts[0] == username) {
                file.close();
                return true;
            }
        }
    }

    file.close();
    return false;
}

bool Log_menu::isUserExists(const QString& username, const QString& password)
{
    QFile file("users.csv");

    if (!file.open(QIODevice::ExistingOnly | QIODevice::ReadOnly)) {
        return false;
    }

    QTextStream text(&file);
    while (!text.atEnd()) {
        QString line = text.readLine();
        // Разбиваю строку запятой (Формат: "имя,соль:хэш")
        QStringList parts = line.split(',');
        if (parts.size() >= 1) {
            if (parts[0] == username and Hash::verifyString_s(password.toStdString(), parts[1].toStdString(), FIXED_SALT)){
                file.close();
                return true;
            }
        }
    }

    file.close();
    return false;
}

bool isAllDirectoryExists(){
    fs::path dir = "userdata";
    if (fs::exists(dir)) {return true;}
    return false;
}

bool isUserDirectoryExists(const QString& username){
    fs::path dir = fs::path("userdata") / username.toStdString();
    if (fs::exists(dir)) {return true;}
    return false;
}

bool Log_menu::user_login()
{
    // trimmed() удаляет пробелы в начале и конце строки
    QString username = ui->Login_text->text().trimmed();
    QString password = ui->Password_text->text();

    User newUser(username, password);
    UserManager::getInstance().setUser(newUser);

    if (username.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите имя пользователя!");
        return false;
    }

    else if (password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите пароль!");
        return false;
    }

    else if (!isUsernameExists(username)) {
        QMessageBox::warning(this, "Ошибка", "Пользователь не существует!");
        return false;
    }

    else if (!isAllDirectoryExists()){
        fs::path dir = "userdata";
        fs::create_directory(dir);
        return false;
    }

    else if (!isUserDirectoryExists(username)){
        //current_user.createDirectory();

        fs::path dir = "userdata";
        fs::path Udir = dir / username.toStdString();
        fs::create_directory(Udir);
        return false;
    }

    else if (isUserExists(username, password)) {
        main_menu = new Main_menu(username);
        main_menu->show();
        this->hide();
        return true;
    }

    else {
        QMessageBox::critical(this, "Ошибка", "Неизвестная ситуация!");
        return false;
    }
}

void Log_menu::on_Sign_in_PB_clicked()
{
    user_login();
}
