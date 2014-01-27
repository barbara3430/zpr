#include <QtGui/QApplication>
#include <QThread>

#include "lobbygui.h"
#include "pokerclient.h"
#include "gamewindow.h"
#include "startwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QThread clientThread;
    PokerClient::getInstance().moveToThread(&clientThread);
    clientThread.start();
    StartWindow sw;
    sw.show();

	QObject::connect(&a, SIGNAL(aboutToQuit()), &clientThread, SLOT(quit()));
    return a.exec();
}
