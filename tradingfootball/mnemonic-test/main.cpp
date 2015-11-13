#include "testmnemonicdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestMnemonicDialog w;
    w.show();

    return a.exec();
}
