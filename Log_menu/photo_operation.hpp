#ifndef PHOTO_OPERATION_HPP
#define PHOTO_OPERATION_HPP

#include "avatar_choice.h"
#include "include/nlohmann/json.hpp"

class PhotoOperation{
public:
    PhotoOperation()
        :userManager_(UserManager::getInstance())
        ,user_(userManager_.getUser()){}
    //void getNumber(QString number){number_ = number;}
    bool isImage(const std::string& photoPath) {
        static const std::unordered_set<std::string> image_extensions = {
            ".jpg", ".jpeg", ".png", ".bmp", ".webp", ".tiff", ".svg"
        };

        std::string ext = fs::path(photoPath).extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

        // Проверяем наличие в списке
        return image_extensions.find(ext) != image_extensions.end();
    }
    virtual ~PhotoOperation() = default;
    virtual void execute(const QString& photoPath, const QString& number) = 0;

protected:
    UserManager& userManager_;
    User& user_;
    //QString number_;
};

class UploadPhoto: public PhotoOperation{
public:
    virtual void execute(const QString& photoPath, const QString& number) override{
        if (isImage(photoPath.toStdString())){}

        if (!isAllStorages()) return;
        if (number.isNull())  qDebug() << "The number doesn't exist";

        QString imageTitle = user_.getPath() + "/photos/" + getCurrentTime() + "-No" + number + ".png";
        QImage  image(photoPath);

        if (!image.isNull()) image.save(imageTitle);
        else qDebug()<< "Error";
    }
private:
    QString getCurrentTime(){
        QDateTime now = QDateTime::currentDateTime();
        QString currentTime = now.toString("yyyy-MM-dd-HH-mm-ss");
        return currentTime;
    }
    bool isAllStorages(){
        fs::path generalDir = fs::path("userdata");
        if (!fs::exists(generalDir))    {fs::create_directory(generalDir);}

        fs::path userDir = generalDir / fs::path(user_.getUsername().toStdString());
        if (!fs::exists(userDir))    {fs::create_directory(userDir);}

        fs::path photoDir = userDir / fs::path("photos");
        if (!fs::exists(photoDir))    {fs::create_directory(photoDir);}

        if (fs::exists(photoDir)) return true;
        else return false;
    }
};


class DeletePhoto: public PhotoOperation{
public:
    virtual void execute(const QString& userDirPath, const QString& Qnumber) override{
        short Snumber = Qnumber.toShort();

        QDir dir(userDirPath);
        if(!dir.exists())  return;
        QStringList filters;
        filters << "*.jpg" << "*.jpeg" << "*.png" << "*.bmp" << "*.webp" << "*.tiff" << "*.svg";

        dir.setNameFilters(filters);
        dir.setFilter(QDir::Files);
        QStringList list = dir.entryList(filters);
        if (Qnumber == "all"){
            for (QString i : list)
            {
                QFile::remove(userDirPath + "/" + i);
            } return;
        }
        if (user_.normalNumberOfPhotos(Snumber)){
            for (QString i : list)
            {
                if (i.at(i.length() - 5) == Qnumber or i.at(i.length() - 6) == Qnumber){
                    QFile::remove(userDirPath + "/" + i);
                }
            } return;
        }
    }
};
class SetAvatarPhoto: public PhotoOperation{
    virtual void execute(const QString& photoPath, const QString& number) override{}
};
class ChoosePhoto: public PhotoOperation{
    virtual void execute(const QString& photoPath, const QString& number) override{}
};
#endif //PHOTO_OPERATION_HPP
