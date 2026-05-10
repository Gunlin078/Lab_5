#ifndef MAIN_MENU_H
#define MAIN_MENU_H
#include "avatar_choice.h"
#include "colorutils.h"
#include <QWidget>
#include <QMainWindow>
#include <QVector>      // Для использования динамического массива
#include <QPixmap>      // Для работы с изображениями
#include <QLineEdit>

class Avatar_choice;

namespace Ui {
class Main_menu;
}

class Main_menu : public QWidget
{
    Q_OBJECT

public:
    explicit Main_menu(const QString& username = "Гость", QWidget *parent = nullptr);
    ~Main_menu();

private slots:
    void on_Change_plag_PB_clicked(); // Кнопка для смены аватарки на случайную

private:
    void loadPhotos();           // Загрузка всех изображений через пути
    Ui::Main_menu *ui;
    void resizeEvent(QResizeEvent *event);
    Avatar_choice* avatar_choice;  // Окно выбора аватара
    QVector<QPixmap> photos_;      // Хранилище для фоток
    QLineEdit*  Intro_line;       // Добро пожаловать, 'user'
    QString username_;
};

#endif
