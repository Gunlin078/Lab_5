#include "avatar_choice.h"
#include "ui_avatar_choice.h"

const QString STUB_PATH = ":/resourses/photos/Stub_1.jpg";
const QString STUB_STYLE = "image: url("+STUB_PATH+");";

namespace fs = std::filesystem;
using json = nlohmann::json;

Avatar_choice::Avatar_choice(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Avatar_choice)
    , download_mode_(false)
    , currentUser_(UserManager::getInstance().getUser())
    , username_(currentUser_.getUsername())
    , userDirPath_(currentUser_.getPath())
    , userPhotoPath_(userDirPath_+"/photos")
{
    ui->setupUi(this);
    QObject::connect(ui->photo_upload_PB, &QPushButton::clicked, [this]() {download_mode_ = true;
        ui->photo_change_PB->show();
        ui->photo_upload_PB->hide();
    });
    QObject::connect(ui->photo_change_PB, &QPushButton::clicked, [this]() {download_mode_ = false;
    ui->photo_change_PB->hide();
    ui->photo_upload_PB->show();
    });
    QObject::connect(ui->photo_change1_PB, &QPushButton::clicked, [this](){onAvatarPBSclicked(ui->label_1);});
    QObject::connect(ui->photo_change2_PB, &QPushButton::clicked, [this](){onAvatarPBSclicked(ui->label_2);});
    QObject::connect(ui->photo_change3_PB, &QPushButton::clicked, [this](){onAvatarPBSclicked(ui->label_3);});
    QObject::connect(ui->photo_change4_PB, &QPushButton::clicked, [this](){onAvatarPBSclicked(ui->label_4);});
    QObject::connect(ui->photo_clean_PB,   &QPushButton::clicked, [this]()
                     {deliter.execute(userPhotoPath_, "all");
                     for (auto lb : this->findChildren<QLabel*>()) {lb->setStyleSheet("");
                     installPlaceholdersOnEmptyQLabels();
                     emit transfer_selected_avatar(STUB_STYLE);}
    });

    installExistingPhoto();
    installPlaceholdersOnEmptyQLabels();
}

Avatar_choice::~Avatar_choice()
{
    delete ui;
}

void Avatar_choice::saveInJSON(const QString& path,  const QString& type, const QString& number){
    fs::path jsonPath = userDirPath_.toStdString() / fs::path("userdata.json");

    if (!fs::exists(jsonPath)) {
        qDebug() << "The file doesn't exist";
        std::ofstream file(jsonPath);
        file.close();}
    json j;
    std::ifstream i(jsonPath);
    if (i.is_open() and fs::file_size(jsonPath) > 0){
        i>>j;
        i.close();
    }
    if (type == "Avatar"){
        j["Avatar"]["label_"+number.toStdString()]=path.toStdString();}
    else if (type == "Selected avatar"){
        j["Selected avatar"] = path.toStdString();}
    else{
        qDebug() << "Wrong type";}
    std::ofstream o(jsonPath);
    if (o.is_open()){
        o<<j.dump(4);
        o.close();
    }
}

QString Avatar_choice::getTheSelectedAvatarFromJSON(){
    fs::path jsonPath = userDirPath_.toStdString() / fs::path("userdata.json");
    std::ifstream i(jsonPath);
    json j;
    if (i.is_open() and fs::file_size(jsonPath) > 0){
        i>>j;
        i.close();

        if (j.contains("Selected avatar"))   return QString::fromStdString(j["Selected avatar"]);
    }
    return QString();
}

void Avatar_choice::installExistingPhoto(){
    QDir dir(userPhotoPath_);
    QStringList list = dir.entryList();
    for (QString photo : list){
        if (photo.length()<6){continue;}
        QString number = photo.at(photo.length() - 5);
        if      (number == "1"){ui->label_1->setStyleSheet("image: url("+ userPhotoPath_ + "/" + photo +");");}
        else if (number == "2"){ui->label_2->setStyleSheet("image: url("+ userPhotoPath_ + "/" + photo +");");}
        else if (number == "3"){ui->label_3->setStyleSheet("image: url("+ userPhotoPath_ + "/" + photo +");");}
        else if (number == "4"){ui->label_4->setStyleSheet("image: url("+ userPhotoPath_ + "/" + photo +");");}
        else deliter.execute(userPhotoPath_, "all");
    }

    QString selectedAvatar = getTheSelectedAvatarFromJSON();
    QLabel* label = this->findChild<QLabel*>(selectedAvatar);
    if (label){
        QString style = label->styleSheet();
        emit transfer_selected_avatar(style);
        qDebug()<<style<<" srazu";}
}

void Avatar_choice::call_installExistingPhoto(){installExistingPhoto();}

