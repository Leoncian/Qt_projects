#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QSqlQuery>
#include "db_manager.h"
#include "mainwindow.h" // Add this line
#include <unistd.h>
#include <fcntl.h>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginWindow; }
QT_END_NAMESPACE

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    int flag;
    int buzzerState;
    Ui::LoginWindow *ui;
    void test_db();
    MainWindow *sensorMainWindow; // Declare a pointer to MainWindow
};
#endif // LOGINWINDOW_H
