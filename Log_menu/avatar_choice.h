#ifndef AVATAR_CHOICE_H
#define AVATAR_CHOICE_H

#include <QWidget>
#include <QLabel>
#include <QDir>
#include <QPushButton>
#include <QFileDialog>
#include <QDateTime>
#include <QMessageBox>
#include <filesystem>
#include <fstream>
//#include <include\nlohmann\json.hpp>
//Source Code Pro Medium

#include "main_menu.h"
#include "my_dialogue.hpp"
#include "user_manager.hpp"
#include "photo_operation.hpp"

namespace Ui {
class Avatar_choice;
}

class Avatar_choice : public QWidget
{
    Q_OBJECT
signals:
    void transfer_selected_avatar(const QString& selectedAvatar); //Отправка сигнала с путём выбранной авы
public:
    explicit Avatar_choice(QWidget *parent = nullptr);
    //std::pair<QString, QString> getTheSelectedAvatarFromJSON();
    QString getTheSelectedAvatarFromJSON();
    User& currentUser_;
    void installExistingPhoto();
    void call_installExistingPhoto();
    void installPlaceholdersOnEmptyQLabels();
    //void fillTheFolderWithStubs();
    void onAvatarPBSclicked(QLabel*); // Установка фотографии на аву или загрузка новой
    void saveInJSON(const QString& path,  const QString& type, const QString& number);
    ~Avatar_choice();
private:
    Ui::Avatar_choice *ui;
    QString username_;
    QString userDirPath_;
    QString userPhotoPath_;
    QString selectAvatar_;
    DeletePhoto deliter;
    UploadPhoto uploader;
    bool download_mode_ = true;
};

//QString getCurrentTime();
//void saveImageInUserFolder(const QString& pathImage, const QString &number, const QString& userDirPath);
short userImagesCount(const QString &userDirPath);
//void cleaningExcessImages(const QString& numberPhoto, const QString& userDirPath);

#endif // AVATAR_CHOICE_H
