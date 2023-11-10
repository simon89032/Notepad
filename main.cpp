#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QDir>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString mainFilePath = QCoreApplication::applicationFilePath();
    QFileInfo mainFileInfo(mainFilePath);
    QString projectDir = mainFileInfo.absolutePath();

    QDir::setCurrent(projectDir);

    MainWindow w;
    w.show();

    qDebug() << "Current working directory: " << QDir::currentPath();

    QFile styleFile(":/css/styles.qss");

    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream textStream(&styleFile);
        QString styleSheet = textStream.readAll();
        a.setStyleSheet(styleSheet);

        qDebug() << "QSS file loaded successfully!";
    } else {
        qWarning() << "Failed to load the QSS file!";
    }

    a.setWindowIcon(QIcon(":/res/icons/main-icon.png"));

    return a.exec();
}
