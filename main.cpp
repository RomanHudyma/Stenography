#include "stenography.h"
#include <QApplication>
#include <Windows.h>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    Stenography w;
    w.show();

    return a.exec();
}
