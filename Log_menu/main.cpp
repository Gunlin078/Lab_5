#include "log_menu.h"
#include "colorutils.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Log_menu log_menu;
    log_menu.show();
    //log_menu->close();
    return a.exec();
}
