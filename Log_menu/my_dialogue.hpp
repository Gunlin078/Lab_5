#ifndef MY_DIALOGUE_HPP
#define MY_DIALOGUE_HPP

#include <QString>
#include <QDialog>
#include <QListWidget>
#include <QLabel>
#include <QDir>
#include <QVBoxLayout>
#include <filesystem>
namespace fs = std::filesystem;

class UserDataFileDialog: public QDialog{
    Q_OBJECT  // Макрос Qt для поддержки сигналов/слотов и метаобъектной системы

public:
    UserDataFileDialog(const QString& folderPath, QWidget* parent = nullptr)
        : QDialog(parent),
        folderPath_(folderPath) {
        setWindowTitle("Выберите фото из одобренных партией:");

        // Вертикальный менеджер компоновки для окна
        QVBoxLayout* layout = new QVBoxLayout(this);

        // ===== Список файлов =====
        listWidget_ = new QListWidget(this);
        listWidget_->setSelectionMode(QAbstractItemView::SingleSelection);
        layout->addWidget(listWidget_);
        listWidget_->setMouseTracking(true);  // Включаем отслеживание движения мыши

        // ===== Кнопки =====
        QHBoxLayout* buttonLayout = new QHBoxLayout();  // Горизонтальная компоновка для кнопок
        QPushButton* okButton = new QPushButton("Выбрать", this);      // Кнопка подтверждения
        QPushButton* cancelButton = new QPushButton("Отмена", this);   // Кнопка отмены

        // Добавляем кнопки в горизонтальную компоновку
        buttonLayout->addWidget(okButton);
        buttonLayout->addWidget(cancelButton);
        layout->addLayout(buttonLayout);  // Добавляем компоновку с кнопками в основную компоновку

        thumbnail = new QLabel(this);
        thumbnail->setStyleSheet("background-color: rgba(255, 255, 255, 180);"
                                 "color: red;"
                                 "border: 1px solid black;"
                                 "padding: 5px;");
        thumbnail->setAttribute(Qt::WA_TranslucentBackground);
        thumbnail->setAlignment(Qt::AlignCenter);
        thumbnail->setParent(this);
        positionLabel();


        connect(okButton, &QPushButton::clicked, this, &UserDataFileDialog::onOk);
        connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
        connect(listWidget_, &QListWidget::itemDoubleClicked, this, UserDataFileDialog::onOk);

        connect(listWidget_, &QListWidget::itemClicked, this, [this](QListWidgetItem* item){
            if (item) {showThumbnail(item->text());}
        });
        loadFiles();
    }

    // Геттер: возвращает полный путь к выбранному файлу
    QString getSelectedFile() const {return seletedFile_;}

private slots:
    void onOk(){
        if(listWidget_->currentItem()){
            seletedFile_ = folderPath_ + "/" + listWidget_->currentItem()->text();
            accept();
        }
    }

private:
    void loadFiles(){
        QDir dir(folderPath_);
        QStringList filters;    filters << "*.png" << "*.jpg";

        QStringList files = dir.entryList(filters, QDir::Files | QDir::NoDotAndDotDot);

        for(const QString& file : files){
            listWidget_->addItem(file);
        }
    }
    /*
    void showThumbnail(const QString& filename){

        QString fullPath = folderPath_+ "/" + filename;
        if (!QFile::exists(fullPath)) return;
          QPixmap pixmap(fullPath);
        if (pixmap.isNull()) return;
        pixmap = pixmap.scaled(100, 100);//, Qt::KeepAspectRatio, Qt::SmoothTransformation
        QString toolTipHTML = QString("<img src ='%1' width ='%2' height='%3'>")
                                  .arg(fullPath)
                                  .arg(pixmap.width())
                                  .arg(pixmap.height());
        QListWidgetItem* item = listWidget_->currentItem();
        if (item)   item->setToolTip(toolTipHTML);
        QString currentStyle = thumbnail->styleSheet();
        thumbnail->setStyleSheet(currentStyle + "image: url(" + fullPath +");");
        qDebug()<<thumbnail->styleSheet();
    }*/
    void showThumbnail(const QString& filename){
        QString fullPath = folderPath_ + "/" + filename;
        if (!QFile::exists(fullPath)) return;
        QPixmap pixmap(fullPath);
        if (pixmap.isNull()) return;

        // Масштабируем до размера thumbnail
        pixmap = pixmap.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // Устанавливаем pixmap напрямую, без styleSheet
        thumbnail->setPixmap(pixmap);

    }
    void positionLabel() {
        if (thumbnail) {
            int X = 165;
            int Y = 15;
            int Width = 120;
            int Height = 120;

            thumbnail->setGeometry(X,  Y,  Width,  Height); // width() - Width - X
            thumbnail->raise(); // Поднять на передний план
        }
    }
    QLabel* thumbnail = nullptr;
    QString folderPath_;      // Путь к папке, где ищем файлы
    QString seletedFile_;     // Путь к выбранному файлу (получаю при выборе)
    QListWidget* listWidget_; // Виджет со списком файлов
};

#endif // MY_DIALOGUE_H
