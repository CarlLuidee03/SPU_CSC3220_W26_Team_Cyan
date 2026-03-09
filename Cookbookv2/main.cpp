#include "mainwindow.h"
#include "database/DatabaseManager.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!DatabaseManager::instance().connect("cookbook.db"))
    {
        QMessageBox::critical(nullptr, "Database Error", "Could not open cookbook database.");
        return 1;
    }

    MainWindow w;
    w.show();

    return a.exec();
}