/*
void Avatar_choice::fillTheFolderWithStubs(){
    if (ui->label_1->styleSheet() == ""){ui->label_1->setStyleSheet(STUB_STYLE); uploader.execute(STUB_PATH, "1");}
    if (ui->label_2->styleSheet() == ""){ui->label_2->setStyleSheet(STUB_STYLE); uploader.execute(STUB_PATH, "2");}
    if (ui->label_3->styleSheet() == ""){ui->label_3->setStyleSheet(STUB_STYLE); uploader.execute(STUB_PATH, "3");}
    if (ui->label_4->styleSheet() == ""){ui->label_4->setStyleSheet(STUB_STYLE); uploader.execute(STUB_PATH, "4");}
}*/

void Avatar_choice::installPlaceholdersOnEmptyQLabels(){
    if (ui->label_1->styleSheet() == ""){ui->label_1->setStyleSheet(STUB_STYLE);}
    if (ui->label_2->styleSheet() == ""){ui->label_2->setStyleSheet(STUB_STYLE);}
    if (ui->label_3->styleSheet() == ""){ui->label_3->setStyleSheet(STUB_STYLE);}
    if (ui->label_4->styleSheet() == ""){ui->label_4->setStyleSheet(STUB_STYLE);}
}

short userImagesCount(const QString& userDirPath){
    QDir dir(userDirPath);
    if(!dir.exists()){return 0;}
    QStringList filters;
    filters << "*.png" << "*.jpg";

    dir.setNameFilters(filters);
    dir.setFilter(QDir::Files);

    if (dir.count()>4){return 5;}
    return dir.count();
}

void Avatar_choice::onAvatarPBSclicked(QLabel *selectLabel){
    if(selectLabel == nullptr) return;
    if(download_mode_ == true){
        QDir dir(QDir::currentPath());
        QString PhotoPath = dir.filePath(":/resourses/photos");

        UserDataFileDialog dialog (PhotoPath, this);
        dialog.setFixedSize(300,320);
        dialog.move(800,300);
        if (dialog.exec() == QDialog::Accepted){
            QString path = dialog.getSelectedFile();

            selectLabel->setStyleSheet("image: url("+path+");");

            QString number = (selectLabel->objectName()).right(1);
            deliter.execute(userPhotoPath_, number);
            uploader.execute(path, number);
            saveInJSON(path, "Avatar", number);
        }
    }
    if(download_mode_ == false){
        if (!currentUser_.normalNumberOfPhotos(userImagesCount(userPhotoPath_))){
            ui->label_1->setStyleSheet("");
            ui->label_2->setStyleSheet("");
            ui->label_3->setStyleSheet("");
            ui->label_4->setStyleSheet("");
            deliter.execute(userPhotoPath_, "all");
            //fillTheFolderWithStubs();
            installPlaceholdersOnEmptyQLabels();
            installExistingPhoto();
            emit transfer_selected_avatar(selectLabel->styleSheet());
            QMessageBox::warning(this, "Данные стёрты", "А не надо было лезть куда не надо");
            return;
        }
        emit transfer_selected_avatar(selectLabel->styleSheet());
        qDebug()<<selectLabel->styleSheet()<<" tik";
        saveInJSON(selectLabel->objectName(), "Selected avatar", 0);
    }
}
/*
            ui->label_1->setStyleSheet(STUB_STYLE);
            ui->label_2->setStyleSheet(STUB_STYLE);
            ui->label_3->setStyleSheet(STUB_STYLE);
            ui->label_4->setStyleSheet(STUB_STYLE);
            //cleaningExcessImages("all", userDirPath_);
*/
/*
void cleaningExcessImages(const QString& n, const QString& userDirPath){
    QDir dir(userDirPath);
    if(!dir.exists())  return;
    QStringList filters;
    filters << "*.png" << "*.jpg";

    dir.setNameFilters(filters);
    dir.setFilter(QDir::Files);

    QStringList list = dir.entryList(filters);
    if (n == "all"){
        for (QString i : list)
        {
            QFile::remove(userDirPath + "/" + i);
        }
    }
    else if (n == "1" or n == "2" or n == "3" or n == "4"){
        for (QString i : list)
        {
            if (i.at(i.length() - 5) == n){
                QFile::remove(userDirPath + "/" + i);
            }
        }
    }
}

void saveImageInUserFolder(const QString& pathImage, const QString& number, const QString& userDirPath){

    QDir dir;
    if (!dir.exists(userDirPath)){dir.mkpath(userDirPath);};

    QString imageTitle = userDirPath + "/" + getCurrentTime() + "-No" + number + ".png";
    QImage  image(pathImage);

    if (!image.isNull())    {image.save(imageTitle);}
    else{qDebug()<< "Error";}
}
QString getCurrentTime(){
    QDateTime now = QDateTime::currentDateTime();
    QString currentTime = now.toString("yyyy-MM-dd-HH-mm-ss");
    return currentTime;
}
*/
