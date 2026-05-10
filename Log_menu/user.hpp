#ifndef USER_CLASS_HPP
#define USER_CLASS_HPP

#include <QString>
#include <QDialog>
#include <QListWidget>
#include <QDir>
#include <QVBoxLayout>
#include <filesystem>
namespace fs = std::filesystem;

class User{
public:
    User()
        :username_("")
        ,password_("")
        ,maxAvatarCount(4)
        ,activeAvatar_("C:/Qt/Lab_5/Log_menu/resourses/Images/Stub_1.png"){}
    User(QString& username, QString& password)
        :username_(username)
        ,password_(password)
        ,dirPath_ ("userdata/" + username_)
        ,maxAvatarCount(4)
        ,activeAvatar_("C:/Qt/Lab_5/Log_menu/resourses/Images/Stub_1.png"){
        //fs::create_directory(dirPath_.toStdString());
    }
    QString getPath(){return dirPath_;}
    QString getUsername(){return username_;}
    QString getPassword(){return password_;}
    void setUsername(QString& username){username_ = username; dirPath_ = "userdata/" + username_;}
    void setPassword(QString& password){password_ = password;}

    bool normalNumberOfPhotos(const short& NumberOfAvailablePhotos){return maxAvatarCount>=NumberOfAvailablePhotos;}
    void createDirectory(){
        fs::path dir = dirPath_.toStdString();
        if (fs::exists(dir)) return;
        fs::create_directory(dir);
    }
    //QMap <char, QString>availableAvatars;

private:
    QString username_;
    QString password_; //Хэшированный
    QString dirPath_;
    QString activeAvatar_;
    short maxAvatarCount;
};

#endif
