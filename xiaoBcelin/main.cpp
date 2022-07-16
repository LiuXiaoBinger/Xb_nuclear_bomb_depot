#include "mychatdialog.h"
#include <QApplication>
#include"ckernel.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //FeiQDialog w;
   // w.show();
    Ckernel ::GetInstance();

    return a.exec();
}
