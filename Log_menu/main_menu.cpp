#include "main_menu.h"
#include "ui_main_menu.h"
#include <QRandomGenerator>
#include <QDir>
#include <QDebug>
#include <QIcon>

Main_menu::Main_menu(const QString& username, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Main_menu)
    , username_(username)
{
    ui->setupUi(this);
    avatar_choice = new Avatar_choice();
    avatar_choice->move(1210,240);
    //ColorUtils colorUtils;
    //colorUtils.change_color(this);  //Функция []

    // Загружаем все изображения из папки photos в вектор
    loadPhotos();

    ui->Intro_line->setText("Добро пожаловать, " + username_);
    connect(avatar_choice, &Avatar_choice::transfer_selected_avatar, this,[this](const QString& avatar)
            {ui->Avatar_Lb->setStyleSheet(avatar);});
    avatar_choice->call_installExistingPhoto();//отложенная инициализация

    QObject::connect(ui->Choose_PB, &QPushButton::clicked, [this]() {
        if (avatar_choice->isVisible()) {
            avatar_choice->close();
            return;
        }
        avatar_choice->show();
    });
}

void Main_menu::resizeEvent(QResizeEvent *event)
{
    // Сохраняем исходное соотношение сторон (например, ширина/высота)
    static double aspectRatio = (double)width() / height();

    // Принудительно устанавливаем новый размер с сохранением пропорций
    int newWidth = event->size().width();
    int newHeight = (int)(newWidth / aspectRatio);

    resize(newWidth, newHeight);
}

Main_menu::~Main_menu()
{
    delete ui;
}
/**
 * Слот для обработки нажатия на кнопку "Change_plag"
 * Выбирает случайное изображение из вектора и устанавливает его на label
 */
void Main_menu::on_Change_plag_PB_clicked()
{
    qDebug()<< ui->Avatar_Lb->styleSheet();
    if (photos_.isEmpty()) {
        qDebug() << "There are no images uploaded to change!";
        return;
    }
    // Генерируем случайное число от 0 до (размер вектора - 1)
    // global() - получаем глобальный генератор случайных чисел
    int randomIndex = QRandomGenerator::global()->bounded(photos_.size());

    qDebug() << "Random index selected:" << randomIndex + 1;

    ui->Avatar_Lb->setPixmap(photos_[randomIndex]); // Установить пикчу с рандомным индексом

    ui->Avatar_Lb->setScaledContents(true);     // Масштабирование изображения под размер label
}

void Main_menu::loadPhotos()
{
    photos_.clear();

    QStringList resourcePaths;

    resourcePaths << "../../../Log_menu/resourses/photos/1.jpg"
                  << "../../../Log_menu/resourses/photos/2.jpg"
                  << "../../../Log_menu/resourses/photos/3.jpg"
                  << "../../../Log_menu/resourses/photos/4.jpg"
                  << "../../../Log_menu/resourses/photos/5.jpg"
                  << "../../../Log_menu/resourses/photos/6.jpg"
                  << "../../../Log_menu/resourses/photos/7.jpg"
                  << "../../../Log_menu/resourses/photos/8.jpg"
                  << "../../../Log_menu/resourses/photos/9.jpg"
                  << "../../../Log_menu/resourses/photos/10.jpg"
                  << "../../../Log_menu/resourses/photos/11.jpg"
                  << "../../../Log_menu/resourses/photos/12.jpg";

    // Проверяем, загрузилось ли что то
    if (resourcePaths.isEmpty()) {
        qDebug() << "No images were found!";
        return;
    }

    // Проходим по всем путям и загружаем изображения
    for (const QString &path : resourcePaths) {
        // Пытаемся загрузить изображение из ресурсов
        QPixmap pixmap(path);

        // Проверяем, успешно ли загрузилось изображение
        if (!pixmap.isNull()) {
            // Добавляем в вектор
            photos_.append(pixmap);
        } else {
            qDebug() << " Error loading:" << path;
        }
    }
}
