#include "writer.h"
#include <QCoreApplication>

int main()
{
//    QCoreApplication a(argc, argv);

    Writer *w = new Writer();
    w->DCCPolling();
    delete w;

    return 1; //a.exec();
}
