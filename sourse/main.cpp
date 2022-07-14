#include "app.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile stylesheet("qss/stylesheet.qss");

    if(!stylesheet.open(QFile::ReadOnly))
    {
        qDebug()<<"Error load qss styles";
    }
    QString styleSheet = QLatin1String(stylesheet.readAll());

    qApp->setStyleSheet(styleSheet);

    App w(&a);

    w.setWindowTitle("Double Filter");
    w.setWindowIcon(QIcon("images/App_icon.png"));
    w.resize(1280,720);

    w.show();

    return a.exec();
}
