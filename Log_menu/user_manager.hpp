#ifndef USER_MANAGER_HPP
#define USER_MANAGER_HPP
#include "user.hpp"

//Singleton
class UserManager{
private:
    User current_user_;
    UserManager(){}

public:
    UserManager(const UserManager&) = delete;
    UserManager& operator=(const UserManager&) = delete;

    static UserManager& getInstance(){
        static UserManager instance;
        return instance;
    }

    User& getUser()     {return current_user_;}
    bool isLoggedIn()   {return !current_user_.getUsername().isEmpty();}
    void logOut()       {current_user_ = User();}
    void setUser(const User& user){current_user_ = user;}

    /* //Немного синтаксического сахара
    static User& getCUser(){return getInstance().getUser();}
    static bool& isCUserLoggedIn(){return getInstance().isLoggedIn();}
    static void  LogOutCUser(){getInstance().logOut();}
    static void  setCUser(const User& user){getInstance().setUser(user);}
    */
};

#endif
